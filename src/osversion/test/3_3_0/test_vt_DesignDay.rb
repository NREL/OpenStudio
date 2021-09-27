#require '/usr/local/openstudio-3.2.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

dd = OpenStudio::Model::DesignDay.new(m)

m.save('test_vt_DesignDay.osm', true)
