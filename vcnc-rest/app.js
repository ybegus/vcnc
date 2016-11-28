'use strict'; // eslint-disable-line strict
/*
 *    Copyright (C) 2015-2016    IC Manage Inc.
 *
 *    See the file 'COPYING' for license information.
 */
// Set the DEBUG environment variable to enable debug output
// process.env.DEBUG = 'swagger:middleware';

const config = require('./lib/configuration');
const express = require('express');
const Middleware = require('swagger-express-middleware');
const path = require('path');
const http = require('http');
const cors = require('cors');
//
//  Initialize the C++ extension
//
const CnctrqClient = require(path.join(__dirname,
				 'addon/build/Release/cnctrq_client')).CnctrqClient;
const cnctrqClient = new CnctrqClient;
cnctrqClient.call_me_first(__dirname);
const fulfill202 = require('./lib/fulfill202');
/**
 *  Top-level Express application object.
 *  @module
 */
module.exports = (() => {
  //
  // ROUTES ===========================================================

  const app = express();

  //
  //  Configure the static content
  //
  app.use(
    '/v1/doc/api',
    express.static(path.join(__dirname, 'static/swagger-ui/dist'))
  );
  app.use(
    '/v1/doc',
    express.static(path.join(__dirname, 'static/doc-html'))
  );
  app.use(
    '/v1/doc/api/spec',
    express.static(path.join(__dirname, 'api/swagger.json'))
  );

  const middleware = new Middleware(app);

  //
  //  Any app.use(..) outside this block (i.e, lexically after) are,
  //  in effect, /earlier/ in the processing chain, because this
  //  block is a callback whose execution is deferred.
  //
  middleware.init(path.join(__dirname, 'api/swagger.yaml'), () => {
    //
    //  Enable CORS
    //
    app.use(cors({ credentials: true, origin: true }))
    //
    //  Metadata middleware
    //
    app.use(middleware.metadata());
    //
    //  Parse the request, convert to approprate data types.
    //
    app.use(middleware.parseRequest());
    //
    //  Very, very crude URL logging
    //
    app.use((req, res, next) => {
      if ('VELSTOR_VCNC_LOG_REQUESTS' in process.env) {
        console.log(req.method, req.originalUrl); // eslint-disable-line no-console
      }
      next();
    });
    //
    //  Validate the request against the API definition, returning HTTP codes
    //  as appropriate.
    //
    app.use(middleware.validateRequest());
    //
    //  Install routers for each API version and variation
    //
    const v1Router = express.Router(); // eslint-disable-line new-cap
    require(path.join(__dirname, 'routes/v1'))(v1Router);
    app.use('/v1', v1Router);
    //
    //  Install a test router
    //
    const testRouter = express.Router(); // eslint-disable-line new-cap
    require(path.join(__dirname, 'routes/test'))(testRouter);
    app.use('/v1/_test', testRouter);
    //
    // Catch undefined URLs (404) and forward to error handler
    //
    app.use((req, res, next) => {
      if (req.originalUrl === '/v1/doc/api/spec') {
        //
        //  Swagger-ui needs this url to 404 and
        //  also return the default response.
        //
        next();
      } else if (req.originalUrl === '/') {
        res.redirect('/v1/doc');
      } else {
        //
        //  Otherwise, if we go here, we got here, we missed
        //  a URL that was outside the swagger-express-middleware
        //  domain, so perform our uniform JSON response handling.
        //
        const err = new Error('URL not found.');
        err.status = 404;
        next(err);
      }
    });
    // ERROR HANDLING ====================================================
    //
    // This error handling follows the policy that every response body shall
    // contain a JSON object having both an 'error_sym' and a 'message'
    // property.
    //
    // When the server runs in 'development' mode, the object will also
    // have a 'stack' property whose value is the stack trace.
    //
    // The CNC server can throw exception/errors in three different contexts:
    // a) synchronously to Express handling (app.use(..)); b) during an Express
    // callback; and c) within libuv as part of processing a node/V8 extension.
    //
    // The code here handles the ((a)) context.  Handling the ((b)) context
    // is very cumbersome - it involves try/catch within every route handling
    // method.  Case ((c)) is similar to ((b)), only within the C++ extension
    // context.
    //
    // The policy is to not handle the ((b)) and ((c)) cases.  The CNC server
    // will terminate, and in production will be automatically restarted. Testing
    // will have to discover internal bugs which can generate such exceptions.
    // This will be sufficient as long as there is no user (REST) input which can
    // generate such errors.
    //
    // http://stackoverflow.com/a/15913211 is a good explanation of the issues.
    //
    function errorHandler(err, req, res, next, stack) {
      if (err.status === 404) {
        res
        .status(err.status)
        .json({ error_sym: 'ENOENT', message: err.message });
      } else {
        res
        .status(err.status || 500)
        .json({
          error_sym: 'EREMOTEIO',
          message: err.message,
          stack,
        });
        next(err);  // logs this error to the server console
      }
    }

    //
    // During development, a stack trace is sent to the client.
    // An error always causes the server to log the stack to stdout/stderr.
    //
    app.use((err, req, res, next) => {
      errorHandler(
        err,
        req,
        res,
        next,
        app.get('env') === 'development' ? err.stack : undefined
      );
    });


    //
    //  Announce we are ready.
    //
    const port = config.server.port;
    console.log('INFO: accepting REST connections on port', port); // eslint-disable-line
    // create http service
    http.createServer(app).listen(port);
  });

  //
  //  An undocumented error generator for testing.
  //
  app.use('/_fail/sync', () => {
    throw new Error('mock synchronous error for testing');
  });
  //
  //  Install the 202 fulfillment controller
  //
  app.get(`${fulfill202.route}/:id`, (req, res) => {
    // console.log('routed to fulfill ');
    // const result = fulfill202.fetchFulfillmentResource(req.params.id);
    fulfill202.fetchFulfillmentResource(req.params.id)
    .then((result) => {
      // console.log('about to fulfill ', result);
      res.status(200).send(result);
    });
  });

  //
  //  Serve the web admin console on its own port.
  if (config.web.enabled) {
    const admin = express();
    admin.use(
      '/',
      express.static(path.join(__dirname, 'static/vcnc-web'))
    );
    const adminPort = parseInt(config.server.port) + parseInt(config.web.offset);
    console.log('INFO: accepting vcnc-web connections on port', adminPort); // eslint-disable-line
    http.createServer(admin).listen(adminPort);
  }
})();
