########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ExteriorLights_Test < Minitest::Test

  def test_ExteriorLights
    model = OpenStudio::Model::Model.new
    exteriorLightsDefinition = OpenStudio::Model::ExteriorLightsDefinition.new(model)
    exteriorLightsDefinition.setDesignLevel(2303.0.to_f)
    # both constructors should work
    exteriorLights = OpenStudio::Model::ExteriorLights.new(exteriorLightsDefinition)
    exteriorLights = OpenStudio::Model::ExteriorLights.new(exteriorLightsDefinition,true)
  end

end
