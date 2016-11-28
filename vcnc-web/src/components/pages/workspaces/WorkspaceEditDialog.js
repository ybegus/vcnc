import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import * as editActions from '../../../actions/workspaceEditActions.js';
import * as workspacesActions from '../../../actions/workspacesActions.js';
import Dialog from 'material-ui/Dialog';
import FlatButton from 'material-ui/FlatButton';
import TextField from 'material-ui/TextField';
import FloatingActionButton from 'material-ui/FloatingActionButton';
import ContentAdd from 'material-ui/svg-icons/content/add';
import WorkspaceSpecEntryCard from './WorkspaceSpecEntryCard';

const WorkspaceEditDialog = (props) => {
  const styles = {
    name: {
      margin: 0,
      padding: 0,
    },
    entry_add_button: {
      float: 'right',
      marginRight: 0,
      marginTop: 15,
    },
    spec_list: {
      paddingTop: '10px',
      paddingBottom: '50px',
    },
  };

  const actionButtons = [
    <FlatButton
      label="Cancel"
      primary={true}
      onTouchTap={() => props.actions.workspaceEditCancel()}
    />,
    <FlatButton
      label="Submit"
      primary={true}
      disabled={!props.valid}
      onTouchTap={() => {
        props.actions.workspacesReplace(
          props.index,
          { name: props.name, spec: props.spec, originalName: props.originalName },
        );
        props.actions.workspaceEditCancel();
      }}
    />,
  ];

  return (
    <Dialog
      title="Edit Workspace"
      actions={actionButtons}
      modal={true}
      open={props.open}
    >
      <div style={styles.spec_list}>
        <TextField
          id="name"
          style={styles.name}
          errorText={props.errors.name}
          floatingLabelText="Name"
          floatingLabelFixed={false}
          hintText="/path/to/workspace"
          defaultValue={props.name}
          onChange={(e) => props.actions.workspaceEditName(e.target.value)}
        />
        {
          props.spec.map((entry, index) => (
            <WorkspaceSpecEntryCard
              editable={true}
              entry={entry}
              entryCount={props.spec.length}
              index={index}
              key={props.spec[index].reactKey}
            />
            )
          )
        }
        <FloatingActionButton
          style={styles.entry_add_button}
          secondary={true}
          mini={true}
          onTouchTap={() => props.actions.workspaceEditEntryCreate()}
        >
          <ContentAdd />
        </FloatingActionButton>
      </div>
    </Dialog>
  );
};

WorkspaceEditDialog.propTypes = {
  actions: PropTypes.object,
  errors: PropTypes.object,
  index: PropTypes.number,
  name: PropTypes.string,
  open: PropTypes.bool,
  spec: PropTypes.array,
  valid: PropTypes.bool,
};

function mapStateToProps(state) {
  return {
    errors: state.workspaceEdit.errors || {},
    index: state.workspaceEdit.index,
    name: state.workspaceEdit.name,
    originalName: state.workspaceEdit.originalName,
    open: state.workspaceEdit.open,
    spec: state.workspaceEdit.spec,
    valid: state.workspaceEdit.valid,
  };
}

function mapDispatchToProps(dispatch) {
  return {
    actions: {
      ...bindActionCreators(workspacesActions, dispatch),
      ...bindActionCreators(editActions, dispatch),
    },
  };
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(WorkspaceEditDialog);
