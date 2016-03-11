require 'openstudio'

filePath = OpenStudio::Path.new(ARGV[0])
puts "Debugging forward translation of " + filePath.to_s
logPath = filePath.parent_path / OpenStudio::Path.new("DebugForwardTranslator.log")
logFile = OpenStudio::FileLogSink.new(logPath)
logFile.setLogLevel(-3)
OpenStudio::Logger::instance.standardOutLogger.disable

model = OpenStudio::Model::Model::load(filePath).get
translator = OpenStudio::EnergyPlus::ForwardTranslator.new
workspace = translator.translateModel(model)

puts "Translator encountered " + translator.errors.size.to_s + " errors."
puts "Translator encountered " + translator.warnings.size.to_s + " warnings."
puts "Please see " + logPath.to_s + " for more information."