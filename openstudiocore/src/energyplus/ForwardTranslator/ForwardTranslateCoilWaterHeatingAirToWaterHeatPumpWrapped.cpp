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
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpWrapped.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_Wrapped_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilWaterHeatingAirToWaterHeatPumpWrapped( 
    CoilWaterHeatingAirToWaterHeatPumpWrapped & modelObject)
{
  IdfObject idfObject(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_Wrapped);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  //double ratedHeatingCapacity() const;
  //double ratedCOP() const;
  //double ratedSensibleHeatRatio() const;
  //double ratedEvaporatorInletAirDryBulbTemperature() const;
  //double ratedEvaporatorInletAirWetBulbTemperature() const;
  //double ratedCondenserWaterTemperature() const;
  //boost::optional<double> ratedEvaporatorAirFlowRate() const;
  //bool isRatedEvaporatorAirFlowRateAutocalculated() const;
  //bool evaporatorFanPowerIncludedinRatedCOP() const;
  //double crankcaseHeaterCapacity() const;
  //double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;
  //std::string evaporatorAirTemperatureTypeforCurveObjects() const;
  //Curve heatingCapacityFunctionofTemperatureCurve() const;
  //Curve heatingCapacityFunctionofAirFlowFractionCurve() const;
  //Curve heatingCOPFunctionofTemperatureCurve() const;
  //Curve heatingCOPFunctionofAirFlowFractionCurve() const;
  //Curve partLoadFractionCorrelationCurve() const;

  //{ Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::Name, "Name", "Name"},
  //{ Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedHeatingCapacity, "RatedHeatingCapacity", "Rated Heating Capacity"},
  {
    auto value = modelObject.ratedHeatingCapacity();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedHeatingCapacity,value);
  }

  {
    auto value = modelObject.ratedCOP();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCOP,value);
  }

  {
    auto value = modelObject.ratedSensibleHeatRatio();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedSensibleHeatRatio,value);
  }

  {
    auto value = modelObject.ratedEvaporatorInletAirDryBulbTemperature();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirDryBulbTemperature,value);
  }

  {
    auto value = modelObject.ratedEvaporatorInletAirWetBulbTemperature();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirWetBulbTemperature,value);
  }

  {
    auto value = modelObject.ratedCondenserWaterTemperature();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCondenserWaterTemperature,value);
  }

  if( modelObject.isRatedEvaporatorAirFlowRateAutocalculated() ) {
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate,"Autocalculate"); 
  } else if( auto value = modelObject.ratedEvaporatorAirFlowRate() ) {
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate,value.get());
  }

  {
    if( modelObject.evaporatorFanPowerIncludedinRatedCOP() ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorFanPowerIncludedinRatedCOP,"Yes");
    } else {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorFanPowerIncludedinRatedCOP,"No");
    }
  }

  {
    auto value = modelObject.crankcaseHeaterCapacity();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::CrankcaseHeaterCapacity,value);
  }

  {
    auto value = modelObject.maximumAmbientTemperatureforCrankcaseHeaterOperation();
    idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,value);
  }

  {
    auto value = modelObject.evaporatorAirTemperatureTypeforCurveObjects();
    idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirTemperatureTypeforCurveObjects,value);
  }

  {
    auto curve = modelObject.heatingCapacityFunctionofTemperatureCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCapacityFunctionofAirFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofAirFlowFractionCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCOPFunctionofTemperatureCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.heatingCOPFunctionofAirFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofAirFlowFractionCurveName,idf->name().get());
    }
  }

  {
    auto curve = modelObject.partLoadFractionCorrelationCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::PartLoadFractionCorrelationCurveName,idf->name().get());
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
