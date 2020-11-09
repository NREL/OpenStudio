require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

def lowtemprad_constflow(model)

  coolingHighWaterTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  coolingLowWaterTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  coolingHighControlTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  coolingLowControlTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  heatingHighWaterTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  heatingLowWaterTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  heatingHighControlTempSched  = OpenStudio::Model::ScheduleConstant.new(model)
  heatingLowControlTempSched  = OpenStudio::Model::ScheduleConstant.new(model)

  coolingHighWaterTempSched.setValue(15.0)
  coolingLowWaterTempSched.setValue(10.0)
  coolingHighControlTempSched.setValue(25.0)
  coolingLowControlTempSched.setValue(21.0)
  heatingHighWaterTempSched.setValue(50.0)
  heatingLowWaterTempSched.setValue(30.0)
  heatingHighControlTempSched.setValue(20.0)
  heatingLowControlTempSched.setValue(17.0)

  heat_coil = OpenStudio::Model::CoilHeatingLowTempRadiantConstFlow.new(model,heatingHighWaterTempSched,heatingLowWaterTempSched,heatingHighControlTempSched,heatingLowControlTempSched)
  cool_coil = OpenStudio::Model::CoilCoolingLowTempRadiantConstFlow.new(model,coolingHighWaterTempSched,coolingLowWaterTempSched,coolingHighControlTempSched,coolingLowControlTempSched)

  lowtempradiant = OpenStudio::Model::ZoneHVACLowTempRadiantConstFlow.new(model,model.alwaysOnDiscreteSchedule,heat_coil,cool_coil,200.0)
  lowtempradiant.setName("ZoneHVACLowTempRadiantConstFlow")

  lowtempradiant.setRadiantSurfaceType("Floors")
  lowtempradiant.setHydronicTubingInsideDiameter(0.154)
  lowtempradiant.setHydronicTubingLength(200)
  lowtempradiant.setTemperatureControlType("MeanRadiantTemperature")
  pumpflowratesch = OpenStudio::Model::ScheduleConstant.new(model)
  pumpflowratesch.setValue(1.0)
  lowtempradiant.setPumpFlowRateSchedule(pumpflowratesch)
  lowtempradiant.setRatedFlowRate(0.005)
  lowtempradiant.setMotorEfficiency(0.9)
  lowtempradiant.setRatedPowerConsumption(1200)
  lowtempradiant.setRatedPumpHead(30000)
  lowtempradiant.setFractionofMotorInefficienciestoFluidStream(0.7)
  lowtempradiant.setNumberofCircuits("CalculateFromCircuitLength")
  lowtempradiant.setCircuitLength(120)

  return lowtempradiant
end

def lowtemprad_varflow(model)
  heatingControlTemperatureSched  = OpenStudio::Model::ScheduleConstant.new(model)
  coolingControlTemperatureSched  = OpenStudio::Model::ScheduleConstant.new(model)

  heatingControlTemperatureSched.setValue(10.0)
  coolingControlTemperatureSched.setValue(15.0)


  heat_coil = OpenStudio::Model::CoilHeatingLowTempRadiantVarFlow.new(model,heatingControlTemperatureSched)
  cool_coil = OpenStudio::Model::CoilCoolingLowTempRadiantVarFlow.new(model,coolingControlTemperatureSched)

  lowtempradiant = OpenStudio::Model::ZoneHVACLowTempRadiantVarFlow.new(model,model.alwaysOnDiscreteSchedule,heat_coil,cool_coil)

  lowtempradiant.setRadiantSurfaceType("Floors")
  lowtempradiant.setHydronicTubingInsideDiameter(0.154)
  lowtempradiant.setHydronicTubingLength(200)
  lowtempradiant.setTemperatureControlType("MeanRadiantTemperature")
  lowtempradiant.setNumberofCircuits("CalculateFromCircuitLength")
  lowtempradiant.setCircuitLength(120)

  return lowtempradiant
end

def lowtemprad_electric(model)

  heatingTemperatureSched  = OpenStudio::Model::ScheduleConstant.new(model)
  heatingTemperatureSched.setValue(10.0)

  lowtempradiant = OpenStudio::Model::ZoneHVACLowTemperatureRadiantElectric.new(model,model.alwaysOnDiscreteSchedule,heatingTemperatureSched)
  lowtempradiant.setRadiantSurfaceType("Floors")
  lowtempradiant.setMaximumElectricalPowertoPanel(1000)
  lowtempradiant.setTemperatureControlType("MeanRadiantTemperature")

  return lowtempradiant
end


m = Model.new

electric = lowtemprad_electric(m)
constflow = lowtemprad_constflow(m)
varflow = lowtemprad_varflow(m)

m.save('test_vt_ZoneHVACLowTemp.osm', true)
