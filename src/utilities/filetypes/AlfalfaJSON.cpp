#include "AlfalfaJSON.hpp"
#include "AlfalfaJSON_Impl.hpp"
#include "../alfalfa/AlfalfaComponents.hpp"
#include "../core/PathHelpers.hpp"

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
        Json::Value root;
        for (AlfalfaPoint point : m_points) {
          root[point.id()] = point.toJSON();
        }
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
  }  // namespace detail

  AlfalfaJSON::AlfalfaJSON() : m_impl(std::shared_ptr<detail::AlfalfaJSON_Impl>(new detail::AlfalfaJSON_Impl())) {}

  AlfalfaJSON::AlfalfaJSON(const std::string& s) : m_impl(std::shared_ptr<detail::AlfalfaJSON_Impl>(new detail::AlfalfaJSON_Impl(s))) {}

  AlfalfaJSON::AlfalfaJSON(const openstudio::path& p) : m_impl(std::shared_ptr<detail::AlfalfaJSON_Impl>(new detail::AlfalfaJSON_Impl(p))) {}

  AlfalfaPoint AlfalfaJSON::exposeConstant(float value, const std::string& display_name) {
    AlfalfaConstant component(value);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    exposePoint(point);
    return point;
  }

  AlfalfaPoint AlfalfaJSON::exposeMeter(const std::string& meter_name, const std::string& display_name) {
    std::string _display_name = display_name;
    AlfalfaMeter component(meter_name);
    if (display_name.size() == 0) {
      _display_name = "Meter for " + meter_name;
    }
    AlfalfaPoint point(_display_name);
    point.setOutput(component);
    exposePoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeMeter(const openstudio::IdfObject& output_meter, const std::string& display_name) {
    IddObjectType object_type = output_meter.iddObject().type();
    boost::optional<std::string> meter_name = boost::none;
    if (object_type == IddObjectType::Output_Meter) {
      meter_name = output_meter.getString(Output_MeterFields::KeyName);
    } else if (object_type == IddObjectType::OS_Output_Meter) {
      meter_name = output_meter.getString(OS_Output_MeterFields::Name);
    } else {
      LOG(Error, "Unable to create Meter from Object of type " + object_type.valueDescription());
      return boost::none;
    }

    if (!meter_name.is_initialized()) {
      LOG(Error, "Unable to create Meter from Output Meter without a Name");
      return boost::none;
    }

    boost::optional<AlfalfaPoint> result = exposeMeter(display_name, meter_name.get());
    return result;
  }

  AlfalfaPoint AlfalfaJSON::exposeActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type,
                                           const std::string& display_name) {
    std::string _display_name = display_name;
    AlfalfaActuator component(component_name, component_type, control_type);
    if (display_name.size() == 0) {
      _display_name = "Actuator for " + component_name + ":" + component_type + ":" + control_type;
    }
    AlfalfaPoint point(_display_name);
    point.setOutput(component);
    point.setInput(component);
    exposePoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeActuator(const openstudio::IdfObject& actuator, const std::string& display_name) {
    IddObjectType idd_type = actuator.iddObject().type();
    std::string _display_name = display_name;
    boost::optional<std::string> name;
    boost::optional<std::string> component_name;
    boost::optional<std::string> component_type;
    boost::optional<std::string> control_type;

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_Actuator) {
      name = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::Name);
      component_name = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName);
      component_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
      control_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
    } else if (idd_type == IddObjectType::EnergyManagementSystem_Actuator) {
      name = actuator.getString(EnergyManagementSystem_ActuatorFields::Name);
      component_name = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName);
      component_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
      control_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
    } else {
      LOG(Error, "Unable to create Actuator from Object of type " + idd_type.valueDescription());
      return boost::none;
    }

    if (!component_name.is_initialized()) {
      LOG(Error, "Unable to create Actuator from EMS Actuator without a Component Name");
      return boost::none;
    }
    if (!component_type.is_initialized()) {
      LOG(Error, "Unable to create Actuator from EMS Actuator without a Component Type");
      return boost::none;
    }
    if (!control_type.is_initialized()) {
      LOG(Error, "Unable to create Actuator from EMS Actuator without a Control Type");
      return boost::none;
    }

    if (display_name.size() == 0) {
      if (name.is_initialized()) {
        _display_name = name.get();
      }
    }

    boost::optional<AlfalfaPoint> result = exposeActuator(component_name.get(), component_type.get(), control_type.get(), _display_name);
    return result;
  }

  AlfalfaPoint AlfalfaJSON::exposeOutputVariable(const std::string& variable_key, const std::string& variable_name, const std::string& display_name) {
    std::string _display_name = display_name;
    AlfalfaOutputVariable component(variable_key, variable_name);
    if (display_name.size() == 0) {
      _display_name = "Output Variable for " + variable_key + ":" + variable_name;
    }
    AlfalfaPoint point(_display_name);
    point.setOutput(component);
    exposePoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeOutputVariable(const openstudio::IdfObject& output_variable, const std::string& display_name) {
    IddObjectType idd_type = output_variable.iddObject().type();
    std::string _display_name = display_name;
    boost::optional<std::string> name;
    boost::optional<std::string> variable_key;
    boost::optional<std::string> variable_name;

    if (idd_type == IddObjectType::OS_Output_Variable) {
      name = output_variable.getString(OS_Output_VariableFields::Name);
      variable_key = output_variable.getString(OS_Output_VariableFields::KeyValue);
      variable_name = output_variable.getString(OS_Output_VariableFields::VariableName);
    } else if (idd_type == IddObjectType::Output_Variable) {
      variable_key = output_variable.getString(Output_VariableFields::KeyValue);
      variable_name = output_variable.getString(Output_VariableFields::VariableName);
    } else if (idd_type == IddObjectType::OS_EnergyManagementSystem_OutputVariable) {
      name = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::Name);
      variable_key = "EMS";
      variable_name = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName);
    } else if (idd_type == IddObjectType::EnergyManagementSystem_OutputVariable) {
      name = output_variable.getString(EnergyManagementSystem_OutputVariableFields::Name);
      variable_key = "EMS";
      variable_name = output_variable.getString(EnergyManagementSystem_OutputVariableFields::EMSVariableName);
    } else {
      LOG(Error, "Unable to create Output Variable from Object of type " + idd_type.valueDescription());
      return boost::none;
    }

    if (!variable_key.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from Output Variable without a Key");
      return boost::none;
    }
    if (!variable_name.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from Output Variable without a Variable Name");
      return boost::none;
    }

    if (display_name.size() == 0) {
      if (name.is_initialized()) {
        _display_name = name.get();
      }
    }

    boost::optional<AlfalfaPoint> result = exposeOutputVariable(variable_key.get(), variable_name.get(), _display_name);
    return result;
  }

  AlfalfaPoint AlfalfaJSON::exposeGlobalVariable(const std::string& variable_name, const std::string& display_name) {
    std::string _display_name = display_name;
    AlfalfaGlobalVariable component(variable_name);
    if (display_name.size() == 0) {
      _display_name = "Global Variable for " + variable_name;
    }
    AlfalfaPoint point(_display_name);
    point.setInput(component);
    point.setOutput(component);
    exposePoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::exposeGlobalVariable(const openstudio::IdfObject& global_variable, const std::string& display_name) {
    IddObjectType idd_type = global_variable.iddObject().type();
    boost::optional<std::string> variable_name;

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_GlobalVariable) {
      variable_name = global_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::Name);
    } else if (idd_type == IddObjectType::EnergyManagementSystem_GlobalVariable) {
      variable_name = global_variable.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName);
    } else {
      LOG(Error, "Unable to create Global Variable from Object of type " + idd_type.valueDescription());
      return boost::none;
    }

    if (!variable_name.is_initialized()) {
      LOG(Error, "Unable to create Global Variable from EMS Output Variable without a Name");
      return boost::none;
    }

    boost::optional<AlfalfaPoint> result = exposeGlobalVariable(variable_name.get(), display_name);
    return result;
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

  bool AlfalfaJSON::saveAs(const openstudio::path& p) {
    return m_impl->saveAs(p);
  }

  bool AlfalfaJSON::save() const {
    return m_impl->save();
  }

  std::vector<AlfalfaPoint> AlfalfaJSON::getPoints() {
    return m_impl->getPoints();
  }

}  // namespace alfalfa
}  // namespace openstudio
