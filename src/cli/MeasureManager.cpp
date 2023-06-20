#include "MeasureManager.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/core/Filesystem.hpp"
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
  rubyEngine->exec("puts 'Hello from ruby'");
  pythonEngine->exec("print('Hello from python')");
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

boost::optional<BCLMeasureInfo> MeasureManager::getAndUpdateMeasure(const openstudio::path& measureDirPath, bool force_reload,
                                                                    const boost::optional<openstudio::path>& osmOrIdfPath_,
                                                                    const boost::optional<model::Model>& model_,
                                                                    const boost::optional<Workspace>& workspace_) {

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

    // TODO: the readme.md generation from readme.md.erb requires ruby.
    fmt::print("Warn: readme.md generation from ERB is not supported yet\n");

    // TODO: Also need to mimic OSMeasureInfoGetter::getInfo which is actually implemented in ruby itself...
    // basically we need to compute the arguments of the measures, inspect outputs... so that requires doing something similar to what we do in the
    // `run` method

    fmt::print("Measure at {} uses language = {}.\n", measureDirPathStr, measure.measureLanguage().valueName());

    auto scriptPath_ = measure.primaryScriptPath();
    if (!scriptPath_) {
      throw std::runtime_error(
        fmt::format("Unable to locate primary Ruby script path for BCLMeasure '{}' located at '{}'", measure.name(), measureDirPathStr));
    }

    // TODO: Here we need to do two things:
    // * Find the class name by scanning the measure.rb/.py, then instantiate the measure
    // * Do the same as in ApplyMeasure and compute the arguments with a dumb model/workspace, or the supplied one (for --compute_arguments)
    ScriptEngineInstance* thisEngine = nullptr;
    ScriptObject measureScriptObject;
    // openstudio::measure::OSMeasure* measurePtr = nullptr;

    std::string className;
    MeasureType measureType;

    std::string name;
    std::string description;
    std::string taxonomy;
    std::string modelerDescription;

    std::vector<measure::OSArgument> arguments;
    std::vector<measure::OSOutput> outputs;

    if (measure.measureLanguage() == MeasureLanguage::Ruby) {
      // same as the beginning of the OSMeasureInfoGetter::infoExtractorRubyFunction
      auto importCmd = fmt::format(R"ruby(
currentObjects = Hash.new
ObjectSpace.each_object(OpenStudio::Measure::OSMeasure) do |obj|
  currentObjects[obj] = true
end

measurePath = "{}"
ObjectSpace.garbage_collect
# This line is REQUIRED or the ObjectSpace order will change when garbage collection runs automatically
# If ~12 measures are added and garbage collection runs, the following loop to grab the first measure
# will get the wrong one and return incorrect arguments
ObjectSpace.garbage_collect
load measurePath # need load in case have seen this script before

# Make them global, so C++ can grab them
$measure = nil
$measure_type = String.new
$measure_name = String.new
ObjectSpace.each_object(OpenStudio::Measure::OSMeasure) do |obj|
  if not currentObjects[obj]
    if obj.is_a? OpenStudio::Measure::ModelMeasure
      $measure = obj
      $measure_type = "ModelMeasure"
    elsif obj.is_a? OpenStudio::Measure::EnergyPlusMeasure
      $measure = obj
      $measure_type = "EnergyPlusMeasure"
    elsif obj.is_a? OpenStudio::Measure::ReportingMeasure
      $measure = obj
      $measure_type = "ReportingMeasure"
    end
  end
end

if not $measure
  raise "Unable to extract OpenStudio::Measure::OSMeasure object from " +
       measurePath + ". The script should contain a class that derives " +
      "from OpenStudio::Measure::OSMeasure and should close with a line stating " +
      "the class name followed by .new.registerWithApplication."
end

$measure_name = $measure.class.to_s
puts "#{{$measure}}, #{{$measure_type}}, #{{$measure_name}}"
)ruby",
                                   scriptPath_->generic_string());
      fmt::print("Debug: importCmd=\n{}\n\n", importCmd);
      thisEngine = &rubyEngine;

      rubyEngine->exec(importCmd);

      fmt::print("Import done\n");
      rubyEngine->exec("puts $measure_type");

      // TODO: gotta figure out a way to retrieve a frigging string
      // ScriptObject measureTypeObject = rubyEngine->eval("measure_type");
      ScriptObject measureTypeObject = rubyEngine->eval("$measure_type");
      std::string measureTypeStr = *(rubyEngine->getAs<std::string*>(measureTypeObject));
      fmt::print("measureTypeStr={}\n", measureTypeStr);
      measureType = MeasureType(measureTypeStr);

      measureScriptObject = rubyEngine->eval("$measure");
      ScriptObject measureClassNameObject = rubyEngine->eval("$measure_name");
      className = *(*thisEngine)->getAs<std::string*>(measureClassNameObject);
      fmt::print("className={}\n", className);

      if (measureType == MeasureType::ModelMeasure) {
        auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

        name = measurePtr->name();
        description = measurePtr->description();
        taxonomy = measurePtr->taxonomy();
        modelerDescription = measurePtr->modeler_description();

        if (model_) {
          arguments = measurePtr->arguments(*model_);
        } else {
          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
        }
        outputs = measurePtr->outputs();

      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::EnergyPlusMeasure*>(measureScriptObject);
        name = measurePtr->name();
        description = measurePtr->description();
        taxonomy = measurePtr->taxonomy();
        modelerDescription = measurePtr->modeler_description();

        if (workspace_) {
          arguments = measurePtr->arguments(*workspace_);
        } else {
          openstudio::Workspace workspace(openstudio::StrictnessLevel::Draft, openstudio::IddFileType::EnergyPlus);
          arguments = measurePtr->arguments(workspace);
        }
        outputs = measurePtr->outputs();
      } else if (measureType == MeasureType::ReportingMeasure) {
        auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ReportingMeasure*>(measureScriptObject);
        name = measurePtr->name();
        description = measurePtr->description();
        taxonomy = measurePtr->taxonomy();
        modelerDescription = measurePtr->modeler_description();

        if (model_) {
          arguments = measurePtr->arguments(*model_);
        } else {
          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
        }
        outputs = measurePtr->outputs();

      } else {
        throw std::runtime_error("Unknown");
      }

      if (name.empty()) {
        name = className;
      }

    } else if (measure.measureLanguage() == MeasureLanguage::Python) {
      // TODO: call initialization of the pythonEngine
      auto importCmd = fmt::format(R"python(
import importlib.util
import inspect
spec = importlib.util.spec_from_file_location(f"throwaway", "{}")

module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
class_members = inspect.getmembers(module, lambda x: inspect.isclass(x) and issubclass(x, openstudio.measure.OSMeasure))
assert len(class_members) == 1
measure_name, measure_typeinfo = class_members[0]
print(f"{{measure_name}}, {{measure_typeinfo}}")
measure_type = None
if issubclass(measure_typeinfo, openstudio.measure.ModelMeasure):
    measure_type = "ModelMeasure"
elif issubclass(measure_typeinfo, openstudio.measure.EnergyPlusMeasure):
    measure_type = "EnergyPlusMeasure"
elif issubclass(measure_typeinfo, openstudio.measure.ReportingMeasure):
    measure_type = "ReportingMeasure"
print(f"{{measure_name}}, {{measure_typeinfo}}, {{measure_type}}")
)python",
                                   scriptPath_->generic_string());
      pythonEngine->exec(importCmd);
      thisEngine = &pythonEngine;
      // measureScriptObject = pythonEngine->eval(fmt::format("module.{}()", className));
      measureScriptObject = pythonEngine->eval("measure_typeinfo()");

      // TODO: gotta figure out a way to retrieve a frigging string
      ScriptObject measureTypeObject = pythonEngine->eval("measure_type");
      std::string measureTypeStr = *(*thisEngine)->getAs<std::string*>(measureTypeObject);
      measureType = MeasureType(measureTypeStr);

      ScriptObject measureClassNameObject = pythonEngine->eval("measure_name");
      className = *(*thisEngine)->getAs<std::string*>(measureClassNameObject);

      fmt::print("measureTypeStr={}\n", measureTypeStr);

      if (measureType == MeasureType::ModelMeasure) {
        auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

        name = measurePtr->name();
        description = measurePtr->description();
        taxonomy = measurePtr->taxonomy();
        modelerDescription = measurePtr->modeler_description();

        if (model_) {
          arguments = measurePtr->arguments(*model_);
        } else {
          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
        }
        outputs = measurePtr->outputs();

      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::EnergyPlusMeasure*>(measureScriptObject);
        name = measurePtr->name();
        description = measurePtr->description();
        taxonomy = measurePtr->taxonomy();
        modelerDescription = measurePtr->modeler_description();

        if (workspace_) {
          arguments = measurePtr->arguments(*workspace_);
        } else {
          openstudio::Workspace workspace(openstudio::StrictnessLevel::Draft, openstudio::IddFileType::EnergyPlus);
          arguments = measurePtr->arguments(workspace);
        }
        outputs = measurePtr->outputs();
      } else if (measureType == MeasureType::ReportingMeasure) {
        auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ReportingMeasure*>(measureScriptObject);
        name = measurePtr->name();
        description = measurePtr->description();
        taxonomy = measurePtr->taxonomy();
        modelerDescription = measurePtr->modeler_description();

        if (model_) {
          arguments = measurePtr->arguments(*model_);
        } else {
          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
        }
        outputs = measurePtr->outputs();

      } else {
        throw std::runtime_error("Unknown");
      }

      if (name.empty()) {
        name = className;
      }
    }

    openstudio::measure::OSMeasureInfo info(measureType, className, name, description, taxonomy, modelerDescription, arguments, outputs);
    info.update(measure);

    for (auto& arg : arguments) {
      fmt::print("arg={}\n", arg.displayName());
      // auto outputs = measurePtr->outputs();
    }

    // Save the xml file with changes triggered by checkForUpdatesFiles() / checkForUpdatesXML() above
    measure.save();

    if (osmOrIdfPath_) {
      measureInfo_->measureInfos.insert_or_assign(*osmOrIdfPath_, std::move(info));
    }
  }

  return *measureInfo_;
}

void MeasureManager::reset() {
  m_osms.clear();
  m_idfs.clear();
  m_measures.clear();
}

MeasureManagerServer::MeasureManagerServer(unsigned port, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine)
  : m_measureManager(rubyEngine, pythonEngine), m_url(fmt::format("http://localhost:{}/", port)) {

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
    my_measures_dir.clear();
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
    auto& localBCL = openstudio::LocalBCL::instance();
    std::vector<web::json::value> result;
    for (auto& measure : localBCL.measures()) {
      result.emplace_back(utility::conversions::to_string_t(measure.name()));
    }
    message.reply(web::http::status_codes::OK, web::json::value::array(result));
    return;
  }

  if (uri == "/update_measures") {
    auto response = web::json::value::object();
    response["serviceName"] = web::json::value::string("C++ Measure Manager");
    response["http_method"] = web::json::value::string(uri);
    message.reply(web::http::status_codes::NotImplemented, response);
    return;
  }

  if (uri == "/compute_arguments") {
    auto response = web::json::value::object();
    response["serviceName"] = web::json::value::string("C++ Measure Manager");
    response["http_method"] = web::json::value::string(uri);
    message.reply(web::http::status_codes::NotImplemented, response);
    return;
  }

  if (uri == "/create_measure") {
    auto response = web::json::value::object();
    response["serviceName"] = web::json::value::string("C++ Measure Manager");
    response["http_method"] = web::json::value::string(uri);
    message.reply(web::http::status_codes::NotImplemented, response);
    return;
  }

  if (uri == "/duplicate_measure") {
    auto response = web::json::value::object();
    response["serviceName"] = web::json::value::string("C++ Measure Manager");
    response["http_method"] = web::json::value::string(uri);
    message.reply(web::http::status_codes::NotImplemented, response);
    return;
  }

  message.reply(web::http::status_codes::NotFound, web::json::value::string("404: Unknown Endpoint"));
}

}  // namespace openstudio
