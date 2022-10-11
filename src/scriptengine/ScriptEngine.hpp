#ifndef SCRIPTENGINE_included
#define SCRIPTENGINE_included

#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/DynamicLibrary.hpp"
#include <any>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>

namespace openstudio {
class Measure;
class ScriptEngine;

struct ScriptObject
{
  std::any object;
};
}  // namespace openstudio

using ScriptEngineFactoryType = openstudio::ScriptEngine*(int, char**);

namespace openstudio {

class ScriptEngine
{
 public:
  ScriptEngine([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    registerType<openstudio::Measure*>("openstudio::Measure *");
  }

  virtual ~ScriptEngine() = default;
  ScriptEngine(const ScriptEngine&) = delete;
  ScriptEngine(ScriptEngine&&) = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;
  ScriptEngine& operator=(ScriptEngine&&) = delete;

  virtual ScriptObject eval(std::string_view sv) = 0;

  // execute string without expecting a return value
  virtual void exec(std::string_view sv) = 0;

  template <typename T>
  T getAs(ScriptObject& obj) {
    void* result = getAs_impl(obj, typeid(T));
    if (result) {
      return static_cast<T>(result);
    } else {
      throw std::bad_cast();
    }
  }

  template <typename T>
  void registerType(std::string name) {
    types.emplace(std::cref(typeid(T)), std::move(name));
  }

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  virtual void* getAs_impl(ScriptObject& obj, const std::type_info&) = 0;

  const std::string& getRegisteredTypeName(const std::type_info& type) {
    const auto& found_name = types.find(type);

    if (found_name != types.end()) {
      return found_name->second;
    }

    throw std::runtime_error("unknown type requested");
  }

 private:
  struct Compare
  {
    bool operator()(const std::reference_wrapper<const std::type_info>& lhs, const std::reference_wrapper<const std::type_info>& rhs) const {
      return lhs.get().before(rhs.get());
    }
  };

  std::map<std::reference_wrapper<const std::type_info>, std::string, Compare> types;
};

inline std::unique_ptr<openstudio::ScriptEngine> loadScriptEngine(std::string_view libraryBaseName, int argc, char* argv[]) {
  auto enginePath = openstudio::getOpenStudioModuleDirectory() / openstudio::getSharedLibraryName(libraryBaseName);
  openstudio::DynamicLibrary engineLib(enginePath);

  const std::function<ScriptEngineFactoryType> factory = engineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");

  return std::unique_ptr<openstudio::ScriptEngine>(factory(argc, argv));
}

}  // namespace openstudio

#endif
