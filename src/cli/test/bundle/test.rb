require 'rubygems'

def local_gems
   Gem::Specification.sort_by{ |g| [g.name.downcase, g.version] }.group_by{ |g| g.name }
end

# list installed gems
puts local_gems.map{ |name, specs|
  [name, specs.map{ |spec| spec.version.to_s }.join(',')].join(' ')
}

# test a simple gem
require 'tilt'
puts Tilt::VERSION
raise "Tilt version does not match" unless Tilt::VERSION == '2.0.8'

# test a more complex / larger gem, that should override the existing version installed
# openstudio-workflow 2.0.0.pre1 is compatible with bundler 2.1 (OS 3.x),
# and already older than currently used at this time (2.0.0)
require 'openstudio'
require 'openstudio-workflow'
puts OpenStudio::Workflow::VERSION
raise "OpenStudio Workflow gem version does not match" unless OpenStudio::Workflow::VERSION == '2.4.0'
