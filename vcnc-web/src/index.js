/* eslint-disable import/default */

import React from 'react';
import { render } from 'react-dom';
import { Provider } from 'react-redux';
import { Router, browserHistory } from 'react-router';
import routes from './routes';
import configureStore from './store/configureStore';
import preloadedState from './store/preloadedState';
import { workspacesGetAll } from './actions/workspacesActions.js';
require('./favicon.ico'); // Tell webpack to load favicon.ico
// Yep, that's right. You can import SASS/CSS files too! Webpack will run
// the associated loader and plug this into the page.
// import './styles/styles.css';
//
const store = configureStore(preloadedState);
store.dispatch(workspacesGetAll());

render(
  <Provider store={store}>
    <Router history={browserHistory} routes={routes} />
  </Provider>, document.getElementById('app')
);
