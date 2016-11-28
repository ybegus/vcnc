import * as types from './actionTypes.js';

export function settingsCurrentVtrqSet(vtrqId) {
  return { type: types.SETTINGS_CURRENT_VTRQ_SET, payload: vtrqId };
}
