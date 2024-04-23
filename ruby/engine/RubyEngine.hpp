/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RUBYENGINE_included
#define RUBYENGINE_included

// TODO:
// #include <rubyengine_export.h>  // from generate_export_header(rubyengine)

#include <ScriptEngine.hpp>
#include <ScriptEngineAPI.hpp>
#include <RubyInterpreter.hpp>

namespace openstudio {

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

  virtual void setupEmbeddedGems(const std::vector<openstudio::path>& includeDirs, const std::vector<openstudio::path>& gemPathDirs,
                                 const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath,
                                 const openstudio::path& bundleGemDirPath, const std::string& bundleWithoutGroups) override;

  virtual std::string inferMeasureClassName(const openstudio::path& measureScriptPath) override;

  virtual ScriptObject loadMeasure(const openstudio::path& measureScriptPath, std::string_view className) override;

  virtual int numberOfArguments(ScriptObject& methodObject, std::string_view methodName) override;

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;

  bool getAs_impl_bool(ScriptObject& obj) override;
  int getAs_impl_int(ScriptObject& obj) override;
  double getAs_impl_double(ScriptObject& obj) override;
  std::string getAs_impl_string(ScriptObject& obj) override;

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
