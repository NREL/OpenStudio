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
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/StringHelpers.hpp"

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
  boost::smatch matches;
  //bool result;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Program);
  m_idfObjects.push_back(idfObject);
  m_map.insert(std::make_pair(modelObject.handle(), idfObject));
  //IdfObject program = createAndRegisterIdfObject(openstudio::IddObjectType::EnergyManagementSystem_Program,modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }
 
  std::string a;
  std::string b;
  std::string c;
  std::string aa;
  std::string bb;
  std::string name;
  openstudio::Handle uuids;
  QString temp;
  //int i;
  int found = 0;

  for (const IdfExtensibleGroup& eg : modelObject.extensibleGroups()) {
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OptionalString line = eg.getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine);
    //TODO translate UIDs to UID.name in the program
    if (line) {
      //result = boost::regex_search(line.get(), matches, openstudio::uuidInString());
      //boost::sregex_token_iterator iter(line.get().begin(), line.get().end(), openstudio::uuidInString(),0);

      //split string on whitespaces to isolate possible uids
      std::vector<std::string> results = splitString(line.get(), ' ');
      for (size_t j = 0; j < results.size(); j++) {
        if (results.at(j).size() == 38) {
          //possible uid so set found to 1
          found = 1;
          //remove {} from uid string
          std::string possible_uid = results.at(j).substr(1, results.at(j).size() - 2);
          //look to see if uid is in the model and return the object
          UUID uid = toUUID(possible_uid);
          UUID test = toUUID(results.at(j));
          //std::vector<Handle> handle = &uid;
          Model m = modelObject.model();
          //TODO cant get below to work so try the harder way
          //m.getModelObjects<model::ModelObject>(&uid);
          std::vector<model::ModelObject> modelObjects = m.getModelObjects<model::ModelObject>();
          if (modelObjects.size() > 0) {
            for (size_t k = 0; k < modelObjects.size(); k++) {
              name = modelObjects.at(k).nameString();
              uuids = modelObjects.at(k).handle();
              temp = modelObjects.at(k).handle().toString();
              if (modelObjects.at(k).handle() == uid) {
                found++;
              };
            }
          };
        }
      }

      group.setString(EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, *line);
    }
  }
  return idfObject;
}

} // energyplus

} // openstudio

