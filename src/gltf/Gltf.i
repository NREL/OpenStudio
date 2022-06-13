#ifndef GLTF_I
#define GLTF_I

#ifdef SWIGPYTHON
%module openstudiogltf
#endif

#define UTILITIES_API
#define MODEL_API
#define GLTF_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>

%ignore openstudio::gltf::detail;

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
%import <model/Model.i>
#else
%import(module="openstudiomodel") <model/Model.hpp>
%import(module="openstudiomodel") <model/ModelObject.hpp>
#endif

// ignore specific overload of GltfForwardTranslator::modelToGLTF to avoid dealing with std::function<void(double)>updatePercentage
%ignore openstudio::gltf::GltfForwardTranslator::modelToGLTF(const model::Model& model, std::function<void(double)> updatePercentage, const path& outputPath);

%{
  #include <utilities/core/Path.hpp>
  #include <gltf/GltfBoundingBox.hpp>
//  #include <gltf/GltfMaterialData.hpp>
  #include <gltf/GltfModelObjectMetaData.hpp>
  #include <gltf/GltfMetaData.hpp>
  #include <gltf/GltfUserData.hpp>
  #include <gltf/GltfForwardTranslator.hpp>

  using namespace openstudio::gltf;
  using namespace openstudio;
%}

%include <gltf/GltfBoundingBox.hpp>
//%include <gltf/GltfMaterialData.hpp>
%include <gltf/GltfModelObjectMetaData.hpp>
%include <gltf/GltfMetaData.hpp>
%include <gltf/GltfUserData.hpp>
%include <gltf/GltfForwardTranslator.hpp>

// create an instantiation of the vector classes
%template(GltfUserDataVector) std::vector<openstudio::gltf::GltfUserData>;
%template(GltfModelObjectMetaDataVector) std::vector<openstudio::gltf::GltfModelObjectMetaData>;
// %template(GltfMaterialDataVector) std::vector<openstudio::gltf::GltfMaterialDataVector>;

%template(OptionalGltfUserData) boost::optional<openstudio::gltf::GltfUserData>;

#if defined SWIGRUBY
  // This isn't super clean and there might be a better way with typemaps in SWIG rather than using a String in between,
  // but still helpful I think: we redefine toJSON that will return a native ruby hash. 'json' is part of ruby stdlib since at least ruby 2.5.5
  %init %{
    rb_eval_string("OpenStudio::Gltf::GltfForwardTranslator.class_eval { define_method(:modelToGLTFJSON) { |model| json_str = self.modelToGLTFString(model); require 'json'; JSON.load(json_str);  }; }");
 %}
#endif

#if defined SWIGPYTHON

  // Let's use monkey-patching via unbound functions
  %pythoncode %{
# Manually added: Lazy-load the json module (python std lib) and return a dict via modelToGLTFString
def _modelToGLTFJSON(self, *args) -> dict:
    import json
    return json.loads(self.modelToGLTFString(*args))

GltfForwardTranslator.modelToGLTFJSON = _modelToGLTFJSON
  %}
#endif

#endif // GLTF_I


