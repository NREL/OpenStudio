########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class IddEnums_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_IddFileType

    epIdd = OpenStudio::IddFileType.new("EnergyPlus")
    osIdd = "OpenStudio".to_IddFileType
    assert( epIdd != osIdd )

    assert( osIdd.valueName == "OpenStudio" )
    assert( osIdd.valueDescription == "OpenStudio" )

  end

  def test_IddObjectType

    # The following syntax does not translate from C++
    # iddType = OpenStudio::IddObjectType.new(Openstudio::IddObjectType::Catchall)
    iddType = OpenStudio::IddObjectType.new("Catchall")
    iddType = OpenStudio::IddObjectType.new("UserCustom")
    iddType = "CommentOnly".to_IddObjectType

    iddType = "zone".to_IddObjectType
    assert( iddType.valueDescription() == "Zone" )
    iddType = "zoNe".to_IddObjectType
    assert( iddType.valueDescription() == "Zone" )

    iddType = "simulation data".to_IddObjectType
    assert( iddType.valueName() == "SimulationData" )
    assert( iddType.valueDescription() == "Simulation Data" )

  end

end

