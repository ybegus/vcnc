/* global it describe */
/* eslint-disable no-unused-expressions */
import { expect } from 'chai';
import deepFreeze from 'deep-freeze';
import * as actions from '../actions/workspaceEditActions.js';
import { validateNonNegativeInteger, validatePath } from './validate';
import workspaceEditReducer, { workspaceEditIsValid } from './workspaceEditReducer';

describe('non-negative-int validator', () => {
  it('should detect floats', () => {
    const errors = validateNonNegativeInteger('3.2', 'brian', { bob: 'fork' });
    expect(Object.keys(errors).length).to.equal(2);
  });
  it('should detect text', () => {
    const errors = validateNonNegativeInteger('3.2', 'brian', { bob: 'fork' });
    expect(Object.keys(errors).length).to.equal(2);
  });
  it('should recognize a correction', () => {
    const errors = validateNonNegativeInteger(
      '3',
      'brian',
      { brian: ' ', bob: 'fork' },
    );
    expect(Object.keys(errors).length).to.equal(1);
  });
});
describe('path validator', () => {
  it('should detect relative paths', () => {
    const errors = validatePath('x/y', 'molly', {});
    expect(errors).to.deep.equal({ molly: 'Must begin with /' });
  });
  it('should detect empty paths', () => {
    const errors = validatePath('', 'tom', {});
    expect(errors).to.deep.equal({ tom: 'Cannot be empty' });
  });
  it('should recognize a correction', () => {
    const errors = validatePath('/a/ok', 'sally', { sally: 'Some message' });
    expect(errors).to.deep.equal({ });
  });
});
describe('workspaceEditIsValid', () => {
  it('says empty error object is valid', () => {
    const state = deepFreeze({});
    expect(workspaceEditIsValid(state)).to.equal(true);
  });
  it('says empty spec array is valid', () => {
    const state = deepFreeze({ errors: { spec: [] } });
    expect(workspaceEditIsValid(state)).to.equal(true);
  });
  it('says spec array with empty objects is valid', () => {
    const state = deepFreeze({ errors: { spec: [{}, {}] } });
    expect(workspaceEditIsValid(state)).to.equal(true);
  });
  it('says spec array with mixed objects is invalid', () => {
    const state = deepFreeze({ errors: { spec: [{}, { cow: 'moo' }] } });
    expect(workspaceEditIsValid(state)).to.equal(false);
  });
  it('says one non-spec error is invalid', () => {
    const state = deepFreeze({ errors: { name: 'boo' } });
    expect(workspaceEditIsValid(state)).to.equal(false);
  });
});
describe('Workspace Editor Reducer', () => {
  //  Cancel
  it('should close dialog when cancelled', () => {
    const initialState = deepFreeze({ open: true });
    const action = actions.workspaceEditCancel();
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.open).to.be.false;
  });
  //  Init
  it('should be properly initialized from nothing', () => {
    const initialState = deepFreeze({});
    const action = actions.workspaceEditInit();
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.altered).to.be.false;
    expect(newState.index).to.equal(-1);
    expect(newState.open).to.be.true;
    expect(newState.valid).to.be.true;
  });
  //  Entry Create
  it('should create an additional entry', () => {
    const initialState = deepFreeze({ name: '/nathan', spec: [{ vtrq_id: '11' }] });
    const action = actions.workspaceEditEntryCreate();
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.name).to.equal('/nathan');
    expect(newState.spec.length).to.equal(2);
    expect(newState.spec[0].vtrq_id).to.equal('11');
    expect(newState.spec[1].vtrq_id).to.equal('0');
  });
  //  Entry Delete
  it('should delete an entry', () => {
    const initialState = deepFreeze({ spec: [{ local: true }] });
    const action = actions.workspaceEditEntryDelete(0);
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.spec.length).to.equal(0);
  });
  //  Entry local
  it('should set local to false', () => {
    const initialState = deepFreeze({ spec: [{ local: true }] });
    const action = actions.workspaceEditEntryLocal(0, false);
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.spec[0].local).to.be.false;
  });
  it('should set local to true', () => {
    const initialState = deepFreeze({ spec: [{ local: false }] });
    const action = actions.workspaceEditEntryLocal(0, true);
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.spec[0].local).to.be.true;
  });
  //  Entry Move
  it('does not move the last element downwards', () => {
    const initialState = deepFreeze({ spec: [{ id: 0 }, { id: 1 }, { id: 2 }] });
    const action = actions.workspaceEditEntryMove(2, false);
    const newState = workspaceEditReducer(initialState, action);
    const expectation = [{ id: 0 }, { id: 1 }, { id: 2 }];
    expect(newState.spec).to.deep.equal(expectation);
  });
  it('should move the last element upwards', () => {
    const initialState = deepFreeze({ spec: [{ id: 0 }, { id: 1 }, { id: 2 }] });
    const action = actions.workspaceEditEntryMove(2, true);
    const newState = workspaceEditReducer(initialState, action);
    const expectation = [{ id: 0 }, { id: 2 }, { id: 1 }];
    expect(newState.spec).to.deep.equal(expectation);
  });
  it('should move the first element downwards', () => {
    const initialState = deepFreeze({ spec: [{ id: 0 }, { id: 1 }, { id: 2 }] });
    const action = actions.workspaceEditEntryMove(0, false);
    const newState = workspaceEditReducer(initialState, action);
    const expectation = [{ id: 1 }, { id: 0 }, { id: 2 }];
    expect(newState.spec).to.deep.equal(expectation);
  });
  it('does not move the first element upwards', () => {
    const initialState = deepFreeze({ spec: [{ id: 0 }, { id: 1 }, { id: 2 }] });
    const action = actions.workspaceEditEntryMove(0, true);
    const newState = workspaceEditReducer(initialState, action);
    const expectation = [{ id: 0 }, { id: 1 }, { id: 2 }];
    expect(newState.spec).to.deep.equal(expectation);
  });
  //  Name
  it('should change its name', () => {
    const initialState = deepFreeze({ name: '/x/y', spec: [] });
    const action = actions.workspaceEditName('/z/z');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.name).to.equal('/z/z');
  });
  it('should trap a relative name', () => {
    const initialState = deepFreeze({ name: '/x/y', spec: [] });
    const action = actions.workspaceEditName('x/y');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.valid).to.be.false;
    expect(typeof newState.name).to.equal('string');
  });
  it('should trap an empty name', () => {
    const initialState = deepFreeze({ name: '/x/y', spec: [] });
    const action = actions.workspaceEditName('');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.valid).to.be.false;
    expect(typeof newState.name).to.equal('string');
  });
  it('should recognize a corrected name', () => {
    const initialState = deepFreeze({
      errors: { name: 'Cannot be empty' },
      name: '',
      spec: [],
    });
    const action = actions.workspaceEditName('/valid/name');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.valid).to.be.true;
  });
  //  Entry vtrq ID
  it('should set vtrq id', () => {
    const initialState = deepFreeze({ spec: [{ vtrq_id: '11' }] });
    const action = actions.workspaceEditEntryVtrq(0, '111');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.spec[0].vtrq_id).to.equal('111');
    expect(initialState.spec[0].vtrq_id).to.equal('11');
  });
  //  Entry VP Path
  it('should set VP Path', () => {
    const initialState = deepFreeze({ spec: [{ vp_path: '/a/b' }] });
    const action = actions.workspaceEditEntryVpPath(0, '/y/z');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.spec[0].vp_path).to.equal('/y/z');
  });
  //  Entry vTRQ Path
  it('should set vTRQ Path', () => {
    const initialState = deepFreeze({ spec: [{ vtrq_path: '/a/b' }] });
    const action = actions.workspaceEditEntryVtrqPath(0, '/y/z');
    const newState = workspaceEditReducer(initialState, action);
    expect(newState.spec[0].vtrq_path).to.equal('/y/z');
  });
});
