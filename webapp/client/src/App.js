/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * This source code is licensed under the Apache license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React, { Component } from 'react';
import './App.css';
import { Route, Routes } from 'react-router-dom';

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
        <Routes>
          <Route path="/" element={<HomePage />} />
          <Route path="/about" element={<AboutPage />} />
          <Route path="/design" element={<DesignPage />} />
          <Route path="/design/:inputsid" element={<DesignPage />} />
          <Route path="/docs" element={<DocsPage />} />
          <Route path="/login" element={<LoginPage />} />
          <Route path="/jobactivity/:jobId" element={<JobExecutionPage />} />
          <Route path="/results/:jobId" element={<JobResultsPage />} />
          <Route path="/tour" element={<TourPage />} />
        </Routes>
        <AppFooter />
      </div>
    );
  }
}

export default App;
