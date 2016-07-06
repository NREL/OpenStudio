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
#include "../../model/MeterCustom.hpp"
#include "../../model/MeterCustom_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/Meter_Custom_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>


using namespace openstudio::model;

// I don't think I'm using this
//using namespace std;

// I don't know if I need this
using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateMeterCustom( MeterCustom & modelObject )
{
  
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Meter_Custom);

  m_idfObjects.push_back(idfObject);

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }
  
  // FuelType
  if( (s = modelObject.fuelType()) )
  {
    idfObject.setString(Meter_CustomFields::FuelType,s.get());
  }
  
  // Handle the (Key Name, Output Variable or Meter Name) pairs
  std::vector< std::pair<std::string, std::string> > keyVarGroups = modelObject.keyVarGroups();

  if( !keyVarGroups.empty() )
  {
    for( const auto & keyVarGroup : keyVarGroups )
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

      eg.setString(Meter_CustomExtensibleFields::KeyName,keyVarGroup.first); 
      eg.setString(Meter_CustomExtensibleFields::OutputVariableorMeterName,keyVarGroup.second); 
    }
  }
  else {
    LOG(Error,modelObject.briefDescription() << ": At least one pair of (Key Name, Output Variable or Meter Name) required");
    return boost::none;
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio