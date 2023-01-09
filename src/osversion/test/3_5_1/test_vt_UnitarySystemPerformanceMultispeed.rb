#require '/usr/local/openstudio-3.5.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

perf = UnitarySystemPerformanceMultispeed.new(m)
perf.setSingleModeOperation(false)
perf.addSupplyAirflowRatioField(1.0, 2.0)
perf.addSupplyAirflowRatioField(SupplyAirflowRatioField::fromCoolingRatio(3.0))

m.save('test_vt_UnitarySystemPerformanceMultispeed.osm', true)
