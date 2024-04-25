########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Workspace_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_Workspace

    # load IdfFile
    idfPath = OpenStudio::Path.new(File.join(File.dirname(__FILE__), "../../resources/energyplus/5ZoneAirCooled/in.idf"))
    puts idfPath
    assert(OpenStudio::exists(idfPath))

    oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType)
    assert(oIdfFile.empty? == false)
    idfFile = oIdfFile.get
    assert(idfFile.objects().size() > 0)

    # Construct workspace
    workspace = OpenStudio::Workspace.new(idfFile)

    # get zones
    zones = workspace.getObjectsByType("Zone".to_IddObjectType)
    assert_equal(6, zones.size()) # go figure....

  end

end


