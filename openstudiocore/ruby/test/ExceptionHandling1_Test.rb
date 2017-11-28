
require 'openstudio'

require 'minitest/autorun'

class Exceptions1_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_exceptions1

    model = OpenStudio::Model::Model.new

    sw_point = OpenStudio::Point3d.new(-21.6252552,-36.576,0.0)
    nw_point = OpenStudio::Point3d.new(-21.6252552,6.67511813964844,0.0)
    ne_point = OpenStudio::Point3d.new(51.5267448,6.67511813964844,0.0)
    se_point = OpenStudio::Point3d.new(51.5267448,-36.576,0.0)

    core_polygon = OpenStudio::Point3dVector.new
    core_polygon << sw_point
    core_polygon << nw_point
    core_polygon << ne_point
    core_polygon << se_point

    core_space = OpenStudio::Model::Space::fromFloorPrint(core_polygon, 3.6576, model)

  end
end


