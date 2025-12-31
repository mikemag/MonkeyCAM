/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import { Card, Container, Row, Col } from 'react-bootstrap';

import example_input_png from './media/Example_Input.png';
import example_results_png from './media/Example_Results.png';
import example_overview_svg from './media/Example_Overview.svg';
import example_core_jpg from './media/Example_Core.jpg';

class TourPage extends Component {
  componentDidMount() {
    window.scrollTo(0, 0);
  }

  render() {
    return (
      <Container>
        <Row>
          <Col sm={12}>
            <Card className="mb-3" border="primary">
              <Card.Header>How does it work?</Card.Header>
              <Card.Body>
              <Row>
                <Col sm={12}>
                  <h3 className="text-center">
                    Spend time building boards, not fiddling with CAD/CAM
                  </h3>
                  <br />
                </Col>
              </Row>
              <Row>
                <Col sm={4}>
                  <Card className="mb-3">
                    <Card.Body>
                    <h4>1. Design</h4>
                    Enter your board design, binding layout, and CNC
                    machine/materials config, then hit Run MonkeyCAM.
                    </Card.Body>
                  </Card>
                </Col>
                <Col sm={4}>
                  <Card className="mb-3">
                    <Card.Body>
                    <h4>2. Download</h4>
                    Confirm your design with the Results Overview, then download
                    a .zip of all the G-Code files.
                    </Card.Body>
                  </Card>
                </Col>
                <Col sm={4}>
                  <Card className="mb-3">
                    <Card.Body>
                    <h4>3. Build!</h4>
                    Cut the core, base, and nose/tail fill with your CNC machine
                    and get building!
                    </Card.Body>
                  </Card>
                </Col>
              </Row>
              <Row>
                <Col sm={8} smOffset={2}>
                  <br />
                  <p className="text-center">
                    MonkeyCAM creates nine G-Code programs to cut all parts of a
                    ski or snowboard on your CNC machine. For a real example of
                    the results, see{' '}
                    <span style={{ wordWrap: 'break-word' }}>
                      <a href="https://monkeycam.org/results/5744863563743232">
                        https://monkeycam.org/results/5744863563743232
                      </a>
                    </span>
                  </p>
                  <br />
                </Col>
              </Row>
              </Card.Body>
            </Card>
          </Col>
        </Row>
        <Row>
          <Col sm={10} smOffset={1}>
            <Card className="mb-3">
              <Card.Body>
              <img
                style={{ maxWidth: '100%' }}
                src={example_input_png}
                alt="Example MonkeyCAM Input"
              />
              <h3>Design</h3>
              <p>
                Enter your board design, binding layout, and machine/materials
                configuration in JSON format. Start with a sample ski,
                snowboard, or splitboard to make it easier.
              </p>
              <p>
                When you're ready, hit Run MonkeyCAM. The results will be ready
                in seconds.
              </p>
              </Card.Body>
            </Card>
          </Col>
        </Row>
        <Row>
          <Col sm={10} smOffset={1}>
            <Card className="mb-3">
              <Card.Body>
                <div className="text-center">
                  <img
                    style={{ maxWidth: '100%' }}
                    src={example_results_png}
                    alt="Example MonkeyCAM Results"
                  />
                </div>
              <img
                width={'100%'}
                src={example_overview_svg}
                alt="Example MonkeyCAM Overview"
              />
              <h3>Download</h3>
              <p>
                Check your output in the Results Overview, which shows the
                overall shape of the board and core, and a 2D visualization of
                every G-Code program. If you need to make adjustments, hit
                'Change Design and Run Again' and tweak your design.
              </p>
              <p>
                When you're happy with the results, download a .zip of all of
                the G-Code files. These are kept in permanent storage at Google,
                so you can always go back and get them again.
              </p>
              </Card.Body>
            </Card>
          </Col>
        </Row>
        <Row>
          <Col sm={10} smOffset={1}>
            <Card className="mb-3">
              <Card.Img src={example_core_jpg} alt="Example core build" />
              <Card.Body>
                <h3>Build!</h3>
                <p>
                  You get G-Code programs to cut the base, nose & tail spacers,
                  and the core. Cut the parts with your CNC machine, and they'll
                  all fit together perfectly!
                </p>
                <p>
                  You'll be able to spend your time building boards and riding
                  them.
                </p>
              </Card.Body>
            </Card>
          </Col>
        </Row>
      </Container>
    );
  }
}

export default TourPage;
