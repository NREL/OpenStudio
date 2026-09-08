#require '/usr/local/openstudio-3.11.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

people_def = PeopleDefinition.new(m)
people_def.setMeanRadiantTemperatureCalculationType('SurfaceWeighted')
people = People.new(people_def)
people.setClothingInsulationCalculationMethod('ClothingInsulationSchedule')

m.save('test_vt_People.osm', true)
