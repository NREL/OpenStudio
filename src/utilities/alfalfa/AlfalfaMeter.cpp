#include "AlfalfaMeter.hpp"

#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace alfalfa {
  AlfalfaMeter::AlfalfaMeter(const std::string& meter_name) : AlfalfaComponent("Meter", Capability::Output) {
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
      throw std::runtime_error("Unable to create Meter from Object of type " + idd_type.valueDescription());
    }

    if (!meter_name.is_initialized() || meter_name.get().size() == 0) {
      throw std::runtime_error("Unable to create Meter from Output Meter without a Name");
    }

    parameters["meter_name"] = meter_name.get();
  }
}  // namespace alfalfa
}  // namespace openstudio
