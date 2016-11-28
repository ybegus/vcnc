import * as types from './actionTypes.js';
export function workspaceEditCancel() {
  return { type: types.WORKSPACE_EDIT_CANCEL };
}
export function workspaceEditEntryCreate() {
  return {
    type: types.WORKSPACE_EDIT_ENTRY_CREATE,
    payload: { local: false, vp_path: '/', vtrq_id: 0, vtrq_path: '/' },
  };
}
export function workspaceEditEntryDelete(index) {
  // Deletes the workspace entry at 'index'.
  return { type: types.WORKSPACE_EDIT_ENTRY_DELETE, payload: index };
}

export function workspaceEditEntryLocal(index, local) {
  // Sets the entry's 'local' attribute to 'local'.
  return {
    type: types.WORKSPACE_EDIT_ENTRY_LOCAL,
    payload: { index, local },
  };
}
export function workspaceEditEntryMove(index, up) {
  // If 'up' is true, moves the entry up; otherwise, moves it down.
  return {
    type: types.WORKSPACE_EDIT_ENTRY_MOVE,
    payload: { index, up },
  };
}
export function workspaceEditEntryVpPath(index, path) {
  // Sets the entry's vp_path to 'path'.
  return {
    type: types.WORKSPACE_EDIT_ENTRY_VP_PATH,
    payload: { index, path },
  };
}
export function workspaceEditEntryVtrq(index, trqId) {
  // Sets the entry's vtrq ID to 'trqId'.
  return {
    type: types.WORKSPACE_EDIT_ENTRY_VTRQ,
    payload: { index, trqId },
  };
}
export function workspaceEditEntryVtrqPath(index, path) {
  // Sets the entry's vtrq_path to 'path'.
  return {
    type: types.WORKSPACE_EDIT_ENTRY_VTRQ_PATH,
    payload: { index, path },
  };
}
export function workspaceEditInit(name, spec, index) {
  return {
    type: types.WORKSPACE_EDIT_INIT,
    payload: { index, name, spec },
  };
}
export function workspaceEditName(name) {
  // Changes the workspace name to 'name'.
  return { type: types.WORKSPACE_EDIT_NAME, payload: name };
}
