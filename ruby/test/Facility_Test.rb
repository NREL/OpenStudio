########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Facility_Test < Minitest::Test

  def test_facility_attributes

    model = OpenStudio::Model::Model.new()

    facility = model.getFacility
    assert(facility.totalSiteEnergy.empty?)
  end

end
