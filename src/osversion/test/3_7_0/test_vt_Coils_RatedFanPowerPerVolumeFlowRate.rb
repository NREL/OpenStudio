#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

###############################################################################
#                                DX Two Speed                                 #
###############################################################################

cc2 = CoilCoolingDXTwoSpeed.new(m)

# Before and after insertion point for High speed
cc2.setRatedHighSpeedAirFlowRate(2.0)
cc2.setUnitInternalStaticAirPressure(400.0)

# Before and after insertion point for Low speed
cc2.setRatedLowSpeedAirFlowRate(1.0)
cc2.lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve.setName("LowSpeedTotCapFT")

# Last field
basin_sch = ScheduleConstant.new(m)
basin_sch.setName("BasinHeaterOpSch")
cc2.setBasinHeaterOperatingSchedule(basin_sch)


###############################################################################
#                             DX Var Speed Datas                              #
###############################################################################

ccSp = CoilCoolingDXVariableSpeedSpeedData.new(m)
ccSp.setReferenceUnitRatedAirFlowRate(1.0)
ccSp.setReferenceUnitRatedCondenserAirFlowRate(2.0)
ccSp.energyInputRatioFunctionofAirFlowFractionCurve.setName("ccSp_EIRfFlow")


hcSp = CoilHeatingDXVariableSpeedSpeedData.new(m)
hcSp.setReferenceUnitRatedAirFlowRate(1.0)
hcSp.heatingCapacityFunctionofTemperatureCurve.setName("hcSp_heatCapFT")
hcSp.energyInputRatioFunctionofAirFlowFractionCurve.setName("hcSp_EIRfFlow")


m.save('test_vt_Coils_RatedFanPowerPerVolumeFlowRate.osm', true)
