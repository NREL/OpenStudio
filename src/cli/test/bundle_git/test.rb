require 'rubygems'

def local_gems
   Gem::Specification.sort_by{ |g| [g.name.downcase, g.version] }.group_by{ |g| g.name }
end

# list installed gems
puts local_gems.map{ |name, specs|
  [name, specs.map{ |spec| spec.version.to_s }.join(',')].join(' ')
}

# A gem we do not embed
require 'tilt'
puts Tilt::VERSION
raise "Tilt version does not match" unless Tilt::VERSION == '2.0.8'

# A gem we do embed, different version
require 'simplecov'
puts SimpleCov::VERSION
raise "SimpleCov version does not match" unless SimpleCov::VERSION.to_s == '0.21.2'

# TODO: until we have gems on rubygems, this is unpractical without forcing a
# download from git for all openstudio-extension, measure-tester, and workflow
# gems

# # test a github checkout gem
# require 'openstudio'
# require 'openstudio/extension'
# puts OpenStudio::Extension::VERSION
# raise "OpenStudio Extension version does not match" unless OpenStudio::Extension::VERSION.to_s == '0.8.0'

# did_fail = false
# begin
#   require 'openstudio-standards'
# rescue LoadError
#   did_fail = true
# end
# raise "Should allow to load the embedded openstudio-standards" unless did_fail == false
