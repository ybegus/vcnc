/*
 *	Copyright (C) 2015-2016	IC Manage.
 *
 *	See the file 'COPYING' for license information.
 */

//
//  A small program that provides the configure.ac version and build 
//  information to the node.js server and to the release build script.
//
//  usage:
//    % vcnc_version --version
//

#include <lib/pEnv.H>
#include <rev/Rev.H>
#include <lib/pSession.H>

#include <iostream>

using namespace pepsis::lib;
using namespace std;
//

int 
main(int argc, char* argv[])
{
  pInit init;
  {
    pEnv& env = pEnv::self();
    env.Who("vcnc",::vcnc_version_number(),::vcnc_release_change());
    //
    //  Bring our session to life.
    //
    pSession& ses = pSession::self();
    //
    //  Parse the command line and otherwise set up the environment
    //
    env.Switches(argc, const_cast<const char** >(argv));
    if (env.Status().Error()) {	// a problem, exit.
      pSession::self().Log() << "Use 'vcnc-version --help' to see available options." << pEndl;
      return 1;
    }
    if (env.Status().Exit()) {	// no problem, early exit requested.
      return 0;
    }
    pSession::self().Finish();
    return 0;
  }
}
