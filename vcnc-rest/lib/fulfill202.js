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
const config = require('./configuration.js');
const Redis = require('ioredis');
const json = require('JSON');
//
const redis = new Redis(config.redis.connection);
const fulfillmentUrl = `${config.fulfill202.baseUrl}${config.fulfill202.route}`;
//
//  Redis schema
//
//  The fulfillment feature has two elements: an integer 'next identifier' and
//  and a collection of (identifier, hash) pairs, where 'identifier' is
//  an integer and 'hash' has three elements: 'body', 'delivery' and 'status'.
//
//  'body' is the literal (JSON) text of the response body; 'delivery' is
//  either 'pending', 'completed' or 'failed'; 'status' is the HTTP status
//  code returned by the original (now deferred delivery) operation.
//

/**
 *  Creates a temporary resource for polling.
 *
 *  @return {promise} A promise whose value is the numeric resource id.
 */
function createFulfillmentResource() {
  //
  //  Create the temporary resource in the DB
  //
  //  ... perform a transaction that increments the index; stores the resource's
  //  initial value, and returns the index's pre-increment value.
  //
  // console.log('inside CreateFulfillmentResource');
  const p = redis.incr('fulfill202.index')
    .then((idx) => {
      // console.log('CreateFulfillmentResource processing idx');
      const key = `fulfill202.${idx}`;
      redis.multi()
        .hset(key, 'delivery', 'pending')
        //
        //  Give the back-end (vtrq) a day to respond. Not infinite, because
        //  we want Redis to be able to clean up if the resource is orphaned.
        //
        .expire(key, 60 * 60 * 24)
        .exec();
      return idx;
    })
    .catch((e) => {
      console.log('caught', e);
    });
  return p;
}
/**
 *  Stashes the operational result into the temporary resource.
 *
 *  @param {integer} id The numeric resource identifier.
 *  @param {integer} status The HTTP status of the operation.
 *  @param {object} body The operations's response body.
 *  @return {promise} A promise that delivers an undefined value.
 */
function stashFulfillmentResource(id, status, body) {
  const key = `fulfill202.${id}`;
  // const b = json.stringify(body);
  const p = redis.multi()
    .hmset(
      key, {
        status,
        delivery: 'completed',
        body: json.stringify(body),
      })
    .expire(key, config.fulfill202.expires)
    .exec();
  return p;
}

/**
 *  Fetches the temporary resource at a given numeric id.
 *
 *  @param {integer} id The numberic resource identifier
 *  @return {promise} A promise that delivers an object.
 */
function fetchFulfillmentResource(id) {
  const key = `fulfill202.${id}`;
  const p = redis.hgetall(key)
    .then((result) => {
      // console.log(`fetch ${key} w/`, result);
      if (Object.keys(result).length) {
        //  ... found
        return result;
      }
      //  ... not found
      return ({
        status: 410,
        body: { error_sym: 'ENOENT', message: 'Gone' },
        delivery: 'failed',
      });
    });
  return p;
}

function fulfill202(req, res, operation) {
  const fulfillmentTimeout = config.fulfill202.response * 1000;
  let fulfillmentId;
  //
  //  Set a timeout for a 202 response.
  //  (This is the code template)
  //
  let timeout = setTimeout(
    () => {
      // console.log('setTimeout callback has been invoked');
      //
      //  Clear the timeout to prevent a direct response, below.
      //
      timeout = null;
      //
      //  Tell the client where it can poll the results
      //
      createFulfillmentResource()
      .then((id) => {
        // console.log('sending the 202 message');
        fulfillmentId = id;
        res.header('Location', `${fulfillmentUrl}/${fulfillmentId}`);
        //
        res.status(202).send({
          error_sym: 'ETIME',
          message: `Poll ${fulfillmentUrl}/${fulfillmentId}`,
        });
      });
    },
    fulfillmentTimeout
  );
  //
  //  Call the "TRQ" through the "client"
  //
  operation(
    (result) => {
      // console.log('in operation callback')
      // console.log(result)
      //
      //  Check if the timeout has expired
      //
      const timeoutIsPending = (timeout !== null);
      if (timeoutIsPending) {
        // console.log('clearing timeout')
        clearTimeout(timeout);
        timeout = null;
        //
        //  Return the result in the usual way
        //
        // console.log('timeout has not expired');
        res.status(result.status).send(result.body);
      } else {
        // console.log('timeout has fired')
        //
        //  Stash the result into the db
        //
        // console.log('timeout expired, stashing');
        stashFulfillmentResource(
          fulfillmentId,
          result.status,
          result.body
        );
      }
    }
  );
}
module.exports = {
  fulfill202,
  fetchFulfillmentResource,
  route: config.fulfill202.route,
};
