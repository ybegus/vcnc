import * as types from './actionTypes.js';
import WorkspaceApi from '../api/workspaceApi.js';

const workspaceApi = new WorkspaceApi('cnc', 6130, 0);

export function workspacesDeleteRequest() {
  return { type: types.WORKSPACES_DELETE_REQUEST };
}
export function workspacesDeleteSuccess(index) {
  return { type: types.WORKSPACES_DELETE_SUCCESS, payload: index };
}
export function workspacesDeleteFailure(error) {
  return { type: types.WORKSPACES_DELETE_FAILURE, payload: error };
}
export function workspacesDelete(index, name) {
  return dispatch => {
    dispatch(workspacesDeleteRequest());
    return workspaceApi.deleteWorkspace(name)
    .then(
      () => dispatch(workspacesDeleteSuccess(index)),
      error => dispatch(workspacesDeleteFailure(error))
    );
  };
}

export function workspacesGetAllRequest() {
  return { type: types.WORKSPACES_GET_ALL_REQUEST };
}
export function workspacesGetAllSuccess(workspaces) {
  return { type: types.WORKSPACES_GET_ALL_SUCCESS, payload: workspaces };
}
export function workspacesGetAllFailure(error) {
  return { type: types.WORKSPACES_GET_ALL_FAILURE, payload: error };
}
export function workspacesGetAll() {
  return dispatch => {
    dispatch(workspacesGetAllRequest());
    return workspaceApi.getAllWorkspaces()
    .then(
      workspaces => dispatch(workspacesGetAllSuccess(workspaces)),
      error => dispatch(workspacesGetAllFailure(error))
    );
  };
}

export function workspacesReplaceRequest() {
  return { type: types.WORKSPACES_REPLACE_REQUEST };
}
export function workspacesReplaceSuccess(index, workspace) {
  return { type: types.WORKSPACES_REPLACE_SUCCESS, payload: { index, workspace } };
}
export function workspacesReplaceFailure(error) {
  return { type: types.WORKSPACES_REPLACE_FAILURE, payload: error };
}
export function workspacesReplace(index, workspace) {
  const forRestApi = {
    ...workspace,
    spec: workspace.spec.map(entry => ({
      ...entry,
      vtrq_id: parseInt(entry.vtrq_id, 10),
      reactKey: undefined,
    })),
  };
  return dispatch => {
    dispatch(workspacesReplaceRequest());
    if (!workspace.originalName || workspace.originalName === workspace.name) {
      return workspaceApi.setWorkspace(forRestApi.name, forRestApi.spec)
      .then(
        () => dispatch(workspacesReplaceSuccess(index, workspace)),
        error => dispatch(workspacesReplaceFailure(error))
      );
    }
    return workspaceApi.deleteWorkspace(forRestApi.name)
    .then(
      () => workspaceApi.setWorkspace(forRestApi.name, forRestApi.spec)
    )
    .then(
      () => dispatch(workspacesReplaceSuccess(index, workspace)),
    )
    .catch(
      error => dispatch(workspacesReplaceFailure(error))
    );
  };
}
