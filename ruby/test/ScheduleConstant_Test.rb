########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ScheduleConstant_Test < Minitest::Test

  def test_ScheduleConstant
    model = OpenStudio::Model::Model.new
    schedule = OpenStudio::Model::ScheduleConstant.new(model)
    schedule.setValue(1.0)
    assert_equal(1.0,schedule.value)
  end

end
