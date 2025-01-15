/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AlfalfaJSON.hpp"
#include "AlfalfaJSON_Impl.hpp"

#include "AlfalfaComponent.hpp"
#include "AlfalfaActuator.hpp"
#include "AlfalfaConstant.hpp"
#include "AlfalfaMeter.hpp"
#include "AlfalfaGlobalVariable.hpp"
#include "AlfalfaOutputVariable.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/Logger.hpp"

#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idd/IddEnums.hpp"

#include <json/json.h>
#include <boost/optional.hpp>
#include <boost/none.hpp>
#include <fmt/format.h>

#include <vector>
#include <fstream>
#include <memory>
#include <utility>
#include <string>

namespace openstudio {
namespace alfalfa {
  namespace detail {
    AlfalfaJSON_Impl::AlfalfaJSON_Impl() = default;

    AlfalfaJSON_Impl::AlfalfaJSON_Impl(const openstudio::path& p) {
      setJSONPath(p);
    }

    void AlfalfaJSON_Impl::exposePoint(const AlfalfaPoint& point) {
      m_points.push_back(point);
    }

    std::vector<AlfalfaPoint> AlfalfaJSON_Impl::points() const {
      return m_points;
    }

    bool AlfalfaJSON_Impl::saveAs(const openstudio::path& path) {
      m_JSONPath = path;
      return save();
    }

    bool AlfalfaJSON_Impl::save() const {
      if (m_JSONPath.empty()) {
        LOG(Error, "Unable to write Alfalfa JSON as path has not been specified");
        return false;
      }
      if (makeParentFolder(m_JSONPath)) {
        const Json::Value root = toJSON();
        std::ofstream outFile(openstudio::toSystemFilename(m_JSONPath));

        if (outFile) {
          try {
            outFile << root;
            outFile.close();
            return true;
          } catch (...) {
            LOG(Error, "Unable to write file to path '" << toString(m_JSONPath) << "'.");
            return false;
          }
        }
      }
      LOG(Error, "Unable to write file to path '" << toString(m_JSONPath) << "', because parent directory " << "could not be created.");

      return false;
    }

    void AlfalfaJSON_Impl::setJSONPath(const openstudio::path& p) {
      m_JSONPath = p;
    }

    Json::Value AlfalfaJSON_Impl::toJSON() const {
      Json::Value root;
      for (Json::ArrayIndex i = 0; const auto& point : points()) {
        // No guard here as the toJSON call will throw an exception if the id does not exist.
        root[i++] = point.toJSON();
      }
      return root;
    }
  }  // namespace detail

  AlfalfaJSON::AlfalfaJSON() : m_impl(std::make_shared<detail::AlfalfaJSON_Impl>()) {}

  AlfalfaJSON::AlfalfaJSON(const std::string& s) : m_impl(std::make_shared<detail::AlfalfaJSON_Impl>(s)) {}

  AlfalfaJSON::AlfalfaJSON(const openstudio::path& p) : m_impl(std::make_shared<detail::AlfalfaJSON_Impl>(p)) {}

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeConstant(float value, const std::string& display_name) {
    return exposeFromComponent(AlfalfaConstant{value}, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeMeter(const std::string& meter_name, const std::string& display_name) {
    return exposeFromComponent(AlfalfaMeter{meter_name}, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeActuator(const std::string& component_name, const std::string& component_type,
                                                            const std::string& control_type, const std::string& display_name) {
    return exposeFromComponent(AlfalfaActuator{component_name, component_type, control_type}, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeOutputVariable(const std::string& variable_key, const std::string& variable_name,
                                                                  const std::string& display_name) {
    return exposeFromComponent(AlfalfaOutputVariable{variable_key, variable_name}, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeGlobalVariable(const std::string& variable_name, const std::string& display_name) {
    return exposeFromComponent(AlfalfaGlobalVariable{variable_name}, display_name);
  }

  void AlfalfaJSON::exposePoint(const AlfalfaPoint& point) {
    m_impl->exposePoint(point);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeFromObject(const openstudio::IdfObject& idf_object, const std::string& display_name) {
    const IddObjectType idd_type = idf_object.iddObject().type();
    std::string display_name_ = display_name;

    if (display_name.empty()) {
      if (auto name_ = getName(idf_object)) {
        display_name_ = std::move(*name_);
      }
    }

    boost::optional<AlfalfaComponent> component;
    try {
      if (AlfalfaActuator::acceptsObjectType(idd_type)) {
        component = AlfalfaActuator(idf_object);
      } else if (AlfalfaMeter::acceptsObjectType(idd_type)) {
        component = AlfalfaMeter(idf_object);
      } else if (AlfalfaOutputVariable::acceptsObjectType(idd_type)) {
        component = AlfalfaOutputVariable(idf_object);
      } else if (AlfalfaGlobalVariable::acceptsObjectType(idd_type)) {
        component = AlfalfaGlobalVariable(idf_object);
      } else {
        LOG(Error, "Unable to create Alfalfa Point from Object of type " + idd_type.valueDescription());
        return boost::none;
      }
    } catch (std::runtime_error& error) {
      LOG(Error, error.what());
      return boost::none;
    }
    return exposeFromComponent(*component, display_name_);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeFromComponent(const AlfalfaComponent& component, const std::string& display_name) {
    std::string display_name_ = display_name;
    if (display_name.empty()) {
      display_name_ = component.deriveName();
      if (display_name_.empty()) {
        LOG(Error, fmt::format("{} points must be provided with a display name", component.typeName()));
        return boost::none;
      }
    }

    AlfalfaPoint point(display_name_);
    if (component.canInput()) {
      point.setInput(component);
    }
    if (component.canOutput()) {
      point.setOutput(component);
    }
    exposePoint(point);
    return {std::move(point)};
  }

  boost::optional<std::string> AlfalfaJSON::getName(const openstudio::IdfObject& idf_object) {
    if (auto mo_ = idf_object.optionalCast<openstudio::model::ModelObject>()) {
      if (boost::optional<std::string> display_name = mo_->displayName()) {
        return display_name;
      }
    }
    if (auto name_ = idf_object.name()) {
      if (!name_->empty()) {
        return name_;
      }
    }
    return boost::none;
  }

  bool AlfalfaJSON::saveAs(const openstudio::path& p) {
    return m_impl->saveAs(p);
  }

  bool AlfalfaJSON::save() const {
    return m_impl->save();
  }

  Json::Value AlfalfaJSON::toJSON() const {
    return m_impl->toJSON();
  }

  std::vector<AlfalfaPoint> AlfalfaJSON::points() const {
    return m_impl->points();
  }

}  // namespace alfalfa
}  // namespace openstudio
