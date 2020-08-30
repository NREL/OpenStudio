require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

def test_vt_ConstructionWithInternalSource()
  m = Model.new

  c = ConstructionWithInternalSource.new(m)
  mat = StandardOpaqueMaterial.new(m)
  mat.setName("Material1")
  mat2 = StandardOpaqueMaterial.new(m)
  mat2.setName("Material2")
  mat3 = StandardOpaqueMaterial.new(m)
  mat3.setName("Material3")
  c.setLayers([mat, mat2, mat3])

  c.setSourcePresentAfterLayerNumber(1)
  c.setTemperatureCalculationRequestedAfterLayerNumber(2)

  c.setDimensionsForTheCTFCalculation(1)
  c.setTubeSpacing(0.2)

  r = RenderingColor.new(m)
  r.setName("RenderingColor for InternalSource")
  c.setRenderingColor(r)

  m.save('test_vt_ConstructionWithInternalSource.osm', true)
end

test_vt_ConstructionWithInternalSource()
