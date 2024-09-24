#require '/usr/local/openstudio-3.8.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

sch_day = m.getOutputControlFiles
sch_day.setOutputZoneSizing(false)

m.save('test_vt_OutputControlFiles.osm', true)
