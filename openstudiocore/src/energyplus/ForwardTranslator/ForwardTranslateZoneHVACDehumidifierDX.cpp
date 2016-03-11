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
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/ZoneHVACDehumidifierDX.hpp"
#include "../../model/ZoneHVACDehumidifierDX_Impl.hpp"
#include <utilities/idd/ZoneHVAC_Dehumidifier_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACDehumidifierDX( ZoneHVACDehumidifierDX & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_Dehumidifier_DX, modelObject);

  // AvailabilityScheduleName
  {
    auto schedule = modelObject.availabilitySchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName
  if( auto node = modelObject.inletNode() ) {
    idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::AirInletNodeName,node->name().get());
  }

  // AirOutletNodeName
  if( auto node = modelObject.outletNode() ) {
    idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::AirOutletNodeName,node->name().get());
  }

  // RatedWaterRemoval
  if( (value = modelObject.ratedWaterRemoval()) ) {
    idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::RatedWaterRemoval,value.get());
  }

  // RatedEnergyFactor
  if( (value = modelObject.ratedEnergyFactor()) ) {
    idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::RatedEnergyFactor,value.get());
  }

  // RatedAirFlowRate
  if( (value = modelObject.ratedAirFlowRate()) ) {
    idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::RatedAirFlowRate,value.get());
  }

  // WaterRemovalCurveName
  { 
    auto curve = modelObject.waterRemovalCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::WaterRemovalCurveName,_curve->name().get());
    }
  }

  // EnergyFactorCurveName
  { 
    auto curve = modelObject.energyFactorCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::EnergyFactorCurveName,_curve->name().get());
    }
  }

  // PartLoadFractionCorrelationCurveName
  { 
    auto curve = modelObject.partLoadFractionCorrelationCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::PartLoadFractionCorrelationCurveName,_curve->name().get());
    }
  }

  // MinimumDry-BulbTemperatureforDehumidifierOperation
  if( (value = modelObject.minimumDryBulbTemperatureforDehumidifierOperation()) ) {
    idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::MinimumDryBulbTemperatureforDehumidifierOperation,value.get());
  }

  // MaximumDry-BulbTemperatureforDehumidifierOperation
  if( (value = modelObject.maximumDryBulbTemperatureforDehumidifierOperation()) ) {
    idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::MaximumDryBulbTemperatureforDehumidifierOperation,value.get());
  }

  // Off-CycleParasiticElectricLoad
  if( (value = modelObject.offCycleParasiticElectricLoad()) ) {
    idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::OffCycleParasiticElectricLoad,value.get());
  }

  // CondensateCollectionWaterStorageTankName

  return idfObject;
}

} // energyplus

} // openstudio

