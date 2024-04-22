########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class SkyTemperature_Test < Minitest::Test

  def test_construction_and_parentage

    model = OpenStudio::Model::Model.new

    # no parent when no Site and parent not explicitly set
    skyTemperature = OpenStudio::Model::SkyTemperature.new(model)
    opo = skyTemperature.parent
    assert_equal(true,opo.empty?)

    site = model.getSite
    opo = skyTemperature.parent
    assert_equal(false,opo.empty?)
    skyTempParent = opo.get
    assert(skyTempParent == site)

    # make skyTemperature apply to a RunPeriod
    runPeriod = model.getRunPeriod
    assert(skyTemperature.setParent(runPeriod))
    assert_equal(0,site.children.size)
    assert_equal(1,runPeriod.children.size)

    # make new SkyTemperature that will be immediately parented by site
    globalSkyTemperature = OpenStudio::Model::SkyTemperature.new(model)
    opo = globalSkyTemperature.parent
    assert_equal(false,opo.empty?)
    globalSkyTempParent = opo.get
    assert(globalSkyTempParent == site)
    assert_equal(1,site.children.size)
    assert_equal(1,runPeriod.children.size)

  end

end
