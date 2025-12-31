/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import { Alert, Button, Card, Container, Row, Col, Table } from 'react-bootstrap';
import { CircularProgressbar } from 'react-circular-progressbar';
import Spinner from 'react-spinkit';
import TimeAgo from 'react-timeago';
import { BadJobNotice, cfURLRoot } from './CommonComponents';
import withRouter from './withRouter';

async function getJobStatus(jobId) {
  let res = await fetch(cfURLRoot + '/getJobStatus', {
    accept: 'application/json',
    method: 'POST',
    body: JSON.stringify({
      jobid: jobId
    }),
    headers: {
      'Content-Type': 'application/json'
    }
  });
  checkStatus(res);
  return res.json();
}

// @TODO: dup, factor.
function checkStatus(response) {
  if (response.status >= 200 && response.status < 300) {
    return response;
  }
  console.log('Bad response: ', response);
  const error = new Error(`HTTP Error ${response.statusText}`);
  error.status = response.statusText;
  error.response = response;
  console.log(error);
  throw error;
}

// @TODO: dup, factor.
function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

// @TODO: dup, factor.
function tsToDate(ts) {
  return ts && new Date(parseInt(ts, 10));
}

class JobStatus extends Component {
  loadingString = (
    <div className="text-info">
      <Spinner id="spinner" color="inherit" name="three-bounce" fadeIn="none" />
    </div>
  );

  constructor(props) {
    super(props);
    this.state = {
      progress: 0,
      status: this.loadingString,
      boardName: this.loadingString,
      createdTime: this.loadingString,
      startedTime: undefined,
      pollCount: 0,
      pollDelay: 500,
      donePolling: false,
      alertArea: null,
      sequentialErrorCount: 0
    };
  }

  async updateJobState() {
    let completed = false;
    let updatePollingInterval = false;
    let newState = {
      pollDelay: 1000,
      pollCount: this.state.pollCount + 1,
      donePolling: false,
      sequentialErrorCount: 0
    };
    try {
      const result = await getJobStatus(this.props.jobId);
      console.log(result);
      if (result.code) {
        console.log('Server error getting job state', result.code);
        newState.sequentialErrorCount = this.state.sequentialErrorCount + 1;
      } else {
        Object.assign(newState, {
          progress: result.progress || 0,
          status: result.state,
          createdTime: tsToDate(result.jobCreatedTime),
          startedTime: tsToDate(result.jobStartedTime),
          boardName: result.boardName
        });
        if (result.state === 'Completed' || result.state === 'Failed') {
          completed = true;
        }
      }
    } catch (err) {
      console.log('Server error getting job state', err);
      newState.sequentialErrorCount = this.state.sequentialErrorCount + 1;
    }

    let jobAge = 0;
    if (newState.createdTime instanceof Date) {
      jobAge = (new Date() - newState.createdTime) / 1000;
    }
    if (newState.sequentialErrorCount > 3) {
      newState.donePolling = true;
      updatePollingInterval = true;
      newState.alertArea = (
        <div>
          <Alert variant="danger">
            Too many server errors in a row. Please refresh the page to check on
            the status at a later time.
          </Alert>
          <BadJobNotice />
        </div>
      );
    } else if (newState.pollCount > 45 || jobAge > 120) {
      newState.donePolling = true;
      updatePollingInterval = true;
      newState.alertArea = (
        <div>
          <Alert variant="danger">
            The job is taking much longer than expected. Please refresh the page
            to check on the status at a later time.
          </Alert>
          <BadJobNotice />
        </div>
      );
    } else if (newState.pollCount > 40 || jobAge > 50) {
      newState.pollDelay = 10000;
      newState.alertArea = (
        <div>
          <Alert variant="warning">
            This is taking quite a bit longer than usual, there may be a problem
            with the servers.
          </Alert>
          <BadJobNotice />
        </div>
      );
    } else if (newState.pollCount > 30 || jobAge > 30) {
      newState.pollDelay = 2000;
      newState.alertArea = (
        <Alert variant="warning">
          This is taking longer than usual, please be patient.
        </Alert>
      );
    }

    if (newState.pollDelay !== this.state.pollDelay) {
      updatePollingInterval = true;
    }

    this.setState(newState);

    if (updatePollingInterval && !completed) {
      clearInterval(this.timerID);
      if (!newState.donePolling) {
        this.setupPollingTimer(newState.pollDelay);
      }
    }

    return completed;
  }

  async setupPollingTimer(pollDelay) {
    this.timerID = setInterval(async () => {
      const completed = await this.updateJobState();
      if (completed) {
        clearInterval(this.timerID);
        await sleep(1500); // Give the user a chance to see the job completed.
        this.props.navigate(`/results/${this.props.jobId}`, { replace: true });
      }
    }, pollDelay);
  }

  async componentDidMount() {
    window.scrollTo(0, 0);
    this.setupPollingTimer(this.state.pollDelay);
  }

  componentWillUnmount() {
    clearInterval(this.timerID);
  }

  render() {
    return (
      <div>
        <Row>
          <Col sm={4}>
            <div className="job-status-progress center-block">
              <CircularProgressbar
                value={this.state.progress | 0}
                text={`${this.state.progress | 0}%`}
                className={
                  (this.state.progress | 0) < 100 ? '' : 'complete'
                }
              />
            </div>
          </Col>
          <Col sm={8}>
            <Table striped>
              <tbody>
                <tr>
                  <td>Board</td>
                  <td>{this.state.boardName}</td>
                </tr>
                <tr>
                  <td>Job Id</td>
                  <td>{this.props.jobId}</td>
                </tr>
                {!this.state.startedTime && (
                  <tr>
                    <td>Created</td>
                    <td>
                      <TimeAgo
                        date={this.state.createdTime}
                        title={this.state.createdTime}
                      />
                    </td>
                  </tr>
                )}
                {this.state.startedTime && (
                  <tr>
                    <td>Started</td>
                    <td>
                      <TimeAgo
                        date={this.state.startedTime}
                        title={this.state.startedTime}
                      />
                    </td>
                  </tr>
                )}
                <tr>
                  <td>Status</td>
                  <td>{this.state.status}</td>
                </tr>
              </tbody>
            </Table>
          </Col>
        </Row>
        <Row>
          <Col sm={8} smOffset={2}>
            {this.state.alertArea}
          </Col>
        </Row>
      </div>
    );
  }
}

class JobExecutionPage extends Component {
  constructor(props) {
    super(props);
    this.handleCancelJob = this.handleCancelJob.bind(this);
  }

  handleCancelJob(e) {
    console.log('@TODO: impl job cancellation!');
    // @TODO: just nav back? Try to delete the message from the queue? Mark the job as cancelled?
    e.preventDefault();
  }

  render() {
    const title = <h3 className="text-center">MonkeyCAM Job Status</h3>;

    return (
      <Container>
        <Row>
          <Col sm={8} smOffset={2}>
            <Card className="mb-3" border="primary">
              <Card.Header>{title}</Card.Header>
              <Card.Body>
                <JobStatus jobId={this.props.params.jobId} navigate={this.props.navigate} />
              </Card.Body>
            </Card>
          </Col>
        </Row>
        <Row>
          <Col sm={8} smOffset={2}>
            <div className="text-end">
              <Button variant="primary" onClick={this.handleCancelJob}>
                Cancel Job
              </Button>
            </div>
          </Col>
        </Row>
      </Container>
    );
  }
}

export default withRouter(JobExecutionPage);
