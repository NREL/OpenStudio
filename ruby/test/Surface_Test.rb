########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Surface_Test < Minitest::Test

  def test_Surfaces
    model = OpenStudio::Model::exampleModel

    surfaces = model.getSurfaces
    planarSurfaces = model.getPlanarSurfaces

    assert(surfaces.size > 0)
    assert(planarSurfaces.size > surfaces.size)

    assert(surfaces[0].is_a? OpenStudio::Model::Surface)
    assert(surfaces[0].is_a? OpenStudio::Model::PlanarSurface)
    assert(planarSurfaces[0].is_a? OpenStudio::Model::PlanarSurface)
  end

end
