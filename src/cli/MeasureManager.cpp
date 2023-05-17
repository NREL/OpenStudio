#include "MeasureManager.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../osversion/VersionTranslator.hpp"
#include "../energyplus/ForwardTranslator.hpp"
#include "energyplus/ForwardTranslator.hpp"
#include "utilities/bcl/LocalBCL.hpp"

#include <cpprest/asyncrt_utils.h>
#include <json/json.h>
#include <fmt/format.h>
#include <json/value.h>
#include <pugixml.hpp>

#include <utility>  // make_pair

namespace openstudio {

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

  fmt::print("Attempting to load model '{}'\n", osmPath.generic_string());
  openstudio::osversion::VersionTranslator vt;
  if (auto model_ = vt.loadModel(osmPath)) {
    fmt::print("Successfully loaded model '{}'\n", osmPath.generic_string());
    current.model = std::move(*model_);
    openstudio::energyplus::ForwardTranslator ft;
    current.workspace = ft.translateModel(current.model);
    auto [it, ok] = m_osms.emplace(std::make_pair(osmPath, std::move(current)));
    return it->second;
  }

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

  std::string uri = web::http::uri::decode(message.relative_uri().path());
  std::vector<std::string> paths = web::http::uri::split_path(uri);

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
  }

  message.reply(web::http::status_codes::OK, web::json::value::parse(result.toStyledString())).then([](pplx::task<void> t) {
    try {
      t.get();
    } catch (...) {
      //
    }
  });
}

void MeasureManagerServer::handle_post(web::http::http_request message) {
  std::cout << "Received POST request: " << message.to_string();
  const std::string uri = web::http::uri::decode(message.relative_uri().path());

  if (uri == "/reset") {
    fmt::print("Reseting internal state");
    m_measureManager.reset();
    message.reply(web::http::status_codes::OK, web::json::value());
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
          message.reply(web::http::status_codes::OK, web::json::value(bclMeasure_->xmlString()));
        } else {
          message.reply(web::http::status_codes::BadRequest, web::json::value("Cannot find measure with uid =" + uid));
        }
      } else {
        fmt::print("Missing the uid in the post data\n");
        message.reply(web::http::status_codes::BadRequest, web::json::value("Missing the uid in the post data"));
      }
    });
    return;
  }

  // TODO: for testing only, remove
  if (uri == "/get_model") {
    message.extract_json().then([this, message](web::json::value body) {
      if (body.has_field("osm_path")) {
        auto osmInfo_ = m_measureManager.getModel(openstudio::toPath(body.at("osm_path").as_string()));
        if (osmInfo_) {
          fmt::print("{}\n", osmInfo_->checksum);
          message.reply(web::http::status_codes::OK, web::json::value("OK"));
        } else {
          message.reply(web::http::status_codes::BadRequest, web::json::value("Wrong osm path"));
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
  }

  if (uri == "/compute_arguments") {
  }

  message.reply(web::http::status_codes::NotFound, web::json::value("404: Unknown Endpoint"));
}

}  // namespace openstudio
