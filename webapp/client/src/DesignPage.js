/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import Spinner from 'react-spinkit';

import {
  Button,
  Card,
  Container,
  Row,
  Col,
  Alert,
  Form
} from 'react-bootstrap';

import { UnControlled as CodeMirror } from 'react-codemirror2';
import 'codemirror/lib/codemirror.css';
import 'codemirror/addon/lint/lint.css';
import 'codemirror/addon/hint/show-hint.css';
import 'codemirror/mode/javascript/javascript.js';
import 'codemirror/addon/lint/javascript-lint';
import 'codemirror/addon/lint/lint.js';
import 'codemirror/addon/hint/javascript-hint';
import { JSHINT } from 'jshint';
window.JSHINT = JSHINT;

import { cfURLRoot } from './CommonComponents';
import kofi_button_blue_png from './media/kofi_button_blue.png';
import withRouter from './withRouter';


async function addJob(boardConfig, bindingConfig, machineConfig, bindingDist) {
  const res = await fetch(cfURLRoot + '/addJob', {
    accept: 'application/json',
    method: 'POST',
    body: JSON.stringify({
      boardConfig: boardConfig,
      bindingConfig: bindingConfig,
      machineConfig: machineConfig,
      bindingDist: bindingDist
    }),
    headers: {
      'Content-Type': 'application/json'
    }
  });
  checkStatus(res);
  return res.json();
}

async function loadInputs(inputsid) {
  let res = await fetch(cfURLRoot + '/getJobInputs', {
    accept: 'application/json',
    method: 'POST',
    body: JSON.stringify({
      jobInputsId: inputsid
    }),
    headers: {
      'Content-Type': 'application/json'
    }
  });
  checkStatus(res);
  return res.json();
}

function checkStatus(response) {
  if (response.status >= 200 && response.status < 300) {
    return response;
  }
  console.log('Bad response: ', response);
  const error = new Error(
    `HTTP Error ${response.status}: ${response.statusText}`
  );
  error.status = response.statusText;
  error.response = response;
  console.log(error);
  throw error;
}

const sampleSnowboardConfig = {
  board: {
    name: 'Sample-snowboard',
    'nose length': 18,
    'effective edge length': 136,
    'tail length': 14,
    'sidecut radius': 1100,
    'waist width': 24.6,
    taper: 0.4,
    'nose shape': {
      type: 'Basic Bezier',
      'end handle': 0.5,
      'transition handle': 0.75
    },
    'edge shape': {
      type: 'Basic Arc'
    },
    'tail shape': {
      type: 'Basic Bezier',
      'end handle': 0.5,
      'transition handle': 0.75
    },
    'reference stance width': 52,
    'stance setback': 2,
    'nose and tail spacer width': 2
  },
  profile: {
    'nose thickness': 0.2,
    'center thickness': 0.79,
    'tail thickness': 0.2,
    'nose taper': {
      'taper start': 0.45,
      'start handle': 0.7,
      'end handle': 0.8,
      'taper end': 1.0
    },
    'tail taper': {
      'taper start': 0.51,
      'start handle': 0.68,
      'end handle': 0.85,
      'taper end': 1.0
    }
  }
};

const sampleSkiConfig = {
  board: {
    name: 'Sample-ski',
    'nose length': 11,
    'effective edge length': 161,
    'tail length': 6.5,
    'sidecut radius': 2250,
    'waist width': 9.3,
    taper: 1.4,
    'nose shape': {
      type: 'Basic Bezier',
      'end handle': 0.5,
      'transition handle': 0.75
    },
    'edge shape': {
      type: 'Basic Arc'
    },
    'tail shape': {
      type: 'Flat',
      'flat width': 8.0,
      'end handle': 0.5,
      'transition handle': 0.75
    },
    'stance setback': 6,
    'nose and tail spacer width': 2,
    'tail edge extension': 2
  },
  profile: {
    'nose thickness': 0.2,
    'center thickness': 0.79,
    'tail thickness': 0.2,
    'nose taper': {
      'taper start': 0.45,
      'start handle': 0.7,
      'end handle': 0.8,
      'taper end': 1.0
    },
    'tail taper': {
      'taper start': 0.51,
      'start handle': 0.68,
      'end handle': 0.85,
      'taper end': 1.0
    }
  }
};

const sampleSplitboardConfig = {
  board: {
    name: 'Sample-splitboard',
    splitboard: true,
    'nose length': 18,
    'effective edge length': 136,
    'tail length': 14,
    'sidecut radius': 1100,
    'waist width': 24.6,
    taper: 0.4,
    'nose shape': {
      type: 'Basic Bezier',
      'end handle': 0.5,
      'transition handle': 0.75
    },
    'edge shape': {
      type: 'Basic Arc'
    },
    'tail shape': {
      type: 'Basic Bezier',
      'end handle': 0.5,
      'transition handle': 0.75
    },
    'reference stance width': 52,
    'stance setback': 2,
    'nose and tail spacer width': 2
  },
  profile: {
    'nose thickness': 0.2,
    'center thickness': 0.79,
    'tail thickness': 0.2,
    'nose taper': {
      'taper start': 0.45,
      'start handle': 0.7,
      'end handle': 0.8,
      'taper end': 1.0
    },
    'tail taper': {
      'taper start': 0.51,
      'start handle': 0.68,
      'end handle': 0.85,
      'taper end': 1.0
    }
  }
};

const sampleSnowboardBindingConfig = {
  binding: {
    'nose insert pack': {
      'count nose': 1,
      'count tail': 1,
      offset: 4,
      'horizontal spacing': 4,
      'vertical spacing': 4
    },
    'tail insert pack': {
      'count nose': 1,
      'count tail': 1,
      offset: 4,
      'horizontal spacing': 4,
      'vertical spacing': 4
    }
  }
};

const sampleSkiBindingConfig = {
  binding: {
    'binding name': 'G3-Ion',
    toe: [
      { x: -2.5, y: 2.0 },
      { x: -2.5, y: -2.0 },
      { x: 2.0, y: -2.0 },
      { x: 2.0, y: 2.0 }
    ],
    heel: [
      { x: -1.5, y: 1.8 },
      { x: -1.5, y: -1.8 },
      { x: 3.75, y: -1.8 },
      { x: 3.75, y: 1.8 }
    ]
  }
};

const sampleSplitboardBindingConfig = {
  binding: {
    'binding name': 'Standard Voile Splitboard Mounting',
    source:
      'http://www.voile.com/voile_splitboard_hole_pattern_specs.pdf, retrieved 2018/03/05',
    nose: [
      { x: 6.35, y: -4.28625 },
      { x: 6.35, y: 4.28625 },
      { x: 3.81, y: -4.28625 },
      { x: 3.81, y: 4.28625 },
      { x: 1.27, y: -4.28625 },
      { x: 1.27, y: 4.28625 },
      { x: -1.27, y: -4.28625 },
      { x: -1.27, y: 4.28625 },
      { x: -3.81, y: -4.28625 },
      { x: -3.81, y: 4.28625 },
      { x: -6.35, y: -4.28625 },
      { x: -6.35, y: 4.28625 }
    ],
    center: [
      { x: -2.8575, y: 0.0 },
      { x: 0.0, y: -3.65125 },
      { x: 0.0, y: 3.65125 },

      { x: 15.9385, y: 0.0 },
      { x: 18.6385, y: 0.0 }
    ],
    tail: [
      { x: 6.35, y: -4.28625 },
      { x: 6.35, y: 4.28625 },
      { x: 3.81, y: -4.28625 },
      { x: 3.81, y: 4.28625 },
      { x: 1.27, y: -4.28625 },
      { x: 1.27, y: 4.28625 },
      { x: -1.27, y: -4.28625 },
      { x: -1.27, y: 4.28625 },
      { x: -3.81, y: -4.28625 },
      { x: -3.81, y: 4.28625 },
      { x: -6.35, y: -4.28625 },
      { x: -6.35, y: 4.28625 }
    ]
  }
};

const sampleMachineConfig = {
  machine: {
    'rapid speed': 100,
    'normal speed': 80,

    'bottom rapid height': 0.2,
    'top rapid height': 0.6,

    'core blank thickness': 0.42,

    'base cutout tool': 2,
    'base rapid height': 0.25,
    'base cut thru height': -0.01,

    'guide hole tool': 1,
    'guide hole depth': -0.6,
    'guide hole diameter': 0.503,
    'guide hole offset': 0.5,

    'alignment mark tool': 3,
    'alignment mark offset': 0.5,
    'alignment mark depth': -0.02,
    'alignment mark deep depth': -0.15,

    'edge groove tool': 1,
    'edge groove depth': -0.03,
    'edge groove edge width': 0.354,
    'edge groove overlap percentage': 1.0,

    'insert tool': 1,
    'insert rim depth': -0.045,
    'insert rim diameter': 0.75,
    'insert hole diameter': 0.373,

    'top profile tool': 4,
    'top profile transition speed': 20,
    'top profile deep speed': 50,
    'top profile overlap percentage': 0.5,
    'top profile lead-in length': 4.0,
    'top profile roughing': false,
    'top profile roughing max cut depth': 0.2,
    'top profile roughing offset': 0.02,
    'top profile roughing fuzz': 0.02,

    'core cutout tool': 1,
    'core cutout passes': 2,

    'spacer end overhang': 1,
    'spacer side overhang': 0.5,

    'sidewall overhang': 0.118,

    'edge trench width': 1.2894,
    'edge trench extension': 3,

    'splitboard center gap': 0.1,
    'splitboard center trench width': 0.5,

    tools: [
      {
        id: 1,
        name: 'Quarter Inch Upcut Spiral',
        diameter: 0.247,
        'G-code #': 1
      },
      {
        id: 2,
        name: 'Quarter Inch Straight',
        diameter: 0.25,
        'G-code #': 1
      },
      {
        id: 3,
        name: 'Quarter Inch V',
        diameter: 0.25,
        'G-code #': 1
      },
      {
        id: 4,
        name: '1.5 Inch Straight',
        diameter: 1.496,
        'G-code #': 1
      }
    ]
  }
};

class JSONEditor extends Component {
  constructor(props) {
    super(props);
    this.state = {
      code: (props.jsonDoc && JSON.stringify(props.jsonDoc, null, 2)) || ''
    };
    this.editor = null;
    this.updateCode = this.updateCode.bind(this);
    this.handleEditorDidMount = this.handleEditorDidMount.bind(this);
  }

  updateCode(newCode) {
    this.setState({ code: newCode });
  }

  componentDidUpdate(prevProps) {
    if (this.props.jsonDoc !== prevProps.jsonDoc) {
      const c =
        (this.props.jsonDoc && JSON.stringify(this.props.jsonDoc, null, 2)) ||
        '';
      console.log('JSONDoc setting state');
      this.setState({
        code: c
      });

      if (this.editor) {
        this.editor.setValue(c);
      }
    }
  }

  handleEditorDidMount(editor) {
    this.editor = editor;
  }

  getEditorContents() {
    return this.editor ? this.editor.getValue() : this.state.code;
  }

  render() {
    const options = {
      lineNumbers: true,
      theme: 'eclipse',
      mode: 'application/json',
      tabSize: 2,
      matchBrackets: true,
      autoCloseBrackets: true,
      gutters: ['CodeMirror-lint-markers'],
      lint: true,
      styleActiveLine: true
    };
    return (
      <div className="json-editor-panel">
        <CodeMirror
          value={this.state.code}
          editorDidMount={this.handleEditorDidMount}
          onChange={(editor, data, value) => {}}
          options={options}
        />
      </div>
    );
  }
}

class DesignPage extends Component {
  constructor(props) {
    super(props);
    this.state = {
      boardConfig: '',
      bindingConfig: '',
      bindingDist: '',
      machineConfig: '',
      configErrors: '',
      isLaunching: false,
      isLoadingInputs: false,
      loadingInputsErrors: ''
    };
    this.handleRunMonkeyCAM = this.handleRunMonkeyCAM.bind(this);
    this.handleBindingDistChange = this.handleBindingDistChange.bind(this);
    this.handleBindingDistSubmit = this.handleBindingDistSubmit.bind(this);
    this.handleLoadSampleSnowboard = this.handleLoadSampleSnowboard.bind(this);
    this.handleLoadSampleSki = this.handleLoadSampleSki.bind(this);
    this.handleLoadSampleSplitboard = this.handleLoadSampleSplitboard.bind(
      this
    );
    this.boardEditorRef = React.createRef();
    this.bindingEditorRef = React.createRef();
    this.machineEditorRef = React.createRef();
  }

  handleRunMonkeyCAM = e => {
    try {
      this.setState({ isLaunching: true, configErrors: '' });
      // Good hint on getting lint state out of CodeMirror:
      // https://stackoverflow.com/questions/41534824/codemirror-get-linting-result-from-outside-of-the-editor
      const bc = JSON.parse(this.boardEditorRef.current.getEditorContents());
      const bic = JSON.parse(this.bindingEditorRef.current.getEditorContents());
      const mc = JSON.parse(this.machineEditorRef.current.getEditorContents());
      const bd = this.state.bindingDist;
      addJob(bc, bic, mc, bd)
        .then(result => {
          console.log('Result=', result);
          this.props.navigate(`/jobactivity/${result.jobId}`);
        })
        .catch(e => {
          this.setState({
            isLaunching: false,
            configErrors: 'The server returned an error: ' + e
          });
        });
    } catch (e) {
      this.setState({
        isLaunching: false,
        configErrors:
          'One or more configs are bad, please check for errors in each editor. ' +
          e
      });
    }
  };

  handleLoadSampleSnowboard(e) {
    this.setState({
      boardConfig: sampleSnowboardConfig,
      bindingConfig: sampleSnowboardBindingConfig,
      machineConfig: sampleMachineConfig,
      bindingDist: ''
    });
    e.preventDefault();
  }

  handleLoadSampleSki(e) {
    this.setState({
      boardConfig: sampleSkiConfig,
      bindingConfig: sampleSkiBindingConfig,
      machineConfig: sampleMachineConfig,
      bindingDist: '30'
    });
    e.preventDefault();
  }

  handleLoadSampleSplitboard(e) {
    this.setState({
      boardConfig: sampleSplitboardConfig,
      bindingConfig: sampleSplitboardBindingConfig,
      machineConfig: sampleMachineConfig,
      bindingDist: ''
    });
    e.preventDefault();
  }

  getCMValidationState() {
    if (!this.state.bindingDist) return null;
    const f = parseFloat(this.state.bindingDist);
    if (f > 0) return 'success';
    else return 'error';
  }

  handleBindingDistChange(e) {
    this.setState({ bindingDist: e.target.value });
  }

  handleBindingDistSubmit(e) {
    e.preventDefault();
  }

  componentDidMount() {
    window.scrollTo(0, 0);

    const { inputsid } = this.props.params;
    if (inputsid) {
      this.setState({ isLoadingInputs: true, loadingInputsErrors: '' });
      loadInputs(inputsid)
        .then(result => {
          this.setState({
            isLoadingInputs: false,
            boardConfig: result.boardConfig,
            bindingConfig: result.bindingConfig,
            machineConfig: result.machineConfig,
            bindingDist: result.bindingDist !== 0 ? result.bindingDist : ''
          });
        })
        .catch(e => {
          this.setState({
            isLoadingInputs: false,
            loadingInputsErrors:
              `Failed to fetch configs from storage for id ${inputsid}:` + e
          });
        });
    }
  }

  render() {
    const boardPanelHeader = <h3>Step 1: Board design</h3>;
    const bindingPanelHeader = <h3>Step 2: Binding pattern</h3>;
    const machinePanelHeader = <h3>Step 3: CNC machine configuration</h3>;
    const goPanelHeader = <h3>Step 4: Go!</h3>;
    let invalidConfig = null;
    if (this.state.configErrors) {
      invalidConfig = (
        <Alert variant="danger">{this.state.configErrors}</Alert>
      );
    }

    if (this.state.isLoadingInputs) {
      return (
        <Container>
          <Row>
            <Col sm={{ span: 10, offset: 1 }}>
              <div className="text-info text-center">
                <p>Loading existing inputs</p>
                <Spinner
                  id="spinner"
                  color="inherit"
                  name="three-bounce"
                  fadeIn="none"
                />
              </div>
            </Col>
          </Row>
        </Container>
      );
    }

    const bindingDistValidation = this.getCMValidationState();
    const isBindingDistValid = bindingDistValidation === 'success';
    const isBindingDistInvalid = bindingDistValidation === 'error';

    return (
      <Container>
        <Row>
          <Col sm={{ span: 8, offset: 2 }}>
            <div>
              <p>
                Enter your board, binding, and machine definitions here in JSON
                format. See the{' '}
                <a href="https://github.com/mikemag/MonkeyCAM/blob/master/engine/docs/Configuration_Guide.md">
                  MonkeyCAM v4.0 Configuration Guide
                </a>{' '}
                for a detailed explanation of all of the parameters.
              </p>
              <div className="text-center">
                <Button
                  variant="primary"
                  style={{ margin: 5 }}
                  onClick={this.handleLoadSampleSnowboard}
                >
                  Start with a sample snowboard
                </Button>
                <Button
                  variant="primary"
                  style={{ margin: 5 }}
                  onClick={this.handleLoadSampleSki}
                >
                  Start with a sample ski
                </Button>
                <Button
                  variant="primary"
                  style={{ margin: 5 }}
                  onClick={this.handleLoadSampleSplitboard}
                >
                  Start with a sample splitboard
                </Button>
              </div>
            </div>
            <hr />
          </Col>
        </Row>
        <Row>
          <Col sm={{ span: 8, offset: 2 }}>
            <div className="text-center">
              <p>You can help pay for server costs with a small donation.</p>
              <a
                href="https://ko-fi.com/mikemag"
                target="_blank"
                rel="noreferrer"
              >
                <img
                  style={{ maxWidth: '200px' }}
                  src={kofi_button_blue_png}
                  alt="Support me on Ko-fi"
                />
              </a>
            </div>
            <hr />
          </Col>
        </Row>
        <Row>
          <Col sm={6}>
            <Card className="mb-3 mc-panel">
              <Card.Header>{boardPanelHeader}</Card.Header>
              <Card.Body>
                <JSONEditor
                  ref={this.boardEditorRef}
                  jsonDoc={this.state.boardConfig}
                />
              </Card.Body>
            </Card>
          </Col>
          <Col sm={6}>
            <Card className="mb-3 mc-panel">
              <Card.Header>{bindingPanelHeader}</Card.Header>
              <Card.Body>
                <JSONEditor
                  ref={this.bindingEditorRef}
                  jsonDoc={this.state.bindingConfig}
                />
                <br/>
                <Form onSubmit={this.handleBindingDistSubmit}>
                  <Form.Group controlId="bindingDistance">
                    <Form.Label column={1}><b>Binding distance, for skis</b></Form.Label>
                    <Form.Control
                      type="text"
                      value={this.state.bindingDist}
                      placeholder="Enter binding distance in cm"
                      onChange={this.handleBindingDistChange}
                      isValid={isBindingDistValid}
                      isInvalid={isBindingDistInvalid}
                    />
                    <Form.Control.Feedback type="invalid">
                      Binding distance must be a positive number.
                    </Form.Control.Feedback>
                  </Form.Group>
                </Form>
              </Card.Body>
            </Card>
          </Col>
        </Row>
        <Row>
          <Col sm={6}>
            <Card className="mb-3 mc-panel">
              <Card.Header>{machinePanelHeader}</Card.Header>
              <Card.Body>
                <JSONEditor
                  ref={this.machineEditorRef}
                  jsonDoc={this.state.machineConfig}
                />
              </Card.Body>
            </Card>
          </Col>
          <Col sm={6}>
            <Card className="mb-3 mc-panel">
              <Card.Header>{goPanelHeader}</Card.Header>
              <Card.Body>
                <Row>
                  <Col sm={12}>
                    <div className="run-monkeycam">
                      <Button
                        variant="primary"
                        disabled={this.state.isLaunching}
                        onClick={this.handleRunMonkeyCAM}
                      >
                        Run MonkeyCAM
                      </Button>
                      {this.state.isLaunching && (
                        <div className="text-info">
                          <Spinner
                            id="spinner"
                            color="inherit"
                            name="three-bounce"
                            fadeIn="none"
                          />
                        </div>
                      )}
                    </div>
                  </Col>
                </Row>
                <Row>
                  <Col sm={12}>{invalidConfig}</Col>
                </Row>
              </Card.Body>
            </Card>
          </Col>
        </Row>
      </Container>
    );
  }
}

export default withRouter(DesignPage);
