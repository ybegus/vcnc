import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import * as actions from '../../../actions/workspaceEditActions.js';
import Card from 'material-ui/Card';
import TextField from 'material-ui/TextField';
import Toggle from 'material-ui/Toggle';
import WorkspaceSpecEntryButtons from './WorkspaceSpecEntryButtons';

const styles = {
  block: {
    display: 'flex',
    width: '100%',
  },
  toggle: {
    paddingTop: '25px',
    marginLeft: '20px',
    marginRight: '10px',
    width: '10%',
  },
  toggle_label: {
    color: 'rgba(0, 0, 0, 0.498039)',
  },
  vp_path: {
    marginLeft: '20px',
    width: '25%',
  },
  vtrq_id: {
    marginLeft: '20px',
    width: '10%',
  },
  vtrq_path: {
    marginLeft: '20px',
    width: '45%',
  },
  icon_button_group: {
    display: 'inline-block',
    position: 'relative',
    width: '120px',
    margin: 0,
    marginTop: '15px',
    marginBottom: '15px',
    padding: 0,
  },
  icon_buttons: {
    margin: 0,
    marginLeft: '10px',
    padding: 0,
    width: '20px',
    height: '20px',
  },
  icons: {
    margin: 0,
    padding: 0,
  },
};

function WorkspaceSpecEntryCard(props) {
  const { editable, entry, entryCount, index } = props;
  const entryErrorText = (name) =>
    props.errors &&
      props.errors.spec &&
      props.errors.spec[index] &&
      props.errors.spec[index][name]
  ;
  return (
    <Card>
      <div style={styles.block}>
        <Toggle
          id="toggle"
          style={styles.toggle}
          label="local"
          labelPosition="right"
          labelStyle={styles.toggle_label}
          toggled={entry.local}
          disabled={!editable}
          onToggle={() => props.actions.workspaceEditEntryLocal(index, !entry.local)}
        />
        <TextField
          id="vp-path"
          style={styles.vp_path}
          floatingLabelText="VP Path"
          floatingLabelFixed={false}
          hintText="/"
          defaultValue={entry.vp_path}
          disabled={index === 0 || !editable}
          errorText={entryErrorText('vp_path')}
          onChange={(e) => props.actions.workspaceEditEntryVpPath(index, e.target.value)}
        />
        <TextField
          id="vtrq-id"
          style={styles.vtrq_id}
          floatingLabelText="vTRQ"
          floatingLabelFixed={false}
          hintText="1"
          defaultValue={entry.vtrq_id}
          disabled={!editable}
          errorText={entryErrorText('vtrq_id')}
          onChange={(e) => props.actions.workspaceEditEntryVtrq(index, e.target.value)}
        />
        <TextField
          id="vtrq-path"
          style={styles.vtrq_path}
          floatingLabelText="vTRQ Path"
          floatingLabelFixed={false}
          defaultValue={entry.vtrq_path}
          disabled={!editable}
          errorText={entryErrorText('vtrq_path')}
          onChange={(e) => props.actions.workspaceEditEntryVtrqPath(index, e.target.value)}
        />
        <div>
          {editable &&
            <WorkspaceSpecEntryButtons
              style={styles.icon_button_group}
              buttonStyle={styles.icon_buttons}
              iconStyle={styles.icons}
              index={index}
              entryCount={entryCount}
            />
          }
        </div>
      </div>
    </Card>
  );
}

WorkspaceSpecEntryCard.propTypes = {
  actions: PropTypes.object,
  editable: PropTypes.bool.isRequired,
  entry: PropTypes.object.isRequired,
  entryCount: PropTypes.number.isRequired,
  errors: PropTypes.object,
  index: PropTypes.number.isRequired,
};

function mapStateToProps(state) {
  return { errors: state.workspaceEdit.errors };
}

function mapDispatchToProps(dispatch) {
  return {
    actions: bindActionCreators(actions, dispatch),
  };
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(WorkspaceSpecEntryCard);
