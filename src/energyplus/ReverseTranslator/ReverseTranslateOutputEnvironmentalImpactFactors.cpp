/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputEnvironmentalImpactFactors.hpp"

#include <utilities/idd/Output_EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputEnvironmentalImpactFactors(const WorkspaceObject& workspaceObject) {

    // Reporting Frequency: Optional String
    if (boost::optional<std::string> _reportingFrequency = workspaceObject.getString(Output_EnvironmentalImpactFactorsFields::ReportingFrequency)) {
      openstudio::model::OutputEnvironmentalImpactFactors modelObject(m_model);
      modelObject.setReportingFrequency(_reportingFrequency.get());
      return modelObject;
    }

    return boost::none;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
