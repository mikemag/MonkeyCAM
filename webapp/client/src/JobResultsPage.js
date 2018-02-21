/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import { Button, Grid, Row, Col, Panel, Table, Alert } from 'react-bootstrap';
import { LinkContainer } from 'react-router-bootstrap';
import Spinner from 'react-spinkit';
import TimeAgo from 'react-timeago';
import { BadJobNotice } from './CommonComponents';

class JobExecutionDetails extends Component {
  render() {
    let tableRows = [];
    if (this.props.basicResults) {
      tableRows = tableRows.concat([
        <tr key={1}>
          <td>Job id</td>
          <td>{this.props.basicResults.id}</td>
        </tr>,
        <tr key={2}>
          <td>Job status</td>
          <td>{this.props.basicResults.state}</td>
        </tr>
      ]);
    }

    if (this.props.fullResults) {
      const fr = this.props.fullResults;
      if (fr.jobQueueTime) {
        tableRows = tableRows.concat([
          <tr key={11}>
            <td>Queue time</td>
            <td>{fr.jobQueueTime}s</td>
          </tr>
        ]);
      }
      if (fr.jobRunTime) {
        tableRows = tableRows.concat([
          <tr key={12}>
            <td>Run time</td>
            <td>{fr.jobRunTime}s</td>
          </tr>
        ]);
      }
      if (fr.jobFinishTime) {
        tableRows = tableRows.concat([
          <tr key={13}>
            <td>Finish time</td>
            <td>
              <TimeAgo
                date={fr.jobFinishTime}
                title={new Date(fr.jobFinishTime).toString()}
              />
            </td>
          </tr>
        ]);
      }
      if (fr.monkeyCAMEngineVersion) {
        tableRows = tableRows.concat([
          <tr key={101}>
            <td>Engine version</td>
            <td>{fr.monkeyCAMEngineVersion}</td>
          </tr>
        ]);
      }
      if (fr.monkeyCAMEngineGitInfo) {
        tableRows = tableRows.concat([
          <tr key={102}>
            <td>Engine build</td>
            <td>
              {fr.monkeyCAMEngineGitInfo.branch}{' '}
              <a
                href={
                  'https://github.com/mikemag/MonkeyCAM/tree/' +
                  fr.monkeyCAMEngineGitInfo.commitHash
                }
              >
                {fr.monkeyCAMEngineGitInfo.commitHash}
              </a>{' '}
              committed on {fr.monkeyCAMEngineGitInfo.commitDate}
            </td>
          </tr>
        ]);
      }
    }

    if (tableRows.length > 0) {
      const title = <h3 className="text-center">Job Execution Details</h3>;
      return (
        <Panel header={title}>
          <Table striped condensed>
            <tbody>{tableRows}</tbody>
          </Table>
        </Panel>
      );
    } else {
      return null;
    }
  }
}

class JobErrorDetails extends Component {
  render() {
    const err = this.props.error;
    if (typeof err === 'string') {
      return <div>{err}</div>;
    } else {
      return (
        <div>
          {err.kind}
          {err.configName ? ' in ' + err.configName : null}
          : {err.path || err.key ? "'" : null}
          {err.path ? err.path + '.' : null}
          {err.key}
          {err.path || err.key ? "'" : null}
          {err.key && err.message ? ', ' : null}
          {err.message}
        </div>
      );
    }
  }
}

// @TODO: dup, factor.
function checkStatus(res) {
  if (res.status >= 200 && res.status < 300) {
    return Promise.resolve(res);
  }
  return res.text().then(errText => {
    try {
      const errObj = JSON.parse(errText);
      return Promise.reject(new Error(errObj.message));
    } catch (ex) {
      return Promise.reject(new Error(errText));
    }
  });
}

class JobResultsPage extends Component {
  constructor(props) {
    super(props);
    this.state = {
      hasBasicResults: false,
      basicResults: undefined,
      hasFullResults: false,
      fullResults: undefined,
      serverError: undefined
    };
  }

  async getJobBasicResults(jobId) {
    let res = await fetch(
      'https://us-central1-monkeycam-web-app.cloudfunctions.net/getJobStatus',
      {
        accept: 'application/json',
        method: 'POST',
        body: JSON.stringify({
          jobid: jobId
        }),
        headers: {
          'Content-Type': 'application/json'
        }
      }
    );
    res = await checkStatus(res);
    return res.json();
  }

  async getJobFullResults(jobId) {
    let res = await fetch(
      'https://us-central1-monkeycam-web-app.cloudfunctions.net/getJobFullResults',
      {
        accept: 'application/json',
        method: 'POST',
        body: JSON.stringify({
          jobid: jobId
        }),
        headers: {
          'Content-Type': 'application/json'
        }
      }
    );
    res = await checkStatus(res);
    return res.json();
  }

  componentDidMount() {
    window.scrollTo(0, 0);
    const jobId = this.props.match.params.jobId;
    // @TODO: get basic and full in parallel ;) Or just make one damn call which includes both.
    this.getJobBasicResults(jobId)
      .then(result => {
        console.log('Basic results', result);
        if (result.state !== 'Completed' && result.state !== 'Failed') {
          this.props.history.replace(`/jobactivity/${jobId}`);
          return;
        }
        this.setState({ hasBasicResults: true, basicResults: result });
        this.getJobFullResults(jobId)
          .then(result => {
            console.log('Full results', result);
            this.setState({ hasFullResults: true, fullResults: result });
          })
          .catch(err => {
            console.log('Err getting full results:', err);
            this.setState({ serverError: err });
          });
      })
      .catch(err => {
        console.log('Err getting basic results:', err);
        this.setState({ serverError: err });
      });
  }

  componentWillUnmount() {}

  render() {
    let resultsDesc = null;
    let errorDesc = null;
    let ncFileTable = null;
    let retryDesc = null;
    if (this.state.hasFullResults) {
      if (
        this.state.fullResults.ncFiles &&
        this.state.fullResults.ncFiles.length > 0
      ) {
        this.state.fullResults.ncFiles.sort((a, b) =>
          a.filename.localeCompare(b.filename)
        );
        const tableRows = this.state.fullResults.ncFiles.map((file, index) => {
          return (
            <tr key={index} className="text-left">
              <td>{file.filename}</td>
              <td className="text-right">{file.lineCount}</td>
            </tr>
          );
        });
        ncFileTable = (
          <Table striped condensed>
            <thead>
              <tr>
                <th>NC File Name</th>
                <th className="text-right">Line Count</th>
              </tr>
            </thead>
            <tbody>{tableRows}</tbody>
          </Table>
        );
      }

      if (this.state.basicResults.state === 'Completed') {
        resultsDesc = (
          <div className="text-center">
            <p>
              Generated {this.state.fullResults.ncFiles.length} G-code programs
              for board '{this.state.fullResults.boardName}'.
            </p>
            <Button
              bsStyle="primary"
              style={{ margin: 5 }}
              href={this.state.fullResults.overviewHTMLFileLink}
              target="_blank"
            >
              Results Overview
            </Button>
            <Button
              bsStyle="primary"
              style={{ margin: 5 }}
              href={this.state.fullResults.allFilesLink}
            >
              Download .zip of all files
            </Button>
            <p />
            {ncFileTable}
          </div>
        );
      } else {
        errorDesc = (
          <div>
            <Alert bsStyle="danger" className="text-left">
              <h4>Failed to generate G-code programs</h4>
              <JobErrorDetails
                error={
                  this.state.basicResults.failureReason ||
                  this.state.basicResults.error
                }
              />
            </Alert>
            <BadJobNotice />
          </div>
        );
      }

      const retryLink = `/design/${this.state.basicResults.inputsId}`;
      retryDesc = (
        <div className="text-right">
          <LinkContainer to={retryLink}>
            <Button bsStyle="primary">Change Design and Run Again</Button>
          </LinkContainer>
        </div>
      );
    }

    let spinnerRow = null;
    if (!resultsDesc && !errorDesc && !this.state.serverError) {
      spinnerRow = (
        <div className="text-info text-center">
          <Spinner
            id="spinner"
            color="inherit"
            name="three-bounce"
            fadeIn="none"
          />
        </div>
      );
    }

    let serverError = null;
    if (this.state.serverError) {
      serverError = (
        <div className="text-info text-center">
          <Alert bsStyle="danger">
            The server returned an error: {this.state.serverError.message}
          </Alert>
        </div>
      );
    }

    const title = <h3 className="text-center">MonkeyCAM Results</h3>;
    return (
      <Grid>
        <Row>
          <Col sm={8} smOffset={2}>
            <Panel header={title} bsStyle="primary">
              {spinnerRow}
              {resultsDesc}
              {errorDesc}
              {serverError}
              {retryDesc}
            </Panel>
          </Col>
        </Row>
        <Row>
          <Col sm={8} smOffset={2}>
            <JobExecutionDetails
              basicResults={this.state.basicResults}
              fullResults={this.state.fullResults}
            />
          </Col>
        </Row>
      </Grid>
    );
  }
}

export default JobResultsPage;
