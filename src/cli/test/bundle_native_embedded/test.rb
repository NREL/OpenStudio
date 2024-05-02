require 'rubygems'

def local_gems
   Gem::Specification.sort_by{ |g| [g.name.downcase, g.version] }.group_by{ |g| g.name }
end

# list installed gems
puts local_gems.map{ |name, specs|
  [name, specs.map{ |spec| spec.version.to_s }.join(',')].join(' ')
}
# test a gem that required native extensions
require 'json'
puts JSON::VERSION
raise "json version does not match" unless JSON::VERSION == '2.6.3'
