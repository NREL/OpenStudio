#ifndef ALFALFA_CONSTANT_HPP
#define ALFALFA_CONSTANT_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaActuator : public AlfalfaComponent
  {
    public:
      AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type) :
        AlfalfaComponent("Actuator", ComponentCapabilities::Output | ComponentCapabilities::Input) {
          m_root["component_name"] = component_name;
          m_root["component_type"] = component_type;
          m_root["control_type"] = control_type;
        }
  };

  class AlfalfaConstant : public AlfalfaComponent
  {
   public:
    AlfalfaConstant(float value) :
      AlfalfaComponent("Constant", ComponentCapabilities::Input | ComponentCapabilities::Output) {
      m_root["value"] = value;
    }
  };

  class AlfalfaGlobalVariable : public AlfalfaComponent
  {
    public:
      AlfalfaGlobalVariable(const std::string& variable_name) :
        AlfalfaComponent("GlobalVariable", ComponentCapabilities::Output | ComponentCapabilities::Input) {
          m_root["variable_name"] = variable_name;
        }
  };

  class AlfalfaMeter : public AlfalfaComponent
  {
    public:
      AlfalfaMeter(const std::string& meter_name) :
        AlfalfaComponent("Meter", ComponentCapabilities::Output) {
          m_root["meter_name"] = meter_name;
        }
  };

  class AlfalfaOutputVariable : public AlfalfaComponent
  {
    public:
      AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name) :
        AlfalfaComponent("OutputVariable", ComponentCapabilities::Output | ComponentCapabilities::Input) {
          m_root["variable_key"] = variable_key;
          m_root["variable_name"] = variable_name;
        }
  };


}  // namespace alfalfa
}  // namespace openstudio

#endif
