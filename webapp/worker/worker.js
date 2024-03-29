/*
 * Copyright Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

const archiver = require('archiver');
const fse = require('fs-extra');
const path = require('path');
const spawn = require('child_process').spawn;
const {Storage} = require('@google-cloud/storage');
const gcs = new Storage({projectId: 'monkeycam-web-app'});
const {PubSub} = require(`@google-cloud/pubsub`);

const nconf = require('./config');
const mcj = require('./MonkeyCAMJob');

const stream = require('stream');

let jobQueueSubscription = 'MonkeyCAM-Job-Queue-sub';
let jobQueueTopic = 'MonkeyCAM-Job-Queue';

console.log(process.env)
console.log(process.argv)
console.log(process.versions)

let development = process.env.NODE_ENV !== 'production';

if (development) {
    console.log('Development mode');
    jobQueueSubscription = 'MonkeyCAM-Job-Queue-Test-sub';
    jobQueueTopic = 'MonkeyCAM-Job-Queue-Test';
} else {
    console.log('Production mode');
}

let lastHeartbeatTime = Date.now();

const pubsub = new PubSub({projectId: 'monkeycam-web-app'});

startListeningForJobs(jobQueueSubscription);


function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function setupJobDir(jobDir, boardConfig, bindingConfig, machineConfig) {
    await fse.emptyDir(jobDir);
    await fse.writeFile(
        path.join(jobDir, 'board-config.json'),
        JSON.stringify(boardConfig)
    );
    await fse.writeFile(
        path.join(jobDir, 'binding-config.json'),
        JSON.stringify(bindingConfig)
    );
    await fse.writeFile(
        path.join(jobDir, 'machine-config.json'),
        JSON.stringify(machineConfig)
    );
}

async function runMonkeyCAM(jobDir, inputs, progressId) {
    return new Promise((resolve, reject) => {
        let args = [
            '--board',
            path.join(jobDir, 'board-config.json'),
            '--binding',
            path.join(jobDir, 'binding-config.json'),
            '--machine',
            path.join(jobDir, 'machine-config.json'),
            '--outdir',
            jobDir,
            '--jsonOutputFD',
            '3'
        ];
        if (inputs.bindingDist) {
            args = args.concat(['--bindingdist', inputs.bindingDist]);
        }
        console.log('Spawning MonkeyCAM process w/ args', args);
        const monkeyCamProcess = spawn(
            path.join(nconf.get('monkeycamPath'), 'MonkeyCAM'),
            args,
            {
                stdio: ['pipe', 'pipe', 'pipe', 'pipe'] // fd #3 is jsonOutputFD
            }
        );

        let progressMax = 0;
        let progress = 0;
        let results = {ncFiles: []};
        let stdoutStr = '';
        let stderrStr = '';

        let timerId = setInterval(() => {
            mcj.MonkeyCAMJobProgress.save(progressId, progress);
        }, 2000);

        monkeyCamProcess.stdout.on('data', data => {
            console.log(`stdout: ${data}`);
            stdoutStr += data.toString();
        });

        monkeyCamProcess.stderr.on('data', data => {
            console.log(`stderr: ${data}`);
            stderrStr += data.toString();
        });

        const resultStream = new stream.Writable({
            objectMode: true,
            write(obj, encoding, done) {
                console.log('data:', JSON.stringify(obj));
                if (obj.progressMax) {
                    progressMax = obj.progressMax;
                }
                if (obj.progress) {
                    progress = (obj.progress / progressMax) * 100.0;
                }
                if (obj.version) {
                    results['monkeyCAMVersion'] = obj.version;
                }
                if (obj.git) {
                    results['gitInfo'] = obj.git;
                }
                if (obj.ncFile) {
                    results['ncFiles'].push(obj.ncFile);
                }
                if (obj.error) {
                    results['error'] = obj.error;
                }
                done();
            }
        });
        const jsonStream = require('JSONStream').parse('*');
        const fd3Finished = new Promise((resolve, reject) => {
            resultStream.on('finish', () => {
                resolve();
            });
        });
        monkeyCamProcess.stdio[3].pipe(jsonStream).pipe(resultStream);

        monkeyCamProcess.on('exit', async (code, signal) => {
            console.log('In exit callback...');
            clearInterval(timerId);
            console.log('Waiting for JSON output to flush');
            await fd3Finished; // Wait for the json output to flush completely.
            console.log('Saving...');
            await mcj.MonkeyCAMJobProgress.save(progressId, progress);
            results['exitCode'] = code;
            results['signal'] = signal;
            results['stdout'] = stdoutStr;
            results['stderr'] = stderrStr;
            if (signal && !results['error']) {
                results['error'] = {
                    kind: 'Unknown Fatal Error',
                    message: `Process terminated with signal ${signal}`
                };
            }
            if (code && !results['error']) {
                results['error'] = {
                    kind: 'Unknown Fatal Error',
                    message: `Process exited with code ${code}`
                };
            }
            resolve(results);
        });

        monkeyCamProcess.on('error', async err => {
            console.log('In error callback...');
            console.log(err);
            clearInterval(timerId);
            console.log('Saving...');
            await mcj.MonkeyCAMJobProgress.save(progressId, progress);
            const code = err['code'];
            results['exitCode'] = code;
            results['signal'] = '';
            results['error'] = {
                kind: 'Unknown Fatal Error',
                message: `Process failed to start with code ${code}`
            };
            resolve(results);
        });
    });
}

// @TODO: does JS really not have anything better for us here?!
function formatDate(d) {
    function pad(n) {
        return ('0' + n).slice(-2);
    }

    return `${d.getFullYear()}${pad(d.getMonth() + 1)}${pad(d.getDate())}-${pad(
        d.getHours()
    )}${pad(d.getMinutes())}${pad(d.getSeconds())}`;
}

async function zipAndUploadFile(jobDir, persistentKey, dateString, boardName) {
    return new Promise((resolve, reject) => {
        let zip = archiver.create('zip', {zlib: {level: 9}});
        zip.directory(jobDir, `${boardName}-${dateString}`);
        zip.on('warning', function (err) {
            console.log('Zip warning: ', err);
            if (err.code === 'ENOENT') {
                console.log('Warning from zip:', err);
            } else {
                console.log('Error (from warning) from zip:', err);
                reject(err);
            }
        });
        zip.on('error', function (err) {
            console.log('Zip error: ', err);
            reject(err);
        });
        zip.finalize();

        const bucket = gcs.bucket('monkeycam-results'); // @TODO: dup, factor
        const zipDest = bucket.file(
            `${persistentKey}/${boardName}-${dateString}-results.zip`
        );
        const zipDestStream = zipDest.createWriteStream({public: true});
        zipDestStream.on('error', err => {
            console.log('Error from zipDestStream:', err);
            reject(err);
        });
        zipDestStream.on('finish', () => {
            resolve(`https://storage.googleapis.com/${bucket.name}/${zipDest.name}`);
        });
        zip.pipe(zipDestStream);
    });
}

async function uploadFile(jobDir, persistentKey, dateString, boardName) {
    const bucket = gcs.bucket('monkeycam-results'); // @TODO: dup, factor
    const result = await bucket.upload(
        path.join(jobDir, `${boardName}-overview.html`),
        {
            destination: `${persistentKey}/${boardName}-${dateString}-overview.html`,
            public: true,
            gzip: true
        }
    );
    const file = result[0];
    return `https://storage.googleapis.com/${bucket.name}/${file.name}`;
}

function startListeningForJobs(jobQueueSubscription) {
    const subscription = pubsub.subscription(jobQueueSubscription);

    subscription.on('error', (err) => {
        console.log('Subscription error: ' + JSON.stringify(err));
        process.exit(0);
    });

    subscription.on('close', () => {
        console.log('Subscription closed unexpectedly, exiting');
        process.exit(0);
    });

    subscription.on('debug', msg => console.log('Subscription debug: ' + msg.message));

    subscription.on('message', processMonkeyCAMJob);

    console.log('Listening for jobs on subscription ' + jobQueueSubscription);

    // Backstop, just in case the pubsub queue stalls forever. This has happened intermittently, and I've so far not
    // been able to isolate the cause or necessarily detect it. I've got logging in place, and am exiting on sub
    // close and error, but will there still be issues? So, send some "self-heartbeat" messages on a regular interval,
    // and bail when they fail.
    const necessaryHeartbeatInterval = 1 * 60 * 1000; // nb: ensure this is longer than the max job time!
    setInterval(async () => {
        const d = Date.now() - lastHeartbeatTime;
        if (d > necessaryHeartbeatInterval) {
            console.log('Missing heartbeat, exiting: ' + d / 1000 + 's');
            process.exit(0);
        }
        sendHeartbeatMessage();
    }, 1 * 60 * 1000);
}

async function sendHeartbeatMessage() {
    try {
        const job_queue_topic = pubsub.topic(jobQueueTopic);
        const messageId = await job_queue_topic.publishMessage({json: {id: 'heartbeat'}})
        console.log('Heartbeat sent, message id ' + messageId + ' to topic ' + jobQueueTopic);
    } catch (err) {
        console.log('Heartbeat send error: ' + JSON.stringify(err));
    }
}

async function processMonkeyCAMJob(message) {
    console.log(`Received job message ${message.id}`);
    let messageArgs = JSON.parse(message.data);
    console.log(messageArgs);

    lastHeartbeatTime = Date.now(); // all messages count as heartbeats
    if (messageArgs.id === 'heartbeat') {
        message.ack();
        return;
    }

    const persistentKey = messageArgs.id;
    console.log('Draining id ' + persistentKey);
    let job = null;
    try {
        job = await mcj.MonkeyCAMJob.updateJobState(
            persistentKey,
            mcj.STATE_RUNNING,
            {
                startedAt: Date.now()
            }
        );
        if (job.state != mcj.STATE_RUNNING) {
            console.log('Processing job: failed to transition to running', job.state);
            message.ack();
            return;
        }

        const inputs = await mcj.MonkeyCAMJobInputs.get(job.inputsId);

        const jobDir = `./jobs/${persistentKey}`; // @TODO: tmpfs
        await setupJobDir(
            jobDir,
            inputs.boardConfig,
            inputs.bindingConfig,
            inputs.machineConfig
        );
        console.log('Directory created successfully!');

        const results = await runMonkeyCAM(jobDir, inputs, job.progressId);
        console.log(
            `Child process exited with code ${results.exitCode}, signal ${
                results.signal
            }`
        );
        console.log(results);

        if (results.error) {
            await mcj.MonkeyCAMJob.updateJobState(persistentKey, mcj.STATE_FAILED, {
                monkeyCAMResults: results,
                finishedAt: Date.now()
            });
            message.ack();
            return;
        }

        await mcj.MonkeyCAMJob.updateJobState(persistentKey, mcj.STATE_SAVING, {
            monkeyCAMResults: results
        });

        const dateString = formatDate(new Date());
        const zipPublicURL = await zipAndUploadFile(
            jobDir,
            persistentKey,
            dateString,
            inputs.boardConfig.board.name
        );
        console.log('Finish on zipDestStream:', zipPublicURL);

        const overviewPublicURL = await uploadFile(
            jobDir,
            persistentKey,
            dateString,
            inputs.boardConfig.board.name
        );
        console.log('Finish on overviewDestStream:', overviewPublicURL);

        await mcj.MonkeyCAMJob.updateJobState(persistentKey, mcj.STATE_COMPLETED, {
            finishedAt: Date.now(),
            overviewPublicURL: overviewPublicURL,
            zipPublicURL: zipPublicURL
        });
        message.ack();
        return;
    } catch (err) {
        console.log(`Failure processing job ${persistentKey}`, err);
        if (job) {
            let attemptCount = job.attemptCount || 1;
            let backoffMS = job.backoffMS || 1000 + Math.floor(Math.random() * 1000); // 1s - 2s

            if (attemptCount < 3) {
                await mcj.MonkeyCAMJob.updateJobState(
                    persistentKey,
                    mcj.STATE_FAILED_RETRY,
                    {
                        attemptCount: attemptCount + 1,
                        backoffMS: backoffMS * 2
                    }
                );
                console.log(
                    `Backoff ${backoffMS}ms after failed job, attempt #${attemptCount}`
                );
                await sleep(backoffMS);
                message.nack();
                return;
            }
        }

        // Final backstop is to just give up.
        console.log(`Giving up on failed job ${persistentKey}`);
        await mcj.MonkeyCAMJob.updateJobState(persistentKey, mcj.STATE_FAILED, {
            failureReason: 'Failure running MonkeyCAM: ' + err.message,
            finishedAt: Date.now()
        });
        message.ack();
    }
}
