require 'rubygems'

def local_gems
   Gem::Specification.sort_by{ |g| [g.name.downcase, g.version] }.group_by{ |g| g.name }
end

# list installed gems
puts local_gems.map{ |name, specs| 
  [name, specs.map{ |spec| spec.version.to_s }.join(',')].join(' ') 
}

# test a github checkout gem
require 'openstudio'
require 'openstudio/extension'
puts OpenStudio::Extension::VERSION
raise "OpenStudio Extension version does not match" unless OpenStudio::Extension::VERSION.to_s == '0.1.1'

require 'tilt'
puts Tilt::VERSION
raise "Tilt version does not match" unless Tilt::VERSION == '2.0.8'

did_fail = false
begin
  require 'openstudio-standards'
rescue LoadError
  did_fail = true
end
raise "Should not load openstudio-standards" unless did_fail == true
