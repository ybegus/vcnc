//  See workspaceEditReducer.js for an explanation of how this works.
import * as types from '../actions/actionTypes.js';

const unique = (() => {
  let value = 20;  // TODO until we call workspacesGetAll
  return () => ++value;
})();

const reactifyWorkspace = workspace => ({
  ...workspace,
  reactKey: unique(),
  spec: workspace.spec.map(e => ({ ...e, reactKey: unique() })),
});

const reducers = {
  [types.WORKSPACES_DELETE_SUCCESS]: (state, payload) => {
    const index = payload;
    const copy = state.slice(0);
    copy.splice(index, 1);
    return copy;
  },
  [types.WORKSPACES_GET_ALL_SUCCESS]: (state, payload) =>
    payload.sort().map(reactifyWorkspace),
  [types.WORKSPACES_REPLACE_SUCCESS]: (state, payload) => {
    const { index, workspace } = payload;
    if (index < 0) {
      return [...state, reactifyWorkspace(workspace)];
    }
    const copy = [...state];
    copy[index] = reactifyWorkspace(workspace);
    return copy;
  },
};

const leaveStateUnchanged = (state) => state;

export default (state = [], action) => {
  if (state === null) return {};
  const reducer = reducers[action.type] || leaveStateUnchanged;
  return reducer(state, action.payload);
};
