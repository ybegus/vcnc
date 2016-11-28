#include <cnctrq/cnctrqConsistencyWorker.h>
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
using frqu::pk_semi_synchronous;
using frqu::pk_fully_synchronous;
using cnc::cncSession;

typedef pepsis::peer::cName::name_ptr name_ptr;

namespace cnc {
  namespace cnctrq {

    cnctrqConsistencyWorker::cnctrqConsistencyWorker(int trq_id
                                                     , path_type  path
                                                     , Nan::Callback* callback) 
      : AsyncWorker(callback)
      , _path(path)
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

    cnctrqConsistencyWorker::cnctrqConsistencyWorker(int trq_id
                                                     , path_type  path
                                                     , string_type value
                                                     , Nan::Callback* callback) 
      : AsyncWorker(callback)
      , _path(path)
      , _value(value)
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
    cnctrqConsistencyWorker::~cnctrqConsistencyWorker() { delete _client; }
    //
    //  Execute
    //
    void cnctrqConsistencyWorker::Execute() {
      if (_errcode) 
        return;  // An error has occurred before we were called.
      if (!_value) {
        _errcode = _client->GetProjKind(_path, _consistency);
        return;
      } 
      if (_value == "immediate")
        _errcode = _client->SetProjKind(_path, pk_fully_synchronous);
      else if (_value == "eventual")
        _errcode = _client->SetProjKind(_path, pk_semi_synchronous);
      else
        _errcode = -EINVAL;
    }
    //
    //  HandleOKCallback
    //
    void cnctrqConsistencyWorker::HandleOKCallback() {
      //
      //  Interpret the returned value
      //
      pString consistency;
      if (!_value && !_errcode) {  // we are 'getting' 
        switch (_consistency) {
        case pk_semi_synchronous: 
          consistency = pString("eventual");
          break;
        case pk_fully_synchronous:
          consistency = pString("immediate");
          break;
        default:
          _errcode = -EPROTO;
        }
      }
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
      std::cerr << "ConsistencyWorker:  "  << _value << " & " << _errcode << std::endl;
      if (!_value && !_errcode) {
        Nan::Set(rtn
                 , Nan::New("consistency").ToLocalChecked()
                 , Nan::New(consistency.c_str()).ToLocalChecked());
      }
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
