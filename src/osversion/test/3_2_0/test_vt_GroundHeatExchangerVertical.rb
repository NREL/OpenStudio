include OpenStudio::Model

m = Model.new

ghe = OpenStudio::Model::GroundHeatExchangerVertical.new(m)

# Previously maximumFlowRate was used, designFlowRate was unused
ghe.setMaximumFlowRate(1.15)
ghe.setDesignFlowRate(0.33)

# Field before deleted one
ghe.setGroundTemperature(13.385)
# Field right after
ghe.setGroutThermalConductivity(0.71111)

m.save('test_vt_GroundHeatExchangerVertical.osm', true)
