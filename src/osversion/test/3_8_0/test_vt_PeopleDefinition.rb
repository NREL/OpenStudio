#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

people_def = PeopleDefinition.new(m)
people_def.setMeanRadiantTemperatureCalculationType("ZoneAveraged")

m.save('test_vt_PeopleDefinition.osm', true)
