/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

const cors = require('cors')({ origin: true, maxAge: 60 * 60 });
const mcj = require('./MonkeyCAMJob');

function getJobStatus(req, res) {
  cors(req, res, async () => {
    if (req.body.jobid === undefined) {
      res.status(400).send({ message: 'No jobid defined!' });
      return;
    }
    try {
      console.log('Get job status, jobid=', req.body.jobid);
      const job = await mcj.MonkeyCAMJob.get(req.body.jobid);
      res.json({
        id: job.key.id,
        inputsId: job.inputsId,
        progress: job.progress,
        state: job.stateName,
        boardName: job.boardName,
        jobCreatedTime: job.createdAt,
        jobStartedTime: job.startedAt,
        error: job.monkeyCAMResults ? job.monkeyCAMResults.error || null : null,
        failureReason: job.failureReason
      });
    } catch (err) {
      console.log('Error getting job status', err);
      res.status(500).send({ message: err.message || err.toString() });
    }
  });
}

function getJobFullResults(req, res) {
  cors(req, res, async () => {
    if (req.body.jobid === undefined) {
      res.status(400).send({ message: 'No jobid defined!' });
      return;
    }
    try {
      console.log('Get full results, jobid=', req.body.jobid);
      const job = await mcj.MonkeyCAMJob.get(req.body.jobid);
      console.log(job);
      res.json({
        jobFinishTime: Number(job.finishedAt),
        // @TODO: clocks on different machines :) Created on laptop, run on server with
        // time drift yields negative queue time.
        jobRunTime: (job.finishedAt - job.startedAt) / 1000.0,
        jobQueueTime: (job.startedAt - job.createdAt) / 1000.0,
        monkeyCAMEngineVersion: job.monkeyCAMResults
          ? job.monkeyCAMResults.monkeyCAMVersion
          : null,
        monkeyCAMEngineGitInfo: job.monkeyCAMResults
          ? job.monkeyCAMResults.gitInfo
          : null,
        boardName: job.boardName,
        ncFiles: job.monkeyCAMResults ? job.monkeyCAMResults.ncFiles : [],
        overviewHTMLFileLink: job.overviewPublicURL,
        allFilesLink: job.zipPublicURL
      });
    } catch (err) {
      console.log('Error getting full results', err);
      res.status(500).send({ message: err.message || err.toString() });
    }
  });
}

function getJobInputs(req, res) {
  cors(req, res, async () => {
    if (req.body.jobInputsId === undefined) {
      res.status(400).send({ message: 'No jobInputsId defined!' });
      return;
    }
    try {
      console.log('Get job inputs, inputsid=', req.body.jobInputsId);
      const jobInputs = await mcj.MonkeyCAMJobInputs.get(req.body.jobInputsId);
      res.json({
        id: req.params.inputsId,
        boardConfig: jobInputs.boardConfig,
        bindingConfig: jobInputs.bindingConfig,
        machineConfig: jobInputs.machineConfig,
        bindingDist: jobInputs.bindingDist
      });
    } catch (err) {
      console.log('Error getting job inputs', err);
      res.status(500).send({ message: err.message || err.toString() });
    }
  });
}

function addJob(req, res) {
  cors(req, res, async () => {
    if (
      req.body.boardConfig === undefined ||
      req.body.machineConfig === undefined
    ) {
      res.status(400).send({ message: 'Missing board or machine config' });
      return;
    }
    try {
      console.log('Adding job');
      const jobId = await new mcj.MonkeyCAMJob(
        req.body.boardConfig,
        req.body.bindingConfig || {},
        req.body.machineConfig,
        req.body.bindingDist || 0
      ).enqueue();
      console.log('Added jobid=', jobId);
      res.send({ jobId: jobId });
    } catch (err) {
      console.log('Error adding new job', err);
      res.status(500).send({ message: err.message || err.toString() });
    }
  });
}

module.exports = {
  getJobStatus,
  getJobFullResults,
  getJobInputs,
  addJob
};
