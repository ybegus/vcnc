export default {
  loginModalDialog: {
    vcncAuthority: 'vcnc',
    currentVtrq: 0,
    message: undefined,
    open: true,
  },
  session: {
    restApiClient: {},
    isLoggedIn: false,
  },
  settings: {
    currentVtrq: 0,
    vcncAuthority: 'cnc:6130',
  },
  workspaceEdit: {
    errors: { },
    name: '/new/workspace',
    spec: [
      {
        reactKey: 0,
        vp_path: '/',
        vtrq_id: 0,
        vtrq_path: '/path/on/vtrq',
        local: false,
      },
    ],
    altered: false,
    open: false,
    valid: true,
  },
  workspaces: [],
};
