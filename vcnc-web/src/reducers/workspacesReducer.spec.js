/* global it describe */
/* eslint-disable no-unused-expressions */
import { expect } from 'chai';
import deepFreeze from 'deep-freeze';
import * as actions from '../actions/workspacesActions.js';
import workspacesReducer from './workspacesReducer';

const makeWorkspace = name => ({ name, spec: [] });

describe('Workspaces Reducer', () => {
  //  Create
  //  Delete
  it('should delete an existing workspace', () => {
    const initialState = deepFreeze([makeWorkspace('/billybob')]);
    const action = actions.workspacesDeleteSuccess(0);
    const newState = workspacesReducer(initialState, action);
    expect(newState.length).to.equal(0);
  });
  //  Get All
  it('should replace all existing workspaces', () => {
    const initialState = deepFreeze([
      makeWorkspace('/sally'),
      makeWorkspace('/harry'),
    ]);
    const action = actions.workspacesGetAllSuccess([makeWorkspace('/molly')]);
    const newState = workspacesReducer(initialState, action);
    expect(newState.length).to.equal(1);
    expect(newState[0].name).to.equal('/molly');
  });
  //  Replace
  it('should replace an existing workspace', () => {
    const initialState = deepFreeze([makeWorkspace('/suzie')]);
    const action = actions.workspacesReplaceSuccess(0, makeWorkspace('/molly'));
    const newState = workspacesReducer(initialState, action);
    expect(newState[0].name).to.equal('/molly');
  });
  it('should append a new workspace', () => {
    const initialState = deepFreeze([makeWorkspace('/tiger')]);
    const action = actions.workspacesReplaceSuccess(-1, makeWorkspace('/tigger'));
    const newState = workspacesReducer(initialState, action);
    expect(newState[1].name).to.equal('/tigger');
  });
});
