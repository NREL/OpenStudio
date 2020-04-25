require 'rubygems'

def local_gems
   Gem::Specification.sort_by{ |g| [g.name.downcase, g.version] }.group_by{ |g| g.name }
end

# list installed gems
puts local_gems.map{ |name, specs| 
  [name, specs.map{ |spec| spec.version.to_s }.join(',')].join(' ') 
}
# test a gem that required native extensions
require 'ffi'
puts FFI::VERSION
raise "FFI version does not match" unless FFI::VERSION == '1.9.25'

require 'rubyXL'
puts RubyXL::VERSION
raise "RubyXL version does not match" unless RubyXL::VERSION == '3.3.29'
