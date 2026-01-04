/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import { Container, Nav, Navbar } from 'react-bootstrap';
import { NavLink } from 'react-router-dom';

const AppHeader = () => (
  <Navbar bg="light" expand="lg" fixed="top" className="shadow-sm">
    <Container fluid>
      <Navbar.Brand as={NavLink} to="/">
        MonkeyCAM
      </Navbar.Brand>
      <Navbar.Toggle aria-controls="monkeycam-navbar" />
      <Navbar.Collapse id="monkeycam-navbar">
        <Nav className="me-auto">
          <Nav.Link as={NavLink} to="/design">
            Design
          </Nav.Link>
          <Nav.Link as={NavLink} to="/tour">
            Tour
          </Nav.Link>
          <Nav.Link as={NavLink} to="/docs">
            Docs
          </Nav.Link>
          <Nav.Link as={NavLink} to="/about">
            About
          </Nav.Link>
        </Nav>
        {/*<Nav>*/}
        {/*  <Nav.Link as={NavLink} to="/login">*/}
        {/*    Login*/}
        {/*  </Nav.Link>*/}
        {/*</Nav>*/}
      </Navbar.Collapse>
    </Container>
  </Navbar>
);

export default AppHeader;
