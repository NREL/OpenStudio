/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Output_Variable_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateOutputVariable(const WorkspaceObject& workspaceObject) {

    OptionalString s = workspaceObject.getString(Output_VariableFields::VariableName);
    if (!s) {
      return boost::none;
    }

    openstudio::model::OutputVariable outputVariable(*s, m_model);

    s = workspaceObject.getString(Output_VariableFields::KeyValue);
    if (s) {
      outputVariable.setKeyValue(*s);
    }

    s = workspaceObject.getString(Output_VariableFields::VariableName);
    if (s) {
      outputVariable.setVariableName(*s);
    }

    s = workspaceObject.getString(Output_VariableFields::ReportingFrequency);
    if (s) {
      outputVariable.setReportingFrequency(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(Output_VariableFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        OptionalSchedule schedule = modelObject->optionalCast<Schedule>();
        if (schedule) {
          outputVariable.setSchedule(*schedule);
        }
      }
    }

    return outputVariable;
  }

}  // namespace energyplus

}  // namespace openstudio
