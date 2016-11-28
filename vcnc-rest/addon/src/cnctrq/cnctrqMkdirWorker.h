#ifndef CNC_CNCTRQ_CNCTRQ_MKDIR_WORKER_H
#define CNC_CNCTRQ_CNCTRQ_MKDIR_WORKER_H

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
    class cnctrqMkdirWorker 
      : public Nan::AsyncWorker {
    public:
      //
      //  Constructor.
      //
      cnctrqMkdirWorker(int trq_id
                        , path_type  path
                        , int mode
                        , bool _parents
                        , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqMkdirWorker();
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
      path_type  _path;
      int _mode;
      bool _parents;
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
