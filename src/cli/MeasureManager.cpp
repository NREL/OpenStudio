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

  return result;
}

boost::optional<OSMInfo> MeasureManager::getModel(const openstudio::path& osmPath, bool force_reload) {

  if (!openstudio::filesystem::is_regular_file(osmPath)) {
    fmt::print("Model '{}' does not exist\n", osmPath.generic_string());
    m_osms.erase(osmPath);
    m_measureInfos.erase(osmPath);
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

  m_measureInfos.erase(osmPath);

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
    m_measureInfos.erase(idfPath);
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

  m_measureInfos.erase(idfPath);

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

void MeasureManager::reset() {
  m_osms.clear();
  m_idfs.clear();
  m_measures.clear();
  m_measureInfos.clear();
}

MeasureManagerServer::MeasureManagerServer(unsigned port, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine)
  : m_measureManager(rubyEngine, pythonEngine), m_url("http://localhost:" + std::to_string(port)) {

  m_listener = web::http::experimental::listener::http_listener(utility::conversions::to_string_t(m_url));
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
          message.reply(web::http::status_codes::OK, web::json::value::string(bclMeasure_->xmlString()));
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
