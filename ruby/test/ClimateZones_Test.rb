########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ClimateZones_Test < Minitest::Test

  def test_basic

    model = OpenStudio::Model::Model.new

    climateZones = model.getClimateZones
    assert_equal(0, climateZones.numClimateZones)
    climateZones.pushExtensibleGroup # same as the old Ctor

    assert_equal(1, climateZones.numClimateZones)
    individualZones = climateZones.climateZones
    assert_equal(1,individualZones.size)
    defaultZone = individualZones[0]
    assert_equal("ASHRAE",defaultZone.institution)
    assert_equal("ANSI/ASHRAE Standard 169",defaultZone.documentName)
    assert_equal(OpenStudio::Model::ClimateZones.ashraeDefaultYear,defaultZone.year)
    assert_equal("",defaultZone.value)

    assert(defaultZone.setValue("3C"))

    newZone = climateZones.appendClimateZone(OpenStudio::Model::ClimateZones.cecInstitutionName,
                                                    "12")
    assert_equal(false,newZone.empty)
    assert_equal(2,climateZones.numClimateZones)

  end

end
