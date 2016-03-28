load "#{File.dirname(__FILE__)}/openstudio.rb"
load "openstudio-workflow"

osw_path = ARGV[0]

adapter_options = {
    workflow_filename: osw_path
}
adapter = OpenStudio::Workflow.load_adapter 'local', adapter_options
run_options = {
    debug: true
}
k = OpenStudio::Workflow::Run.new adapter, osw_path, run_options
k.run