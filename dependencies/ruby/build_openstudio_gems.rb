require 'fileutils'

install_dir = ARGV[0]

if /win/.match(RUBY_PLATFORM) || /mingw/.match(RUBY_PLATFORM)
  ENV['PATH'] = "#{ENV['PATH']};C:\\Program Files\\Git\\cmd"
end

if File.exists?(install_dir)
  FileUtils.rm_rf(install_dir)
end

ENV['BUNDLE_WITHOUT'] = 'test'
system("bundle install --without=test --path='#{install_dir}'")

FileUtils.rm_rf("#{install_dir}/ruby/2.0.0/cache")

standards_gem_dir = nil
workflow_gem_dir = nil
Dir.glob("#{install_dir}/ruby/2.0.0/bundler/gems/*").each do |f|
  if /openstudio-standards/i.match(f)
    standards_gem_dir = f
  elsif /openstudio-workflow/i.match(f)
    workflow_gem_dir = f
  end
end

# clean up standards gem
FileUtils.rm_rf("#{standards_gem_dir}/.git")
FileUtils.rm_rf("#{standards_gem_dir}/measures")
FileUtils.rm_rf("#{standards_gem_dir}/openstudio-standards/test")
FileUtils.rm_rf("#{standards_gem_dir}/openstudio-standards/docs")
Dir.glob("#{standards_gem_dir}/openstudio-standards/data/weather/*").each do |f|
  if /CAN_/.match(f)
    FileUtils.rm_f(f)
  end
end

# clean up workflow gem
FileUtils.rm_rf("#{workflow_gem_dir}/.git")
FileUtils.rm_rf("#{workflow_gem_dir}/spec")
FileUtils.rm_rf("#{workflow_gem_dir}/test")

# tar -zcvf openstudio-gems.tar.gz openstudio-gems
# md5sum openstudio-gems.tar.gz