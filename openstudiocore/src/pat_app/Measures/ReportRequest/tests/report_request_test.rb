require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'

require_relative '../measure.rb'

class ReportRequest_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  
  def test_number_of_arguments_and_argument_names
  
    # create an instance of the measure
    measure = ReportRequest.new
    
    # make an empty workspace
    workspace = OpenStudio::Workspace.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(workspace)
    assert_equal(1, arguments.size)
    assert_equal("measures_json", arguments[0].name)
  end

end
