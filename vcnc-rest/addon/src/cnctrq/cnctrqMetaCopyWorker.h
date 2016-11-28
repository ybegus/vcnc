#ifndef CNC_CNCTRQ_CNCTRQ_METACOPY_WORKER_H
#define CNC_CNCTRQ_CNCTRQ_METACOPY_WORKER_H

#include <nan.h>
#include <lib/pString.H>
#include <prtcl/cnctrq/cnctrqClient.H>
#include <prtcl/core/pcSessionExport.H>
#include <frquCore/frquStd.H>

using v8::Array;
using v8::Local;
using v8::Object;
using v8::Value;
using frqu::prtcl::cnctrq::cnctrqClient;
using frqu::prtcl::cnctrq::pidlCncTrqCopyVectorEntry;
using frqu::prtcl::cnctrq::pidlCncTrqCopyVectorRequest;
using frqu::prtcl::cnctrq::pidlCncTrqCopyVectorResponse;
using frqu::prtcl::core::pcSessionExport;
using frqu::path_type;

using namespace pepsis::lib;

namespace cnc {
  namespace cnctrq {

    // https://github.com/nodejs/nan/blob/master/examples/async_pi_estimate/async.cc
    class cnctrqMetaCopyWorker 
      : public Nan::AsyncWorker {
    public:
      //
      //  Constructor.
      //
      cnctrqMetaCopyWorker(int trq_id
                           , bool overwrite
                           , pVector<path_type> srcs
                           , pVector<path_type> dests
                           , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqMetaCopyWorker();
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
      typedef pidlCncTrqCopyVectorRequest request_t;
      typedef pidlCncTrqCopyVectorResponse response_t;
      //
      //  Holds the RPC request message
      //
      request_t* _request;
      //
      //  Holds the RPC response message.
      //
      response_t _response;
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
