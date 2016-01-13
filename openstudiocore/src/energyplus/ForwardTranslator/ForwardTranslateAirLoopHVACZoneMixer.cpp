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
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACZoneMixer( AirLoopHVACZoneMixer & modelObject )
{
  std::string s;
  OptionalString optS;
  OptionalModelObject temp;

  IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ZoneMixer);

  m_idfObjects.push_back(idfObject);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Name ////////////////////////////////////////////////////////////
  s = modelObject.name().get();
  idfObject.setName(s);

  ////////////////////////////////////////////////////////////////////////////
  // Field: Outlet Node Name /////////////////////////////////////////////////
  temp = modelObject.outletModelObject();
  if(temp)
  {
    optS = temp->name();
    if(optS)
      idfObject.setString(openstudio::AirLoopHVAC_ZoneMixerFields::OutletNodeName,*optS);
  }
  ///////////////////////////////////////////////////////////////////////////
  // Field: Inlet 1-N Node Name ///////////////////////////////////////////////
  std::vector<ModelObject> inletModelObjects = modelObject.inletModelObjects();
  int i = 2;
  for( const auto & inletModelObject : inletModelObjects )
  {
    optS = inletModelObject.name();
    if(optS)
    {
      idfObject.setString(i,*optS);
    }
    i++;
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

