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

#include "../../model/MeterCustomDecrement.hpp"
#include "../../model/MeterCustomDecrement_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Meter_CustomDecrement_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateMeterCustomDecrement( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Meter_CustomDecrement ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Meter:CustomDecrement");
    return boost::none;
  }

  boost::optional<MeterCustomDecrement> optMeterCustomDecrement;
  boost::optional<std::string> s;

  // Source Meter Name: get it and pass it to the constructor
  s = workspaceObject.getString(Meter_CustomDecrementFields::SourceMeterName);
  if (s) {
    // Create an OS:Meter:CustomDecrement object
    optMeterCustomDecrement = MeterCustomDecrement(m_model, s.get());
  } else {
    LOG(Error, workspaceObject.briefDescription() << " does not have a Source Meter Name which is required. It will not be translated!");
  }

  MeterCustomDecrement meterCustomDecrement = optMeterCustomDecrement.get();

  
  // Name
  s = workspaceObject.getString(Meter_CustomDecrementFields::Name);
  if (s) {
    meterCustomDecrement.setName(s.get());
  }
  
  // Fuel Type
  s = workspaceObject.getString(Meter_CustomDecrementFields::FuelType);
  if (s) {
    // TODO: JM to DLM: should I also check it's part of the validFuelTypes?
    meterCustomDecrement.setFuelType(s.get());
  }
  
  
  // Get all the (key, var) extensible groups from IDF
  std::vector<IdfExtensibleGroup> keyVarGroups = workspaceObject.extensibleGroups();
  
  // Clean out the (key, var) groups (just in case: the constructor doesn't default any, for now at least!)
  meterCustomDecrement.removeAllKeyVarGroups();

  // Push them all to the object
  for( const auto & keyVarGroup : keyVarGroups )
  {
    meterCustomDecrement.pushExtensibleGroup(keyVarGroup.fields());
  }

  return meterCustomDecrement;
}

} // energyplus

} // openstudio

