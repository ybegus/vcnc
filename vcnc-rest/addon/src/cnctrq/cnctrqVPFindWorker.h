#ifndef CNC_CNCTRQ_CNCTRQ_VPFIND_WORKER_H
#define CNC_CNCTRQ_CNCTRQ_VPFIND_WORKER_H

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

using namespace pepsis::lib;

namespace cnc {
  namespace cnctrq {

    // https://github.com/nodejs/nan/blob/master/examples/async_pi_estimate/async.cc
    class cnctrqVPFindWorker 
      : public Nan::AsyncWorker {
    public:
      //
      //  Constructor.
      //
      cnctrqVPFindWorker(int trq_id
                         , pString  vp_hostname
                         , path_type mount_point
                         , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqVPFindWorker();
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
      typedef frqu::prtcl::iCncTrq::vp_list_t results_t;
      pString  _vp_hostname;
      path_type _mount_point;
      //
      //  Holds the results of the RPC
      //
      results_t _results;
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
