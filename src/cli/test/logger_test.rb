require 'logger'

$logger = Logger.new(STDOUT)
$logger.level = Logger::WARN
$logger.formatter = proc do |severity, datetime, progname, msg|
   "LOGGER - #{msg}\n"
end

OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Error)

$logger.info "STDOUT Info"
$logger.warn "STDOUT Warn"
$logger.error "STDOUT Error"

OpenStudio::logFree(OpenStudio::Info, "test", "Info")
OpenStudio::logFree(OpenStudio::Warn, "test", "Warn")
OpenStudio::logFree(OpenStudio::Error, "test", "Error")
