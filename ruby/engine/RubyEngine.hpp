#ifndef RUBYENGINE_included
#define RUBYENGINE_included

// TODO:
// #include <rubyengine_export.h>  // from generate_export_header(rubyengine)

#include <ScriptEngine.hpp>
#include <ScriptEngineAPI.hpp>
#include <RubyInterpreter.hpp>

namespace openstudio {

namespace measure {
class OSMeasure;
}

class RubyEngine final : public ScriptEngine
{
 public:
  RubyEngine(int argc = 0, char* argv[] = nullptr);
  ~RubyEngine() override;

  RubyEngine(const RubyEngine&) = delete;
  RubyEngine(RubyEngine&&) = delete;
  RubyEngine& operator=(const RubyEngine&) = delete;
  RubyEngine& operator=(RubyEngine&&) = delete;

  ScriptObject eval(std::string_view sv) override;
  void exec(std::string_view sv) override;

  // Prefer this method, it is faster and less complex than the one where you infer the class name
  virtual measure::OSMeasure* loadMeasureKnownClassName(const openstudio::path& measureScriptPath, std::string_view className) override;

  virtual std::pair<std::string, measure::OSMeasure*> loadMeasureInferClassName(const openstudio::path& measureScriptPath) override;

  virtual void setupEmbeddedGems(const std::vector<openstudio::path>& includeDirs, const std::vector<openstudio::path>& gemPathDirs,
                                 const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath,
                                 const openstudio::path& bundleGemDirPath, const std::string& bundleWithoutGroups) override;

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;

  void initRubyEngine();
  std::vector<std::string> includePaths;
  RubyInterpreter rubyInterpreter{includePaths};
};

}  // namespace openstudio

extern "C"
{
  SCRIPTENGINE_API openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]);
}

#endif
