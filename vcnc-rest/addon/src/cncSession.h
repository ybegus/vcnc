/*
 *	Copyright (C) 2015 IC Manage Inc.
 *
 *	See the file 'COPYING' for license information
 */
#if	!defined(cncSession_H)
#define	cncSession_H
#include <lib/pString.H>
#include <lib/pSession.H>
#include <lib/pSingleton.H>
#include <peer/cHost.H>
#include <frquCore/frquStd.H>

using namespace pepsis::lib;
using pepsis::peer::cHost;

namespace cnc {

  struct cncSession : public pSessionBase {

    typedef pString string_type;
    typedef pSmart<cHost> host_ptr;
    cncSession();
    ~cncSession();
      
    ///
    ///  \return Singleton access.
    ///
    static cncSession &self() { return (pSingleton<cncSession>::self()); }
    ///
    ///  Applies the environmental settings that belong to us.
    ///
    void Apply();
    ///
    ///  Clean up on exit.
    ///
    void Final();
    ///
    ///  A utility to convert RPC error codes into HTTP error codes
    ///  and descriptions.
    ///
    ///  devnote:  In the context of a successfully processed RPC, use
    ///    pcSessionExport::ErrorCodeAsText to interpret an RPC error. If the
    ///    processing failed to successfully complete an RPC call, report that
    ///    fact as an HTTP error using cncSession::RPCToHTTPError.
    ///
    void RpcToHttpStatus(const int& rpc_error
                        , int& http_status_code
                        , string_type& error_sym
                        , string_type& http_status_description_brief);

    host_ptr TrqHost(const frqu::frqu_trq_id_type& trq_id);

    host_ptr TrqHost(const pString& trq_id);
  
                             
    //
  };
} // namespace 'cnc'

#endif 
