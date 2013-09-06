/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/DistrictCooling.hpp>
#include <model/DistrictCooling_Impl.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/DistrictCooling_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateDistrictCooling( DistrictCooling& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;
  boost::optional<double> value;
  
  IdfObject idfObject(IddObjectType::DistrictCooling);

  m_idfObjects.push_back(idfObject);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Name ////////////////////////////////////////////////////////////
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Inlet Node Name ////////////////////////////////////////////////////
  temp = modelObject.inletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(openstudio::DistrictCoolingFields::ChilledWaterInletNodeName,*s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Outlet Node Name ///////////////////////////////////////////////////
  temp = modelObject.outletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(openstudio::DistrictCoolingFields::ChilledWaterOutletNodeName,*s);
    }
  }
  ///
  ////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  //Nominal Capacity ///////////////////////////////////////////////////
  if( (value = modelObject.nominalCapacity()) )
  {
   idfObject.setDouble(DistrictCoolingFields::NominalCapacity,value.get());
  }
  //
  ////////////////////////////////////////////////////////////////////////
  
  
  
  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

