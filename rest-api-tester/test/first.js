var chakram = require('chakram/lib/chakram.js');
var expect = chakram.expect;
var Namespace = require('../lib/namespace.js');

describe('vCNC API', () => {
  var config;
  var ns;
  var sandbox;
  before(() => {
    config = {
      vcnc_url_base: 'http://cnc:5501/v1',
      vp: '/tmp/vp/mnt',
      vtrq_id: 1,
      vtrq_base: 'QXHJYCL5LX-vcnc-api-testing',
    };
    ns = new Namespace(config.vcnc_url_base, config.vtrq_id);
    sandbox = `/${config.vtrq_base}`;
  });

  describe('vCNC Namespace API', () => {
    //
    //
    describe("'ns children'", () => {
      var response;
      before('Get the list of root children.', () => {
        response = ns.delete(sandbox, true)
        .then((r) => {
          expect(r).to.have.status(200);
          return ns.mkdir(sandbox, false);
        })
        .then((r) => {
          expect(r).to.have.status(200);
          return ns.children('/');
        });
        return chakram.wait();
      });
      it('should have status 200', () =>
        expect(response).to.have.status(200)
      );
      it('should have error_sym OK', () => {
        console.log('error_sym: ', response);
        expect(response).to.comprise.json('error_sym', 'OK');
      });
      it('should list the root', () => {
        expect(response).to.comprise.json('result', (result) => {
          expect(result).to.be.an('array');
          expect(result).to.include(config.vtrq_base);
        });
        return chakram.wait();
      });
    });
    //
    //
    describe("'ns mkdir -p'", () => {
      var response;
      before('Do a mkdir -p', () => {
        response = ns.mkdir(`${sandbox}/mkdir/-p`, true);
      });
      it('should have status 200', () =>
        expect(response).to.have.status(200)
      );
    });
  });
});
