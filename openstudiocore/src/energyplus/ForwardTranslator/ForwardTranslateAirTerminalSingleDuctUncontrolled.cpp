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
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctUncontrolled( AirTerminalSingleDuctUncontrolled & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<double> value;
  std::string s;

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_Uncontrolled);

  s = modelObject.name().get();
  idfObject.setName(s);

  // hook up required objects
  try {
    idfObject.setString( openstudio::AirTerminal_SingleDuct_UncontrolledFields::AvailabilityScheduleName,
                         modelObject.availabilitySchedule().name().get() );
  }
  catch (std::exception& e) {
    LOG(Error,"Could not translate " << modelObject.briefDescription() << ", because " 
        << e.what() << ".");
    return boost::none;
  }

  temp = modelObject.outletModelObject();
  if(temp)
  {
    optS = temp->name();
    if(optS)
      idfObject.setString(openstudio::AirTerminal_SingleDuct_UncontrolledFields::ZoneSupplyAirNodeName,*optS);
  }

  if( modelObject.isMaximumAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate,"AutoSize");
  }
  else if( (value = modelObject.maximumAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate,value.get());
  }

  m_idfObjects.push_back(idfObject);
  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

