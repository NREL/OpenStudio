#include "AlfalfaJSON.hpp"
#include "../alfalfa/AlfalfaConstant.hpp"
#include "../alfalfa/AlfalfaMeter.hpp"
#include "../alfalfa/AlfalfaActuator.hpp"
#include "../alfalfa/AlfalfaOutputVariable.hpp"
#include "../alfalfa/AlfalfaGlobalVariable.hpp"
#include "../core/PathHelpers.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>

namespace openstudio {
namespace alfalfa {

  AlfalfaJSON::AlfalfaJSON() {}

  AlfalfaJSON::AlfalfaJSON(const std::string& s) {}

  AlfalfaJSON::AlfalfaJSON(const openstudio::path& p) {}

  AlfalfaPoint AlfalfaJSON::addConstant(const std::string& display_name, float value) {
    AlfalfaConstant component(value);
    AlfalfaPoint point(display_name);
    point.setInput(component);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  AlfalfaPoint AlfalfaJSON::addMeter(const std::string& display_name, const std::string& meter_name) {
    AlfalfaMeter component(meter_name);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addMeter(const std::string& display_name, const openstudio::model::OutputMeter& output_meter) {
    boost::optional<std::string> meter_name = output_meter.getString(OS_Output_MeterFields::Name);
    if (!meter_name.is_initialized()) {
      LOG(Error, "Unable to create Meter from Output Meter without a Name");
      return boost::none;
    }
    boost::optional<AlfalfaPoint> result = addMeter(display_name, meter_name.get());
    return result;
  }

  AlfalfaPoint AlfalfaJSON::addActuator(const std::string& display_name, const std::string& component_name, const std::string& component_type,
                                        const std::string& control_type) {
    AlfalfaActuator component(component_name, component_type, control_type);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    point.setInput(component);
    addPoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addActuator(const std::string& display_name,
                                                         const openstudio::model::EnergyManagementSystemActuator& actuator) {
    boost::optional<std::string> component_name = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName);
    boost::optional<std::string> component_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
    boost::optional<std::string> control_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
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
    boost::optional<AlfalfaPoint> result = addActuator(display_name, component_name.get(), component_type.get(), control_type.get());
    return result;
  }

  AlfalfaPoint AlfalfaJSON::addOutputVariable(const std::string& display_name, const std::string& variable_key, const std::string& variable_name) {
    AlfalfaOutputVariable component(variable_key, variable_name);
    AlfalfaPoint point(display_name);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addOutputVariable(const std::string& display_name,
                                                               const openstudio::model::OutputVariable& output_variable) {
    boost::optional<std::string> variable_key = output_variable.getString(OS_Output_VariableFields::KeyValue);
    boost::optional<std::string> variable_name = output_variable.getString(OS_Output_VariableFields::VariableName);
    if (!variable_key.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from Output Variable without a Key");
      return boost::none;
    }
    if (!variable_name.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from Output Variable without a Variable Name");
      return boost::none;
    }
    boost::optional<AlfalfaPoint> result = addOutputVariable(display_name, variable_key.get(), variable_name.get());
    return result;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addOutputVariable(const openstudio::model::OutputVariable& output_variable) {
    boost::optional<std::string> name = output_variable.getString(OS_Output_VariableFields::Name);
    if (!name.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from Output Variable without a Name");
      return boost::none;
    }
    boost::optional<AlfalfaPoint> result = addOutputVariable(name.get(), output_variable);
    return result;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addOutputVariable(const std::string& display_name,
                                                               const openstudio::model::EnergyManagementSystemOutputVariable& output_variable) {
    boost::optional<std::string> variable_name = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName);
    boost::optional<std::string> units = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::Units);
    if (!variable_name.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from EMS Output Variable without a Variable Name");
      return boost::none;
    }
    boost::optional<AlfalfaPoint> result = addOutputVariable(display_name, "EMS", variable_name.get());
    if (units.is_initialized() && result.is_initialized()) {
      result->setUnits(units.get());
    }
    return result;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addOutputVariable(const openstudio::model::EnergyManagementSystemOutputVariable& output_variable) {
    boost::optional<std::string> name = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::Name);
    if (!name.is_initialized()) {
      LOG(Error, "Unable to create Output Variable from EMS Output Variable without a Name");
      return boost::none;
    }
    boost::optional<AlfalfaPoint> result = addOutputVariable(name.get(), output_variable);
    return result;
  }

  AlfalfaPoint AlfalfaJSON::addGlobalVariable(const std::string& display_name, const std::string& variable_name) {
    AlfalfaGlobalVariable component(variable_name);
    AlfalfaPoint point(display_name);
    point.setInput(component);
    point.setOutput(component);
    addPoint(point);
    return point;
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addGlobalVariable(const std::string& display_name,
                                                               const openstudio::model::EnergyManagementSystemGlobalVariable& global_variable) {
    boost::optional<std::string> variable_name = global_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::Name);
    if (variable_name.is_initialized()) {
      boost::optional<AlfalfaPoint> result = addGlobalVariable(display_name, variable_name.get());
      return result;
    } else {
      LOG(Error, "Unable to create Global Variable from EMS Output Variable without a Name");
      return boost::none;
    }
  }

  boost::optional<AlfalfaPoint> AlfalfaJSON::addGlobalVariable(const openstudio::model::EnergyManagementSystemGlobalVariable& global_variable) {
    boost::optional<std::string> name = global_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::Name);
    if (name.is_initialized()) {
      boost::optional<AlfalfaPoint> result = addGlobalVariable(name.get(), global_variable);
      return result;
    } else {
      LOG(Error, "Unable to create Global Variable from EMS Output Variable without a Name");
      return boost::none;
    }
  }

  void AlfalfaJSON::addPoint(const AlfalfaPoint& point) {
    m_points.push_back(point);
  }

  bool AlfalfaJSON::saveAs(const openstudio::path& p) {
    m_JSONPath = p;
    return save();
  }

  bool AlfalfaJSON::save() const {
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

}  // namespace alfalfa
}  // namespace openstudio
