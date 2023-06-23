#include "MeasureManager.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../src/utilities/core/StringHelpers.cpp"
#include "../osversion/VersionTranslator.hpp"
#include "../energyplus/ForwardTranslator.hpp"
#include "energyplus/ForwardTranslator.hpp"
#include "utilities/bcl/LocalBCL.hpp"
#include "utilities/idf/ValidityEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../measure/OSMeasure.hpp"
#include "../measure/ModelMeasure.hpp"
#include "../measure/EnergyPlusMeasure.hpp"
#include "../measure/ReportingMeasure.hpp"
#include "../measure/OSArgument.hpp"
#include "../measure/OSOutput.hpp"
#include "../measure/OSRunner.hpp"
#include "../measure/OSMeasureInfoGetter.hpp"
#include "../model/Model_Impl.hpp"  // For casting
#include "../../ruby/interpreter/RubyException.hpp"

#include <cpprest/asyncrt_utils.h>
#include <json/json.h>
#include <fmt/format.h>
#include <json/value.h>
#include <pugixml.hpp>

#include <utility>  // make_pair
#include <condition_variable>
#include <mutex>
#include <csignal>

namespace openstudio {

template <typename... T>
[[nodiscard]] auto format_to_string_t(fmt::format_string<T...> fmt, T&&... args) -> utility::string_t {
  return utility::conversions::to_string_t(vformat(fmt, fmt::make_format_args(args...)));
}

namespace interrupthandler {
  static std::condition_variable condition_;
  static std::mutex mutex_;
  void handleUserInterrupt(int signal) {
    if (signal == SIGINT) {
      fmt::print("SIGINT trapped ...\n");
      condition_.notify_one();
    }
  }
  void hookSIGINT() {
    std::signal(SIGINT, handleUserInterrupt);
  }

  void waitForUserInterrupt() {
    std::unique_lock<std::mutex> lock{mutex_};
    condition_.wait(lock);
    fmt::print("user has signaled to interrup program...\n");
    lock.unlock();
  }
}  // namespace interrupthandler

MeasureManager::MeasureManager(ScriptEngineInstance& t_rubyEngine, ScriptEngineInstance& t_pythonEngine)
  : rubyEngine(t_rubyEngine), pythonEngine(t_pythonEngine) {
  // rubyEngine->exec("puts 'Hello from ruby'");
  // pythonEngine->exec("print('Hello from python')");
}
//   :
// #if USE_RUBY_ENGINE
//     rubyEngine(t_rubyEngine),
// #endif
// #if USE_PYTHON_ENGINE
//     pythonEngine(t_pythonEngine)
// #endif
// {
// }

Json::Value MeasureManager::internalState() const {
  Json::Value result(Json::objectValue);

  Json::Value osms(Json::arrayValue);
  for (const auto& [k, v] : m_osms) {
    Json::Value osmInfo(Json::objectValue);
    osmInfo["osm_path"] = k.generic_string();
    osmInfo["checksum"] = v.checksum;
    osms.append(std::move(osmInfo));
  }
  result["osm"] = std::move(osms);

  Json::Value idfs(Json::arrayValue);
  for (const auto& [k, v] : m_idfs) {
    Json::Value idfInfo(Json::objectValue);
    idfInfo["idf_path"] = k.generic_string();
    idfInfo["checksum"] = v.checksum;
    idfs.append(std::move(idfInfo));
  }
  result["idf"] = std::move(idfs);

  auto& measures = result["measures"];
  for (const auto& [measureDirPath, bclMeasureInfo] : m_measures) {
    Json::Value mInfo(Json::objectValue);
    measures.append(bclMeasureInfo.measure.toJSON());
  }

  auto& measureInfos = result["measure_info"];
  // Json::Value measureInfos(Json::arrayValue);
  for (const auto& [measureDirPath, bclMeasureInfo] : m_measures) {
    for (const auto& [osmOrIdfPath, measureInfo] : bclMeasureInfo.measureInfos) {
      Json::Value mInfo(Json::objectValue);
      mInfo["measureDirPath"] = measureDirPath.generic_string();
      mInfo["osm_path"] = osmOrIdfPath.generic_string();
      auto& args = mInfo["arguments"];
      for (const auto& arg : measureInfo.bclMeasureArguments()) {
        args.append(arg.toJSON());
      }

      measureInfos.append(std::move(mInfo));
    }
  }
  // result["measure_info"] = std::move(measureInfos);

  return result;
}

size_t MeasureManager::clearMeasureInfoForOsmorIdfPath(const openstudio::path& osmOrIdfPath) {
  size_t totalRemoved = 0;
  for (auto& [key, value] : m_measures) {
    totalRemoved += value.measureInfos.erase(osmOrIdfPath);
  }
  return totalRemoved;
}

boost::optional<OSMInfo> MeasureManager::getModel(const openstudio::path& osmPath, bool force_reload) {

  if (!openstudio::filesystem::is_regular_file(osmPath)) {
    fmt::print("Model '{}' does not exist\n", osmPath.generic_string());
    m_osms.erase(osmPath);
    clearMeasureInfoForOsmorIdfPath(osmPath);
    return boost::none;
  }

  OSMInfo current;
  current.checksum = openstudio::checksum(osmPath);

  if (!force_reload) {
    auto it = m_osms.find(osmPath);
    if (it != m_osms.end()) {
      auto& cached = it->second;
      if (current.checksum == cached.checksum) {
        fmt::print("Using cached model {}\n", osmPath.generic_string());
        return cached;
      } else {
        fmt::print("Checksum of cached model does not match current checksum for '{}'\n", osmPath.generic_string());
      }
    }
  }

  clearMeasureInfoForOsmorIdfPath(osmPath);

  fmt::print("Attempting to load model '{}'\n", osmPath.generic_string());
  openstudio::osversion::VersionTranslator vt;
  if (auto model_ = vt.loadModel(osmPath)) {
    fmt::print("Successfully loaded model '{}'\n", osmPath.generic_string());
    current.model = std::move(*model_);
    openstudio::energyplus::ForwardTranslator ft;
    current.workspace = ft.translateModel(current.model);
    auto [it, ok] = m_osms.insert_or_assign(osmPath, std::move(current));
    return it->second;
  }

  fmt::print("Failed to load model '{}'\n", osmPath.generic_string());
  m_osms.erase(osmPath);

  return boost::none;
}

boost::optional<IDFInfo> MeasureManager::getIdf(const openstudio::path& idfPath, bool force_reload) {

  if (!openstudio::filesystem::is_regular_file(idfPath)) {
    fmt::print("Idf '{}' does not exist\n", idfPath.generic_string());
    m_idfs.erase(idfPath);
    clearMeasureInfoForOsmorIdfPath(idfPath);
    return boost::none;
  }

  IDFInfo current;
  current.checksum = openstudio::checksum(idfPath);

  if (!force_reload) {
    auto it = m_idfs.find(idfPath);
    if (it != m_idfs.end()) {
      auto& cached = it->second;
      if (current.checksum == cached.checksum) {
        fmt::print("Using cached workspace {}\n", idfPath.generic_string());
        return cached;
      } else {
        fmt::print("Checksum of cached workspace does not match current checksum for '{}'\n", idfPath.generic_string());
      }
    }
  }

  clearMeasureInfoForOsmorIdfPath(idfPath);

  fmt::print("Attempting to load idf '{}'\n", idfPath.generic_string());
  if (auto workspace_ = openstudio::Workspace::load(idfPath, openstudio::IddFileType::EnergyPlus)) {
    fmt::print("Successfully loaded idf '{}'\n", idfPath.generic_string());

    if (workspace_->isValid(openstudio::StrictnessLevel::Draft)) {
      current.workspace = std::move(*workspace_);
      auto [it, ok] = m_idfs.insert_or_assign(idfPath, std::move(current));
      return it->second;
    } else {
      fmt::print("Workspace loaded from '{}' is not valid to Draft StrictnessLevel\n", idfPath.generic_string());
    }
  } else {
    fmt::print("Failed to load idf '{}'\n", idfPath.generic_string());
  }

  m_idfs.erase(idfPath);

  return boost::none;
}

boost::optional<BCLMeasure> MeasureManager::getMeasure(const openstudio::path& measureDirPath, bool force_reload) {

  const auto& measureDirPathStr = measureDirPath.string();

  // check if measure exists on disk
  if (!openstudio::filesystem::is_directory(measureDirPath)) {
    fmt::print("Measure '{}' does not exist.\n", measureDirPathStr);
    m_measures.erase(measureDirPath);
    return boost::none;
  }
  if (!openstudio::filesystem::is_regular_file(measureDirPath / "measure.xml")) {
    fmt::print("Measure directory '{}' exists but does not have a measure.xml.\n", measureDirPathStr);
    m_measures.erase(measureDirPath);
    return boost::none;
  }

  boost::optional<BCLMeasureInfo> measureInfo_;
  if (!force_reload) {
    auto it = m_measures.find(measureDirPath);
    if (it != m_measures.end()) {
      measureInfo_ = it->second;
      fmt::print("Using cached measure {}\n", measureDirPath.generic_string());
    }
  }

  if (!measureInfo_) {
    m_measures.erase(measureDirPath);

    // load from disk
    fmt::print("Attempting to load measure '{}'\n", measureDirPathStr);

    boost::optional<BCLMeasure> measure_ = openstudio::BCLMeasure::load(measureDirPath);

    if (!measure_) {
      fmt::print("Failed to load measure '{}'\n", measureDirPathStr);
      return boost::none;
    }
    fmt::print("Successfully loaded measure '{}'\n", measureDirPathStr);
    measureInfo_ = BCLMeasureInfo(std::move(*measure_));
    auto [it, ok] = m_measures.insert({measureDirPath, std::move(*measureInfo_)});
    measureInfo_ = it->second;
  }

  auto& measure = measureInfo_->measure;

  // see if there are updates, want to make sure to perform both checks so do outside of conditional
  bool file_updates = measure.checkForUpdatesFiles();  // checks if any files have been updated
  bool xml_updates = measure.checkForUpdatesXML();     // only checks if xml as loaded has been changed since last save

  auto readmeInPath = measureDirPath / "README.md.erb";
  auto readmeOutPath = measureDirPath / "README.md";

  bool readme_out_of_date = openstudio::filesystem::is_regular_file(readmeInPath) && !openstudio::filesystem::is_regular_file(readmeOutPath);

  // TODO: try catch like in measure_manager.rb?
  bool missing_fields = measure.missingRequiredFields();

  if (file_updates || xml_updates || missing_fields || readme_out_of_date) {
    fmt::print("Changes detected, updating '{}'\n", measureDirPathStr);

    // Clear cache before calling getMeasureInfo
    measureInfo_->measureInfos.clear();

    // TODO: the readme.md generation from readme.md.erb requires ruby.
    fmt::print("Warn: readme.md generation from ERB is not supported yet\n");

    openstudio::measure::OSMeasureInfo info = getMeasureInfo(measureDirPath, measure, openstudio::path{});
    info.update(measure);

    // Save the xml file with changes triggered by checkForUpdatesFiles() / checkForUpdatesXML() above
    measure.save();
  }

  return measure;
}

openstudio::measure::OSMeasureInfo MeasureManager::getMeasureInfo(const openstudio::path& measureDirPath, const BCLMeasure& measure,
                                                                  const openstudio::path& osmOrIdfPath, const boost::optional<model::Model>& model_,
                                                                  const boost::optional<Workspace>& workspace_) {

  if (!m_measures.contains(measureDirPath)) {
    LOG_AND_THROW("Measure isn't recorded in m_measures, that should NOT happen");
  }
  auto& bclMeasureInfo = m_measures.at(measureDirPath);
  auto it2 = bclMeasureInfo.measureInfos.find(osmOrIdfPath);
  if (it2 != bclMeasureInfo.measureInfos.end()) {
    fmt::print("Using cached OSMeasureInfo for '{}', '{}'\n", measureDirPath.generic_string(), osmOrIdfPath.generic_string());
    return it2->second;
  }

  auto scriptPath_ = measure.primaryScriptPath();
  if (!scriptPath_) {
    throw std::runtime_error(
      fmt::format("Unable to locate primary Ruby script path for BCLMeasure '{}' located at '{}'", measure.name(), measureDirPath.generic_string()));
  }

  auto getOrCreateModel = [this, &model_, &osmOrIdfPath]() -> openstudio::model::Model {
    if (model_) {
      return model_->clone().cast<openstudio::model::Model>();
    } else if (!osmOrIdfPath.empty()) {
      // TODO: not sure we want to keep this here or not..
      if (auto osmInfo_ = getModel(osmOrIdfPath)) {
        return osmInfo_->model.clone().cast<openstudio::model::Model>();
      } else {
        LOG_AND_THROW("Failed to load the Model at " << osmOrIdfPath);
      }
    } else {
      return {};
    }
  };

  auto getOrCreateWorkspace = [this, &workspace_, &osmOrIdfPath]() -> openstudio::Workspace {
    if (workspace_) {
      return workspace_->clone();
    } else if (!osmOrIdfPath.empty()) {
      if (auto idfInfo_ = getIdf(osmOrIdfPath)) {
        return idfInfo_->workspace.clone();
      } else {
        LOG_AND_THROW("Failed to load the Model at " << osmOrIdfPath);
      }
    } else {
      return {openstudio::StrictnessLevel::Draft, openstudio::IddFileType::EnergyPlus};
    }
  };

  ScriptEngineInstance* thisEngine = nullptr;
  if (measure.measureLanguage() == MeasureLanguage::Ruby) {
    thisEngine = &rubyEngine;
  } else if (measure.measureLanguage() == MeasureLanguage::Python) {
    thisEngine = &pythonEngine;
  }

  std::string className = (*thisEngine)->inferMeasureClassName(*scriptPath_);
  fmt::print("className={}\n", className);
  if (className.empty()) {
    auto msg = fmt::format("Failed to infer measure name from '{}'", scriptPath_->generic_string());
    fmt::print(stderr, "{}\n", msg);
    return openstudio::measure::OSMeasureInfo(msg);
  }
  ScriptObject measureScriptObject = (*thisEngine)->loadMeasure(*scriptPath_, className);
  if (measureScriptObject.empty()) {
    auto msg = fmt::format("Failed to load measure '{}' from '{}'\n", className, scriptPath_->generic_string());
    fmt::print(stderr, "{}\n", msg);
    return openstudio::measure::OSMeasureInfo(msg);
  }

  MeasureType measureType;

  std::string name;
  std::string description;
  std::string taxonomy;
  std::string modelerDescription;

  std::vector<measure::OSArgument> arguments;
  std::vector<measure::OSOutput> outputs;

  try {
    auto* osMeasurePtr = (*thisEngine)->getAs<openstudio::measure::OSMeasure*>(measureScriptObject);
    measureType = osMeasurePtr->measureType();
    fmt::print("measureType={}\n", measureType.valueName());

    if (measureType == MeasureType::ModelMeasure) {
      auto* measurePtr = static_cast<openstudio::measure::ModelMeasure*>(osMeasurePtr);
      name = measurePtr->name();
      description = measurePtr->description();
      taxonomy = measurePtr->taxonomy();
      modelerDescription = measurePtr->modeler_description();

      auto model = getOrCreateModel();
      arguments = measurePtr->arguments(model);
      outputs = measurePtr->outputs();

    } else if (measureType == MeasureType::EnergyPlusMeasure) {
      auto* measurePtr = static_cast<openstudio::measure::EnergyPlusMeasure*>(osMeasurePtr);
      name = measurePtr->name();
      description = measurePtr->description();
      taxonomy = measurePtr->taxonomy();
      modelerDescription = measurePtr->modeler_description();

      auto workspace = getOrCreateWorkspace();
      arguments = measurePtr->arguments(workspace);
      outputs = measurePtr->outputs();
    } else if (measureType == MeasureType::ReportingMeasure) {
      auto* measurePtr = static_cast<openstudio::measure::ReportingMeasure*>(osMeasurePtr);
      name = measurePtr->name();
      description = measurePtr->description();
      taxonomy = measurePtr->taxonomy();
      modelerDescription = measurePtr->modeler_description();

      auto model = getOrCreateModel();
      // TODO: for ruby at least, need to try the arity... model was added later, at 3.0.0
      arguments = measurePtr->arguments(model);
      outputs = measurePtr->outputs();

    } else {
      throw std::runtime_error("Unknown");
    }
  } catch (const std::exception& e) {
    auto msg = fmt::format("Failed to query information from measure '{}' from '{}': {}\n", className, scriptPath_->generic_string(), e.what());
    fmt::print(stderr, "{}\n", msg);
    return openstudio::measure::OSMeasureInfo(msg);
  }

  if (name.empty()) {
    name = className;
  }

  openstudio::measure::OSMeasureInfo info(measureType, className, name, description, taxonomy, modelerDescription, arguments, outputs);
  auto [it, ok] = bclMeasureInfo.measureInfos.insert({osmOrIdfPath, std::move(info)});
  return it->second;
}

void MeasureManager::reset() {
  m_osms.clear();
  m_idfs.clear();
  m_measures.clear();
}

MeasureManagerServer::MeasureManagerServer(unsigned port, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine)
  : m_measureManager(rubyEngine, pythonEngine),
    m_url(fmt::format("http://localhost:{}/", port)),
    my_measures_dir(openstudio::filesystem::home_path() / "OpenStudio/Measures") {

  web::uri_builder uri_builder;
  uri_builder.set_scheme(utility::conversions::to_string_t("http")).set_host(utility::conversions::to_string_t("0.0.0.0")).set_port(port);
  auto builder_uri = uri_builder.to_uri();
  // fmt::print("builder_uri: {} {} {} {}\n", web::http::uri::decode(builder_uri.scheme()), web::http::uri::decode(builder_uri.host()),
  //            builder_uri.port(), web::http::uri::decode(builder_uri.path()));
  fmt::print("Serving on: {}\n", m_url);

  m_listener = web::http::experimental::listener::http_listener(builder_uri);

  m_listener.support(web::http::methods::GET, [this](auto&& request) { handle_get(std::forward<decltype(request)>(request)); });
  m_listener.support(web::http::methods::POST, [this](auto&& request) { handle_post(std::forward<decltype(request)>(request)); });
}

void MeasureManagerServer::handle_error(pplx::task<void>& t) {
  try {
    t.get();
  } catch (...) {
    // Ignore the error, Log it if a logger is available
  }
}

// TODO:: https://github.com/microsoft/cpprestsdk/blob/master/Release/samples/CasaLens/casalens.cpp
bool MeasureManagerServer::open() {
  auto status = m_listener.open().then([](pplx::task<void> t) { handle_error(t); }).wait();
  return status == pplx::task_group_status::completed;
}
bool MeasureManagerServer::close() {
  auto status = m_listener.close().then([](pplx::task<void> t) { handle_error(t); }).wait();
  return status == pplx::task_group_status::completed;
}

void MeasureManagerServer::handle_get(web::http::http_request message) {
  // Need a mutex?
  std::cout << "Received GET request: " << message.to_string() << "uri=" << message.relative_uri().to_string() << "\n";

  const std::string uri = web::http::uri::decode(message.relative_uri().path());
  // std::vector<std::string> paths = web::http::uri::split_path(uri);

  // Cpprestsdk has it's own json implementation.....
  Json::Value result;
  result["status"] = "running";
  result["my_measures_dir"] = my_measures_dir.generic_string();

  if (uri == "/") {
    // Don't do anything else
  } else if (uri == "/internal_state") {

    auto internalState = m_measureManager.internalState();
    for (const auto& key : internalState.getMemberNames()) {
      result[key] = internalState[key];
    }
  } else {
    message.reply(web::http::status_codes::BadRequest, web::json::value::string("Error, unknown path '" + uri + "'"));
  }

  message.reply(web::http::status_codes::OK, web::json::value::parse(result.toStyledString()));
  // .then([](pplx::task<void> t) {
  //   try {
  //     t.get();
  //   } catch (...) {
  //     //
  //   }
  // });
}

void MeasureManagerServer::handle_post(web::http::http_request message) {
  std::cout << "Received POST request: " << message.to_string();
  const std::string uri = web::http::uri::decode(message.relative_uri().path());

  if (uri == "/reset") {
    fmt::print("Resetting internal state");
    m_measureManager.reset();
    // my_measures_dir = openstudio::filesystem::home_path() / "OpenStudio/Measures";
    message.reply(web::http::status_codes::OK, web::json::value::string("Resetting internal state"));
    return;
  }

  if (uri == "/set") {
    // curl -H "Content-Type: application/json" -X POST  --data '{"my_measures_dir": "/Users/julien/OpenStudio/Measures"}' http://localhost:8090/set
    message.extract_json().then([this, message](web::json::value body) {
      if (body.has_field("my_measures_dir")) {
        this->my_measures_dir = openstudio::toPath(body.at("my_measures_dir").as_string());
      }
      message.reply(web::http::status_codes::OK, web::json::value());
    });
    return;
  }

  if (uri == "/download_bcl_measure") {
    message.extract_json().then([message](web::json::value body) {
      if (body.has_field("uid")) {
        const std::string uid = body.at("uid").as_string();
        const RemoteBCL r;
        if (auto bclMeasure_ = r.getMeasure(uid)) {
          message.reply(web::http::status_codes::OK, web::json::value::parse(bclMeasure_->toJSON().toStyledString()));
        } else {
          message.reply(web::http::status_codes::BadRequest, web::json::value::string(format_to_string_t("Cannot find measure with uid='{}'", uid)));
        }
      } else {
        fmt::print("Missing the uid in the post data\n");
        message.reply(web::http::status_codes::BadRequest, web::json::value::string("Missing the uid in the post data"));
      }
    });
    return;
  }

  // TODO: for testing only, remove
  if (uri == "/get_model") {
    message.extract_json().then([this, message](web::json::value body) {
      if (body.has_field("osm_path")) {
        auto osmPath = openstudio::toPath(body.at("osm_path").as_string());
        auto osmInfo_ = m_measureManager.getModel(osmPath);
        if (osmInfo_) {
          message.reply(web::http::status_codes::OK,
                        web::json::value::string(format_to_string_t("OK, loaded model with checksum {}", osmInfo_->checksum)));
        } else {
          message.reply(web::http::status_codes::BadRequest,
                        web::json::value::string(format_to_string_t("Wrong osm path: '{}'", osmPath.generic_string())));
        }
      }
    });
    return;
  }

  if (uri == "/bcl_measures") {
    bool force_reload = false;  // Not supposed to mess with the BCL Measures!
    auto& localBCL = openstudio::LocalBCL::instance();
    std::vector<web::json::value> result;
    for (auto& measure : localBCL.measures()) {
      auto measureDir = measure.directory();
      if (boost::optional<BCLMeasure> measure_ = m_measureManager.getMeasure(measureDir, force_reload)) {
        result.emplace_back(web::json::value::parse(measure_->toJSON().toStyledString()));
      } else {
        fmt::print("Directory '{}' is not a measure\n", measureDir.generic_string());
      }
    }
    message.reply(web::http::status_codes::OK, web::json::value::array(std::move(result)));
    return;
  }

  if (uri == "/update_measures") {
    message.extract_json().then([this, message](web::json::value body) {
      openstudio::path measuresDir = my_measures_dir;
      bool force_reload = false;
      if (body.has_field("measures_dir")) {
        if (body.at("measures_dir").is_string()) {
          measuresDir = openstudio::toPath(body.at("measures_dir").as_string());
        } else {
          fmt::print("param 'measures_dir' must be a string\n");
        }
      }
      if (body.has_field("force_reload")) {
        if (body.at("force_reload").is_boolean()) {
          force_reload = body.at("force_reload").as_bool();
        } else {
          fmt::print("param 'force_reload' must be a boolean\n");
        }
      }
      // Scan the directory for measures
      std::vector<web::json::value> result;
      for (const auto& dirEnt : openstudio::filesystem::directory_iterator{measuresDir}) {
        if (openstudio::filesystem::is_directory(dirEnt)) {
          const auto& measureDir = dirEnt.path();
          if (boost::optional<BCLMeasure> measure_ = m_measureManager.getMeasure(measureDir, force_reload)) {
            result.emplace_back(web::json::value::parse(measure_->toJSON().toStyledString()));
          } else {
            fmt::print("Directory '{}' is not a measure\n", measureDir.generic_string());
          }
        }
      }
      message.reply(web::http::status_codes::OK, web::json::value::array(std::move(result)));
    });
    return;
  }

  if (uri == "/compute_arguments") {
    message.extract_json().then([this, message](web::json::value body) {
      openstudio::path measureDir;
      bool force_reload = false;
      if (body.has_field("measure_dir") && body.at("measure_dir").is_string()) {
        measureDir = openstudio::toPath(body.at("measure_dir").as_string());
      } else {
        message.reply(web::http::status_codes::BadRequest, web::json::value::string("The 'measure_dir' (string) must be in the post data"));
        return;
      }

      if (body.has_field("force_reload")) {
        if (body.at("force_reload").is_boolean()) {
          force_reload = body.at("force_reload").as_bool();
        } else {
          fmt::print("param 'force_reload' must be a boolean\n");
        }
      }

      auto measure_ = m_measureManager.getMeasure(measureDir, force_reload);
      if (!measure_) {
        auto msg = fmt::format("Cannot load measure at '{}'", measureDir.generic_string());
        fmt::print(stderr, "{}\n", msg);
        message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
        return;
      }

      openstudio::path osmOrIdfPath;
      if (body.has_field("osm_path") && body.at("osm_path").is_string()) {
        osmOrIdfPath = openstudio::toPath(body.at("osm_path").as_string());
      }

      openstudio::measure::OSMeasureInfo info = m_measureManager.getMeasureInfo(measureDir, *measure_, osmOrIdfPath);
      if (auto errorString_ = info.error()) {
        message.reply(web::http::status_codes::OK, web::json::value::string(*errorString_));
        return;
      }
      // TODO: maybe I should write an OSMeasureInfo::toJSON() method, but that'd be duplicating the code in BCLMeasure (BCLXML to be exact).
      // So since the only thing that's different is the OSArgument (OSMeasureInfo) versus BCLMeasureArgument (BCLMeasure), we just override
      auto result = measure_->toJSON();
      if (!osmOrIdfPath.empty()) {
        auto& arguments = result["arguments"];
        arguments.clear();
        for (const measure::OSArgument& argument : info.arguments()) {
          arguments.append(argument.toJSON());
        }
      }
      message.reply(web::http::status_codes::OK, web::json::value::parse(result.toStyledString()));
    });
    return;
  }

  if (uri == "/create_measure") {
    message.extract_json().then([this, message](web::json::value body) {
      static const std::array<std::string, 7> requiredParams = {
        "measure_dir", "display_name", "class_name", "taxonomy_tag", "measure_type", "description", "modeler_description",
      };
      for (const auto& requiredParam : requiredParams) {
        if (!body.has_field(requiredParam) || !body.at(requiredParam).is_string()) {
          auto msg = fmt::format("The '{}' (string) must be in the post data.", requiredParam);
          fmt::print("{}\n", msg);
          message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
          return;
        }
      }

      MeasureType measureType = MeasureType::ModelMeasure;
      auto measureTypeString = body.at("measure_type").as_string();
      try {
        measureType = MeasureType(measureTypeString);
      } catch (const std::exception& e) {
        auto msg = fmt::format("Couldn't convert '{}' to a MeasureType: {}", measureTypeString, e.what());
        fmt::print("{}\n", msg);
        message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
        return;
      }

      MeasureLanguage measureLanguage = MeasureLanguage::Ruby;
      if (body.has_field("measure_language") && body.at("measure_language").is_string()) {
        auto measureLanguageString = body.at("measure_language").as_string();
        try {
          measureLanguage = MeasureLanguage(measureLanguageString);
        } catch (const std::exception& e) {
          auto msg = fmt::format("Couldn't convert '{}' to a MeasureLanguage: {}", measureLanguageString, e.what());
          fmt::print("{}\n", msg);
          message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
          return;
        }
      }

      const openstudio::path measureDir = body.at("measure_dir").as_string();
      // This is throwy when the directory already exists
      if (openstudio::filesystem::is_directory(measureDir)) {
        auto msg = fmt::format("The directory already exists at '{}'.", measureDir.generic_string());
        fmt::print("{}\n", msg);
        message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
        return;
      }
      const BCLMeasure measure(body.at("display_name").as_string(), body.at("class_name").as_string(), measureDir,
                               body.at("taxonomy_tag").as_string(), measureType, body.at("description").as_string(),
                               body.at("modeler_description").as_string(), measureLanguage);

      if (boost::optional<BCLMeasure> measure_ = m_measureManager.getMeasure(measureDir, true)) {
        message.reply(web::http::status_codes::OK, web::json::value::parse(measure_->toJSON().toStyledString()));
      } else {
        fmt::print("Failed to update measure after creation, this shouldn't happen.");
        message.reply(web::http::status_codes::BadRequest, web::json::value::parse(measure.toJSON().toStyledString()));
      }
    });
    return;
  }

  if (uri == "/duplicate_measure") {

    message.extract_json().then([this, message](web::json::value body) {
      // Required parameters:
      openstudio::path oldMeasureDir;
      if (body.has_field("old_measure_dir") && body.at("old_measure_dir").is_string()) {
        oldMeasureDir = openstudio::toPath(body.at("old_measure_dir").as_string());
      } else {
        message.reply(web::http::status_codes::BadRequest, web::json::value::string("The 'old_measure_dir' (string) must be in the post data"));
        return;
      }

      openstudio::path newMeasureDir;
      if (body.has_field("measure_dir") && body.at("measure_dir").is_string()) {
        newMeasureDir = openstudio::toPath(body.at("measure_dir").as_string());
      } else {
        message.reply(web::http::status_codes::BadRequest, web::json::value::string("The 'measure_dir' (string) must be in the post data"));
        return;
      }

      bool force_reload = false;
      if (body.has_field("force_reload")) {
        if (body.at("force_reload").is_boolean()) {
          force_reload = body.at("force_reload").as_bool();
        } else {
          fmt::print("param 'force_reload' must be a boolean\n");
        }
      }

      boost::optional<BCLMeasure> oldMeasure_ = m_measureManager.getMeasure(oldMeasureDir, force_reload);
      if (!oldMeasure_) {
        auto msg = fmt::format("Cannot load measure at '{}'.", oldMeasureDir.generic_string());
        fmt::print("{}\n", msg);
        message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
        return;
      }

      auto& oldMeasure = *oldMeasure_;

      auto newMeasure_ = oldMeasure.clone(newMeasureDir);
      if (!newMeasure_) {
        auto msg = fmt::format("Cannot copy measure from '{}' to {}'", oldMeasureDir.generic_string(), newMeasureDir.generic_string());
        fmt::print("{}\n", msg);
        message.reply(web::http::status_codes::BadRequest, web::json::value::string(msg));
        return;
      }
      auto& newMeasure = *newMeasure_;
      // Force updating the UID
      newMeasure.changeUID();
      newMeasure.incrementVersionId();

      if (body.has_field("display_name") && body.at("display_name").is_string()) {
        newMeasure.setDisplayName(body.at("display_name").as_string());
      }
      if (body.has_field("class_name") && body.at("class_name").is_string()) {
        std::string className = body.at("class_name").as_string();
        newMeasure.setName(openstudio::toUnderscoreCase(className));
        newMeasure.setClassName(className);
      }
      if (body.has_field("taxonomy_tag") && body.at("taxonomy_tag").is_string()) {
        newMeasure.setTaxonomyTag(body.at("taxonomy_tag").as_string());
      }

      // Changing the measure Language is not supported!
      auto newMeasureLanguage = oldMeasure.measureLanguage();
      // if (body.has_field("measure_language") && body.at("measure_language").is_string()) {
      //   auto measureLanguageString = body.at("measure_type").as_string();
      //   try {
      //     newMeasureLanguage = MeasureLanguage(measureLanguageString);
      //     newMeasure.setMeasureLanguage(measureLanguage);
      //   } catch (...) {
      //     fmt::print("Couldn't convert '{}' to a MeasureLanguage", measureLanguageString);
      //   }
      // }

      // Changing the measure Type should maybe not be supported either, the method signatures will be wrong and it might be missing
      // energyPlusOutputRequests
      auto newMeasureType = oldMeasure.measureType();
      if (body.has_field("measure_type") && body.at("measure_type").is_string()) {
        auto measureTypeString = body.at("measure_type").as_string();
        try {
          newMeasureType = MeasureType(measureTypeString);
          newMeasure.setMeasureType(newMeasureType);
        } catch (...) {
          fmt::print("Couldn't convert '{}' to a MeasureType", measureTypeString);
        }
      }

      if (body.has_field("description") && body.at("description").is_string()) {
        newMeasure.setDescription(body.at("description").as_string());
      }
      if (body.has_field("modeler_description") && body.at("modeler_description").is_string()) {
        newMeasure.setModelerDescription(body.at("modeler_description").as_string());
      }

      newMeasure.updateMeasureScript(oldMeasure.measureType(), newMeasureType, oldMeasure.measureLanguage(), newMeasureLanguage,
                                     oldMeasure.className(), newMeasure.className(), newMeasure.displayName(), newMeasure.description(),
                                     newMeasure.modelerDescription());
      newMeasure.updateMeasureTests(oldMeasure.className(), newMeasure.className());

      fmt::print("Cloned the {} {} with class name '{}' from '{}' to '{}'\n", oldMeasure.measureLanguage().valueName(),
                 oldMeasure.measureType().valueName(), oldMeasure.className(),
                 openstudio::toString(openstudio::filesystem::canonical(oldMeasure.directory())),
                 openstudio::toString(openstudio::filesystem::canonical(newMeasure.directory())));

      newMeasure.checkForUpdatesFiles();
      newMeasure.checkForUpdatesXML();
      newMeasure.save();

      if (boost::optional<BCLMeasure> measure_ = m_measureManager.getMeasure(newMeasureDir, true)) {
        message.reply(web::http::status_codes::OK, web::json::value::parse(measure_->toJSON().toStyledString()));
        return;
      } else {
        fmt::print("Failed to update measure after duplication, this shouldn't happen.");
        message.reply(web::http::status_codes::BadRequest, web::json::value::parse(newMeasure.toJSON().toStyledString()));
        return;
      }
    });
    return;
  }

  message.reply(web::http::status_codes::NotFound, web::json::value::string("404: Unknown Endpoint"));
}

}  // namespace openstudio
