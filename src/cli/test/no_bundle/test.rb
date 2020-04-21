require 'rubygems'

def local_gems
   Gem::Specification.sort_by{ |g| [g.name.downcase, g.version] }.group_by{ |g| g.name }
end

# list installed gems
puts local_gems.map{ |name, specs|
  [name, specs.map{ |spec| spec.version.to_s }.join(',')].join(' ')
}

require 'openstudio'
require 'openstudio-standards'
puts OpenstudioStandards::VERSION
