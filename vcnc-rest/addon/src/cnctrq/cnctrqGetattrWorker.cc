#include <cnctrq/cnctrqGetattrWorker.h>
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

    cnctrqGetattrWorker::cnctrqGetattrWorker(int trq_id
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
    //
    //  Destructor.
    //
    cnctrqGetattrWorker::~cnctrqGetattrWorker() { delete _client; }
    //
    //  Execute
    //
    void cnctrqGetattrWorker::Execute() {
      if (_errcode) 
        return;  // An error has occurred before we were called.
      _errcode = _client->Getattr(_path
                                  , _stat);
    }
    //
    //  HandleOKCallback
    //
    void cnctrqGetattrWorker::HandleOKCallback() {
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
      //
      //   {
      //     "error_sym": "OK",
      //     "stat": {
      //       "st_dev": "0x0000000000000000",
      //       "st_ino": "0x0000000000000000",
      //       "st_mode": 544,
      //       "st_nlink": "0x0000000000000000",
      //       "st_uid": 101,
      //       "st_gid": 102,
      //       "st_rdev": "0x0000000000000000",
      //       "st_size": "0x0000000000000000",
      //       "st_blksize": "0x0000000000000000",
      //       "st_blocks": "0x0000000000000000",
      //       "st_atime": "2015-09-10T11:32:50.522",
      //       "st_mtime": "2015-09-10T11:32:50.522",
      //       "st_ctime": "2015-09-10T11:32:50.522"
      //   }
      //
      Local<Object> stat = Nan::New<Object>();
      pString member;
      member = pcSessionExport::self().UInt64AsString(_stat.st_dev);
      Nan::Set(stat
               , Nan::New("st_dev").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().UInt64AsString(_stat.st_ino);
      Nan::Set(stat
               , Nan::New("st_ino").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      Nan::Set(stat
               , Nan::New("st_mode").ToLocalChecked()
               , Nan::New(_stat.st_mode));
      member = pcSessionExport::self().UInt64AsString(_stat.st_mode);
      Nan::Set(stat
               , Nan::New("st_mode").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      Nan::Set(stat
               , Nan::New("st_uid").ToLocalChecked()
               , Nan::New(_stat.st_uid));
      Nan::Set(stat
               , Nan::New("st_gid").ToLocalChecked()
               , Nan::New(_stat.st_gid));
      member = pcSessionExport::self().UInt64AsString(_stat.st_rdev);
      Nan::Set(stat
               , Nan::New("st_rdev").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().UInt64AsString(_stat.st_size);
      Nan::Set(stat
               , Nan::New("st_size").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().UInt64AsString(_stat.st_blksize);
      Nan::Set(stat
               , Nan::New("st_blksize").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().UInt64AsString(_stat.st_blocks);
      Nan::Set(stat
               , Nan::New("st_blocks").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().TimestampAsISO8601(_stat.st_atime);
      Nan::Set(stat
               , Nan::New("st_atime").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().TimestampAsISO8601(_stat.st_ctime);
      Nan::Set(stat
               , Nan::New("st_ctime").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      member = pcSessionExport::self().TimestampAsISO8601(_stat.st_mtime);
      Nan::Set(stat
               , Nan::New("st_mtime").ToLocalChecked()
               , Nan::New(member.c_str()).ToLocalChecked());
      Nan::Set(rtn
               , Nan::New("stat").ToLocalChecked()
               , stat);
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
