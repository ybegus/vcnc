#ifndef CNC_CNCTRQ_CNCTRQ_CLIENT_H
#define CNC_CNCTRQ_CNCTRQ_CLIENT_H

#include <node.h>
#include <node_object_wrap.h>

namespace cnc {
  namespace cnctrq {

    class cnctrqClient : public node::ObjectWrap {
    public:
      static void Init(v8::Local<v8::Object> exports);

    private:
      explicit cnctrqClient();
      ~cnctrqClient();

      //
      //  Define the methods of the extension object.
      //
      static v8::Persistent<v8::Function> constructor;
      static void New(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void CallMeFirst(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void ConsistencyGet(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void ConsistencySet(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void Delete(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void Getattr(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void List(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void MetaCopy(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void Mkdir(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void ShutdownTRQ(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void VPDisconnect(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void VPFind(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void VPProperties(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void WorkspaceChildren(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void WorkspaceDelete(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void WorkspaceGet(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void WorkspacePut(const v8::FunctionCallbackInfo<v8::Value>& info);
      static void WorkspaceSet(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
  } // namespace 'cnctrq'
} // namespace 'cnc'

#endif
