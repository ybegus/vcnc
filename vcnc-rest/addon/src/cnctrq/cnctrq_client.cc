//
//  Taken from https://nodejs.org/api/addons#addons_wrapping_c_objects
//

#include <node.h>
#include "cnctrq/cnctrqClient.h"

using v8::Local;
using v8::Object;

namespace cnc {
  namespace cnctrq {

    void InitAll(Local<Object> exports) {
      //
      //  Statically initialize the root extension object.
      //
      cnc::cnctrq::cnctrqClient::Init(exports);
      //
      //  Create a new of whatever we associated with the constructor (the V8
      //  function template from ToTrqspace::Init() ) and set as the value
      //  of module.exports
      //
      // module->Set(Nan::New("exports").ToLocalChecked(),
      //            Nan::New<v8::FunctionTemplate>(CreateObject)->GetFunction());
    }

  } // namespace 'cnctrq'
} // namespace 'cnc'

NODE_MODULE(cnctrq_client, cnc::cnctrq::InitAll)
