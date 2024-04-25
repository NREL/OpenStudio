########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'json'
require 'minitest/autorun'

class EpJSON_Test < Minitest::Test

  def setup

    @idfFile = OpenStudio::IdfFile.new("EnergyPlus".to_IddFileType)
    building = OpenStudio::IdfObject.new("Building".to_IddObjectType)
    building.setName("Building 1")
    building.setDouble(1, 0.0) # North Axis
    @idfFile.addObject(building)

    @ep_version = Gem::Version.new(OpenStudio::energyPlusVersion)
  end

  # def teardown
  # end


  def common_asserts(json, ep_version)
    assert(json[:Version])
    assert(Gem::Version.new(json[:Version][:"Version 1"][:version_identifier]) == ep_version)
    assert(json[:Building])
    assert(json[:Building][:"Building 1"][:north_axis] == 0.0)
  end

  def test_epJSON_String

    json_str = OpenStudio::EPJSON::toJSONString(@idfFile);
    json = JSON.parse(json_str, symbolize_names: true)

    common_asserts(json, @ep_version)
  end

  def test_epJSON

    # This should directly convert it to a a hash
    json = OpenStudio::EPJSON::toJSON(@idfFile);
    common_asserts(json, @ep_version)
  end

end


