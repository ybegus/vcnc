import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import * as actions from '../../../actions/workspaceEditActions.js';
import WorkspaceCard from './WorkspaceCard';
import FloatingActionButton from 'material-ui/FloatingActionButton';
import ContentAdd from 'material-ui/svg-icons/content/add';

const styles = {
  floating_button: {
    position: 'fixed',
    bottom: '20px',
    right: '20px',
  },
  root: {
    marginTop: '15px',
    marginRight: '30px',
    marginLeft: '30px',
  },
};

const WorkspacesPage = (props) => {
  const renderWorkspace = (workspace, index) => (
    <WorkspaceCard
      index={index}
      key={workspace.reactKey}
      name={workspace.name}
      spec={workspace.spec}
    />
  );

  return (
    <div style={styles.root}>
      {props.workspaces.map(renderWorkspace)}
      <FloatingActionButton
        style={styles.floating_button}
        secondary={true}
        onMouseUp={() => {
          props.actions.workspaceEditInit(); // args intentionally undefined
        }}
      >
        <ContentAdd />
      </FloatingActionButton>
    </div>
  );
};

WorkspacesPage.propTypes = {
  actions: PropTypes.object.isRequired,
  workspaces: PropTypes.array.isRequired,
};

function mapStateToProps(state) {
  return {
    workspaces: state.workspaces,
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
)(WorkspacesPage);
