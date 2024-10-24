#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

chiller_electric_eir = ChillerElectricEIR.new(m)

chiller_electric_reforumulatedeir = ChillerElectricReformulatedEIR.new(m)

m.save('test_vt_ChillerElectric.osm', true)
