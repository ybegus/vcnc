'use strict';  // eslint-disable-line strict
const chakram = require('chakram');

class Namespace {
  constructor(base, vtrqid) {
    this.base = base;
    this.vtrqid = vtrqid;
    this.vtrq_id_base = [
      base,
      'vtrq',
      'namespace',
      vtrqid,
    ].join('/');
  }

  //
  //  private
  scrunch(method, path, operator, params, body) {
    const endpoint = [
      this.vtrq_id_base,
      encodeURIComponent(path),
    ];
    if (operator) {
      endpoint.push(operator);
    }
    const url = `${endpoint.join('/')}`;
    console.log('scrunch', method, url);
    const options = {};
    if (params) {
      options.qs = params;
    }
    if (body) {
      options.json = body;
    }
    return chakram.request(method, url, options);
  }
  //
  //
  //  Returns a promise that resolves to a chakram response object.
  children(path) {
    return this.scrunch('GET', path, 'children');
  }
  //
  //  Returns a promise that resolves to a chakram response object.
  delete(path, recursive) {
    return this.scrunch('DELETE', path, '', { recursive });
  }
  //
  //
  mkdir(path, parents) {
    return this.scrunch('POST', path, 'mkdir', { parents });
  }
}

module.exports = Namespace;
