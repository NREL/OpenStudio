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
#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/StringHelpers.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Subroutine_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Subroutine_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemSubroutine(EnergyManagementSystemSubroutine & modelObject)
{
  boost::optional<std::string> s;
  const int subs[] = {1};
  std::string possible_uid;
  size_t pos;
  std::string newline;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Subroutine);
  m_idfObjects.push_back(idfObject);
  //m_map.insert(std::make_pair(modelObject.handle(), idfObject));
  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }

  const Model m = modelObject.model();
  boost::optional<ModelObject> mObject;

  for (const IdfExtensibleGroup& eg : modelObject.extensibleGroups()) {
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OptionalString line = eg.getString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine);
    if (line) {
      //find uids
      newline = line.get();
      boost::sregex_token_iterator j(line.get().begin(), line.get().end(), uuidInString(), subs);

      while (j != boost::sregex_token_iterator()) {
        possible_uid = *j++;
        //look to see if uid is in the model and return the object
        UUID uid = toUUID(possible_uid);
        mObject = m.getModelObject<model::ModelObject>(uid);
        if (mObject) {
          //replace uid with namestring
          pos = newline.find(possible_uid);
          if (pos + 38 <= newline.length()) {
            newline.replace(pos, 38, mObject.get().nameString());
          }
        }
      }
      group.setString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, newline);
    }
    OptionalString comment = eg.fieldComment(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine);
    if (comment) {
      group.setFieldComment(EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, comment.get());
    }
  }
  return idfObject;
}

} // energyplus

} // openstudio

