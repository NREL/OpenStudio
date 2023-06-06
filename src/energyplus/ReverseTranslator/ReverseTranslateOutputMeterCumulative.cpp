/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"

#include <utilities/idd/Output_Meter_Cumulative_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateOutputMeterCumulative(const WorkspaceObject& workspaceObject) {
    openstudio::model::OutputMeter meter(m_model);

    OptionalString s = workspaceObject.getString(Output_Meter_CumulativeFields::KeyName);
    if (s) {
      meter.setName(*s);
    }

    s = workspaceObject.getString(Output_Meter_CumulativeFields::ReportingFrequency);
    if (s) {
      meter.setReportingFrequency(*s);
    }

    meter.setMeterFileOnly(false);

    meter.setCumulative(true);

    return meter;
  }

}  // namespace energyplus

}  // namespace openstudio
