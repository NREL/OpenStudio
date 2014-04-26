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
  //std::string baseName = modelObject.name().get();

  //  A3 ,Field Availability Schedule Name
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      idfObject.setString(Fan_OnOffFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
    }
  }
  
  //  N1 ,Field Fan Efficiency
  
  value = modelObject.fanEfficiency();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::FanTotalEfficiency, *value);
  }
  
  //  N2 Pressure Rise
  
  value = modelObject.pressureRise();
  
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::PressureRise, *value);
  }
  
  // N3,  Field Maximum Flow Rate

  value = modelObject.maximumFlowRate();
  if( value )
  {
    idfObject.setDouble(Fan_OnOffFields::MaximumFlowRate, value.get());
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
    idfObject.setDouble(Fan_OnOffFields::MotorInAirstreamFraction, *value);
  }
  
  // A4 Air Inlet Node Name

  if( boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>() )
    {
      idfObject.setString(Fan_OnOffFields::AirInletNodeName, inletNode->name().get());
    }
  }

  // A5 , Field Air Outlet Node Name
  
  if( boost::optional<ModelObject> ouletModelObject = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> ouletNode = ouletModelObject->optionalCast<Node>() )
    {
      idfObject.setString(Fan_OnOffFields::AirOutletNodeName, ouletNode->name().get());
    }
  }

  // A6 , Field Fan Power Ratio Function of Speed Ratio Curve Name

  boost::optional<Curve> fanPowerRatioFunctionofSpeedRatioCurve = modelObject.fanPowerRatioFunctionofSpeedRatioCurve();

  if( fanPowerRatioFunctionofSpeedRatioCurve )
  {
    boost::optional<IdfObject> _fanPowerRatioFunctionofSpeedRatioCurve = translateAndMapModelObject(fanPowerRatioFunctionofSpeedRatioCurve.get());

    if( _fanPowerRatioFunctionofSpeedRatioCurve && _fanPowerRatioFunctionofSpeedRatioCurve->name() )
    {
      idfObject.setString(Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName, _fanPowerRatioFunctionofSpeedRatioCurve->name().get());
    }
  }
  
  // A7 , Field Fan Efficiency Ratio Function of Speed Ratio Curve Name

  boost::optional<Curve> fanEfficiencyRatioFunctionofSpeedRatioCurve = modelObject.fanEfficiencyRatioFunctionofSpeedRatioCurve();

  if( fanEfficiencyRatioFunctionofSpeedRatioCurve )
  {
    boost::optional<IdfObject> _fanEfficiencyRatioFunctionofSpeedRatioCurve = translateAndMapModelObject(fanEfficiencyRatioFunctionofSpeedRatioCurve.get());

    if( _fanEfficiencyRatioFunctionofSpeedRatioCurve && _fanEfficiencyRatioFunctionofSpeedRatioCurve->name() )
    {
      idfObject.setString(Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName, _fanEfficiencyRatioFunctionofSpeedRatioCurve->name().get());
    }
  }
  
  // A8 , Field End-Use Subcategory

   idfObject.setString(Fan_OnOffFields::EndUseSubcategory, modelObject.endUseSubcategory());

  return idfObject;
}

} // energyplus

} // openstudio

