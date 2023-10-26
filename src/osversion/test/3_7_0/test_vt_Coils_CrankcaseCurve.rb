#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model


CRANKCASE_HEATERCAPACITY = 100.0
MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP = 11.0

m = Model.new
basin_sch = ScheduleConstant.new(m)
basin_sch.setName("BasinHeaterOpSch")

# OS:Coil:Cooling:DX:CurveFit:Performance - inserted at 3
opmode = CoilCoolingDXCurveFitOperatingMode.new(m)
coil = CoilCoolingDXCurveFitPerformance.new(m, opmode)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
opmode2 = CoilCoolingDXCurveFitOperatingMode.new(m)
opmode2.setName("AlternativeOpMode2")
coil.setAlternativeOperatingMode2(opmode2)


# OS:Coil:Cooling:DX:SingleSpeed - inserted at 27
coil = CoilCoolingDXSingleSpeed.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.setBasinHeaterOperatingSchedule(basin_sch)


# OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode - inserted at 6
coil = CoilCoolingDXTwoStageWithHumidityControlMode.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.setBasinHeaterOperatingSchedule(basin_sch)


# OS:Coil:Cooling:DX:MultiSpeed - inserted at 13
coil = CoilCoolingDXMultiSpeed.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.setFuelType("Electricity")


# OS:Coil:Heating:DX:SingleSpeed - inserted at 19
coil = CoilHeatingDXSingleSpeed.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.setResistiveDefrostHeaterCapacity(1000.0)


# OS:Coil:Heating:DX:MultiSpeed - inserted at 8
coil = CoilHeatingDXMultiSpeed.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.setRegionnumberforCalculatingHSPF(4)


# OS:Coil:Heating:DX:VariableSpeed - inserted at 13
coil = CoilHeatingDXVariableSpeed.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.getTarget(coil.numFields - 1).get.setName("CoilHeatingDXVariableSpeed Speed Data List")


# OS:Coil:WaterHeating:AirToWaterHeatPump - inserted at 20
coil = CoilWaterHeatingAirToWaterHeatPump.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.partLoadFractionCorrelationCurve().setName("CoilWaterHeatingAirToWaterHeatPump PLFCorrelationCurve")


# OS:Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed - inserted at 18
coil = CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.getTarget(coil.numFields - 1).get.setName("CoilWaterHeatingAirToWaterHeatPumpVariableSpeed Speed Data List")


# OS:Coil:WaterHeating:AirToWaterHeatPump:Wrapped - inserted at 13
coil = CoilWaterHeatingAirToWaterHeatPumpWrapped.new(m)
# Before / after insertion
coil.setCrankcaseHeaterCapacity(CRANKCASE_HEATERCAPACITY)
coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(MAX_OA_OR_AMBIENT_TEMP_FOR_CRANKCASE_OP)
# Last
coil.partLoadFractionCorrelationCurve().setName("CoilWaterHeatingAirToWaterHeatPumpWrapped PLFCorrelationCurve")


m.save("test_vt_Coils_CrankcaseCurve.osm", true)
