/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import { Grid, Row, Col, Panel } from 'react-bootstrap';
import { Link } from 'react-router-dom';

const title = (
  <div>
    <h3 className="text-center">MonkeyCAM</h3>
    <p className="text-center" style={{ fontVariant: 'small-caps' }}>
      ski and snowboard design in the cloud
    </p>
  </div>
);

class HomePage extends Component {
  componentDidMount() {
    window.scrollTo(0, 0);
  }

  render() {
    return (
      <Grid>
        <Row>
          <Col sm={8} smOffset={2}>
            <Panel header={title} bsStyle="primary">
              <p className="text-center">
                <a href="https://github.com/mikemag/MonkeyCAM">MonkeyCAM</a> is
                an open source CAD/CAM program for designing skis and
                snowboards, and then instantly generating all of the {' '}
                <a href="http://en.wikipedia.org/wiki/G-code">G-Code</a>{' '}
                programs needed to cut the key parts with a{' '}
                <a href="http://en.wikipedia.org/wiki/Numerical_control">
                  CNC machine
                </a>.
              </p>
              <p className="text-center">
                This site runs MonkeyCAM for you in the cloud.
              </p>
              <hr />
              <h3 className="text-center">
                <Link to="/design">Design</Link>
              </h3>
              <p className="text-center">
                Get started by providing your board design and machine
                configuration
              </p>
              <hr />
              <h3 className="text-center">
                <Link to="/tour">Tour</Link>
              </h3>
              <p className="text-center">
                See an example of what MonkeyCAM can do
              </p>
              <hr />
              <h3 className="text-center">
                <Link to="/docs">Docs</Link>
              </h3>
              <p className="text-center">
                Read up on how to use MonkeyCAM and the programs it generates
              </p>
            </Panel>
          </Col>
        </Row>
      </Grid>
    );
  }
}

export default HomePage;
