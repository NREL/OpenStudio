#require '/usr/local/openstudio-3.7.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

hx = HeatExchangerAirToAirSensibleAndLatent.new(m)
hx.setName("ERV")

hx.autosizeNominalSupplyAirFlowRate

hx.setSensibleEffectivenessat100HeatingAirFlow(0.76)
hx.setLatentEffectivenessat100HeatingAirFlow(0.68)

hx.setSensibleEffectivenessat75HeatingAirFlow(0.81)
hx.setLatentEffectivenessat75HeatingAirFlow(0.73)

hx.setSensibleEffectivenessat100CoolingAirFlow(0.74)
hx.setLatentEffectivenessat100CoolingAirFlow(0.67)

hx.setSensibleEffectivenessat75CoolingAirFlow(0.80)
hx.setLatentEffectivenessat75CoolingAirFlow(0.72)

# Field right after is the supply air inlet node
controller = ControllerOutdoorAir.new(m)
oaSystem = AirLoopHVACOutdoorAirSystem.new(m, controller)
hx.addToNode(oaSystem.outboardOANode.get)

# Last field
hx.setEconomizerLockout(false)

m.save('test_vt_HeatExchangerAirToAirSensibleAndLatent.osm', true)
