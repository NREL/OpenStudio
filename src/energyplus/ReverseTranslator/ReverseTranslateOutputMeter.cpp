/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"

#include <utilities/idd/Output_Meter_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateOutputMeter(const WorkspaceObject& workspaceObject) {
    openstudio::model::OutputMeter meter(m_model);

    OptionalString s = workspaceObject.getString(Output_MeterFields::KeyName);
    if (s) {
      meter.setName(*s);
    }

    s = workspaceObject.getString(Output_MeterFields::ReportingFrequency);
    if (s) {
      meter.setReportingFrequency(*s);
    }

    meter.setMeterFileOnly(false);

    meter.setCumulative(false);

    return meter;
  }

}  // namespace energyplus

}  // namespace openstudio
