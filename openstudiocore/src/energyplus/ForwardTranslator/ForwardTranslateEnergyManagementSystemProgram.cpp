/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Program_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Program_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemProgram(EnergyManagementSystemProgram & modelObject)
{
  boost::optional<std::string> s;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Program);
  m_idfObjects.push_back(idfObject);
  m_map.insert(std::make_pair(modelObject.handle(), idfObject));
  //IdfObject program = createAndRegisterIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Program,modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }
 
  for (const IdfExtensibleGroup& eg : modelObject.extensibleGroups()) {
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OptionalString line = eg.getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine);
    //TODO translate UIDs to UID.name in the program
    if (line) {
      group.setString(EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, *line);
    }
  }
  return idfObject;
}

} // energyplus

} // openstudio

