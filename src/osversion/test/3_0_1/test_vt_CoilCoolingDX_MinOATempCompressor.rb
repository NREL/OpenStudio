require '/usr/local/openstudio-3.0.0/Ruby/openstudio'

include OpenStudio::Model

# Generate all tests for #3976
# field 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation'

def test_vt_CoilCoolingDXSingleSpeed()
  m = Model.new
  c = CoilCoolingDXSingleSpeed.new(m)
  # Making sure the field before & after the insertion point have something we
  # can test for
  c.partLoadFractionCorrelationCurve.setName("CC DX SingleSpeed PartLoadFrac Correlation Curve")
  c.setNominalTimeForCondensateRemovalToBegin(1000)
  # And the last field in object too
  c.setBasinHeaterOperatingSchedule(m.alwaysOffDiscreteSchedule)

  m.save('test_vt_CoilCoolingDXSingleSpeed.osm', true)
end

def test_vt_CoilCoolingDXMultiSpeed()
  m = Model.new

  c = CoilCoolingDXMultiSpeed.new(m)

  # Field before
  c.setCondenserType("EvaporativelyCooled")

  # Field after is unused (storage tank)

  # Last field
  c.setFuelType("Electricity")

  m.save('test_vt_CoilCoolingDXMultiSpeed.osm', true)
end

def test_vt_CoilCoolingDXVariableSpeed()
  m = Model.new

  c = CoilCoolingDXVariableSpeed.new(m)

  # Field before
  c.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(11)

  # Field after is unused (storage tank)

  # Last field is a speed data list, no need to change that

  m.save('test_vt_CoilCoolingDXVariableSpeed.osm', true)
end


def test_vt_CoilCoolingDXTwoStageWithHumidityControlMode()
  m = Model.new
  c = CoilCoolingDXTwoStageWithHumidityControlMode.new(m)

  # Field before is unused (storage tank)

  # Field after
  c.setBasinHeaterCapacity(100)
  # Last field
  c.setBasinHeaterSetpointTemperature(3)

  m.save('test_vt_CoilCoolingDXTwoStageWithHumidityControlMode.osm', true)
end

def test_vt_CoilCoolingDXTwoSpeed()
  m = Model.new

  c = CoilCoolingDXTwoSpeed.new(m)

  # First insertion
  # Before
  c.setRatedHighSpeedAirFlowRate(1.2)
  # After, Air Inlet Node
  a = AirLoopHVAC.new(m)
  c.addToNode(a.supplyOutletNode)
  c.inletModelObject.get.setName("Coil Inlet Node Name")

  # Second insertion
  # Before
  c.setCondenserType("EvaporativelyCooled")
  # After
  c.setHighSpeedEvaporativeCondenserEffectiveness(0.5)


  # Last field
  sch = ScheduleConstant.new(m)
  sch.setName("Basin Heater Operating Schedule Name")
  c.setBasinHeaterOperatingSchedule(sch)

  m.save('test_vt_CoilCoolingDXTwoSpeed.osm', true)
end

test_vt_CoilCoolingDXSingleSpeed()
test_vt_CoilCoolingDXTwoStageWithHumidityControlMode()
test_vt_CoilCoolingDXVariableSpeed()
test_vt_CoilCoolingDXMultiSpeed()
test_vt_CoilCoolingDXTwoSpeed()
