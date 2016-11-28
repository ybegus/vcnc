import { combineReducers } from 'redux';
import workspaces from './workspacesReducer';
import settings from './settingsReducer';
import workspaceEdit from './workspaceEditReducer';

const rootReducer = combineReducers({
  workspaces,
  settings,
  workspaceEdit,
});

export default rootReducer;
