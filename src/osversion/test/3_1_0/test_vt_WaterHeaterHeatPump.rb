require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

wh = WaterHeaterHeatPump.new(m)

# Field before insertion point
wh.setMinimumInletAirTemperatureforCompressorOperation(12.5)

# Field after
wh.setCompressorLocation("Outdoors")

# Last Field
wh.setControlSensorLocationInStratifiedTank("Heater2")

m.save('test_vt_WaterHeaterHeatPump.osm', true)

