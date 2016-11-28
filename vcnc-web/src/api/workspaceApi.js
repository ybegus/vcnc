import axios from 'axios';

// const pathRoot = '/webdemo';
// const pathRoot = '/';


// const unPathName = name => name.slice(pathRoot.length);

const encodedName = name => encodeURIComponent(name);

const handleAxiosError = (error) => {
  if (error.response) {
    //  vcnc api guarantees 'error_sym' and 'message' properties.
    throw new TypeError(JSON.stringify(
      { ...error.response.data, status: error.response.status })
    );
  }
  //  We never reached the vcnc; synthesize an error object here. The lack of
  //  a 'status' property signifies this class of error.
  throw new TypeError(JSON.stringify({
    error_sym: error.code,
    message: error.code === 'ENOTFOUND' ? 'vcnc server not found' : error.code,
  }));
};

/*
const delay = (time, fulfill) => new Promise(fulfill => setTimeout(fulfill, time));

const fulfill202 = axr => {
  if (axr && axr.status !== 202) {
    return Promise.resolve(axr);
  }
  return delay(() => {
    });
};
*/

class WorkspaceApi {
  constructor(host, port, vtrqId, chroot = '/') {
    this.host = host;
    this.port = port;
    this.axios = axios.create({
      baseURL: `http://${host}:${port}/v1/vtrq/workspaces/${vtrqId}`,
      headers: { Accept: 'application/json' },
      timeout: 1400, // mocha waits 2000 ms
    });
    this.chroot = chroot;
  }
  pathName = name => this.chroot + name;

  /* example from http://stackoverflow.com/questions/30505960/use-promise-to-wait-until-polled-condition-is-satisfied
  waitForResult = () => {
    return new Promise((resolve, reject) => {
      (
        () => {
          if (lib.foo) return resolve();
          setTimeout(waitForResult, 30);
        })();
    });
  };
  */
  //
  //
  //
  /*
  waitForFulfill = url => {
    return new Promise((resolve, reject) => {(
      function pollForFulfill() {
        this.axios.get(url)
        .then(
          axr => {
            if (axr.data.status === 'fulfilled') {
              return {data: axr.payload};
            }
            setTimeout(pollForFulfill, 5);
          })
        .catch(handleAxiosError);
      })();
    });
  };
  */
  //
  //  Returns a promise that resolves to a list of workspace specifications.
  //
  //  'axr' means 'axios response'
  //
  getAllWorkspaces = () => {
    let accumulator = [];
    const ax = this.axios;
    const pathName = this.pathName;

    const getChildWorkspaces = node =>
      ax.get(`/${encodedName(pathName(node))}/children`)
      .then(
        axr => Promise.all(axr.data.children.map(name => `${node}/${name}`).map(this.getWorkspace)),
        error => {
          if (error.response && error.response.status === 404) {
            return [];
          }
          throw error;
        })
      .catch(handleAxiosError);

    const recurse = node =>
      getChildWorkspaces(node)
      .then(childrenOfNode => {
        if (childrenOfNode.length > 0) {
          accumulator = accumulator.concat(childrenOfNode);
        }
        return Promise.all(
          childrenOfNode.map(child => `${child.name}`).map(n => recurse(n)))
        .then(grandchildrenOfNode => {
          if (grandchildrenOfNode.length > 0) {
            accumulator =
              accumulator
              .concat(grandchildrenOfNode)
              .filter(e => e !== undefined)
              .filter(e => e.spec && e.spec.length > 0);
          }
        });
      })
      .catch(handleAxiosError);

    return recurse('')
    .then(() => accumulator);
  };

  getWorkspace = name => {
    const target = `/${encodedName(this.pathName(name))}`;
    return this.axios.get(target)
    .then(
      axr => ({ name, spec: axr.data.spec }),
      error => {
        if (error.response && error.response.status === 404) {
          return { name, spec: [] };
        }
        throw error;
      }
    );
  };

  setWorkspace = (name, spec) => {
    const target = this.pathName(name);
    return this.axios.post(
      '',
      { name: target, spec },
      { params: { overwrite: true } },
    )
    .then(
      axr => axr.data,
      handleAxiosError
    );
  };

  deleteWorkspace = name => {
    const target = `${encodedName(this.pathName(name))}`;
    return this.axios.delete(target)
    .then(
      axr => axr.data,
      handleAxiosError
    );
  };
}

export default WorkspaceApi;

