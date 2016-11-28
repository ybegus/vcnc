#include <cnctrq/cnctrqDeleteWorker.h>
#include <prtcl/core/pcSessionExport.H>
#include <cncSession.h>
#include <peer/cSession.H>
#include <sys/stat.h>
#include <prtcl/iCncTrq.H>

#include <iostream>

using v8::Array;
using v8::Context;
using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Local;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

using cnc::cncSession;
using pepsis::peer::cSession;
using frqu::prtcl::core::pcSessionExport;
using frqu::prtcl::cnctrq::cnctrqClient;

typedef pepsis::peer::cName::name_ptr name_ptr;
typedef pepsis::peer::cHost::host_ptr host_ptr;

namespace cnc {
  namespace cnctrq {

    cnctrqDeleteWorker::cnctrqDeleteWorker(int trq_id
                                           , pVector<path_type> paths
                                           , bool recursive
                                           , Nan::Callback* callback)
      : AsyncWorker(callback)
      , _errcode(0)
      , _client(0)
      , _recursive(recursive)
      , _root("/")
    {
      //
      //  Prepare the result vector.
      //
      const int size(paths.size());
      _results.reserve(size);
      for (int i = 0; i < size; ++i) {
        _results.push_back(result_element_t(paths[i]));
      }
      //
      //  Obtain the TRQ's hostname from its TRQ ID.
      //
      name_ptr trq_hostname
        = pcSessionExport::self().HostnameByTrqID(trq_id);
      if (!trq_hostname) {
        _errcode = -EHOSTDOWN;
        return;
      }
      //
      //  Get the associated cHost, and check if a channel
      //  to the host can be created.  Eventually we will have central
      //  cHost management.  (HostByTrqID)
      //
      host_ptr trq_host = cSession::self().Host(trq_hostname);
      if (!trq_host  || !trq_host->Connect()) {
        _errcode = -EHOSTDOWN;
        return;
      }
      //
      //
      //  Create the RPC client
      //
      _client = new cnctrqClient(trq_host);
    }
    //
    //  Destructor.
    //
    cnctrqDeleteWorker::~cnctrqDeleteWorker() { delete _client; }
    //
    //  Execute
    //
    void cnctrqDeleteWorker::Execute() {
      if (_errcode)
        return;  // An error has occurred before we were called.
      for (results_t::iterator itr = _results.begin()
             ; itr != _results.end()
             ; ++itr) {
        itr->_errcode = _client->Delete(itr->_path, _recursive);
      }
    }
    //
    //  HandleOKCallback
    //
    void cnctrqDeleteWorker::HandleOKCallback() {
      //
      //  Elaborate the error code.
      //
      int http_status;
      string_type error_sym;
      string_type error_description_brief;
      cncSession::self().RpcToHttpStatus(_errcode
                                        , http_status
                                        , error_sym
                                        , error_description_brief);
      //
      //  Allocate the return object
      //
      Local<Object> rtn = Nan::New<Object>();
      Nan::Set(rtn
               , Nan::New("errcode").ToLocalChecked()
               , Nan::New(_errcode));
      Nan::Set(rtn
               , Nan::New("http_status").ToLocalChecked()
               , Nan::New(http_status));
      Nan::Set(rtn
               , Nan::New("error_sym").ToLocalChecked()
               , Nan::New(error_sym.c_str()).ToLocalChecked());
      Nan::Set(rtn
               , Nan::New("error_description_brief").ToLocalChecked()
               , Nan::New(error_description_brief.c_str()).ToLocalChecked());
      //
      //  Marshall the results
      //  {
      //    "result": [
      //      {
      //        "error_sym": "OK",
      //        "path": "/path/to/be/deleted",
      //      }
      //    ]
      //  }
      //
      Local<Array> result = Nan::New<Array>(_results.size());
      int i(0);
      for (results_t::iterator itr = _results.begin()
             ; itr != _results.end()
             ; ++i, ++itr) {
        Local<Object> elem = Nan::New<Object>();
        int idummy;
        pString dummy;
        pString error_sym;
        cncSession::self().RpcToHttpStatus(itr->_errcode
                                          , idummy
                                          , error_sym
                                          , dummy);
        Nan::Set(elem
                 , Nan::New("error_sym").ToLocalChecked()
                 , Nan::New(error_sym.c_str()).ToLocalChecked());
        Nan::Set(elem
                 , Nan::New("path").ToLocalChecked()
                 , Nan::New(itr->_path().c_str()).ToLocalChecked());
        Nan::Set(result, i, elem);
      }
      Nan::Set(rtn
               , Nan::New("result").ToLocalChecked()
               , result);
      //
      //  Prepare for callback
      //
      const unsigned argc = 1;
      Local<Value> argv[argc] = { rtn };
      callback->Call(argc, argv);
    }
    //
  } // namespace 'cnctrq'
} // namespace 'cnc'
