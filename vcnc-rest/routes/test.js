/*
 *    Copyright (C) 2016    IC Manage Inc.
 *
 *    See the file 'COPYING' for license information.
 */
/**
 *  Defines routes for test operations
 *  @module
 */
'use strict'; // eslint-disable-line strict
const fulfill202 = require('../lib/fulfill202.js').fulfill202;
function cnctrqClientMoxiate(input, latency, cb) {
  //  Set the timeout emulating the backend latency
  setTimeout(
    () => {
      // This line happens on the "TRQ"
      cb({
        http_status: 200,
        result: { result: input + 1 },
      });
    },
    latency * 1000
  );
}


/** Defines our endpoints
 * @param app The express.router which implements our functionality.
 */
module.exports = (app) => {
  //
  //  Variable fulfillment latency
  //
  app.post('/moxiate', (req, res) => {
    fulfill202(req, res, (cb) => {  // lambda is "operation"
      cnctrqClientMoxiate(
        req.query.input,
        req.query.latency,
        (result) => {
          cb(result);
        }
      );
    });
  });
};
