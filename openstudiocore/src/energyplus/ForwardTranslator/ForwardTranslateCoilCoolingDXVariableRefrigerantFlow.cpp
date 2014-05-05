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
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/CoilCoolingDXVariableRefrigerantFlow.hpp>
#include <model/CoilCoolingDXVariableRefrigerantFlow_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXVariableRefrigerantFlow( CoilCoolingDXVariableRefrigerantFlow & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<model::Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // RatedTotalCoolingCapacity

  if( modelObject.isRatedTotalCoolingCapacityAutosized() )
  {
    idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity,"Autosize");
  }
  else if( (value = modelObject.ratedTotalCoolingCapacity()) )
  {
    idfObject.setDouble(Coil_Cooling_DX_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity,value.get());
  }

  // RatedSensibleHeatRatio

  if( modelObject.isRatedSensibleHeatRatioAutosized() )
  {
    idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::GrossRatedSensibleHeatRatio,"Autosize");
  }
  else if( (value = modelObject.ratedSensibleHeatRatio()) )
  {
    idfObject.setDouble(Coil_Cooling_DX_VariableRefrigerantFlowFields::GrossRatedSensibleHeatRatio,value.get());
  }

  // RatedAirFlowRate

  if( modelObject.isRatedAirFlowRateAutosized() )
  {
    idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::RatedAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.ratedAirFlowRate()) )
  {
    idfObject.setDouble(Coil_Cooling_DX_VariableRefrigerantFlowFields::RatedAirFlowRate,value.get());
  }

  // CoolingCapacityRatioModifierFunctionofTemperatureCurveName

  if( boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioModifierFunctionofTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // CoolingCapacityModifierCurveFunctionofFlowFractionName

  if( boost::optional<model::Curve> curve = modelObject.coolingCapacityModifierCurveFunctionofFlowFraction() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoolingCapacityModifierCurveFunctionofFlowFractionName,_curve->name().get());
    }
  }

  // TODO CoilAirInletNode
  // TODO CoilAirOutletNode

  return idfObject;
}

} // energyplus

} // openstudio

