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
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirConditionerVariableRefrigerantFlow.hpp>
#include <model/AirConditionerVariableRefrigerantFlow_Impl.hpp>
#include <model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp>
#include <model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/ZoneTerminalUnitList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idf/IdfExtensibleGroup.hpp>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirConditionerVariableRefrigerantFlow( AirConditionerVariableRefrigerantFlow & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::AirConditioner_VariableRefrigerantFlow);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpName,*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<model::Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // RatedTotalCoolingCapacity

  if( modelObject.isRatedTotalCoolingCapacityAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity,"Autosize");
  }
  else if( (value = modelObject.ratedTotalCoolingCapacity()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity,value.get());
  }

  // RatedCoolingCOP

  if( (value = modelObject.ratedCoolingCOP()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedCoolingCOP,value.get());
  }

  // MinimumOutdoorTemperatureinCoolingMode

  if( (value = modelObject.minimumOutdoorTemperatureinCoolingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode,value.get());
  }

  // MaximumOutdoorTemperatureinCoolingMode

  if( (value = modelObject.maximumOutdoorTemperatureinCoolingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode,value.get());
  }


  // CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.coolingCapacityRatioModifierFunctionofLowTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName,_curve->name().get());
    } 
  }
  
  // CoolingCapacityRatioBoundaryCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.coolingCapacityRatioBoundaryCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName,_curve->name().get());
    }
  }

  // CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.coolingCapacityRatioModifierFunctionofHighTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName,_curve->name().get());
    }
  }
  
  // CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,_curve->name().get());
    }
  }

  // CoolingEnergyInputRatioBoundaryCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.coolingEnergyInputRatioBoundaryCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName,_curve->name().get());
    }
  }

  // CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,_curve->name().get());
    }
  }

  // CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,_curve->name().get());
    }
  }

  // CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName
  
  if( boost::optional<model::CurveCubic> curve = modelObject.coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,_curve->name().get());
    }
  }
  
  // CoolingCombinationRatioCorrectionFactorCurveName
  
  if( boost::optional<model::CurveCubic> curve = modelObject.coolingCombinationRatioCorrectionFactorCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName,_curve->name().get());
    }
  }

  // CoolingPartLoadFractionCorrelationCurveName
  
  if( boost::optional<model::CurveCubic> curve = modelObject.coolingPartLoadFractionCorrelationCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName,_curve->name().get());
    }
  }  

  // RatedTotalHeatingCapacity
  
  if( modelObject.isRatedTotalHeatingCapacityAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity,"Autosize");
  }
  else if( (value = modelObject.ratedTotalHeatingCapacity()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity,value.get());
  }

  // RatedTotalHeatingCapacitySizingRatio

  if( (value = modelObject.ratedTotalHeatingCapacitySizingRatio()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio,value.get());
  }
  
  // RatedHeatingCOP

  if( (value = modelObject.ratedHeatingCOP()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCOP,value.get());
  }
  
  // MinimumOutdoorTemperatureinHeatingMode
  
  if( (value = modelObject.minimumOutdoorTemperatureinHeatingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode,value.get());
  }
  
  // MaximumOutdoorTemperatureinHeatingMode
  
  if( (value = modelObject.maximumOutdoorTemperatureinHeatingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode,value.get());
  }

  // HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatingCapacityRatioModifierFunctionofLowTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName,_curve->name().get());
    }
  }
  
  // HeatingCapacityRatioBoundaryCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.heatingCapacityRatioBoundaryCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName,_curve->name().get());
    }
  }
  
  // HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatingCapacityRatioModifierFunctionofHighTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName,_curve->name().get());
    }
  }

  // HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,_curve->name().get());
    }
  }

  // HeatingEnergyInputRatioBoundaryCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.heatingEnergyInputRatioBoundaryCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName,_curve->name().get());
    }
  }  

  // HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,_curve->name().get());
    }
  }

  // HeatingPerformanceCurveOutdoorTemperatureType

  if( (s = modelObject.heatingPerformanceCurveOutdoorTemperatureType()) )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType,s.get());
  }

  // HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,_curve->name().get());
    }
  }

  // HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName
  
  if( boost::optional<model::CurveCubic> curve = modelObject.heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,_curve->name().get());
    }
  }

  // HeatingCombinationRatioCorrectionFactorCurveName
  
  if( boost::optional<model::CurveCubic> curve = modelObject.heatingCombinationRatioCorrectionFactorCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName,_curve->name().get());
    }
  }

  // HeatingPartLoadFractionCorrelationCurveName

  if( boost::optional<model::CurveCubic> curve = modelObject.heatingPartLoadFractionCorrelationCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName,_curve->name().get());
    }
  }

  // MinimumHeatPumpPartLoadRatio

  if( (value = modelObject.minimumHeatPumpPartLoadRatio()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio,value.get());
  }

  // ZoneNameforMasterThermostatLocation

  if( boost::optional<model::ThermalZone> zone = modelObject.zoneforMasterThermostatLocation() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation,zone->name().get());
  }

  // MasterThermostatPriorityControlType

  if( (s = modelObject.masterThermostatPriorityControlType()) )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType,s.get());
  }

  // ThermostatPriorityScheduleName
  
  if( boost::optional<model::Schedule> schedule = modelObject.thermostatPrioritySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ThermostatPriorityScheduleName,_schedule->name().get());
    }
  }

  // HeatPumpWasteHeatRecovery
  
  if( modelObject.heatPumpWasteHeatRecovery() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery,"Yes");
  }
  else
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery,"No");
  }
  
  // EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode
  
  if( (value = modelObject.equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,value.get());
  }

  // VerticalHeightusedforPipingCorrectionFactor

  if( (value = modelObject.verticalHeightusedforPipingCorrectionFactor()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor,value.get());
  }
  
  // PipingCorrectionFactorforLengthinCoolingModeCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.pipingCorrectionFactorforLengthinCoolingModeCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName,_curve->name().get());
    }
  }

  // PipingCorrectionFactorforHeightinCoolingModeCoefficient
  
  if( (value = modelObject.pipingCorrectionFactorforHeightinCoolingModeCoefficient()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient,value.get()); 
  }

  // EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode
  
  if( (value = modelObject.equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,value.get());
  }
  
  // PipingCorrectionFactorforLengthinHeatingModeCurveName
  
  if( boost::optional<model::CurveCubic> curve = modelObject.pipingCorrectionFactorforLengthinHeatingModeCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName,_curve->name().get());
    }
  }

  // PipingCorrectionFactorforHeightinHeatingModeCoefficient
  
  if( (value = modelObject.pipingCorrectionFactorforHeightinHeatingModeCoefficient()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient,value.get());
  }

  // CrankcaseHeaterPowerperCompressor

  if( (value = modelObject.crankcaseHeaterPowerperCompressor()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor,value.get());
  }
  
  // NumberofCompressors
  
  {
    int number = modelObject.numberofCompressors();

    idfObject.setUnsigned(AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors,(unsigned)number);
  }
  
  // RatioofCompressorSizetoTotalCompressorCapacity
  
  if( (value = modelObject.ratioofCompressorSizetoTotalCompressorCapacity()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity,value.get());
  }
  
  // MaximumOutdoorDrybulbTemperatureforCrankcaseHeater
  
  if( (value = modelObject.maximumOutdoorDrybulbTemperatureforCrankcaseHeater()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater,value.get());
  }

  // DefrostStrategy

  if( (s = modelObject.defrostStrategy()) )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy,s.get());
  }
  
  // DefrostControl
  
  if( (s = modelObject.defrostControl()) )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostControl,s.get());
  }
  
  // DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.defrostEnergyInputRatioModifierFunctionofTemperatureCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // DefrostTimePeriodFraction

  if( (value = modelObject.defrostTimePeriodFraction()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction,value.get());
  }

  // ResistiveDefrostHeaterCapacity

  if( modelObject.isResistiveDefrostHeaterCapacityAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity,"Autosize");
  }
  else if( (value = modelObject.resistiveDefrostHeaterCapacity()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity,value.get());
  }

  // MaximumOutdoorDrybulbTemperatureforDefrostOperation
  
  if( (value = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,value.get());
  }
  
  // CondenserInletNodeName

  if( boost::optional<model::ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<IdfObject> _mo = translateAndMapModelObject(mo.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserInletNodeName,_mo->name().get());
    }
  }
  
  // CondenserOutletNodeName

  if( boost::optional<model::ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<IdfObject> _mo = translateAndMapModelObject(mo.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNodeName,_mo->name().get());
    }
  }
  
  // WaterCondenserVolumeFlowRate
  
  if( modelObject.isWaterCondenserVolumeFlowRateAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate,"Autosize");
  }
  else if( (value = modelObject.waterCondenserVolumeFlowRate()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate,value.get());
  }

  // EvaporativeCondenserEffectiveness

  if( (value = modelObject.evaporativeCondenserEffectiveness()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness,value.get());
  }

  // EvaporativeCondenserAirFlowRate

  if( modelObject.isEvaporativeCondenserAirFlowRateAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.evaporativeCondenserAirFlowRate()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate,value.get());
  }

  // EvaporativeCondenserPumpRatedPowerConsumption
  
  if( modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  }
  else if( ( value = modelObject.evaporativeCondenserPumpRatedPowerConsumption()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption,value.get());
  }

  // BasinHeaterCapacity
  
  if( (value = modelObject.basinHeaterCapacity()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity,value.get());
  }
  
  // BasinHeaterSetpointTemperature

  if( (value = modelObject.basinHeaterSetpointTemperature()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature,value.get());
  }

  // BasinHeaterOperatingScheduleName

  if( boost::optional<model::Schedule> schedule = modelObject.basinHeaterOperatingSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingScheduleName,_schedule->name().get());
    }
  }

  // FuelType

  if( (s = modelObject.fuelType()) )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::FuelType,s.get());
  }

  // MinimumOutdoorTemperatureinHeatRecoveryMode
  
  if( (value = modelObject.minimumOutdoorTemperatureinHeatRecoveryMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode,value.get());
  }

  // MaximumOutdoorTemperatureinHeatRecoveryMode
  
  if( (value = modelObject.maximumOutdoorTemperatureinHeatingMode()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode,value.get());
  }

  // HeatRecoveryCoolingCapacityModifierCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryCoolingCapacityModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName,_curve->name().get());
    }
  }

  // InitialHeatRecoveryCoolingCapacityFraction
  
  if( (value = modelObject.initialHeatRecoveryCoolingEnergyFraction()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction,value.get());
  }

  // HeatRecoveryCoolingCapacityTimeConstant
  
  if( (value = modelObject.heatRecoveryCoolingCapacityTimeConstant()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant,value.get());
  }

  // HeatRecoveryCoolingEnergyModifierCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryCoolingEnergyModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName,_curve->name().get());
    }
  }

  // InitialHeatRecoveryCoolingEnergyFraction

  if( (value = modelObject.initialHeatRecoveryCoolingEnergyFraction()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction,value.get());
  }

  // HeatRecoveryCoolingEnergyTimeConstant

  if( (value = modelObject.heatRecoveryCoolingEnergyTimeConstant()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant,value.get());
  }  

  // HeatRecoveryHeatingCapacityModifierCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryHeatingCapacityModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName,_curve->name().get());
    }
  }

  // InitialHeatRecoveryHeatingCapacityFraction
  
  if( (value = modelObject.initialHeatRecoveryHeatingCapacityFraction()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction,value.get());
  }

  // HeatRecoveryHeatingCapacityTimeConstant

  if( (value = modelObject.heatRecoveryHeatingCapacityTimeConstant()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant,value.get());
  }

  // HeatRecoveryHeatingEnergyModifierCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryHeatingEnergyModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName,_curve->name().get());
    }
  }

  // InitialHeatRecoveryHeatingEnergyFraction

  if( (value = modelObject.initialHeatRecoveryHeatingEnergyFraction()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction,value.get());
  }

  // HeatRecoveryHeatingEnergyTimeConstant

  if( (value = modelObject.heatRecoveryHeatingEnergyTimeConstant()) )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant,value.get());
  }

  // Terminal Unit List
  
  IdfObject _zoneTerminalUnitList(IddObjectType::ZoneTerminalUnitList);

  std::string terminalUnitListName = modelObject.name().get() + " Terminal List";

  _zoneTerminalUnitList.setString(ZoneTerminalUnitListFields::ZoneTerminalUnitListName,terminalUnitListName);

  idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitListName,terminalUnitListName);

  m_idfObjects.push_back(_zoneTerminalUnitList);

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = modelObject.terminals();

  for( std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow>::iterator it = terminals.begin();
       it != terminals.end();
       ++it )
  {
    boost::optional<IdfObject> _terminal = translateAndMapModelObject(*it);

    OS_ASSERT(_terminal);
     
    IdfExtensibleGroup eg = _zoneTerminalUnitList.pushExtensibleGroup();

    eg.setString(ZoneTerminalUnitListExtensibleFields::ZoneTerminalUnitName,_terminal->name().get());
  }

  // CondenserType

  if( modelObject.plantLoop() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserType,"WaterCooled");
  }
  else
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserType,"AirCooled");
  }  

  // CondenserInletNodeName

  OptionalModelObject omo = modelObject.inletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserInletNodeName,*s );
    }
  }

  // CondenserOutletNodeName

  omo = modelObject.outletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNodeName,*s );
    }
  }

  // WaterCondenserVolumeFlowRate

  if( modelObject.isWaterCondenserVolumeFlowRateAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate,"Autosize");
  }
  else if( value = modelObject.waterCondenserVolumeFlowRate() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate,value.get()); 
  }

  // EvaporativeCondenserEffectiveness

  if( value = modelObject.evaporativeCondenserEffectiveness() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness,value.get());
  }

  // EvaporativeCondenserAirFlowRate

  if( modelObject.isEvaporativeCondenserAirFlowRateAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate,"Autosize");
  }
  else if( value = modelObject.evaporativeCondenserAirFlowRate() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate,value.get());
  }

  // EvaporativeCondenserPumpRatedPowerConsumption

  if( modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  }
  else if( value = modelObject.evaporativeCondenserPumpRatedPowerConsumption() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption,value.get());
  }

  // BasinHeaterCapacity

  if( value = modelObject.basinHeaterCapacity() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity,value.get());
  }

  // BasinHeaterSetpointTemperature

  if( value = modelObject.basinHeaterSetpointTemperature() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature,value.get());
  } 

  // BasinHeaterOperatingScheduleName

  if( boost::optional<model::Schedule> schedule = modelObject.basinHeaterOperatingSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingScheduleName,_schedule->name().get());
    }
  }

  // FuelType
  
  if( s = modelObject.fuelType() )
  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::FuelType,s.get());
  }

  // MinimumOutdoorTemperatureinHeatRecoveryMode

  if( value = modelObject.minimumOutdoorTemperatureinHeatRecoveryMode() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode,value.get());
  }

  // MaximumOutdoorTemperatureinHeatRecoveryMode

  if( value = modelObject.maximumOutdoorTemperatureinHeatRecoveryMode() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode,value.get());
  }

  // HeatRecoveryCoolingCapacityModifierCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryCoolingCapacityModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName,_curve->name().get());
    } 
  }

  // InitialHeatRecoveryCoolingCapacityFraction

  if( value = modelObject.initialHeatRecoveryCoolingCapacityFraction() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction,value.get());
  }

  // HeatRecoveryCoolingCapacityTimeConstant

  if( value = modelObject.heatRecoveryCoolingEnergyTimeConstant() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant,value.get());
  }

  // HeatRecoveryCoolingEnergyModifierCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryCoolingEnergyModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName,_curve->name().get());
    } 
  }

  // InitialHeatRecoveryCoolingEnergyFraction
  
  if( value = modelObject.initialHeatRecoveryCoolingEnergyFraction() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction,value.get());
  }

  // HeatRecoveryCoolingEnergyTimeConstant
  
  if( value = modelObject.heatRecoveryCoolingEnergyTimeConstant() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant,value.get());
  }

  // HeatRecoveryHeatingCapacityModifierCurveName

  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryHeatingCapacityModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName,_curve->name().get());
    } 
  }
  
  // InitialHeatRecoveryHeatingCapacityFraction

  if( value = modelObject.initialHeatRecoveryHeatingCapacityFraction() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction,value.get());
  }
  
  // HeatRecoveryHeatingCapacityTimeConstant

  if( value = modelObject.heatRecoveryHeatingCapacityTimeConstant() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant,value.get());
  }
  
  // HeatRecoveryHeatingEnergyModifierCurveName
  
  if( boost::optional<model::CurveBiquadratic> curve = modelObject.heatRecoveryHeatingEnergyModifierCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName,_curve->name().get());
    } 
  }

  // InitialHeatRecoveryHeatingEnergyFraction
  
  if( value = modelObject.initialHeatRecoveryHeatingEnergyFraction() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction,value.get());
  } 

  // HeatRecoveryHeatingEnergyTimeConstant

  if( value = modelObject.heatRecoveryHeatingEnergyTimeConstant() )
  {
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant,value.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

