/* eslint-disable no-nested-ternary */
import React, { Component, PropTypes } from 'react';
import Title from 'react-title-component';
import AppBar from 'material-ui/AppBar';
import spacing from 'material-ui/styles/spacing';
import MuiThemeProvider from 'material-ui/styles/MuiThemeProvider';
import getMuiTheme from 'material-ui/styles/getMuiTheme';
import injectTapEventPlugin from 'react-tap-event-plugin';
import { darkWhite, lightWhite, grey900 } from 'material-ui/styles/colors';
import AppNavDrawer from './AppNavDrawer';
import withWidth, { MEDIUM, LARGE } from 'material-ui/utils/withWidth';
import WorkspaceEditDialog from './pages/workspaces/WorkspaceEditDialog';

const muiTheme = getMuiTheme();

// Needed for onTouchTap
// http://stackoverflow.com/a/34015469/988941
injectTapEventPlugin();

class Master extends Component {

  static propTypes = {
    children: PropTypes.node,
    location: PropTypes.object,
    width: PropTypes.number.isRequired,
  };

  static contextTypes = {
    router: PropTypes.object.isRequired,
  };

  state = {
    navDrawerOpen: false,
  };

  componentWillMount() {
    this.setState({
      muiTheme: getMuiTheme(),
    });
  }

  getStyles() {
    const styles = {
      appBar: {
        position: 'fixed',
        // Needed to overlap the examples
        zIndex: this.state.muiTheme.zIndex.appBar + 1,
        top: 0,
      },
      root: {
        paddingTop: spacing.desktopKeylineIncrement,
        minHeight: 400,
      },
      content: {
        margin: spacing.desktopGutter,
      },
      contentWhenMedium: {
        margin: `${spacing.desktopGutter * 2}px ${spacing.desktopGutter * 3}px`,
      },
      footer: {
        backgroundColor: grey900,
        textAlign: 'center',
      },
      a: {
        color: darkWhite,
      },
      p: {
        margin: '0 auto',
        padding: 0,
        color: lightWhite,
        maxWidth: 356,
      },
      iconButton: {
        color: darkWhite,
      },
    };

    if (this.props.width === MEDIUM || this.props.width === LARGE) {
      styles.content = Object.assign(styles.content, styles.contentWhenMedium);
    }

    return styles;
  }

  handleTouchTapLeftIconButton = () => {
    this.setState({
      navDrawerOpen: !this.state.navDrawerOpen,
    });
  };

  handleChangeRequestNavDrawer = (open) => {
    this.setState({
      navDrawerOpen: open,
    });
  };

  handleChangeList = (event, value) => {
    this.context.router.push(value);
    this.setState({
      navDrawerOpen: false,
    });
  };

  render() {
    const {
      location,
      children,
    } = this.props;

    let {
      navDrawerOpen,
    } = this.state;

    const router = this.context.router;
    const styles = this.getStyles();
    const title =
      router.isActive('/settings') ? 'Settings' :
      router.isActive('/workspaces') ? 'Workspaces' : '';

    let docked = false;

    if (this.props.width === LARGE && title !== '') {
      docked = true;
      navDrawerOpen = true;

      styles.root.paddingLeft = 256;
      styles.footer.paddingLeft = 256;
    }

    return (
      <MuiThemeProvider muiTheme={muiTheme}>
        <div>
          <Title render="PeerCache" />
          <AppBar
            onLeftIconButtonTouchTap={this.handleTouchTapLeftIconButton}
            title={title}
            zDepth={0}
            style={styles.appBar}
          />
          <div style={styles.root}>
            {children}
          </div>
          <AppNavDrawer
            location={location}
            docked={docked}
            onRequestChangeNavDrawer={this.handleChangeRequestNavDrawer}
            onChangeList={this.handleChangeList}
            open={navDrawerOpen}
          />
          <WorkspaceEditDialog />
        </div>
      </MuiThemeProvider>
    );
  }
}

export default withWidth()(Master);
