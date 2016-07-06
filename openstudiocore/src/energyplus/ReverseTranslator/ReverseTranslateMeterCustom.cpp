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

#include "../ReverseTranslator.hpp"

#include "../../model/MeterCustom.hpp"
#include "../../model/MeterCustom_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Meter_Custom_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateMeterCustom( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Meter_Custom ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Meter:Custom");
    return boost::none;
  }

  // Create an OS:Meter:Custom object
  MeterCustom meterCustom = MeterCustom( m_model );

 
  // Name
  boost::optional<std::string> s = workspaceObject.getString(Meter_CustomFields::Name);
  if (s) {
    meterCustom.setName(s.get());
  }
  
  // Fuel Type
  s = workspaceObject.getString(Meter_CustomFields::FuelType);
  if (s) {
    // TODO: JM to DLM: should I also check it's part of the validFuelTypes?
    meterCustom.setFuelType(s.get());
  }
  
  // Get all the (key, var) extensible groups from IDF
  std::vector<IdfExtensibleGroup> keyVarGroups = workspaceObject.extensibleGroups();
  
  // Clean out the (key, var) groups (just in case: the constructor doesn't default any, for now at least!)
  meterCustom.removeAllKeyVarGroups();

  // Push them all to the object
  for( const auto & keyVarGroup : keyVarGroups )
  {
    meterCustom.pushExtensibleGroup(keyVarGroup.fields());
  }

  return meterCustom;
}

} // energyplus

} // openstudio

