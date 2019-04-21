/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

const { Datastore } = require('@google-cloud/datastore');
const ds = new Datastore({ projectId: 'monkeycam-web-app' });
const { PubSub } = require('@google-cloud/pubsub');

const STATE_UNKNOWN = 0;
const STATE_CREATED = 1;
const STATE_QUEUED = 2;
const STATE_FAILED_RETRY = 3;
const STATE_RUNNING = 4;
const STATE_SAVING = 5;
const STATE_COMPLETED = 6;
const STATE_FAILED = 7;

const STATE_NAMES = [
  'Unknown',
  'Created',
  'Queued',
  'Failed, will retry',
  'Running',
  'Saving',
  'Completed',
  'Failed'
];

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

class MonkeyCAMJobInputs {
  constructor(boardConfig, bindingConfig, machineConfig, bindingDist) {
    this.boardConfig = boardConfig;
    this.bindingConfig = bindingConfig;
    this.machineConfig = machineConfig;
    this.bindingDist = bindingDist;
    this.key = null;
  }

  static _jobInputsFromDSEntity(key, entity) {
    let ji = new MonkeyCAMJobInputs();
    ji.key = key;
    Object.assign(ji, entity);
    return ji;
  }

  _updateFromDSEntity(entity) {
    Object.assign(this, entity);
  }

  _toDSEntity() {
    if (this.key == null) this.key = ds.key('JobInputs');
    const excludeFromIndexes = [
      'boardConfig',
      'bindingConfig',
      'machineConfig',
      'bindingDist'
    ];
    const excludedFields = ['key'];
    let e = { key: this.key, data: [] };
    Object.keys(this).forEach(key => {
      if (!excludedFields.includes(key)) {
        let v = { name: key, value: this[key] };
        if (excludeFromIndexes.includes(key)) {
          v['excludeFromIndexes'] = true;
        }
        e.data.push(v);
      }
    });
    return e;
  }

  async save() {
    const entity = this._toDSEntity();
    await ds.save(entity);
    console.log(`Saved ${this.key.path} to data store.`);
    return this.key.id;
  }

  static async get(id) {
    const key = ds.key(['JobInputs', ds.int(id)]);
    const data = await ds.get(key);
    const entity = data[0];
    if (!entity) {
      let err = new Error('Invalid job inputs id');
      err.code = 404;
      throw err;
    }
    return this._jobInputsFromDSEntity(key, entity);
  }
}

class MonkeyCAMJobProgress {
  static async create() {
    const key = ds.key('JobProgress');
    return await MonkeyCAMJobProgress._save(key, 0);
  }

  static async save(id, progress) {
    const key = ds.key(['JobProgress', ds.int(id)]);
    return await MonkeyCAMJobProgress._save(key, progress);
  }

  static async _save(key, progress) {
    const entity = {
      key: key,
      data: [{ name: 'p', value: progress, excludeFromIndexes: true }]
    };
    await ds.save(entity);
    console.log(`Saved ${key.path} to data store: ${progress}`);
    return key.id;
  }

  static async get(id) {
    const key = ds.key(['JobProgress', ds.int(id)]);
    const data = await ds.get(key);
    const entity = data[0];
    if (!entity) {
      let err = new Error('Invalid job progress id');
      err.code = 404;
      throw err;
    }
    return entity.p;
  }
}

class MonkeyCAMJob {
  constructor(boardConfig, bindingConfig, machineConfig, bindingDist) {
    this.inputs = new MonkeyCAMJobInputs(
      boardConfig,
      bindingConfig,
      machineConfig,
      bindingDist
    );
    this.key = null;
    this.inputsId = null;
    this.state = STATE_CREATED;
    if (boardConfig) this.boardName = boardConfig.board.name;
  }

  static _jobFromDSEntity(key, entity) {
    let job = new MonkeyCAMJob();
    job.key = key;
    Object.assign(job, entity);
    return job;
  }

  _updateFromDSEntity(entity) {
    Object.assign(this, entity);
  }

  _toDSEntity() {
    if (this.key == null) this.key = ds.key('Job');
    const excludeFromIndexes = [
      'inputsId',
      'progressId',
      'overviewPublicURL',
      'zipPublicURL',
      'startedAt',
      'finishedAt',
      'monkeyCAMResults',
      'attemptCount',
      'backoffMS'
    ];
    const excludedFields = ['key', 'inputs'];
    let e = { key: this.key, data: [] };
    Object.keys(this).forEach(key => {
      if (!excludedFields.includes(key)) {
        let v = { name: key, value: this[key] };
        if (excludeFromIndexes.includes(key)) {
          v['excludeFromIndexes'] = true;
        }
        e.data.push(v);
      }
    });
    return e;
  }

  static async updateJobState(id, newState, props = {}, opts = {}) {
    const key = ds.key(['Job', ds.int(id)]);
    const maxAttempts = opts.maxAttempts || 3;
    let attemptCount = 0;
    let backoffMS = opts.backoffMS || 100;
    while (attemptCount++ < maxAttempts) {
      if (attemptCount > 1) {
        console.log(`Backoff ${backoffMS}ms in updateJobState to ${newState}`);
        await sleep(backoffMS);
        backoffMS *= 2;
      }
      const transaction = ds.transaction();
      try {
        await transaction.run();
        const data = await transaction.get(key);
        const entity = data[0];
        if (!entity) {
          console.log(
            'Should be impossible: Failed to find job in updateJobState',
            key
          );
          await transaction.rollback();
          continue;
        }
        let job = MonkeyCAMJob._jobFromDSEntity(key, entity);
        console.log('updateJobState', job.state, newState, props);
        if (newState > job.state || newState === STATE_FAILED_RETRY) {
          job.state = newState;
          Object.assign(job, props);
          transaction.save(job._toDSEntity());
          if (opts.sleep) {
            await sleep(opts.sleep); // For forcing races in testing.
          }
          await transaction.commit();
          return job;
        } else {
          console.log('Tried to update to lower job state');
          await transaction.rollback();
          return job;
        }
      } catch (err) {
        console.log(
          'Error running updateJobState transaction, rolling back',
          id,
          newState,
          props,
          err
        );
        try {
          await transaction.rollback();
        } catch (err) {
          // Rollback may fail because commit already rolled back on contention, or due to
          // transient server failure. We will retry in all cases.
          console.log('Rollback failed in updateJobState', err);
        }
        continue;
      }
    }
    console.log('Reached maximum attempts in updateJobState, giving up.');
    throw new Error('Unable to update persistent job state');
  }

  async enqueue(jobQueueTopic) {
    this.createdAt = Date.now();
    this.inputsId = await this.inputs.save();
    this.progressId = await MonkeyCAMJobProgress.create();
    const jobEntity = this._toDSEntity();
    await ds.save(jobEntity);
    console.log(`Saved ${jobEntity.key.path} to data store.`, this);

    try {
      const pubsub = new PubSub({ projectId: 'monkeycam-web-app' });
      const job_queue_topic = pubsub.topic(jobQueueTopic);
      const dataBuffer = Buffer.from(
        JSON.stringify({ id: this.key.id, inputsId: this.inputsId })
      );
      const messageId = await job_queue_topic.publish(dataBuffer);
      console.log(
        'Published message id ' + messageId + ' to topic ' + jobQueueTopic
      );
      try {
        await MonkeyCAMJob.updateJobState(this.key.id, STATE_QUEUED);
      } catch (err) {
        console.log('Warning: Failed updating state after enqueue.', err);
      }
      return this.key.id;
    } catch (err) {
      await MonkeyCAMJob.updateJobState(this.key.id, STATE_FAILED, {
        failureReason: 'Failed to enqueue worker message',
        finishedAt: Date.now()
      });
      throw err;
    }
  }

  static async get(id) {
    const key = ds.key(['Job', ds.int(id)]);
    const data = await ds.get(key);
    const entity = data[0];
    if (!entity) {
      let err = new Error('Invalid job id');
      err.code = 404;
      throw err;
    }
    let job = this._jobFromDSEntity(key, entity);
    job.stateName = STATE_NAMES[job.state];
    job.zombie = false;
    if (job.state < STATE_COMPLETED) {
      job.progress = await MonkeyCAMJobProgress.get(job.progressId);
    } else {
      job.progress = 100;
    }
    return job;
  }
}

module.exports = {
  MonkeyCAMJob,
  MonkeyCAMJobInputs,
  MonkeyCAMJobProgress,
  STATE_FAILED_RETRY,
  STATE_RUNNING,
  STATE_SAVING,
  STATE_COMPLETED,
  STATE_FAILED
};
