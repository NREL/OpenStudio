########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Site_Test < Minitest::Test

  def test_construct_all_children

    model = OpenStudio::Model::Model.new

    site = model.getSite
    numSiteChildren = 0
    assert_equal(numSiteChildren,site.children.size)

    climateZones = model.getClimateZones
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

    designDay = OpenStudio::Model::DesignDay.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

    skyTemperature = OpenStudio::Model::SkyTemperature.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

    weatherFile = model.getWeatherFile
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

    weatherFileConditionType = OpenStudio::Model::WeatherFileConditionType.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

    weatherFileDays = OpenStudio::Model::WeatherFileDays.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

  end

  def test_get_optional_site

    model = OpenStudio::Model::Model.new
    site = model.getOptionalSite
    assert(site.empty?)

  end

end
