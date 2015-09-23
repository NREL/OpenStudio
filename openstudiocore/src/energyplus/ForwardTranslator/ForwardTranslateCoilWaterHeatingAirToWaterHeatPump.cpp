/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../../model/CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilWaterHeatingAirToWaterHeatPump( 
    CoilWaterHeatingAirToWaterHeatPump & modelObject)
{
  IdfObject idfObject(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  {
    auto value = modelObject.ratedHeatingCapacity();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedHeatingCapacity,value);
  }

  {
    auto value = modelObject.ratedCOP();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCOP,value);
  }

  {
    auto value = modelObject.ratedSensibleHeatRatio();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedSensibleHeatRatio,value);
  }

  {
    auto value = modelObject.ratedEvaporatorInletAirDryBulbTemperature();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorInletAirDryBulbTemperature,value);
  }

  {
    auto value = modelObject.ratedEvaporatorInletAirWetBulbTemperature();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorInletAirWetBulbTemperature,value);
  }

  {
    auto value = modelObject.ratedCondenserInletWaterTemperature();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserInletWaterTemperature,value);
  }

  if( modelObject.isRatedEvaporatorAirFlowRateAutosized() ) {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate,"Autosize");
  } else if( auto value = modelObject.ratedEvaporatorAirFlowRate() ) {
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate,value.get());
  }

  if( modelObject.isRatedCondenserWaterFlowRateAutosized() ) {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate,"Autosize");
  } else if( auto value = modelObject.ratedCondenserWaterFlowRate() ) {
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate,value.get());
  }

  if( modelObject.evaporatorFanPowerIncludedinRatedCOP() ) {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorFanPowerIncludedinRatedCOP,"Yes"); 
  } else {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorFanPowerIncludedinRatedCOP,"No");
  }

  if( modelObject.condenserPumpPowerIncludedinRatedCOP() ) {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpPowerIncludedinRatedCOP,"Yes");
  } else {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpPowerIncludedinRatedCOP,"No");
  }

  if( modelObject.condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() ) {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP,"Yes");
  } else {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP,"No");
  }

  {
    auto value = modelObject.condenserWaterPumpPower();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterPumpPower,value);
  }

  {
    auto value = modelObject.fractionofCondenserPumpHeattoWater();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::FractionofCondenserPumpHeattoWater,value);
  }

  {
    auto value = modelObject.crankcaseHeaterCapacity();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::CrankcaseHeaterCapacity,value);
  }

  {
    auto value = modelObject.maximumAmbientTemperatureforCrankcaseHeaterOperation();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPumpFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,value);
  }

  {
    auto value = modelObject.evaporatorAirTemperatureTypeforCurveObjects();
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirTemperatureTypeforCurveObjects,value);
  }

  {
    auto curve = modelObject.heatingCapacityFunctionofTemperatureCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCapacityFunctionofAirFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofAirFlowFractionCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCapacityFunctionofWaterFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofWaterFlowFractionCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCOPFunctionofTemperatureCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCOPFunctionofAirFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofAirFlowFractionCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCOPFunctionofWaterFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofWaterFlowFractionCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.partLoadFractionCorrelationCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPumpFields::PartLoadFractionCorrelationCurveName,idf->name().get());
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
