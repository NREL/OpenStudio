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
require 'openstudio'
require 'openstudio-standards'
puts OpenstudioStandards::VERSION
raise "OpenStudio Standards version does not match" unless OpenstudioStandards::VERSION == '0.2.0'
