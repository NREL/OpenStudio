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
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingWater( CoilHeatingWater & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Coil_Heating_Water);

  m_idfObjects.push_back(idfObject);

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  Schedule sched = modelObject.availableSchedule();
  boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
  if( _sched )
  {
    idfObject.setString(Coil_Heating_WaterFields::AvailabilityScheduleName,
                        _sched->name().get() );
  }

  // UFactorTimesAreaValue

  if( modelObject.isUFactorTimesAreaValueAutosized() )
  {
    idfObject.setString(Coil_Heating_WaterFields::UFactorTimesAreaValue,"Autosize");
  }
  else if( (value = modelObject.uFactorTimesAreaValue()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::UFactorTimesAreaValue,value.get());
  }

  // MaximumWaterFlowRate

  if( modelObject.isMaximumWaterFlowRateAutosized() )
  {
    idfObject.setString(Coil_Heating_WaterFields::MaximumWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumWaterFlowRate()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::MaximumWaterFlowRate,value.get());
  }

  // WaterInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.waterInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterFields::WaterInletNodeName,node->name().get());
    }
  }

  // WaterOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.waterOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterFields::WaterOutletNodeName,node->name().get());
    }
  }

  // AirInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.airInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterFields::AirInletNodeName,node->name().get());
    }
  }

  // AirOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.airOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterFields::AirOutletNodeName,node->name().get());
    }
  }

  // PerformanceInputMethod

  s = modelObject.performanceInputMethod();
  if( s )
  {
    idfObject.setString(Coil_Heating_WaterFields::PerformanceInputMethod,s.get());
  }

  // RatedCapacity

  if( modelObject.isRatedCapacityAutosized() )
  {
    idfObject.setString(Coil_Heating_WaterFields::RatedCapacity,"Autosize");
  }
  else if( (value = modelObject.ratedCapacity()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::RatedCapacity,value.get());
  }

  // RatedInletWaterTemperature

  if( (value = modelObject.ratedInletWaterTemperature()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::RatedInletWaterTemperature,value.get());
  }

  // RatedInletAirTemperature

  if( (value = modelObject.ratedInletAirTemperature()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::RatedInletAirTemperature,value.get());
  }

  // RatedOutletWaterTemperature

  if( (value = modelObject.ratedOutletWaterTemperature()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::RatedOutletWaterTemperature,value.get());
  }

  // RatedOutletAirTemperature

  if( (value = modelObject.ratedOutletAirTemperature()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::RatedOutletAirTemperature,value.get());
  }

  // RatedRatioforAirandWaterConvection

  if( (value = modelObject.ratedRatioForAirAndWaterConvection()) )
  {
    idfObject.setDouble(Coil_Heating_WaterFields::RatedRatioforAirandWaterConvection,value.get());
  }
  
  return boost::optional<IdfObject>(idfObject);
}

//((Name)(Name))
//((AvailabilityScheduleName)(Availability Schedule Name))

} // energyplus

} // openstudio

