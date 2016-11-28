import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import TextField from 'material-ui/TextField';
import * as actions from '../../../actions/settingsActions.js';

class SettingsPage extends React.Component {
  constructor(props, context) {
    super(props, context);
    this.handleHostChange = this.handleHostChange.bind(this);
  }

  handleHostChange(event) {
    // this.props.actions.createWorkspace(this.state.workspce);
    console.log(`Saving ${event.target.value}`);
  }

  render() {
    return (
      <div>
        <TextField
          id="settings-vcnc-host"
          floatingLabelText="vCNC Host"
          floatingLabelFixed={false}
          hintText="vcnc"
          value={this.props.settings.host}
          onChange={this.handleHostChange}
        />
        <br />
        <TextField
          id="settings-vcnc-port"
          floatingLabelText="vCNC Port"
          floatingLabelFixed={false}
          hintText="5500"
          value={this.props.settings.port}
          onChange={this.handlePortChange}
        />
        <br />
      </div>
    );
  }
}

SettingsPage.propTypes = {
  actions: PropTypes.object.isRequired,
  settings: PropTypes.object.isRequired,
};

function mapStateToProps(state) {
  return {
    settings: state.settings,
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
)(SettingsPage);
