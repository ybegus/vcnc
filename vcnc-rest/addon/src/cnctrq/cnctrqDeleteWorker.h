#ifndef CNC_CNCTRQ_CNCTRQ_DELETE_WORKER_H
#define CNC_CNCTRQ_CNCTRQ_DELETE_WORKER_H

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
using frqu::prtcl::core::pcSessionExport;
using frqu::path_type;

using namespace pepsis::lib;

namespace cnc {
  namespace cnctrq {

    // https://github.com/nodejs/nan/blob/master/examples/async_pi_estimate/async.cc
    class cnctrqDeleteWorker 
      : public Nan::AsyncWorker {
    public:
      //
      //  Constructor.
      //
      cnctrqDeleteWorker(int trq_id
                         , pVector<path_type> paths
                         , bool recursive
                         , Nan::Callback* callback);
      //
      //  Destructor.
      //
      ~cnctrqDeleteWorker();
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
      //  One element of the result vector.
      //
      struct result_element_t {
        result_element_t()
          : _errcode(-1) { }
        //
        result_element_t(const path_type& path)
          : _path(path)
          , _errcode(-1) { }
        //
        result_element_t(const path_type& path
                         , int errcode) 
          : _path(path)
          , _errcode(errcode) { }
        //
        path_type _path;  // the requested path to be deleted
        int _errcode; // The code returned by the TRQ RPC
      };
      typedef pVector<result_element_t> results_t;
      //
      //  Holds the results of the RPCs
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
      //  True if recursive delete is desired; false otherwise.
      //
      bool _recursive;
      //
      //  Recursively deletes a path on the TRQ.
      //
      int _delete_path(const path_type& path);
      //
      //  A useful constant string
      //
      path_type _root;
      //
    };
  } // namespace 'cnctrq'
} // namespace 'cnc'
#endif
