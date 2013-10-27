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
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFanOnOff( FanOnOff & modelObject )
{
  //setup boost optionals to use to store get method returns
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<Node> node;
  OptionalModelObject mo;
  
  // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
  // Also sets the idfObjects name

  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Fan_OnOff,modelObject);
  
  // Model Name
  std::string baseName = modelObject.name().get();

  //  A3 ,Field Availability Schedule Name
  
  Schedule availabilitySchedule = modelObject.availabilitySchedule();
  translateAndMapModelObject(availabilitySchedule);
  
  s = availabilitySchedule.name();
  
  if(s)
  {
    idfObject.setString(Fan_OnOffFields::AvailabilityScheduleName,*s);
  }
  
  //  N1 ,Field Fan Efficiency
  
  value = modelObject.fanEfficiency();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::FanEfficiency,*value);
  }
  
  //  N2 Pressure Rise
  
  value = modelObject.pressureRise();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::PressureRise,*value);
  }
  
  // N3,  Field Maximum Flow Rate
  
  value = modelObject.maximumFlowRate();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::MaximumFlowRate,*value);
  }
  
  else
  {
    idfObject.setString(Fan_OnOffFields::MaximumFlowRate,"Autosize");
  }
  
  // N4,  Field Motor Efficiency
  
  value = modelObject.motorEfficiency();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::MotorEfficiency,*value);
  }
  
  //   N5, Field Motor In Airstream Fraction

  value = modelObject.motorInAirstreamFraction();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::MotorInAirstreamFraction,*value);
  }
  
  // A4 Air Inlet Node Name
  
  mo = modelObject.inletModelObject();
  if(mo)
  {
    s = mo->name();
    if(s)
    {
      idfObject.setString(openstudio::Fan_OnOffFields::AirInletNodeName,*s);
    }
  }

  // A5 , Field Air Outlet Node Name
  
  mo = modelObject.outletModelObject();
  if(mo)
  {
    s = mo->name();
    if(s)
    {
      idfObject.setString(openstudio::Fan_OnOffFields::AirOutletNodeName,*s);
    }
  }

  // A6 , Field Fan Power Ratio Function of Speed Ratio Curve Name
  
  Curve cb1 =  modelObject.fanPowerRatioFunctionofSpeedRatioCurve();
  
  translateAndMapModelObject(cb1);
  
  idfObject.setString(Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName,cb1.name().get());
  
  // A7 , Field Fan Efficiency Ratio Function of Speed Ratio Curve Name
  
  Curve cb2 =  modelObject.fanEfficiencyRatioFunctionofSpeedRatioCurve();
  
  translateAndMapModelObject(cb2);
  
  idfObject.setString(Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName,cb2.name().get());
  
  
  // A8 , Field End-Use Subcategory

   idfObject.setString(Fan_OnOffFields::EndUseSubcategory,modelObject.endUseSubcategory());

  return idfObject;
}

} // energyplus

} // openstudio

