/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AlfalfaMeter.hpp"

#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaMeter::AlfalfaMeter(const std::string& meter_name) : m_meter_name(meter_name) {
    if (meter_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaMeter: meter_name must be non-empty");
    }
  }

  AlfalfaMeter::AlfalfaMeter(const IdfObject& output_meter) {
    const IddObjectType idd_type = output_meter.iddObject().type();
    if (idd_type == IddObjectType::Output_Meter) {
      m_meter_name = output_meter.getString(Output_MeterFields::KeyName).value_or("");
    } else if (idd_type == IddObjectType::OS_Output_Meter) {
      m_meter_name = output_meter.getString(OS_Output_MeterFields::Name).value_or("");
    } else {
      throw std::runtime_error(fmt::format("Error creating AlfalfaMeter: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (m_meter_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaMeter: Object is missing a meter name");
    }
  }

  std::string AlfalfaMeter::deriveName() const {
    return "Meter for " + m_meter_name;
  }

  Json::Value AlfalfaMeter::toJSON() const {
    Json::Value parameters;
    parameters["meter_name"] = m_meter_name;

    return parameters;
  }

  std::string AlfalfaMeter::meterName() const {
    return m_meter_name;
  }

  bool AlfalfaMeter::acceptsObjectType(const IddObjectType& idd_type) {
    return idd_type.value() == IddObjectType::OS_Output_Meter || idd_type.value() == IddObjectType::Output_Meter;
  }

}  // namespace alfalfa
}  // namespace openstudio
