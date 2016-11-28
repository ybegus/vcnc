/*
 *    Copyright (C) 2015-2016    IC Manage Inc.
 *
 *    See the file 'COPYING' for license information.
 */
/**
 *  Defines routes for every API v1 operation.
 *  @module
 */

'use strict'; // eslint-disable-line strict
const fulfill202 = require('../lib/fulfill202.js').fulfill202;
const CnctrqClient = require('../addon/build/Release/cnctrq_client.node').CnctrqClient;

const cnctrqClient = new CnctrqClient;
const json = require('JSON');
const config = require('../lib/configuration.js');

//
//  Inject latency for testing
//
function latency() {
  if (config.test.latency === undefined || config.test.latency === 0) {
    return Promise.resolve();
  }
  const p = new Promise((resolve) => {
    setTimeout(
      () => {
        resolve();
      },
      config.test.latency * 1000);
  });
  return p;
}
//
//  Constructs and returns an object which represents the operation's outcome.
//
//  The return value has two top-level properties. 'status' contains the
//  operation's http status code, and 'body' is the HTTP response body.
//
function adapter(fromRpc, onSuccess, onFailure) {
  const status = fromRpc.http_status;
  const rtn = {
    status,
    body: {},
  };
  rtn.body.error_sym = fromRpc.error_sym;
  rtn.body.message = fromRpc.error_description_brief;
  //
  const successful = (200 <= status && status < 300); // eslint-disable-line yoda
  const moreProperties = successful ? onSuccess : onFailure;
  Object.assign(rtn.body, moreProperties);
  return rtn;
}

/** Defines our endpoints
 * @param app The express.router which implements our functionality.
 */
module.exports = (app) => {
  //
  //  Vector delete node
  //
  app.post('/vtrq/delete_nodes/:vtrq_id', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency(() => {
          cnctrqClient.remove(
            req.pathParams.vtrq_id,
            req.body.delete_paths,
            req.query.recursive,
            (result) => {
              cb(adapter(result, { result: result.result }));
            });
        });
      });
  });

  //
  //  Vector metadata Copy
  //
  app.post('/vtrq/meta_copy/:vtrq_id', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.meta_copy(
            req.pathParams.vtrq_id,
            req.query.overwrite,
            req.body.copy_paths,
            (result) => {
              cb(adapter(result, { result: result.result }));
            });
        });
      });
  });

  app.delete('/vtrq/namespace/:vtrq_id/:url_path', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.remove(
            req.pathParams.vtrq_id,
            [req.pathParams.url_path],
            req.query.recursive,
            (result) => {
              if (result.http_status === 200) {
                cb(adapter(result, result.result[0]));
              } else {
                cb(adapter(result));
              }
            });
        });
      });
  });

  app.get('/vtrq/namespace/:vtrq_id/:url_path/children', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.list(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            (result) => {
              cb(adapter(result, { result: result.result }));
            });
        });
      });
  });

  app.get('/vtrq/namespace/:vtrq_id/:url_path', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.getattr(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            (result) => {
              cb(adapter(result, { stat: result.stat }));
            });
        });
      });
  });

  app.get('/vtrq/namespace/:vtrq_id/:path/consistency', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.consistency_get(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            (result) => {
              cb(adapter(result, { consistency: result.consistency }));
            });
        });
      });
  });

  app.post('/vtrq/namespace/:vtrq_id/:path/consistency', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.consistency_set(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            req.body.consistency,
            (result) => {
              cb(adapter(result));
            });
        });
      });
  });

  //
  //
  //
  app.post('/vtrq/namespace/:vtrq_id/:path/mkdir', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.mkdir(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            req.query.mode,
            req.query.parents,
            (result) => {
              cb(adapter(result, { result: result.result }));
            });
        });
      });
  });

  //
  //  Shutdown TRQ
  //
  app.delete('/vtrq/service/:vtrq_id', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.shutdown_vtrq(
            req.pathParams.vtrq_id,
            (result) => {
              cb(adapter(result));
            });
        });
      });
  });

  //
  //  Discover VP IDs having certain qualities
  //
  app.get('/vtrq/vp/:vtrq_id', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.vp_find(
            req.pathParams.vtrq_id,
            req.query.vp_host,
            req.query.mount_point,
            (result) => {
              cb(adapter(result, { vp_ids: result.vp_ids }));
            });
        });
      });
  });

  //
  //  Retrieve information about a specific VP
  //
  app.get('/vtrq/vp/:vtrq_id/:vp_id', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.vp_properties(
            req.pathParams.vtrq_id,
            req.pathParams.vp_id,
            (result) => {
              cb(adapter(
                result,
                {
                  gid: result.gid,
                  hostname: result.hostname,
                  mount_point: result.mount_point,
                  uid: result.uid,
                  user: result.user,
                  workspace: result.workspace,
                }));
            });
        });
      });
  });

  //
  //  Retrieve the workspaces names at this workspace path.
  //
  app.get('/vtrq/workspaces/:vtrq_id/:url_path/children', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.workspace_children(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            (result) => {
              cb(adapter(result, { children: result.children }));
            });
        });
      });
  });

  //
  //  Retrieves the workspace specification at this path.
  //
  app.get('/vtrq/workspaces/:vtrq_id/:url_path', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.workspace_get(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            (result) => {
              //
              //  this happens because we receive the workspace specification
              //  from the RPC as a JSON string rather than an object. Code at
              //  this level manipulates objects.
              //
              if (result.http_status === 200) {
                cb(adapter(result, { spec: json.parse(result.spec) }));
              } else {
                cb(adapter(result));
              }
            });
        })
      });
  });

  //
  //  Updates a workspace specification at this path.
  //  'put' is implicitly "overwrite: true".  It is an error 'put'
  //  to a workspace that doesn't exist.
  //
  app.put('/vtrq/workspaces/:vtrq_id/:url_path', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.workspace_put(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            json.stringify(req.body.spec),
            req.query.push,
            (result) => {
              cb(adapter(result));
            });
        });
      });
  });

  //
  //  Sets a workspace specification at this path.
  //
  app.post('/vtrq/workspaces/:vtrq_id', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.workspace_set(
            req.pathParams.vtrq_id,
            req.body.name,
            json.stringify(req.body.spec),
            req.query.overwrite,
            req.query.push,
            (result) => {
              cb(adapter(result));
            });
        });
      });
  });

  //
  //  Deletes a workspace specification at this path.
  //
  app.delete('/vtrq/workspaces/:vtrq_id/:url_path', (req, res) => {
    fulfill202(
      req,
      res,
      (cb) => {
        latency()
        .then(() => {
          cnctrqClient.workspace_delete(
            req.pathParams.vtrq_id,
            req.pathParams.url_path,
            (result) => {
              cb(adapter(result));
            });
        });
      });
  });
};
