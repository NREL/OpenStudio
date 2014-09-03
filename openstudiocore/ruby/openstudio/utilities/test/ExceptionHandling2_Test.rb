require 'openstudio'

require 'minitest/autorun'

class Exceptions2_Test < Minitest::Test
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_exceptions2

    workflow = OpenStudio::Runmanager::Workflow.new("ModelToIdf->ExpandObjects->EnergyPlus")


    job = workflow.create(OpenStudio::Path.new("/tmp/outdir"), OpenStudio::Path.new("somefile.osm"))

  end
end


