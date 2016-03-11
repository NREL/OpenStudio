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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/CoilWaterHeatingDesuperheater.hpp"
#include "../../model/CoilWaterHeatingDesuperheater_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_WaterHeating_Desuperheater_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilWaterHeatingDesuperheater( CoilWaterHeatingDesuperheater & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_WaterHeating_Desuperheater, modelObject);

// AvailabilityScheduleName
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

// SetpointTemperatureScheduleName
  boost::optional<Schedule> setpointTemperatureSchedule = modelObject.setpointTemperatureSchedule();

  if( setpointTemperatureSchedule )
  {
    boost::optional<IdfObject> _setpointTemperatureSchedule = translateAndMapModelObject(setpointTemperatureSchedule.get());

    if( _setpointTemperatureSchedule && _setpointTemperatureSchedule->name() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::SetpointTemperatureScheduleName,_setpointTemperatureSchedule->name().get());
    }
  }

// DeadBandTemperatureDifference
  d = modelObject.deadBandTemperatureDifference();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::DeadBandTemperatureDifference,d.get());
  }

// RatedHeatReclaimRecoveryEfficiency
  d = modelObject.ratedHeatReclaimRecoveryEfficiency();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::RatedHeatReclaimRecoveryEfficiency,d.get());
  }

// RatedInletWaterTemperature
  d = modelObject.ratedInletWaterTemperature();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::RatedInletWaterTemperature,d.get());
  }

// RatedOutdoorAirTemperature
  d = modelObject.ratedOutdoorAirTemperature();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::RatedOutdoorAirTemperature,d.get());
  }

// MaximumInletWaterTemperatureforHeatReclaim
  d = modelObject.maximumInletWaterTemperatureforHeatReclaim();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::MaximumInletWaterTemperatureforHeatReclaim,d.get());
  }

// HeatReclaimEfficiencyFunctionofTemperatureCurveName
  boost::optional<CurveBiquadratic> heatReclaimCurve = modelObject.heatReclaimEfficiencyFunctionofTemperatureCurve();

  if( heatReclaimCurve )
  {
    boost::optional<IdfObject> _heatReclaimCurve = translateAndMapModelObject(heatReclaimCurve.get());

    if( _heatReclaimCurve && _heatReclaimCurve->name() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::HeatReclaimEfficiencyFunctionofTemperatureCurveName,_heatReclaimCurve->name().get());
    }
  }

// WaterInletNodeName
  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::WaterInletNodeName,node->name().get());
    }
  }

// WaterOutletNodeName
  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::WaterOutletNodeName,node->name().get());
    }
  }

// TankObjectType
// TankName
  if( boost::optional<ModelObject> heatRejectionTarget = modelObject.heatRejectionTarget() )
  {
    boost::optional<IdfObject> _heatRejectionTarget = translateAndMapModelObject(heatRejectionTarget.get());

    if( _heatRejectionTarget && _heatRejectionTarget->name() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::TankObjectType,_heatRejectionTarget->iddObject().name());
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::TankName,_heatRejectionTarget->name().get());
    }
  }

// HeatingSourceObjectType
// HeatingSourceName
  if( boost::optional<ModelObject> heatingSource = modelObject.heatingSource() )
  {
    boost::optional<IdfObject> _heatingSource = translateAndMapModelObject(heatingSource.get());

    if( _heatingSource && _heatingSource->name() )
    {
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceObjectType,_heatingSource->iddObject().name());
      idfObject.setString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceName,_heatingSource->name().get());
    }
  }

// WaterFlowRate
  d = modelObject.waterFlowRate();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::WaterFlowRate,d.get());
  }

// WaterPumpPower
  d = modelObject.waterPumpPower();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::WaterPumpPower,d.get());
  }

// FractionofPumpHeattoWater
  d = modelObject.fractionofPumpHeattoWater();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::FractionofPumpHeattoWater,d.get());
  }

// OnCycleParasiticElectricLoad
  d = modelObject.onCycleParasiticElectricLoad();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::OnCycleParasiticElectricLoad,d.get());
  }

// OffCycleParasiticElectricLoad
  d = modelObject.offCycleParasiticElectricLoad();
  if (d) {
    idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::OffCycleParasiticElectricLoad,d.get());
  }
  
  return idfObject;
}

} // energyplus

} // openstudio

