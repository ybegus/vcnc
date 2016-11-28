/*
 *	Copyright (C) 2010-2015	IC Manage Inc
 *
 *	See the file 'COPYING' for license information.
 */
#include <cncSession.h>
#include <peer/cSession.H>
#include <peer/cNullAuth.H>
#include <peer/cName.H>
#include <iostream>
#include <prtcl/core/pcSessionExport.H>
#include <prtcl/core/pcSessionExport.H>
#include <prtcl/cnctrq/cnctrqClient.H>
#include <prtcl/cnctrq/cnctrqCncSession.H>
#include <prtcl/cnctrq/cnctrqProtocolsClient.H>
#include <sstream>

//
using namespace pepsis::lib;
using namespace pepsis::peer;

using frqu::prtcl::core::pcSessionExport;
using pepsis::lib::pString;
using pepsis::peer::cSession;
using pepsis::peer::cName;

typedef pSmart<cName> name_ptr;

namespace cnc {

  //
  //	Constructor.
  //
  cncSession::cncSession() :
    pSessionBase("cnc")
  {
    //
    pEnv::self();
    //
    //pSession::self().Info() << "I'm constructing cncSession and I need cSession and pcSessionExport!" << pEndl;
    Needs(&pepsis::peer::cSession::self());
    Needs(&frqu::prtcl::core::pcSessionExport::self());
    Needs(&frqu::prtcl::cnctrq::cnctrqCncSession::self()); // registers cnctrq client protocols.
    pcSessionExport::self().NoMentor(true);
    //
    //  No command-line switches or configuration at this time.
  }
  //
  //	Destructor.
  //
  cncSession::~cncSession() { }
  //
  //	Applies the environmental settings that belong to us.
  //
  void cncSession::Apply()
  {
    using namespace frqu::prtcl::cnctrq;
    //pSession::self().Info() << "I'm applying cncSession!" << pEndl;
    //
    //  Register our protocols
    //
    typedef pSmart<cProtocol> protocol_ptr;
    protocol_ptr auth = protocol_ptr(new cNullAuth);
    cSession::self().Registrar().Authentication(auth);
    //
    //  Activate the event loop that will receive responses from TRQs.
    //
    if (cSession::self().Activate(cSession::el_spawn)) {
      pSession::self().Error() << "Node failed to activate peer communication" 
                               << pEndl;
      ::exit(1);  // error exit
    }
  }
  //
  //  Clean up on exit.
  //
  void cncSession::Final()
  {
    // intentionally empty
  }

  void cncSession::RpcToHttpStatus(const int& rpc_error
                                  , int& http_status_code
                                  , string_type& error_sym
                                  , string_type& http_status_description_brief) {
    switch ((rpc_error < 0) ? -rpc_error : rpc_error) {
    case 0:
      http_status_code = 200;
      error_sym = pString("OK");
      http_status_description_brief = pString("Processed.");
      break;
    case EPERM:
      http_status_code = 401;
      error_sym = pString("EPERM");
      http_status_description_brief = pString("Operation not permitted.");
      break;
    case EEXIST:
      http_status_code = 409;
      error_sym = pString("EEXIST");
      http_status_description_brief = pString("Already exists.");
      break;
    case ENOTDIR:
      http_status_code = 409;
      error_sym = pString("ENOTDIR");
      http_status_description_brief = pString("Not a directory");
      break;
    case ENOENT:
      http_status_code = 404;
      error_sym = pString("ENOENT");
      http_status_description_brief = pString("Not found.");
      break;
    case EHOSTDOWN:
      http_status_code = 504;
      error_sym = pString("EHOSTDOWN");
      http_status_description_brief = pString("Unable to contact TRQ.");
      break;
    case EINVAL:
      http_status_code = 400;
      error_sym = pString("EINVAL");
      http_status_description_brief = pString("Invalid request.");
      break;
    case ENOTEMPTY:
      http_status_code=409;
      error_sym = pString("ENOTEMPTY");
      http_status_description_brief = pString("Directory not empty.");
      break;
    case EPROTO:
      http_status_code=500;
      error_sym = pString("EPROTO");
      http_status_description_brief = pString("Internal protocol error.");
      break;
    case EUNATCH:
      http_status_code=500;
      error_sym = pString("EUNATCH");
      http_status_description_brief = pString("Protocol driver not attached.");
      break;
    default:
      std::stringstream ss;
      ss << "RPCToHTTPError: Unknown server error (" << rpc_error << ")";
      std::cerr << ss.str() << std::endl;
      http_status_code = 500;
      error_sym = pString("EREMOTEIO");
      http_status_description_brief.append(ss.str());
    }
  }

  cncSession::host_ptr cncSession::TrqHost(const frqu::frqu_trq_id_type& trq_id) {
    name_ptr name = pcSessionExport::self().HostnameByTrqID(trq_id);
    // TODO  keep a local TRQID-host_ptr association.
    return cSession::self().Host(name);
  }

  cncSession::host_ptr cncSession::TrqHost(const pString& trq_id) {
    return TrqHost(pcSessionExport::self().StringAsUInt64(trq_id));
  }

} // namespace 'cnc'
