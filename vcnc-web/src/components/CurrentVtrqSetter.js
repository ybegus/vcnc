/**
 * Created by nick on 10/4/16.
 */
import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import * as actions from '../actions/settingsActions.js';
import VtrqSetter from './VtrqSetter.js';

const CurrentVtrqSetter = props => (
  <VtrqSetter
    value={props.currentVtrq}
    onChange={(event, index, val) => props.actions.settingsCurrentVtrqSet(val)}
    style={{ width: 181 }}
  />
);

CurrentVtrqSetter.propTypes = {
  actions: PropTypes.object,
  currentVtrq: PropTypes.number,
};

function mapStateToProps(state) {
  return {
    currentVtrq: state.settings.currentVtrq,
  };
}

function mapDispatchToProps(dispatch) {
  return {
    actions: bindActionCreators(actions, dispatch),
  };
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(CurrentVtrqSetter);
