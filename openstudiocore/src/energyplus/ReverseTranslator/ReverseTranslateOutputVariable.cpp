/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateOutputVariable( const WorkspaceObject & workspaceObject )
{

  OptionalString s = workspaceObject.getString(Output_VariableFields::VariableName);
  if(!s){
    return boost::none;
  }

  openstudio::model::OutputVariable outputVariable( *s,  m_model );

  s = workspaceObject.getString(Output_VariableFields::KeyValue);
  if(s){
    outputVariable.setKeyValue(*s);
  }

  s = workspaceObject.getString(Output_VariableFields::VariableName);
  if(s){
    outputVariable.setVariableName(*s);
  }

  s = workspaceObject.getString(Output_VariableFields::ReportingFrequency);
  if(s){
    outputVariable.setReportingFrequency(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(Output_VariableFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject( *target );
    if (modelObject){
      OptionalSchedule schedule = modelObject->optionalCast<Schedule>();
      if (schedule){
        outputVariable.setSchedule(*schedule);
      }
    }
  }

  return outputVariable;
}

} // energyplus

} // openstudio

