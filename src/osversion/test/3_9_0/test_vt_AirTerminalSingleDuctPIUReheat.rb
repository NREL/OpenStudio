#require '/usr/local/openstudio-3.8.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

schedule = m.alwaysOnDiscreteSchedule

fan = OpenStudio::Model::FanConstantVolume.new(m, schedule)
coil = OpenStudio::Model::CoilHeatingElectric.new(m, schedule)
terminal = OpenStudio::Model::AirTerminalSingleDuctSeriesPIUReheat.new(m, fan, coil)

fan = OpenStudio::Model::FanConstantVolume.new(m, schedule)
coil = OpenStudio::Model::CoilHeatingElectric.new(m, schedule)
terminal = OpenStudio::Model::AirTerminalSingleDuctParallelPIUReheat.new(m, schedule, fan, coil)

m.save('test_vt_AirTerminalSingleDuctPIUReheat.osm', true)
