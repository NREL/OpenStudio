/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASUREMANAGER_H
#define MEASUREMANAGER_H

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/ThreadSafeDeque.hpp"
#include "../scriptengine/ScriptEngine.hpp"

#include "../model/Model.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../measure/OSMeasureInfoGetter.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include <future>

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

struct IDFInfo
{
  std::string checksum;
  openstudio::Workspace workspace;
};

struct BCLMeasureInfo
{
  explicit BCLMeasureInfo(openstudio::BCLMeasure t_measure) : measure(std::move(t_measure)) {}
  openstudio::BCLMeasure measure;
  // Map of osmPath to measureInfo
  std::map<openstudio::path, openstudio::measure::OSMeasureInfo> measureInfos;
};

class MeasureManager
{
 public:
  MeasureManager(ScriptEngineInstance& t_rubyEngine, ScriptEngineInstance& t_pythonEngine);

  boost::optional<OSMInfo> getModel(const openstudio::path& osmPath, bool force_reload = false);
  boost::optional<IDFInfo> getIdf(const openstudio::path& idfPath, bool force_reload = false);
  boost::optional<BCLMeasure> getMeasure(const openstudio::path& measureDirPath, bool force_reload = false);
  openstudio::measure::OSMeasureInfo getMeasureInfo(const openstudio::path& measureDirPath, const BCLMeasure& measure,
                                                    const openstudio::path& osmOrIdfPath = "",
                                                    const boost::optional<model::Model>& model_ = boost::none,
                                                    const boost::optional<Workspace>& workspace_ = boost::none);

  //  boost::optional<openstudio::measure::OSMeasureInfo> getMeasureInfo(const openstudio::path& measureDirPath, const openstudio::BCLMeasure& measure,
  //                                                                     const boost::optional<model::Model>& model,
  //                                                                     const boost::optional<Workspace>& workspace);

  // getMeasureInfo;
  // computeArguments
  // getMeasureHash;
  Json::Value internalState() const;

  std::size_t clearMeasureInfoForOsmorIdfPath(const openstudio::path& osmOrIdfPath);

  void reset();

 private:
  REGISTER_LOGGER("MeasureManager");
  //#if USE_RUBY_ENGINE
  ScriptEngineInstance& rubyEngine;
  //#endif
  //#if USE_PYTHON_ENGINE
  ScriptEngineInstance& pythonEngine;
  //#endif

  std::map<openstudio::path, OSMInfo> m_osms;
  std::map<openstudio::path, IDFInfo> m_idfs;
  std::map<openstudio::path, BCLMeasureInfo> m_measures;
};

class MeasureManagerServer
{
 public:
  explicit MeasureManagerServer(unsigned port, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine);

  bool open();
  bool close();
  void do_tasks_forever();

 protected:
 private:
  struct ResponseType
  {
    web::http::status_code status_code;
    web::json::value body;
  };

  // Request handlers
  ResponseType internal_state(const web::json::value& body);
  ResponseType reset(const web::json::value& body);
  ResponseType set(const web::json::value& body);
  ResponseType download_bcl_measure(const web::json::value& body);
  ResponseType get_model(const web::json::value& body);
  ResponseType bcl_measures(const web::json::value& body);
  ResponseType compute_arguments(const web::json::value& body);
  ResponseType create_measure(const web::json::value& body);
  ResponseType duplicate_measure(const web::json::value& body);
  ResponseType update_measures(const web::json::value& body);

  // Generally request handler, to ensure the work is done on the main thread.
  // See commit message at https://github.com/NREL/OpenStudio/commit/3c4a1c32fd096ca183c5668e2aafe99ac6564fb4#diff-9785c162dbb96e5fdead1b101c7a2d639460e0bdb0d95c8ff21be7a451a8f377
  using memRequestHandlerFunPtr = ResponseType (MeasureManagerServer::*)(const web::json::value& body);
  void handle_request(const web::http::http_request& message, const web::json::value& body, memRequestHandlerFunPtr request_handler);

  void handle_get(web::http::http_request message);
  void handle_post(web::http::http_request message);
  static void handle_error(pplx::task<void>& t);
  MeasureManager m_measureManager;
  web::http::experimental::listener::http_listener m_listener;
  ThreadSafeDeque<std::packaged_task<ResponseType()>> tasks;

  std::string m_url;
  openstudio::path my_measures_dir;
};

}  // namespace openstudio

#endif  // MEASUREMANAGER_H
