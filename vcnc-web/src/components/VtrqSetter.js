/**
 * Created by nick on 10/4/16.
 */
import React, { PropTypes } from 'react';
import DropDownMenu from 'material-ui/DropDownMenu';
import MenuItem from 'material-ui/MenuItem';

const VtrqSetter = props => (
  <DropDownMenu
    value={props.value}
    onChange={props.onChange}
    style={props.style}
  >
    {[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10].map(vtrqid => (
      <MenuItem
        key={vtrqid}
        value={vtrqid}
        primaryText={vtrqid}
      />
    ))}
  </DropDownMenu>
);

VtrqSetter.propTypes = {
  onChange: PropTypes.func.isRequired,
  style: PropTypes.object.isRequired,
  value: PropTypes.number.isRequired,
};

export default VtrqSetter;
