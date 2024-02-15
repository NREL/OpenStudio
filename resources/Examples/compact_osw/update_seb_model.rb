# Just for convenience: having to VT the seb model which is at 1.11.5
# can be very time consuming, especially on debug builds

require 'openstudio'

model_path = File.join(__dir__, 'files/seb.osm')
raise "#{model_path} not found" if !File.file?(model_path)

# Starting at 3.7.0, no need to explicitly call vt, but still doing it
# m = OpenStudio::Model::Model::load(model_path).get()
vt = OpenStudio::OSVersion::VersionTranslator.new
OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Debug)
m = vt.loadModel(model_path).get()
m.save(model_path, true)
