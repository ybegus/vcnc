#include <cnctrq/cnctrqVPPropertiesWorker.h>
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

using pepsis::lib::pString;
using frqu::prtcl::core::pcSessionExport;
using cnc::cncSession;

typedef pepsis::peer::cName::name_ptr name_ptr;

namespace cnc {
  namespace cnctrq {

    cnctrqVPPropertiesWorker::cnctrqVPPropertiesWorker(int trq_id
                                                       , pString  vp_id
                                                       , Nan::Callback* callback) 
      : AsyncWorker(callback)
      , _vp_id(vp_id)
      , _uid(0), _gid(0) // fake for now.  See <B9510>
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
    cnctrqVPPropertiesWorker::~cnctrqVPPropertiesWorker() { delete _client; }
    //
    //  Execute
    //
    void cnctrqVPPropertiesWorker::Execute() {
      if (_errcode) 
        return;  // An error has occurred before we were called.
      _errcode = _client->VPProperties(pcSessionExport::self().StringAsUInt64(_vp_id)
                                       , _pm_id
                                       , _pm_hostname
                                       , _user
                                       , _workspace
                                       , _vp_hostname
                                       , _vp_mount_path);
    }
    //
    //  HandleOKCallback
    //
    void cnctrqVPPropertiesWorker::HandleOKCallback() {
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
      //    "gid": 101,
      //    "hostname": "mercury"
      //    "mount_point": "/path/to/mount/point",
      //    "pm_id": "0x7234567890ABCDEF0"
      //    "uid": 101,
      //    "user": "/marketing/sally",
      //    "workspace": "/some/workspace/name"
      //    "vp_id": "0x1234567890ABCDEF0"
      //  }
      //

      Nan::Set(rtn
               , Nan::New("gid").ToLocalChecked()
               , Nan::New(_gid));
      Nan::Set(rtn
               , Nan::New("hostname").ToLocalChecked()
               , Nan::New(_vp_hostname.c_str()).ToLocalChecked());
      Nan::Set(rtn
               , Nan::New("mount_point").ToLocalChecked()
               , Nan::New(_vp_mount_path().c_str()).ToLocalChecked());
      pString pm_id = pcSessionExport::self().UInt64AsString(_pm_id);
      Nan::Set(rtn
               , Nan::New("pm_id").ToLocalChecked()
               , Nan::New(pm_id.c_str()).ToLocalChecked());
      Nan::Set(rtn
               , Nan::New("uid").ToLocalChecked()
               , Nan::New(_uid));
      Nan::Set(rtn
               , Nan::New("user").ToLocalChecked()
               , Nan::New(_user().c_str()).ToLocalChecked());
      Nan::Set(rtn
               , Nan::New("workspace").ToLocalChecked()
               , Nan::New(_workspace().c_str()).ToLocalChecked());
      Nan::Set(rtn
               , Nan::New("vp_id").ToLocalChecked()
               , Nan::New(_vp_id.c_str()).ToLocalChecked());
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
