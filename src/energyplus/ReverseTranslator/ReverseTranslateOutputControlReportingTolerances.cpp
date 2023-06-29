/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputControlReportingTolerances.hpp"
#include "../../model/OutputControlReportingTolerances_Impl.hpp"

#include <utilities/idd/OutputControl_ReportingTolerances_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputControlReportingTolerances(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // This is a Unique ModelObject
    openstudio::model::OutputControlReportingTolerances modelObject = m_model.getUniqueModelObject<OutputControlReportingTolerances>();

    if (boost::optional<double> _toleranceforTimeHeatingSetpointNotMet =
          workspaceObject.getDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, true)) {
      modelObject.setToleranceforTimeHeatingSetpointNotMet(_toleranceforTimeHeatingSetpointNotMet.get());
    }

    if (boost::optional<double> _toleranceforTimeCoolingSetpointNotMet =
          workspaceObject.getDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, true)) {
      modelObject.setToleranceforTimeCoolingSetpointNotMet(_toleranceforTimeCoolingSetpointNotMet.get());
    }

    result = modelObject;
    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
