########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ElectricLoadCenterDistribution_Test < Minitest::Test

  def test_ElectricLoadCenterDistribution_Photovoltaics

    model = OpenStudio::Model::Model.new

    elcd = OpenStudio::Model::ElectricLoadCenterDistribution.new(model)
    assert_equal(elcd.generators.size, 0)
    assert(elcd.inverter.empty?)

    simple_panel = OpenStudio::Model::GeneratorPhotovoltaic::simple(model)
    assert(simple_panel.electricLoadCenterDistribution.empty?)

    elcd.addGenerator(simple_panel)
    assert(!simple_panel.electricLoadCenterDistribution.empty?)
    assert_equal(elcd.generators.size, 1)
    assert(elcd.inverter.empty?)

    eod_panel = OpenStudio::Model::GeneratorPhotovoltaic::equivalentOneDiode(model)
    assert(eod_panel.electricLoadCenterDistribution.empty?)

    elcd.addGenerator(eod_panel)
    assert(!eod_panel.electricLoadCenterDistribution.empty?)
    assert_equal(elcd.generators.size, 2)
    assert(elcd.inverter.empty?)

    inverter = OpenStudio::Model::ElectricLoadCenterInverterSimple.new(model)
    assert(inverter.electricLoadCenterDistribution.empty?)

    elcd.setInverter(inverter)
    assert(!inverter.electricLoadCenterDistribution.empty?)
    assert(!elcd.inverter.empty?)

  end

end
