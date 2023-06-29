/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputVariable(OutputVariable& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_Variable, modelObject);

    if (!modelObject.isKeyValueDefaulted()) {
      idfObject.setString(Output_VariableFields::KeyValue, modelObject.keyValue());
    }

    idfObject.setString(Output_VariableFields::VariableName, modelObject.variableName());

    if (!modelObject.isReportingFrequencyDefaulted()) {
      idfObject.setString(Output_VariableFields::ReportingFrequency, modelObject.reportingFrequency());
    }

    if (modelObject.schedule()) {
      idfObject.setString(Output_VariableFields::ScheduleName, modelObject.schedule()->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
