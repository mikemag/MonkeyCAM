/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import { Container, Row, Col } from 'react-bootstrap';

const AppFooter = () => (
  <footer className="footer bg-light border-top py-2">
    <Container>
    <Row>
      <Col className="text-center" xs={6} sm={4}>
        <a href="https://github.com/mikemag/MonkeyCAM/issues">Report a bug</a>
      </Col>
      <Col className="text-center" xs={6} sm={4}>
        Source on <a href="https://github.com/mikemag/MonkeyCAM">GitHub</a>
      </Col>
      <Col className="text-center" xs={12} sm={4}>
        Contact: <a href="https://github.com/mikemag/MonkeyCAM/discussions">MonkeyCAM Discussions</a>
      </Col>
    </Row>
    </Container>
  </footer>
);

export default AppFooter;
