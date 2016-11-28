// //  Taken from nodejs/node-addon-examples/7_factory_wrap/nan
//

#include <iostream>
#include <stdlib.h>

#include <cncSession.h>
#include <cnctrq/cnctrqClient.h>
#include <cnctrq/cnctrqConsistencyWorker.h>
#include <cnctrq/cnctrqDeleteWorker.h>
#include <cnctrq/cnctrqGetattrWorker.h>
#include <cnctrq/cnctrqListWorker.h>
#include <cnctrq/cnctrqMetaCopyWorker.h>
#include <cnctrq/cnctrqMkdirWorker.h>
#include <cnctrq/cnctrqShutdownWorker.h>
#include <cnctrq/cnctrqVPFindWorker.h>
#include <cnctrq/cnctrqVPPropertiesWorker.h>
#include <cnctrq/cnctrqWorkspaceWorker.h>
#include <rev/Rev.H>

#include <lib/pStd.H>
#include <frquCore/frquStd.H>

#include <iostream> // only for development

using node::ObjectWrap;

using v8::Array;
using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

using pepsis::lib::pVector;
using frqu::path_type;
using pepsis::lib::pSession;

namespace cnc {
  namespace cnctrq {

    Persistent<Function> cnctrqClient::constructor;

    //
    //  Constructor
    //
    cnctrqClient::cnctrqClient() { /* intentionally empty */ }
    //
    //  Destructor
    //
    cnctrqClient::~cnctrqClient() { /* intentionally empty */ }

    void cnctrqClient::Init(Local<Object> exports) {
      Isolate* isolate = exports->GetIsolate();
      //
      //  Create the V8 template from the ::New static method.
      //
      Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
      tpl->SetClassName(String::NewFromUtf8(isolate, "cnctrqClient"));
      //  http://stackoverflow.com/questions/16600735/what-is-an-internal-field-count-and-what-is-setinternalfieldcount-used-for
      //  There are 17 methods, so I added one just to be safe.  
      tpl->InstanceTemplate()->SetInternalFieldCount(18); // why is this needed?
      //
      //  Configure the methods.
      //
      NODE_SET_PROTOTYPE_METHOD(tpl, "call_me_first", CallMeFirst);
      NODE_SET_PROTOTYPE_METHOD(tpl, "consistency_get", ConsistencyGet);
      NODE_SET_PROTOTYPE_METHOD(tpl, "consistency_set", ConsistencySet);
      NODE_SET_PROTOTYPE_METHOD(tpl, "remove", Delete);
      NODE_SET_PROTOTYPE_METHOD(tpl, "getattr", Getattr);
      NODE_SET_PROTOTYPE_METHOD(tpl, "list", List);
      NODE_SET_PROTOTYPE_METHOD(tpl, "meta_copy", MetaCopy);
      NODE_SET_PROTOTYPE_METHOD(tpl, "mkdir", Mkdir);
      NODE_SET_PROTOTYPE_METHOD(tpl, "shutdown_vtrq", ShutdownTRQ);
      NODE_SET_PROTOTYPE_METHOD(tpl, "vp_disconnect", VPDisconnect);
      NODE_SET_PROTOTYPE_METHOD(tpl, "vp_find", VPFind);
      NODE_SET_PROTOTYPE_METHOD(tpl, "vp_properties", VPProperties);
      NODE_SET_PROTOTYPE_METHOD(tpl, "workspace_children", WorkspaceChildren);
      NODE_SET_PROTOTYPE_METHOD(tpl, "workspace_delete", WorkspaceDelete);
      NODE_SET_PROTOTYPE_METHOD(tpl, "workspace_get", WorkspaceGet);
      NODE_SET_PROTOTYPE_METHOD(tpl, "workspace_put", WorkspacePut);
      NODE_SET_PROTOTYPE_METHOD(tpl, "workspace_set", WorkspaceSet);
      //
      //  Associate the C++ class with the JavaScript constructor
      //
      constructor.Reset(isolate, tpl->GetFunction());
      exports->Set(String::NewFromUtf8(isolate, "CnctrqClient"), tpl->GetFunction());
    }

    //
    //  JavaScript Constructor (using 'new')
    //
    void cnctrqClient::New(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Fetch the constructor arguments
      //
      if (args.Length() != 0) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "No arguments expected to constructor.")));
        return;
      }
      if (!args.IsConstructCall()) {
        // Invoked as plain function `cnctrqClient(...)`.
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Must by constructed by 'new'.")));
        return;
    }
      // Invoked as constructor: `new cnctrqClient(...)`
      cnctrqClient* obj = new cnctrqClient();
      obj->Wrap(args.This());
      args.GetReturnValue().Set(args.This());
    }

    void cnctrqClient::CallMeFirst(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 1) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected one argument: __dirname")));
        return;
      }
      //
      if (!args[0]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string first argument (__dirname)")));
        return;
      }
      pString app_dir(*String::Utf8Value(args[0]->ToString()));
      //
      //  Bring up pepsis
      //
      pEnv& env = pEnv::self();
      env.Who("vcnc",::vcnc_version_number(),::vcnc_release_change());
      cncSession::self(); // touch to begin session initialization.
      const int argc(2);
      //
      //  ... Verify the version number by printing it to console.
      //
      pSession::self().Info() << "vcnc  version " << env.Version().c_str()
                              << " build " << env.Change().c_str()
                              << pEndl;
      //
      //  ... Locate the configuration file.
      //
      const char* env_path = getenv("VELSTOR_VCNC_CONFIG");
      pString config_spec("--config=");
      if (env_path) {
        config_spec.append(env_path);
      } else {
        config_spec.append(app_dir);
        config_spec.append("/config/vcnc-pepsis.conf");
      }
      pSession::self().Info() << "vcnc using  " << config_spec << pEndl;
      //
      //  ... Construct fake command line arguments.
      //
      const char* argv[argc] = {
        "vcnc"
        , config_spec.c_str() };
      env.Switches(argc, argv);
      if (env.Status().Error()) {
        pSession::self().Error()
          << "Failed to find or process configuration file: "
          << config_spec
          << pEndm;
        exit(1);
      }
      if (env.Status().Exit()) {
        pSession::self().Error()
          << "Unexpected result of processing configuration file: "
          << config_spec
          << pEndm;
        exit(2);
      }
    }

    void cnctrqClient::ConsistencyGet(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, path, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqConsistencyWorker(trq_id
                                                        , path
                                                        , new Nan::Callback(callback)));

    }

    void cnctrqClient::ConsistencySet(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 4) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected four arguments: trq_id, path, consistency value, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      string_type value(*String::Utf8Value(args[2]->ToString()));
      //
      if (!args[3]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fourth argument.")));
        return;
      }
      Local<Function> callback = args[3].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqConsistencyWorker(trq_id
                                                        , path
                                                        , value
                                                        , new Nan::Callback(callback)));

    }

    void cnctrqClient::Delete(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 4) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected four arguments: trq_id, paths, recursive, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      //
      if (!args[1]->IsArray()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected array second argument (paths)")));
        return;
      }
      pVector<path_type> paths;
      Local<Array> v8_paths = Local<Array>::Cast(args[1]);
      const int length(v8_paths->Length());
      for (int i = 0; i < length; ++i) {
        if (!v8_paths->Get(i)->IsString()) {
          isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected paths array values to be strings")));
          return;
        }
        Local<String> path = Local<String>::Cast(v8_paths->Get(i));
        //
        //  Extract the C++ values of the array element and save them
        //  to pass to the AsyncQueueWorker
        //
        paths.push_back(path_type(*String::Utf8Value(path->ToString())));
      }
      //
      if (!args[2]->IsBoolean()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected boolean third argument (recursive)")));
        return;
      }
      bool recursive = args[2]->ToBoolean()->BooleanValue();
      //
      if (!args[3]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fourth argument.")));
        return;
      }
      Local<Function> callback = args[3].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqDeleteWorker(trq_id
                                                   , paths
                                                   , recursive
                                                   , new Nan::Callback(callback)));
    }

    void cnctrqClient::Getattr(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, path, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqGetattrWorker(trq_id
                                                    , path
                                                    , new Nan::Callback(callback)));

    }

    void cnctrqClient::List(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, path, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqListWorker(trq_id
                                                 , path
                                                 , new Nan::Callback(callback)));

    }

#if 0
    void cnctrqClient::ConsistencySet(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 4) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected four arguments: trq_id, path, consistency value, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      string_type value(*String::Utf8Value(args[2]->ToString()));
      //
      if (!args[3]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fourth argument.")));
        return;
      }
      Local<Function> callback = args[3].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqConsistencyWorker(trq_id
                                                        , path
                                                        , value
                                                        , new Nan::Callback(callback)));

    }
#endif
    void cnctrqClient::MetaCopy(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 4) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected four arguments: trq_id, src, dest, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsBoolean()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected boolean second argument (overwrite)")));
        return;
      }
      bool overwrite = args[1]->ToBoolean()->BooleanValue();
      //
      if (!args[2]->IsArray()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected array third argument (paths)")));
        return;
      }
      pVector<path_type> srcs;
      pVector<path_type> dests;
      Local<Array> paths = Local<Array>::Cast(args[2]);
      const int length(paths->Length());
      for (int i = 0; i < length; ++i) {
        if (!paths->Get(i)->IsObject()) {
          isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected paths array values to be objects")));
          return;
        }
        Local<Object> obj = Local<Object>::Cast(paths->Get(i));
        Nan::MaybeLocal<Value> mays = Nan::Get(obj
                                               , Nan::New("src").ToLocalChecked());
        Nan::MaybeLocal<Value> mayd = Nan::Get(obj
                                               , Nan::New("dest").ToLocalChecked());
        //
        //  Extract and validate this array element.
        //
        Local<Value> s ;
        Local<Value> d ;
        if (!mays.ToLocal<Value>(&s) || !mayd.ToLocal<Value>(&d)) {
          isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "'src' or 'dest' member name expected.")));
          return;
        }
        if (!s->IsString() || !d->IsString()) {
          isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "'src' and 'dest' are strings.")));
          return;
        }
        //
        //  Extract the C++ values of the array element and save them
        //  to pass to the AsyncQueueWorker
        //
        srcs.push_back(path_type(*String::Utf8Value(s->ToString())));
        dests.push_back(path_type(*String::Utf8Value(d->ToString())));
      }
      //
      if (!args[3]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fourth argument.")));
        return;
      }
      Local<Function> callback = args[3].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqMetaCopyWorker(trq_id
                                                     , overwrite
                                                     , srcs
                                                     , dests
                                                     , new Nan::Callback(callback)));

    }

    void cnctrqClient::Mkdir(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 5) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected five arguments: trq_id, path, mode, parents, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      if (!args[2]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int mode = args[2]->ToNumber()->NumberValue();
      //
      if (!args[3]->IsBoolean()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected boolean fourth argument (push)")));
        return;
      }
      bool parents(args[3]->ToBoolean()->BooleanValue());
      //
      if (!args[4]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fifth argument.")));
        return;
      }
      Local<Function> callback = args[4].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqMkdirWorker(trq_id
                                                  , path
                                                  , mode
                                                  , parents
                                                  , new Nan::Callback(callback)));
    }

    void cnctrqClient::ShutdownTRQ(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 2) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected two arguments: trq_id and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the second argument.")));
        return;
      }
      Local<Function> callback = args[1].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqShutdownWorker(trq_id
                                                     , new Nan::Callback(callback)));

    }

    void cnctrqClient::VPDisconnect(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      isolate->ThrowException(
        Exception::TypeError(
          String::NewFromUtf8(isolate, "cnctrqClient.cc: VPDisconnect not yet implemented.")));
    }

    void cnctrqClient::VPFind(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 4) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected four arguments: trq_id, vp_hostname, mount_point, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      pString vp_hostname;
      if (args[1]->IsString()) {
        vp_hostname = *String::Utf8Value(args[1]->ToString());
      } else if (args[1]->IsUndefined() || args[1]->IsNull()) {
        ; // intentionally empty
      } else {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (vp hostname)")));
        return;
      }
      //
      path_type mount_point;
      if (args[2]->IsString()) {
        mount_point = *String::Utf8Value(args[2]->ToString());
      } else if (args[2]->IsUndefined() || args[2]->IsNull()) {
        ; // intentionally empty
      } else {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string third argument (vp mount path)")));
        return;
      }
      //
      if (!args[3]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fourth argument.")));
        return;
      }
      Local<Function> callback = args[3].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqVPFindWorker(trq_id
                                                   , vp_hostname
                                                   , mount_point
                                                   , new Nan::Callback(callback)));
    }

    void cnctrqClient::VPProperties(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, vp_id, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (vp id)")));
        return;
      }
      pString vp_id(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqVPPropertiesWorker(trq_id
                                                         , vp_id
                                                         , new Nan::Callback(callback)));
    }

    void cnctrqClient::WorkspaceChildren(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, path, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqWorkspaceWorker(trq_id
                                                      , path
                                                      , cnctrqWorkspaceWorker::op_children
                                                      , new Nan::Callback(callback)));
    }

    void cnctrqClient::WorkspaceDelete(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, path, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqWorkspaceWorker(trq_id
                                                      , path
                                                      , cnctrqWorkspaceWorker::op_delete
                                                      , new Nan::Callback(callback)));
    }

    void cnctrqClient::WorkspaceGet(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 3) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected three arguments: trq_id, path, and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the third argument.")));
        return;
      }
      Local<Function> callback = args[2].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqWorkspaceWorker(trq_id
                                                      , path
                                                      , cnctrqWorkspaceWorker::op_get
                                                      , new Nan::Callback(callback)));
    }

    void cnctrqClient::WorkspacePut(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 5) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected five arguments: trq_id, path, specification, push and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string third argument (spec)")));
        return;
      }
      string_type spec(*String::Utf8Value(args[2]->ToString()));
      //
      //
      if (!args[3]->IsBoolean()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected boolean fourth argument (push)")));
        return;
      }
      bool push(args[3]->ToBoolean()->BooleanValue());
      //
      if (!args[4]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the fifth argument.")));
        return;
      }
      Local<Function> callback = args[4].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqWorkspaceWorker(trq_id
                                                      , path
                                                      , spec
                                                      , true // overwrite
                                                      , push
                                                      , cnctrqWorkspaceWorker::op_set
                                                      , new Nan::Callback(callback)));
    }

    void cnctrqClient::WorkspaceSet(const FunctionCallbackInfo<Value>& args) {
      Isolate* isolate = args.GetIsolate();
      //
      //  Get the argument values
      //
      if (args.Length() < 6) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected six arguments: trq_id, path, specification, overwrite, push and callback")));
        return;
      }
      //
      if (!args[0]->IsNumber()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected integer first argument (trq_id)")));
        return;
      }
      int trq_id = args[0]->ToNumber()->NumberValue();
      //
      if (!args[1]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string second argument (path)")));
        return;
      }
      path_type path(*String::Utf8Value(args[1]->ToString()));
      //
      if (!args[2]->IsString()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected string third argument (spec)")));
        return;
      }
      string_type spec(*String::Utf8Value(args[2]->ToString()));
      //
      if (!args[3]->IsBoolean()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected boolean fourth argument (overwrite)")));
        return;
      }
      bool overwrite(args[3]->ToBoolean()->BooleanValue());
      //
      if (!args[4]->IsBoolean()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected boolean fifth argument (push)")));
        return;
      }
      bool push(args[4]->ToBoolean()->BooleanValue());
      //
      if (!args[5]->IsFunction()) {
        isolate->ThrowException(
          Exception::TypeError(
            String::NewFromUtf8(isolate, "Expected callback function as the sixth argument.")));
        return;
      }
      Local<Function> callback = args[5].As<Function>();
      //
      //  Prepare the async worker
      //
      Nan::AsyncQueueWorker(new cnctrqWorkspaceWorker(trq_id
                                                      , path
                                                      , spec
                                                      , overwrite
                                                      , push
                                                      , cnctrqWorkspaceWorker::op_set
                                                      , new Nan::Callback(callback)));
    }

  } // namespace 'cnctrq'
} // namespace 'cnc'
