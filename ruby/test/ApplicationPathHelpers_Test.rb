########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ApplicationPathHelpers_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_energyplus
    ep_dir = OpenStudio::getEnergyPlusDirectory
    assert(File.exist?(ep_dir.to_s))
    assert(File.directory?(ep_dir.to_s))

    ep = OpenStudio::getEnergyPlusExecutable
    assert(File.exist?(ep.to_s))
    assert(!File.directory?(ep.to_s))
    assert(File.executable?(ep.to_s))
  end


end


