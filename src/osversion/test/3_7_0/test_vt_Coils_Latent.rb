#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

MAX_CYCLING_RATE = 3.5
HEAT_PUMP_TIME_CONSTANT = 90.0
HEAT_PUMP_FAN_DELAY_TIME = 120.0

def make_unitary(m, name)
  u = AirLoopHVACUnitarySystem.new(m)
  u.setName(name)
  # before deleted
  u.setOutdoorDryBulbTemperatureSensorNodeName("sensor")
  # Deleted and moved to child
  u.setMaximumCyclingRate(MAX_CYCLING_RATE)
  u.setFractionofOnCyclePowerUse(0.05) # Just deleted
  u.setHeatPumpTimeConstant(HEAT_PUMP_TIME_CONSTANT)
  u.setHeatPumpFanDelayTime(HEAT_PUMP_FAN_DELAY_TIME)
  # After deletion
  u.setAncilliaryOnCycleElectricPower(2.0)
  # Last
  u.setAncilliaryOffCycleElectricPower(1.0)
  return u
end

def make_cc_eqfit(m, name)
  cc = CoilCoolingWaterToAirHeatPumpEquationFit.new(m)
  cc.setName(name)
  # Before after insertion point
  cc.coolingPowerConsumptionCurve.setName("coolingPowerConsumptionCurve")
  cc.setNominalTimeforCondensateRemovaltoBegin(1.5)
  # Last field (three new fields at end)
  cc.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.02)
  return cc
end

def make_cc_vsdeqfit(m, name)
  cc = CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.new(m)
  cc.setName(name)
  # Before after insertion point
  cc.setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0.02)
  cc.setUseHotGasReheat(true)
  # Last field
  cc.getTarget(cc.numFields - 1).get.setName("#{name} Speed Data List")
  return cc
end

def make_cc_dx_vsd(m, name)
  cc = CoilCoolingDXVariableSpeed.new(m)
  cc.setName(name)
  # Before after insertion point
  cc.setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0.02)
  cc.energyPartLoadFractionCurve.setName("EnergyPartLoadFractionCurve")

  # Before after second insertion point
  cc.setCrankcaseHeaterCapacity(100.0)
  cc.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(11.0)

  # Last field
  cc.getTarget(cc.numFields - 1).get.setName("#{name} Speed Data List")
  return cc
end

def make_hc_eqfit(m, name)
  hc = CoilHeatingWaterToAirHeatPumpEquationFit.new(m)
  hc.setName(name)
  hc.heatingPowerConsumptionCurve.setName("heatingPowerConsumptionCurve")
  return hc
end

def make_hc_vsdeqfit(m, name)
  hc = CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.new(m)
  hc.setName(name)
  # This one has zero changes
  hc.getTarget(hc.numFields - 1).get.setName("#{name} Speed Data List")
  return hc
end

def make_solo_model()
  m = Model.new
  # Solo coils
  cc_eq_solo = make_cc_eqfit(m, "CC Eq Solo")
  cc_vsd_eq_solo = make_cc_vsdeqfit(m, "CC VsdEq Solo")
  cc_dx_vsd_solo = make_cc_dx_vsd(m, "CC DXVsd Solo")
  hc_eq_solo = make_hc_eqfit(m, "HC Eq Solo")
  hc_vsd_eq_solo = make_hc_vsdeqfit(m, "HC VsdEq Solo")
  m.save("test_vt_Coils_Latent_solo.osm", true)
end

def make_unitary_eq_model()
  m = Model.new
  u_cc_eq = make_unitary(m, "Unitary CC Eq")
  cc_eq = make_cc_eqfit(m, "CC Eq Unitary")
  u_cc_eq.setCoolingCoil(cc_eq)

  u_hc_eq = make_unitary(m, "Unitary HC Eq")
  hc_eq = make_hc_eqfit(m, "HC Eq Unitary")
  u_hc_eq.setHeatingCoil(hc_eq)

  u_both_eq = make_unitary(m, "Unitary Both Eq")
  cc_eq = make_cc_eqfit(m, "CC Eq Unitary Both")
  u_both_eq.setCoolingCoil(cc_eq)
  hc_eq = make_hc_eqfit(m, "HC Eq Unitary Both")
  u_both_eq.setHeatingCoil(hc_eq)

  m.save("test_vt_Coils_Latent_unitary_eqfit.osm", true)
end

def make_unitary_vsdeqfit_model()
  m = Model.new
  u_cc_vsd_eq = make_unitary(m, "Unitary CC VsdEq")
  cc_vsd_eq = make_cc_vsdeqfit(m, "CC VsdEq Unitary")
  u_cc_vsd_eq.setCoolingCoil(cc_vsd_eq)

  u_cc_dx_csv = make_unitary(m, "Unitary CC DXVsd")
  cc_dx_vsd = make_cc_dx_vsd(m, "CC DXVsd Unitary")
  u_cc_dx_csv.setCoolingCoil(cc_dx_vsd)

  u_hc_vsd_eq = make_unitary(m, "Unitary HC VsdEq")
  hc_vsd_eq = make_hc_vsdeqfit(m, "HC VsdEq Unitary")
  u_hc_vsd_eq.setHeatingCoil(hc_vsd_eq)

  u_both_eq = make_unitary(m, "Unitary Both VsdEq")
  cc_eq = make_hc_vsdeqfit(m, "CC VsdEq Unitary Both")
  u_both_eq.setCoolingCoil(cc_eq)
  hc_eq = make_hc_vsdeqfit(m, "HC VsdEq Unitary Both")
  u_both_eq.setHeatingCoil(hc_eq)

  m.save("test_vt_Coils_Latent_unitary_vsdeqfit.osm", true)
end

# Make some ZoneHVACWaterToAirHeatPump (wahp)
def make_wahp_eqfit(m, name)
  # add water to air heat pump with variable speed coils to next available zone
  supplyFan = OpenStudio::Model::FanOnOff.new(m)
  supplyFan.setName("#{name} Fan On Off")

  cc = make_cc_eqfit(m, "CC Eq WAHP")
  hc = make_hc_eqfit(m, "HC Eq WAHP")

  supplementalHC = OpenStudio::Model::CoilHeatingElectric.new(m)
  wahp = OpenStudio::Model::ZoneHVACWaterToAirHeatPump.new(m, m.alwaysOnDiscreteSchedule, supplyFan, hc, cc, supplementalHC)
  wahp.setName(name)
  return wahp
end

def make_wahp_vsdeqfit(m, name)
  # add water to air heat pump with variable speed coils to next available zone
  supplyFan = OpenStudio::Model::FanOnOff.new(m)
  supplyFan.setName("#{name} Fan On Off")

  cc = make_cc_vsdeqfit(m, "CC VsdEq WAHP")
  hc = make_hc_vsdeqfit(m, "HC VsdEq WAHP")

  supplementalHC = OpenStudio::Model::CoilHeatingElectric.new(m)
  wahp = OpenStudio::Model::ZoneHVACWaterToAirHeatPump.new(m, m.alwaysOnDiscreteSchedule, supplyFan, hc, cc, supplementalHC)
  wahp.setName(name)
  return wahp
end

def make_wahp_model()
  m = Model.new
  wahp_eq = make_wahp_eqfit(m, "WAHP Eq")
  wahp_vsd_eq = make_wahp_eqfit(m, "WAHP VsdEq")
  m.save("test_vt_Coils_Latent_wahp.osm", true)
end


###############################################################################
#                                   M A I N                                   #
###############################################################################

make_solo_model()
make_unitary_eq_model()
make_unitary_vsdeqfit_model()
make_wahp_model()
