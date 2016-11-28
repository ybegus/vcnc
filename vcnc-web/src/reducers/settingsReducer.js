import * as types from '../actions/actionTypes.js';

export default (state = [], action) => {
  switch (action.type) {
    case types.SET_VCNC_HOST:
      return [...state,
        Object.assign({}, action.host),
      ];
    default:
      return state;
  }
};
