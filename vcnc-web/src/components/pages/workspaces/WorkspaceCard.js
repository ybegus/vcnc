import React, { PropTypes } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import * as editActions from '../../../actions/workspaceEditActions.js';
import * as workspacesActions from '../../../actions/workspacesActions.js';
import { Card, CardHeader, CardText, CardActions } from 'material-ui/Card';
import FlatButton from 'material-ui/FlatButton';
import WorkspaceSpecEntryCard from './WorkspaceSpecEntryCard';

const styles = {
  actions_group: {
    display: 'flex',
    float: 'right',
    margin: 0,
    marginRight: '0px',
    marginBottom: '15px',
  },
  card: {
    // intentionally empty
  },
  card_container: {
    paddingBottom: '53px',
  },
  icon_button_group: {
    position: 'static',
    float: 'right',
    margin: 0,
    marginRight: '5px',
    padding: 0,
  },
  icon_buttons: {
    margin: 0,
    padding: 0,
    width: '20px',
    height: '20px',
  },
  spec_list: {
    paddingBottom: '20px',
  },
};

const WorkspaceCard = (props) => (
  <Card style={styles.card}>
    <CardHeader
      title={props.name}
      actAsExpander={true}
      showExpandableButton={true}
    />
    <CardText expandable={true} style={styles.card_container}>
      {
        props.spec.map((entry, index) => (
          <WorkspaceSpecEntryCard
            key={props.spec[index].reactKey}
            editable={false}
            entry={entry}
            entryCount={props.spec.length}
            index={index}
            style={styles.spec_list}
          />
        ))
      }
      <CardActions style={styles.actions_group}>
        <FlatButton
          label="Edit"
          primary={true}
          onClick={() => {
            props.actions.workspaceEditInit(
              props.name,
              props.spec,
              props.index);
          }}
        />
        <FlatButton
          label="Delete"
          primary={true}
          onClick={() => {
            props.actions.workspacesDelete(props.index, props.name);
          }}
        />
      </CardActions>
    </CardText>
  </Card>
);

WorkspaceCard.propTypes = {
  actions: PropTypes.object.isRequired,
  index: PropTypes.number.isRequired,
  name: PropTypes.string.isRequired,
  spec: PropTypes.array.isRequired,
};

function mapStateToProps(state, ownProps) {
  return {
    workspace: state.workspaces[ownProps.index],
  };
}

function mapDispatchToProps(dispatch) {
  return {
    actions: {
      ...bindActionCreators(editActions, dispatch),
      ...bindActionCreators(workspacesActions, dispatch),
    },
  };
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(WorkspaceCard);
