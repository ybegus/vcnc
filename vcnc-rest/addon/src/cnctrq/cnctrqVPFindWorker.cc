#include <cnctrq/cnctrqVPFindWorker.h>
#include <prtcl/core/pcSessionExport.H>
#include <cncSession.h>

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

using frqu::prtcl::core::pcSessionExport;
using cnc::cncSession;

typedef pepsis::peer::cName::name_ptr name_ptr;

namespace cnc {
  namespace cnctrq {

    cnctrqVPFindWorker::cnctrqVPFindWorker(int trq_id
                                           , pString  vp_hostname
                                           , path_type mount_point
                                           , Nan::Callback* callback) 
      : AsyncWorker(callback)
      , _vp_hostname(vp_hostname)
      , _mount_point(mount_point)
      , _errcode(0)
      , _client(0)
    {
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
      //  Create the RPC client
      //
      _client = new frqu::prtcl::cnctrq::cnctrqClient(trq_hostname);
    }
    //
    //  Destructor.
    //
    cnctrqVPFindWorker::~cnctrqVPFindWorker() { delete _client; }
    //
    //  Execute
    //
    void cnctrqVPFindWorker::Execute() {
      if (_errcode) 
        return;  // An error has occurred before we were called.
      _errcode = _client->VPFind(_vp_hostname, _mount_point, _results);
    }
    //
    //  HandleOKCallback
    //
    void cnctrqVPFindWorker::HandleOKCallback() {
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
      //    "error_sym": "OK",
      //    "vp_ids": [
      //      "0x1234567890ABCDEF0"
      //    ]
      //  }
      //
      Local<Array> vp_ids = Nan::New<Array>(_results.size());
      int i(0);
      for (results_t::iterator itr = _results.begin()
             ; itr != _results.end()
             ; ++itr, ++i) {
        //  encode the uint64 *itr into a
        pString vp_id = pcSessionExport::self().UInt64AsString(*itr);
        Nan::Set(vp_ids, i, Nan::New(vp_id.c_str()).ToLocalChecked());
      }
      Nan::Set(rtn
               , Nan::New("vp_ids").ToLocalChecked()
               , vp_ids);
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
