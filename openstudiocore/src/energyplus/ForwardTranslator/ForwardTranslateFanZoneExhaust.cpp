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

#include <energyplus/ForwardTranslator.hpp>
#include <model/FanZoneExhaust.hpp>
#include <model/FanZoneExhaust_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/Fan_ZoneExhaust_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFanZoneExhaust( FanZoneExhaust & modelObject )
{
  //setup boost optionals to use to store get method returns
  boost::optional<std::string> s;
  boost::optional<double> value;
  
  // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
  // Also sets the idfObjects name

  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Fan_ZoneExhaust,modelObject);
  
  // Model Name (A2)
  std::string baseName = modelObject.name().get();

  // A3, Field Availability Schedule Name
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      idfObject.setString(Fan_ZoneExhaustFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
    }
  }
  
  // N1, Field Fan Efficiency
  
  value = modelObject.fanEfficiency();
  
  if( value )
  {
    idfObject.setDouble(Fan_ZoneExhaustFields::FanTotalEfficiency, *value);
  }
  
  // N2, Field Pressure Rise
  
  value = modelObject.pressureRise();
  
  if( value )
  {
    idfObject.setDouble(Fan_ZoneExhaustFields::PressureRise, *value);
  }
  
  // N3, Field Maximum Flow Rate

  value = modelObject.maximumFlowRate();
  if( value )
  {
    idfObject.setDouble(Fan_ZoneExhaustFields::MaximumFlowRate, value.get());
  }

  // A4, Field Air Inlet Node Name
  boost::optional<Node> inletNode = modelObject.inletNode();
  
  if( inletNode )
  {
    boost::optional<Node> _inletNode = modelObject.inletNode();
    
    if( _inletNode && _inletNode->name() )
    {
      idfObject.setString(Fan_ZoneExhaustFields::AirInletNodeName, _inletNode->name().get());
    }
  }

  // A5, Field Air Outlet Node Name
  boost::optional<Node> outletNode = modelObject.outletNode();
  
  if( outletNode )
  {
    boost::optional<Node> _outletNode = modelObject.outletNode();
    
    if( _outletNode && _outletNode->name() )
    {
      idfObject.setString(Fan_ZoneExhaustFields::AirOutletNodeName, _outletNode->name().get());
    }
  }
  
  // A6 , Field End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if( s )
  {
    idfObject.setString(Fan_ZoneExhaustFields::EndUseSubcategory, *s);
  }

  // A7, Field Flow Fraction Schedule Name
  boost::optional<Schedule> flowFractionSchedule = modelObject.flowFractionSchedule();

  if( flowFractionSchedule )
  {
    boost::optional<IdfObject> _flowFractionSchedule = translateAndMapModelObject(flowFractionSchedule.get());

    if( _flowFractionSchedule && _flowFractionSchedule->name() )
    {
      idfObject.setString(Fan_ZoneExhaustFields::FlowFractionScheduleName, _flowFractionSchedule->name().get());
    }
  }
  
  // A8 , Field System Availability Manager Coupling Mode
  s = modelObject.systemAvailabilityManagerCouplingMode();
  
  if( s )
  {
    idfObject.setString(Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode, *s);
  }  
  
  // A9, Field Minimum Zone Temperature Limit Schedule Name
  boost::optional<Schedule> minimumZoneTemperatureLimitSchedule = modelObject.minimumZoneTemperatureLimitSchedule();

  if( minimumZoneTemperatureLimitSchedule )
  {
    boost::optional<IdfObject> _minimumZoneTemperatureLimitSchedule = translateAndMapModelObject(minimumZoneTemperatureLimitSchedule.get());

    if( _minimumZoneTemperatureLimitSchedule && _minimumZoneTemperatureLimitSchedule->name() )
    {
      idfObject.setString(Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName, _minimumZoneTemperatureLimitSchedule->name().get());
    }
  }  
  
  // A10, Field Balanced Exhaust Fraction Schedule Name
  boost::optional<Schedule> balancedExhaustFractionSchedule = modelObject.balancedExhaustFractionSchedule();

  if( balancedExhaustFractionSchedule )
  {
    boost::optional<IdfObject> _balancedExhaustFractionSchedule = translateAndMapModelObject(balancedExhaustFractionSchedule.get());

    if( _balancedExhaustFractionSchedule && _balancedExhaustFractionSchedule->name() )
    {
      idfObject.setString(Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName, _balancedExhaustFractionSchedule->name().get());
    }
  }  

  return idfObject;
}

} // energyplus

} // openstudio

