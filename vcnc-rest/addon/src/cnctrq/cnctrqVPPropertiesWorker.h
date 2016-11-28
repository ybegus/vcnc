#ifndef CNC_CNCTRQ_CNCTRQ_VPPROPERTIES_WORKER_H
#define CNC_CNCTRQ_CNCTRQ_VPPROPERTIES_WORKER_H

#include <nan.h>
#include <lib/pString.H>
#include <prtcl/cnctrq/cnctrqClient.H>
#include <prtcl/core/pcSessionExport.H>
#include <frquCore/frquStd.H>
#include <prtcl/iCncTrq.H>

using v8::Array;
using v8::Local;
using v8::Object;
using v8::Value;
using frqu::prtcl::cnctrq::cnctrqClient;
using frqu::prtcl::core::pcSessionExport;
using frqu::path_type;
using frqu::frqu_trq_id_type;

using namespace pepsis::lib;

namespace cnc {
  namespace cnctrq {

    // https://github.com/nodejs/nan/blob/master/examples/async_pi_estimate/async.cc
    class cnctrqVPPropertiesWorker 
      : public Nan::AsyncWorker {
    public:
      //
      //  Constructor.
      //
      cnctrqVPPropertiesWorker(int trq_id
                               , pString  vp_id
                               , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqVPPropertiesWorker();
      //
      //  Execute
      //
      void Execute();
      //
      //  HandleOKCallback
      //
      void HandleOKCallback();
      //
    private:
      //
      //  Inputs to the RPC
      //
      pString  _vp_id;
      //
      //  Results of the RPC
      //
      frqu_trq_id_type _pm_id;  
      string_type _pm_hostname;
      path_type _user;
      path_type _workspace;
      string_type _vp_hostname;
      path_type _vp_mount_path;
      int _uid;  // fake for now.  See <B9510>
      int _gid;  // fake for now.  See <B9510>
      
      //
      //  The iCncTrq RPC error code (from errno.h)
      //
      int _errcode;
      //
      //  The CncTrq RPC client object.
      //
      cnctrqClient* _client;
      //
    };
  } // namespace 'cnctrq'
} // namespace 'cnc'
#endif
