require '/usr/local/openstudio-3.0.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new
c = CoilCoolingDXSingleSpeed.new(m)
# Making sure the field before & after the insertion point have something we
# can test for
c.partLoadFractionCorrelationCurve.setName("CC DX SingleSpeed PartLoadFrac Correlation Curve")
c.setNominalTimeForCondensateRemovalToBegin(1000)
# And the last field in object too
c.setBasinHeaterOperatingSchedule(m.alwaysOffDiscreteSchedule)

m.save('test_vt_CoilCoolingDXSingleSpeed.osm', true)
