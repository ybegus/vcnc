/* global it describe */
/* eslint-disable no-unused-expressions */
import { expect } from 'chai';
import WorkspaceApi from './workspaceApi';

describe('Workspace API', () => {
  const workspaceSpec = [{ vp_path: '/', vtrq_id: 1, vtrq_path: '/', local: false }];
  const workspaceApi = new WorkspaceApi('cnc', 5502, 2, '/vcnc-test');

  it('should retrieve all the workspace specs', () =>
    workspaceApi.getAllWorkspaces()
    .then(data => expect(data).to.deep.equal([]))
  );

  it('should set a workspace', () =>
    workspaceApi.setWorkspace('/woozy', workspaceSpec)
    .then(() => true)
  );

  it('should get a workspace', () =>
    workspaceApi.getWorkspace('/woozy')
    .then(data => {
      expect(data.name).to.equal('/woozy');
      expect(data.spec).to.deep.equal(workspaceSpec);
    })
  );

  it('should see the new workspace', () =>
    workspaceApi.getAllWorkspaces()
    .then(data => {
      expect(data.length).to.equal(1);
      expect(data[0].name).to.equal('/woozy');
      expect(data[0].spec).to.deep.equal(workspaceSpec);
    })
  );

  it('should delete a workspace', () =>
    workspaceApi.deleteWorkspace('/woozy')
    .then(data => expect(data).to.deep.equal({
      error_sym: 'OK',
      message: 'Processed.',
    }))
  );
  /*
  it('should notice the vcnc host is down', () => {
    const api = new WorkspaceApi('vcnc', 5501, 1);
    return api.getAllWorkspaces()
    .then(
      () => { expect(true).to.equal(false); },
      error => {
        expect(JSON.parse(error.message)).to.deep.equal({
          error_sym: 'ENOTFOUND',
          message: 'vcnc server not found',
        });
      });
  });

  /*
  it('should notice the vtrq is down', () => {
    const api = new WorkspaceApi('cnc', 5501, 100);
    return api.getAllWorkspaces()
    .then(
      () => { expect(true).to.equal(false); },
      error => {
        expect(JSON.parse(error.message)).to.deep.equal({
          error_sym: 'EHOSTDOWN',
          message: 'Unable to contact TRQ.',
          status: 504,
        });
      });
  });

  it('should notice the url is a different website', () => {
    const api = new WorkspaceApi('www.icmanage.com', 80, 100);
    return api.getAllWorkspaces()
    .then(
      (data) => { console.log('diff', data); expect(data.status).to.equal(404); });
  });
  */
});
