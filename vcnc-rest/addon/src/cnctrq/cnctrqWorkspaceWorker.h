#pragma once

#include <nan.h>
#include <lib/pString.H>
#include <prtcl/cnctrq/cnctrqClient.H>
#include <prtcl/core/pcSessionExport.H>
#include <frquCore/frquStd.H>
#include <prtcl/core/pidlWorkspace.H>
#include <prtcl/iCncTrq.H>

using v8::Array;
using v8::Local;
using v8::Object;
using v8::Value;
using frqu::prtcl::cnctrq::cnctrqClient;
using frqu::prtcl::core::pcSessionExport;
using frqu::prtcl::pidlWorkspace;
using frqu::path_type;
using frqu::string_type;
using frqu::prtcl::iCncTrq;

using namespace pepsis::lib;

namespace cnc {
  namespace cnctrq {

    // https://github.com/nodejs/nan/blob/master/examples/async_pi_estimate/async.cc
    class cnctrqWorkspaceWorker 
      : public Nan::AsyncWorker {
    public:
      enum {
        // values are tied to ./routes/v1.js
        op_children = 0,
        op_delete = 1,
        op_get = 2,
        op_set = 3
      };
      //
      //  Constructor for 'getting'.
      //
      cnctrqWorkspaceWorker(int trq_id
                            , path_type  path
                            , int operation
                            , Nan::Callback* callback);
      //
      //  Constructor for 'setting'.
      //
      cnctrqWorkspaceWorker(int trq_id
                            , path_type  path
                            , pString spec
                            , bool overwrite
                            , bool push
                            , int operation
                            , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqWorkspaceWorker();
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
      string_type _value;
      string_type _input_spec;
      bool _overwrite;
      bool _push;
      int _operation;
      //
      //  Outputs from the RPC
      pidlWorkspace _workspace;
      iCncTrq::string_list_t _children;
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

