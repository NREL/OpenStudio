/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/OutputControlReportingTolerances.hpp"
#include <utilities/idd/OutputControl_ReportingTolerances_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputControlReportingTolerances(OutputControlReportingTolerances& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::OutputControl_ReportingTolerances, modelObject);

    boost::optional<double> value;

    if ((value = modelObject.toleranceforTimeCoolingSetpointNotMet())) {
      idfObject.setDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, value.get());
    }

    if ((value = modelObject.toleranceforTimeHeatingSetpointNotMet())) {
      idfObject.setDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
