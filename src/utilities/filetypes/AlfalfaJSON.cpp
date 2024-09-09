#include "AlfalfaJSON.hpp"
#include "AlfalfaJSON_Impl.hpp"
#include "utilities/alfalfa/AlfalfaActuator.hpp"
#include "utilities/alfalfa/AlfalfaOutputVariable.hpp"
#include "utilities/alfalfa/AlfalfaGlobalVariable.hpp"
#include "utilities/alfalfa/AlfalfaMeter.hpp"
#include "utilities/alfalfa/AlfalfaConstant.hpp"
#include "../core/PathHelpers.hpp"
#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <iostream>
namespace openstudio {
namespace alfalfa {
  namespace detail {
    AlfalfaJSON_Impl::AlfalfaJSON_Impl() {}

    AlfalfaJSON_Impl::AlfalfaJSON_Impl(const std::string& s) {}

    AlfalfaJSON_Impl::AlfalfaJSON_Impl(const openstudio::path& p) {}

    void AlfalfaJSON_Impl::exposePoint(const AlfalfaPoint& point) {
      m_points.push_back(point);
    }

    std::vector<AlfalfaPoint> AlfalfaJSON_Impl::getPoints() {
      return m_points;
    }

    bool AlfalfaJSON_Impl::saveAs(const openstudio::path& path) {
      m_JSONPath = path;
      return save();
    }

    bool AlfalfaJSON_Impl::save() const {
      if (makeParentFolder(m_JSONPath)) {
        Json::Value root = toJSON();
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
      LOG(Error, "Unable to write file to path '" << toString(m_JSONPath) << "', because parent directory "
                                                  << "could not be created.");

      return false;
    }

    Json::Value AlfalfaJSON_Impl::toJSON() const {
      Json::Value root;
      for (AlfalfaPoint point : m_points) {
        // No guard here as the toJSON call will throw an exception if the id does not exist.
        root[point.id().get()] = point.toJSON();
      }
      return root;
    }
  }  // namespace detail

  AlfalfaJSON::AlfalfaJSON() : m_impl(std::shared_ptr<detail::AlfalfaJSON_Impl>(new detail::AlfalfaJSON_Impl())) {}

  AlfalfaJSON::AlfalfaJSON(const std::string& s) : m_impl(std::shared_ptr<detail::AlfalfaJSON_Impl>(new detail::AlfalfaJSON_Impl(s))) {}

  AlfalfaJSON::AlfalfaJSON(const openstudio::path& p) : m_impl(std::shared_ptr<detail::AlfalfaJSON_Impl>(new detail::AlfalfaJSON_Impl(p))) {}

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeConstant(float value, const std::string& display_name) {
    AlfalfaConstant component(value);
    return exposePoint(component, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeMeter(const std::string& meter_name, const std::string& display_name) {
    AlfalfaMeter component(meter_name);
    return exposePoint(component, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeMeter(const openstudio::IdfObject& output_meter, const std::string& display_name) {
    std::string _display_name = display_name;
    boost::optional<std::string> name = getName(output_meter);

    try {
      AlfalfaMeter component(output_meter);
      if (display_name.size() == 0) {
        if (name.is_initialized()) {
          _display_name = name.get();
        }
      }
      return exposePoint(component, _display_name);
    } catch (...) {
      return boost::none;
    }
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeActuator(const std::string& component_name, const std::string& component_type,
                                                            const std::string& control_type, const std::string& display_name) {
    AlfalfaActuator component(component_name, component_type, control_type);
    return exposePoint(component, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeActuator(const openstudio::IdfObject& actuator, const std::string& display_name) {
    std::string _display_name = display_name;
    boost::optional<std::string> name = getName(actuator);

    try {
      AlfalfaActuator component(actuator);

      if (display_name.size() == 0) {
        if (name.is_initialized()) {
          _display_name = name.get();
        }
      }
      return exposePoint(component, _display_name);
    } catch (...) {
      return boost::none;
    }
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeOutputVariable(const std::string& variable_key, const std::string& variable_name,
                                                                  const std::string& display_name) {
    AlfalfaOutputVariable component(variable_key, variable_name);
    return exposePoint(component, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeOutputVariable(const openstudio::IdfObject& output_variable, const std::string& display_name) {
    std::string _display_name = display_name;
    boost::optional<std::string> name = getName(output_variable);

    try {
      AlfalfaOutputVariable component(output_variable);

      if (display_name.size() == 0) {
        if (name.is_initialized()) {
          _display_name = name.get();
        }
      }

      return exposePoint(component, _display_name);
    } catch (...) {
      return boost::none;
    }
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeGlobalVariable(const std::string& variable_name, const std::string& display_name) {
    AlfalfaGlobalVariable component(variable_name);
    return exposePoint(component, display_name);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeGlobalVariable(const openstudio::IdfObject& global_variable, const std::string& display_name) {
    std::string _display_name = display_name;
    boost::optional<std::string> name = getName(global_variable);
    try {
      AlfalfaGlobalVariable component(global_variable);

      if (display_name.size() == 0) {
        if (name.is_initialized()) {
          _display_name = name.get();
        }
      }

      return exposePoint(component, _display_name);
    } catch (...) {
      return boost::none;
    }
  }

  void AlfalfaJSON::exposePoint(const AlfalfaPoint& point) {
    m_impl->exposePoint(point);
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposePoint(const openstudio::IdfObject& idf_object, const std::string& display_name) {
    IddObjectType idd_type = idf_object.iddObject().type();
    if (idd_type == IddObjectType::OS_Output_Meter || idd_type == IddObjectType::Output_Meter) {
      return exposeMeter(idf_object, display_name);
    } else if (idd_type == IddObjectType::OS_EnergyManagementSystem_Actuator || idd_type == IddObjectType::EnergyManagementSystem_Actuator) {
      return exposeActuator(idf_object, display_name);
    } else if (idd_type == IddObjectType::OS_Output_Variable || idd_type == IddObjectType::Output_Variable
               || idd_type == IddObjectType::EnergyManagementSystem_OutputVariable
               || idd_type == IddObjectType::OS_EnergyManagementSystem_OutputVariable) {
      return exposeOutputVariable(idf_object, display_name);
    } else if (idd_type == IddObjectType::OS_EnergyManagementSystem_GlobalVariable
               || idd_type == IddObjectType::EnergyManagementSystem_GlobalVariable) {
      return exposeGlobalVariable(idf_object, display_name);
    } else {
      LOG(Error, "Unable to create Alfalfa Point from Object of type " + idd_type.valueDescription());
      return boost::none;
    }
    return boost::none;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposePoint(const AlfalfaComponent& component, const std::string& display_name) {
    std::string _display_name = display_name;
    if (display_name.size() == 0) {
      if (component.type == "Actuator") {
        _display_name = "Actuator for " + component.parameters["component_name"].asString() + ":" + component.parameters["component_type"].asString()
                        + ":" + component.parameters["control_type"].asString();
      } else if (component.type == "Constant") {
        LOG(Error, "Constant points must be provided with a display name");
        return boost::none;
      } else if (component.type == "Meter") {
        _display_name = "Meter for " + component.parameters["meter_name"].asString();
      } else if (component.type == "GlobalVariable") {
        _display_name = "Global Variable for " + component.parameters["variable_name"].asString();
      } else if (component.type == "OutputVariable") {
        _display_name =
          "Output Variable for " + component.parameters["variable_key"].asString() + ":" + component.parameters["variable_name"].asString();
      } else {
        LOG(Error, "Invalid component type");
        return boost::none;
      }
    }

    AlfalfaPoint point(_display_name);
    if (component.canInput()) {
      point.setInput(component);
    }
    if (component.canOutput()) {
      point.setOutput(component);
    }
    exposePoint(point);
    boost::optional<AlfalfaPoint> result = point;
    return result;
  }

  boost::optional<std::string> AlfalfaJSON::getName(const openstudio::IdfObject& idf_object) {
    if (dynamic_cast<const openstudio::model::ModelObject*>(&idf_object) != nullptr) {
      boost::optional<std::string> display_name = dynamic_cast<const openstudio::model::ModelObject*>(&idf_object)->displayName();
      if (display_name.is_initialized()) {
        return display_name;
      }
    }
    if (idf_object.name().is_initialized() && idf_object.name().get().size() > 0) {
      return idf_object.name();
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

  std::vector<AlfalfaPoint> AlfalfaJSON::getPoints() {
    return m_impl->getPoints();
  }

}  // namespace alfalfa
}  // namespace openstudio
