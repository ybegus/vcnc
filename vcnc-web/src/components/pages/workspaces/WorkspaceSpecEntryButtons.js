import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import * as actions from '../../../actions/workspaceEditActions.js';
import IconButton from 'material-ui/IconButton';
import NavigationArrowUpward from 'material-ui/svg-icons/navigation/arrow-upward';
import NavigationArrowDownward from 'material-ui/svg-icons/navigation/arrow-downward';
import ActionDelete from 'material-ui/svg-icons/action/delete';

function WorkspaceSpecEntryButtons(props) {
  const { buttonStyle, entryCount, iconStyle, index, style } = props;
  return (
    <div style={style}>
      <IconButton
        style={buttonStyle}
        iconStyle={iconStyle}
        tooltip="Move Up"
        disabled={index < 2}
        onTouchTap={() => props.actions.workspaceEditEntryMove(index, true)}
      >
        <NavigationArrowUpward />
      </IconButton>
      <IconButton
        style={buttonStyle}
        iconStyle={iconStyle}
        tooltip="Move Down"
        disabled={index < 1 || index === entryCount - 1}
        onTouchTap={() => props.actions.workspaceEditEntryMove(index, false)}
      >
        <NavigationArrowDownward style={iconStyle} />
      </IconButton>
      <IconButton
        style={buttonStyle}
        iconStyle={iconStyle}
        tooltip="Delete"
        disabled={index === 0}
        onTouchTap={() => props.actions.workspaceEditEntryDelete(index)}
      >
        <ActionDelete style={iconStyle} />
      </IconButton>
    </div>
  );
}

WorkspaceSpecEntryButtons.propTypes = {
  actions: PropTypes.object.isRequired,
  buttonStyle: PropTypes.object.isRequired,
  entryCount: PropTypes.number.isRequired,
  iconStyle: PropTypes.object.isRequired,
  index: PropTypes.number.isRequired,
  style: PropTypes.object.isRequired,
};

function mapStateToProps() {
  return {};
}

function mapDispatchToProps(dispatch) {
  return {
    actions: bindActionCreators(actions, dispatch),
  };
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(WorkspaceSpecEntryButtons);
