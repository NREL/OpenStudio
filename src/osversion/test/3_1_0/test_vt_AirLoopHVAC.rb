require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

def test_vt_AirLoopHVAC()
  # Generate test for #4039
  # Inserted a required numeric at position 6

  m = Model.new
  a = AirLoopHVAC.new(m)

  a.autosizeDesignSupplyAirFlowRate
  a.supplyInletNode.setName("Supply Inlet Node")

  m.save('test_vt_AirLoopHVAC.osm', true)
end


test_vt_AirLoopHVAC()
