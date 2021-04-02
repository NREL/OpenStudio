#require '/usr/local/openstudio-3.1.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

zc = m.getZoneAirMassFlowConservation()
zc.setAdjustZoneMixingForZoneAirMassFlowBalance(true)
zc.setInfiltrationBalancingMethod("AdjustInfiltrationFlow")
zc.setInfiltrationBalancingZones("AllZones")

m.save('test_vt_ZoneAirMassFlowConservation.osm', true)
