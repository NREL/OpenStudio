#ifndef MEASUREMANAGER_H
#define MEASUREMANAGER_H

#include "../utilities/core/Path.hpp"
#include "../scriptengine/ScriptEngine.hpp"

#include "../model/Model.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../measure/OSMeasureInfoGetter.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#if (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif
#define _TURN_OFF_PLATFORM_STRING  // cpprestsdk has an ugly macro U() that makes fmt break...
#include <cpprest/http_listener.h>
#if (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif

#include <map>
#include <string>

namespace Json {
class Value;
}

namespace openstudio {

struct OSMInfo
{
  std::string checksum;
  openstudio::model::Model model;
  openstudio::Workspace workspace;
};

class MeasureManager
{
 public:
  MeasureManager(ScriptEngineInstance& t_rubyEngine, ScriptEngineInstance& t_pythonEngine);

  boost::optional<OSMInfo> getModel(const openstudio::path& osmPath, bool force_reload = false);

  Json::Value internalState() const;

  void reset();

 private:
  //#if USE_RUBY_ENGINE
  ScriptEngineInstance& rubyEngine;
  //#endif
  //#if USE_PYTHON_ENGINE
  ScriptEngineInstance& pythonEngine;
  //#endif

  std::map<openstudio::path, OSMInfo> m_osms;
  std::map<openstudio::path, openstudio::Workspace> m_idfs;
  std::map<openstudio::path, openstudio::BCLMeasure> m_measures;
  std::map<openstudio::path, openstudio::measure::OSMeasureInfo> m_measureInfos;
};

class MeasureManagerServer
{
 public:
  explicit MeasureManagerServer(unsigned port, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine);

  bool open();
  bool close();

 protected:
 private:
  void handle_get(web::http::http_request message);
  void handle_post(web::http::http_request message);
  static void handle_error(pplx::task<void>& t);
  MeasureManager m_measureManager;
  web::http::experimental::listener::http_listener m_listener;

  std::string m_url;
  openstudio::path my_measures_dir;
};

}  // namespace openstudio

#endif  // MEASUREMANAGER_H
