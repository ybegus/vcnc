#!/bin/bash
#
#  This script uses PeerCache 'vconfgen' to generate configuration files for a PeerCache
#  deployment that runs entirely on the local machine. The local deployment is useful for
#  testing vCNC software.
#
#  Typically, it is not necessary to run this script again when a new PeerCache edition is
#  installed.
#
function generate_vconfgen_input {
  #
  #  PeerCache installation directory
  echo /opt/velstor/current

  ####  vTRQ information  ####

  #
  #  vTRQ host/ip
  echo localhost
  #
  #  PeerCache configuration file
  echo /opt/velstor/configs/velstor-vtrq.conf
  #
  #  vTRQ backing store location
  #  (solid-state storage recommended)
  echo /var/lib/velstor/backing
  #
  #  vtrq meta-data backing store location
  #  (solid-state storage recommended)
  echo /varlib/velstor/backing
  #
  #  vTRQ owner process
  echo root

  ####  vCNC information  ####

  #
  #  vCNC host/ip
  echo localhost

  ####  vP4Stuffer information  ####

  #
  #  Is there a Perforce server?
  echo n
  #
  #  Is there an IC Manage server
  echo n

  ####  vPM information  ####

  #
  #  vPM configuration file
  echo /opt/velstor/configs/velstor-vpm.conf
  #
  #  vPM socket location/name
  echo /tmp/local/velstor.vpm.socket
  #
  #  vPM peer cache size in MB
  echo 40000
  #
  #  vPM peering enabled
  echo true
  #
  #  vPM cache enabled
  echo true
  #
  #  vPM file backing store location
  echo /var/lib/velstor/vpm
  #
  #  vPM meta-data backing store location
  echo /var/lib/velstor/vpm_db
  #
  #  vPM process owner
  echo root

  ####  vDA information  ####

  #  vDA host
  echo localhost
  #
  #  vDA POST logging directory root
  echo /tmp/vDA
  #
  #  vDA GET logging directory root
  echo /tmp/vda-client
  #
  #  vDA process owner
  echo root
}

if [ $# != 1 ]; then
  echo Usage: ./configure-peercache.sh dirname
  echo '  where:'
  echo "    'dirname' is the directory that will hold the generated configuration files."
  echo
  echo  "    'dirname' will be created if it doesn't exist." 
  echo 
  echo  "Use '/opt/velstor/configs' for vCNC development."
  echo
  exit 1
fi

PROG_VCONFGEN=/opt/velstor/current/bin/vconfgen

if [ ! -e ${PROG_VCONFGEN} ]; then
  echo "The PeerCache software must be installed before running this script. See the"
  echo "developer documentation."
  echo
  exit 2
fi

CONFIG_DIR=$1
  
generate_vconfgen_input | ${PROG_VCONFGEN} -c ${CONFIG_DIR}
