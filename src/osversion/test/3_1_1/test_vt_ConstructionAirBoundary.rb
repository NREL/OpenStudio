require '/usr/local/openstudio-3.1.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

c = ConstructionAirBoundary.new(m)
c.setName("My Construction Air Boundary")
c.setSolarAndDaylightingMethod("InteriorWindow")
c.setRadiantExchangeMethod("IRTSurface")
c.setAirExchangeMethod("SimpleMixing")
c.setSimpleMixingAirChangesPerHour(0.3)

m.save('test_vt_ConstructionAirBoundary.osm', true)
