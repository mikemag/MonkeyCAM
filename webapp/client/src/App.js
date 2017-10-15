/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import './App.css';
import { Route, Switch } from 'react-router-dom';

import AboutPage from './AboutPage';
import AppFooter from './AppFooter';
import AppHeader from './AppHeader';
import DesignPage from './DesignPage';
import DocsPage from './DocsPage';
import HomePage from './HomePage';
import LoginPage from './LoginPage';
import JobExecutionPage from './JobExecutionPage';
import JobResultsPage from './JobResultsPage';
import TourPage from './TourPage';

class App extends Component {
  render() {
    return (
      <div className="App">
        <AppHeader />
        <Switch>
          <Route exact path="/" component={HomePage} />
          <Route path="/about" component={AboutPage} />
          <Route path="/design/:inputsid?" component={DesignPage} />
          <Route path="/docs" component={DocsPage} />
          <Route path="/login" component={LoginPage} />
          <Route path="/jobactivity/:jobId" component={JobExecutionPage} />
          <Route path="/results/:jobId" component={JobResultsPage} />
          <Route path="/tour" component={TourPage} />
        </Switch>
        <AppFooter />
      </div>
    );
  }
}

export default App;
