#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

sch_day = ScheduleDay.new(m)
sch_day.setInterpolatetoTimestep(true)

m.save('test_vt_ScheduleDay.osm', true)
