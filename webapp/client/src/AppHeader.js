/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import { Navbar } from 'react-bootstrap';
import { Link } from 'react-router-dom';
import { Nav, NavItem } from 'react-bootstrap';
import { LinkContainer } from 'react-router-bootstrap';

const AppHeader = () => (
  <Navbar fixedTop fluid>
    <Navbar.Header>
      <Navbar.Brand>
        <Link to="/">MonkeyCAM</Link>
      </Navbar.Brand>
      <Navbar.Toggle />
    </Navbar.Header>
    <Navbar.Collapse>
      <Nav>
        <LinkContainer to="/design">
          <NavItem eventKey={1}>Design</NavItem>
        </LinkContainer>
        <LinkContainer to="/tour">
          <NavItem eventKey={1}>Tour</NavItem>
        </LinkContainer>
        <LinkContainer to="/docs">
          <NavItem eventKey={1}>Docs</NavItem>
        </LinkContainer>
        <LinkContainer to="/about">
          <NavItem eventKey={1}>About</NavItem>
        </LinkContainer>
      </Nav>
      <Nav pullRight>
        <LinkContainer to="/login">
          <NavItem eventKey={1}>Login</NavItem>
        </LinkContainer>
      </Nav>
    </Navbar.Collapse>
  </Navbar>
);

export default AppHeader;
