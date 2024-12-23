#require '/usr/local/openstudio-3.9.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

hpwh = WaterHeaterHeatPump.new(m)
# Field before insertion
hpwh.inletAirMixerSchedule.setName("HPWH Inlet Air Mixer Schedule")
# Field after insertion, also last field
hpwh.setControlSensorLocationInStratifiedTank("Heater2")

m.save('test_vt_WaterHeaterHeatPump.osm', true)
