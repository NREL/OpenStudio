#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new
z = ThermalZone.new(m)
sz = z.sizingZone

sz.setZoneCoolingDesignSupplyAirTemperatureInputMethod("SupplyAirTemperature")
sz.setZoneCoolingDesignSupplyAirTemperature(14.0)
sz.setZoneCoolingDesignSupplyAirTemperatureDifference(11.11)
sz.setZoneHeatingDesignSupplyAirTemperatureInputMethod("SupplyAirTemperature")
sz.setZoneHeatingDesignSupplyAirTemperature(40.0)
sz.setZoneHeatingDesignSupplyAirTemperatureDifference(11.11)
sz.setZoneCoolingDesignSupplyAirHumidityRatio(0.0085)
sz.setZoneHeatingDesignSupplyAirHumidityRatio(0.008)
sz.setZoneHeatingSizingFactor(1.1)
sz.setCoolingDesignAirFlowMethod("DesignDay")
sz.setCoolingMinimumAirFlowperZoneFloorArea(0.00081)
sz.setCoolingMinimumAirFlowFraction(0.1)
sz.setHeatingDesignAirFlowMethod("DesignDay")
sz.setHeatingMaximumAirFlowperZoneFloorArea(0.0024)
sz.setHeatingMaximumAirFlow(0.145)
sz.setHeatingMaximumAirFlowFraction(0.3)
sz.setAccountforDedicatedOutdoorAirSystem(false)
sz.setDedicatedOutdoorAirSystemControlStrategy("NeutralSupplyAir")
sz.setDedicatedOutdoorAirLowSetpointTemperatureforDesign(18.0)
sz.setDedicatedOutdoorAirHighSetpointTemperatureforDesign(19.0)

#sz.setZoneLoadSizingMethod("Sensible And Latent Load")
#sz.setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod("HumidityRatioDifference")
#sz.setZoneCoolingDesignSupplyAirHumidityRatioDifference(0.0051)
#sz.setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod("SupplyAirHumidityRatio")
#sz.setZoneHumidificationDesignSupplyAirHumidityRatio(0.004)

#dehumSch = ScheduleConstant.new(m)
#dehumSch.setName("dehumSch")
#sz.setZoneHumidistatDehumidificationSetPointSchedule(dehumSch)

#humSch = ScheduleConstant.new(m)
#humSch.setName("humSch")
#sz.setZoneHumidistatHumidificationSetPointSchedule(humSch)

sz.setDesignZoneAirDistributionEffectivenessinCoolingMode(0.8)
sz.setDesignZoneAirDistributionEffectivenessinHeatingMode(0.7)
sz.setDesignZoneSecondaryRecirculationFraction(0.6)
sz.setDesignMinimumZoneVentilationEfficiency(0.5)

m.save('test_vt_SizingZone.osm', true)
