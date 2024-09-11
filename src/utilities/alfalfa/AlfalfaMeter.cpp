#include "AlfalfaMeter.hpp"

#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaMeter::AlfalfaMeter(const std::string& meter_name) : AlfalfaComponent("Meter", Capability::Output) {
    if (meter_name.size() == 0) {
      throw std::runtime_error("Error creating AlfalfaMeter: meter_name must be non-empty");
    }
    parameters["meter_name"] = meter_name;
  }

  AlfalfaMeter::AlfalfaMeter(const IdfObject& output_meter) : AlfalfaComponent("Meter", Capability::Output) {
    IddObjectType idd_type = output_meter.iddObject().type();
    boost::optional<std::string> meter_name = boost::none;
    if (idd_type == IddObjectType::Output_Meter) {
      meter_name = output_meter.getString(Output_MeterFields::KeyName);
    } else if (idd_type == IddObjectType::OS_Output_Meter) {
      meter_name = output_meter.getString(OS_Output_MeterFields::Name);
    } else {
      throw std::runtime_error(fmt::format("Error creating AlfalfaMeter: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (!meter_name.is_initialized() || meter_name.get().size() == 0) {
      throw std::runtime_error("Error creating AlfalfaMeter: Object is missing a meter name");
    }

    parameters["meter_name"] = meter_name.get();
  }
}  // namespace alfalfa
}  // namespace openstudio
