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
#include <model/CoilHeatingDXSingleSpeed.hpp>
#include <model/CoilHeatingDXSingleSpeed_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Heating_DX_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/core/Assert.hpp>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXSingleSpeedWithoutUnitary( CoilHeatingDXSingleSpeed & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Coil_Heating_DX_SingleSpeed);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName

  boost::optional<std::string> airInletNodeName;

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      if( (s = node->name()) )
      {
        airInletNodeName = s;

        idfObject.setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName,s.get());
      }
    }
  }

  // AirOutletNodeName

  boost::optional<std::string> airOutletNodeName;

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      if( (s = node->name()) )
      {
        airOutletNodeName = s;

        idfObject.setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,s.get());
      }
    }
  }

  // RatedTotalHeatingCapacity

  if( modelObject.isRatedTotalHeatingCapacityAutosized() )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCapacity,"Autosize");
  }
  else if( (value = modelObject.ratedTotalHeatingCapacity()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCapacity,value.get());
  }

  // RatedCOP
  
  if( (value = modelObject.ratedCOP()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCOP,value.get());
  }

  // RatedAirFlowRate

  if( modelObject.isRatedAirFlowRateAutosized() )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.ratedAirFlowRate()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate,value.get());
  }

  // TotalHeatingCapacityFunctionofTemperatureCurveName

  ModelObject curve = modelObject.totalHeatingCapacityFunctionofTemperatureCurve();

  if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::HeatingCapacityFunctionofTemperatureCurveName,_curve->name().get());
  }

  // TotalHeatingCapacityFunctionofFlowFractionCurveName

  curve = modelObject.totalHeatingCapacityFunctionofFlowFractionCurve();

  if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::HeatingCapacityFunctionofFlowFractionCurveName,_curve->name().get());
  }

  // EnergyInputRatioFunctionofTemperatureCurveName

  curve = modelObject.energyInputRatioFunctionofTemperatureCurve();

  if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName,_curve->name().get());
  }

  // EnergyInputRatioFunctionofFlowFractionCurveName
  
  curve = modelObject.energyInputRatioFunctionofFlowFractionCurve();

  if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName,_curve->name().get());
  }

  // PartLoadFractionCorrelationCurveName

  curve = modelObject.partLoadFractionCorrelationCurve();

  if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName,_curve->name().get());
  }

  // DefrostEnergyInputRatioFunctionofTemperatureCurveName

  if( boost::optional<Curve> curve = modelObject.defrostEnergyInputRatioFunctionofTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // MinimumOutdoorDryBulbTemperatureforCompressorOperation

  if( (value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforDefrostOperation

  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforDefrostOperation()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,value.get());
  }

  // CrankcaseHeaterCapacity
  
  if( (value = modelObject.crankcaseHeaterCapacity()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
  
  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,value.get());
  }

  // DefrostStrategy

  if( (s = modelObject.defrostStrategy()) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostStrategy,s.get());
  }

  // DefrostControl
  
  if( (s = modelObject.defrostControl()) )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostControl,s.get());
  }

  // DefrostTimePeriodFraction

  if( (value = modelObject.defrostTimePeriodFraction()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction,value.get());
  }

  // ResistiveDefrostHeaterCapacity

  if( modelObject.isResistiveDefrostHeaterCapacityAutosized() )
  {
    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity,"Autosize");
  }
  else if( (value = modelObject.resistiveDefrostHeaterCapacity()) )
  {
    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity,value.get());
  }

  return idfObject;
}

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXSingleSpeed( CoilHeatingDXSingleSpeed & modelObject )
{
  IdfObject _coilSystemHeatingDX(IddObjectType::CoilSystem_Heating_DX);
    
  m_idfObjects.push_back(_coilSystemHeatingDX);

  boost::optional<IdfObject> _coilHeatingDXSingleSpeed = translateCoilHeatingDXSingleSpeedWithoutUnitary(modelObject);

  OS_ASSERT(_coilHeatingDXSingleSpeed);

  OptionalString s;

  s = modelObject.name();

  // Type and Name

  if( s )
  {
    _coilSystemHeatingDX.setString(CoilSystem_Heating_DXFields::HeatingCoilObjectType,_coilHeatingDXSingleSpeed->iddObject().name());

    _coilSystemHeatingDX.setString(CoilSystem_Heating_DXFields::HeatingCoilName,*s);

    _coilSystemHeatingDX.setName(*s + " CoilSystem");
  }

  // Availability Schedule

  Schedule schedule = modelObject.availabilitySchedule();

  boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule);

  if( _schedule )
  {
    _coilSystemHeatingDX.setString(CoilSystem_Heating_DXFields::AvailabilityScheduleName,_schedule->name().get());
  }

  return _coilSystemHeatingDX;
}

} // energyplus

} // openstudio

