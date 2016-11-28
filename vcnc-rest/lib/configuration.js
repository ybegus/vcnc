const path = require('path');
const yaml = require('yamljs');
const os = require('os');

//
//  TODO: handle parse exception
//
const conf = yaml.load(path.join(__dirname, '../config/vcnc-config.yaml'));

//
//  Get our port number
//
if ('VELSTOR_VCNC_PORT' in process.env) {
  conf.server.port = process.env.VELSTOR_VCNC_PORT;
}

//
//  Compute the default fulfillment URL.
//
if (conf.fulfill202.baseUrl === undefined) {
  const port = conf.server.port;
  conf.fulfill202.baseUrl = `http://${os.hostname()}:${port}`;
}
// console.log('resolved configuration: ', conf);

module.exports = conf;
