import React from 'react';
import { Route, IndexRoute } from 'react-router';
import Master from './components/Master';
import WorkspacesPage from './components/pages/workspaces/Page';

export default (
  <Route path="/" component={Master}>
    <IndexRoute component={WorkspacesPage} />
    <Route path="workspaces" component={WorkspacesPage} />
  </Route>
);
