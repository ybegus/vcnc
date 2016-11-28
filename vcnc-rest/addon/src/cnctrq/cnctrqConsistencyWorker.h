#pragma once

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
using frqu::frqu_proj_kind_type;
using frqu::path_type;
using frqu::string_type;

using namespace pepsis::lib;

namespace cnc {
  namespace cnctrq {

    // https://github.com/nodejs/nan/blob/master/examples/async_pi_estimate/async.cc
    class cnctrqConsistencyWorker 
      : public Nan::AsyncWorker {
    public:
      //
      //  Constructor for 'getting'.
      //
      cnctrqConsistencyWorker(int trq_id
                              , path_type  path
                              , Nan::Callback* callback);
      //
      //  Constructor for 'setting'.
      //
      cnctrqConsistencyWorker(int trq_id
                              , path_type  path
                              , pString value
                              , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqConsistencyWorker();
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
      pString _value;
      //
      //  Outputs from the RPC
      frqu_proj_kind_type _consistency;
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

