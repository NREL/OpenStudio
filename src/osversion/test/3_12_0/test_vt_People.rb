#require '/usr/local/openstudio-3.11.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

people_def = PeopleDefinition.new(m)
people_def.setMeanRadiantTemperatureCalculationType('SurfaceWeighted')
# Field 11 is the first thermal comfort model type in the v3.11 PeopleDefinition IDD.
people_def.setString(11, 'Fanger')
people = People.new(people_def)
people.setClothingInsulationCalculationMethod('ClothingInsulationSchedule')
points = OpenStudio::Point3dVector.new
points << OpenStudio::Point3d.new(0, 0, 0)
points << OpenStudio::Point3d.new(1, 0, 0)
points << OpenStudio::Point3d.new(1, 1, 0)
surface = Surface.new(points, m)
surface.setName('Radiant Surface')
# The legacy v3.11 API does not expose this setter; field 6 is Surface Name/Angle Factor List Name.
people.setString(6, surface.handle.to_s)

m.save('test_vt_People.osm', true)
