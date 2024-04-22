########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Space_Test < Minitest::Test

  def test_Space
    workspace = OpenStudio::Workspace.new
    model = OpenStudio::Model::Model.new

    # add a space
    space = OpenStudio::Model::Space.new(model)

    # test name attributes
    space.setName("Space")
    assert(space.name)
    assert_equal("Space", space.name.get)
  end

end
