//
//  Methodology inspired by https://gist.github.com/deanius/f380994d117dffb5b625a7871a4fa893
//
//  Convention 1:
//    Functions are bound to actions by convention. The Key must match
//    a WORKSPACE_EDIT_* constant from ../actions/actionTypes.js
//
//  Convention 2:
//    The action object must have a key 'payload' whose value contains all the
//    inputs needed by the reducer.
//
//  Convention 3:
//    The 'state' argument is store.workspaceEdit.
//
import { validatePath, validateNonNegativeInteger } from './validate';
import * as types from '../actions/actionTypes.js';

const unique = (() => {
  let value = 20;  // TODO until we call workspacesGetAll
  return () => ++value;
})();

const newEntryTemplate = () => ({
  local: false,
  vp_path: '/',
  vtrq_id: '0',
  vtrq_path: '/',
  reactKey: unique(),
});

const newWorkspaceTemplate = () => ({
  altered: false,
  name: '/new/name',
  spec: [newEntryTemplate()],
  index: -1,
  reactKey: unique(),
  open: true,
  valid: true,
});

export const workspaceEditIsValid = (state) => {
  const errors = state.errors;
  if (!errors) return true;
  if (Object.keys(errors).length === 0) return true;
  if (errors.spec) {
    const filtered = errors.spec.filter((e) => Object.keys(e).length !== 0);
    if (filtered.length > 0) return false;
  }
  return Object.keys(errors).length === (errors.spec ? 1 : 0);
};

const workspaceEditEntryErrorRollup = (state, index, errors) => {
  const rtn = { ...state, errors: { spec: [] } };
  if (errors && state.errors && state.errors.spec) {
    rtn.errors.spec = state.errors.spec.slice(0);
    rtn.errors.spec[index] = errors;
  }
  rtn.valid = workspaceEditIsValid(rtn);
  return rtn;
};

const workspaceEditEntryErrors = (state, index) => (
  (state.errors && state.errors.spec && state.errors.spec[index])
  ? state.errors.spec[index]
  : {}
);

const reducers = {
  [types.WORKSPACE_EDIT_CANCEL]: (state) => { // eslint-disable-line arrow-body-style
    return { ...state, open: false };
  },
  [types.WORKSPACE_EDIT_ENTRY_CREATE]: (state) => {
    const newState = { ...state, spec: [...state.spec] };
    newState.spec.push({ ...newEntryTemplate() });
    return newState;
  },
  [types.WORKSPACE_EDIT_ENTRY_DELETE]: (state, payload) => {
    const index = payload;
    const spec = state.spec;
    const newState = { ...state };
    newState.spec = spec
      .slice(0, index)
      .concat(spec.slice(index + 1));
    return newState;
  },
  [types.WORKSPACE_EDIT_ENTRY_LOCAL]: (state, payload) => {
    const index = payload.index;
    const newState = { ...state, spec: [...state.spec] };
    newState.spec[index] = { ...state.spec[index] };
    newState.spec[index].local = payload.local;
    return newState;
  },
  [types.WORKSPACE_EDIT_ENTRY_MOVE]: (state, payload) => {
    //  'up' means 'earlier in the array (lower index)'
    const { index, up } = payload;
    const down = !up;
    const spec = state.spec;
    const length = spec.length;
    if (length < 2) return state;
    if (down && index >= length - 1) return state;
    if (up && index < 1) return state;
    const newState = { ...state };
    const newSpec = [...spec];
    const removed = newSpec.splice(index, 1);
    if (up) {
      newSpec.splice(index - 1, 0, ...removed);
    } else {
      newSpec.splice(index + 1, 0, ...removed);
    }
    newState.spec = newSpec;
    return newState;
  },
  [types.WORKSPACE_EDIT_ENTRY_VP_PATH]: (state, payload) => {
    const { index, path } = payload;
    const newState = { ...state, spec: [...state.spec], errors: { ...state.errors } };
    newState.spec[index] = { ...state.spec[index] };
    newState.spec[index].vp_path = path;
    const errors = validatePath(
      path,
      'vp_path',
      workspaceEditEntryErrors(state, index)
    );
    return workspaceEditEntryErrorRollup(newState, index, errors);
  },
  [types.WORKSPACE_EDIT_ENTRY_VTRQ]: (state, payload) => {
    const { index, trqId } = payload;
    const newState = { ...state, spec: [...state.spec] };
    newState.spec[index] = { ...state.spec[index] };
    newState.spec[index].vtrq_id = trqId;
    const errors = validateNonNegativeInteger(
      trqId,
      'vtrq_id',
      workspaceEditEntryErrors(newState, index)
    );
    return workspaceEditEntryErrorRollup(newState, index, errors);
  },
  [types.WORKSPACE_EDIT_ENTRY_VTRQ_PATH]: (state, payload) => {
    const { index, path } = payload;
    const newState = { ...state, spec: [...state.spec] };
    newState.spec[index] = { ...state.spec[index] };
    newState.spec[index].vtrq_path = path;
    const errors = validatePath(
      path,
      'vtrq_path',
      workspaceEditEntryErrors(state, index)
    );
    return workspaceEditEntryErrorRollup(newState, index, errors);
  },
  [types.WORKSPACE_EDIT_INIT]: (state, payload) => {
    const { name, spec, index } = payload;
    if (index === undefined) {
      return newWorkspaceTemplate();
    }
    return { name, spec, index, open: true, originalName: name, valid: true };
  },
  [types.WORKSPACE_EDIT_NAME]: (state, payload) => {
    const newState = { ...state };
    newState.name = payload;
    newState.errors = validatePath(payload, 'name', state.errors);
    newState.valid = workspaceEditIsValid(newState);
    return newState;
  },
};

const leaveStateUnchanged = (state) => state;

export default (state = {}, action) => {
  if (state === null) return {};
  const reducer = reducers[action.type] || leaveStateUnchanged;
  return reducer(state, action.payload);
};
