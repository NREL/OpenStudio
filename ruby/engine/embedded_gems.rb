#(const std::vector<openstudio::path>& includeDirs, std::vector<openstudio::path> gemPathDirs,
#                               const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath,
#                               const openstudio::path& bundleGemDirPath, const std::string& bundleWithoutGroups) {

def setup_embedded_gems() #includeDirs, gemPathDirs, gemHomeDir, bundleGemFilePath, bundleGemDirPath, use_bundler, bundleWithoutGroups)

  # Unset RUBY ENVs if previously set (e.g. rvm sets these in shell)

  ENV.delete('GEM_HOME') if ENV['GEM_HOME']
  ENV.delete('GEM_PATH') if ENV['GEM_PATH']
  ENV.delete('BUNDLE_GEMFILE') if ENV['BUNDLE_GEMFILE']
  ENV.delete('BUNDLE_PATH') if ENV['BUNDLE_PATH']
  ENV.delete('BUNDLE_WITHOUT') if ENV['BUNDLE_WITHOUT']

  if !includeDirs.empty?

    includeDirs = includeDirs.concat($LOAD_PATH)

    $logger.info "Setting $LOAD_PATH to #{includeDirs}"
    $LOAD_PATH.clear

    includeDirs.each {|p| $LOAD_PATH << p}
  end

  if !gemPathDirs.empty?

    gemPathDirs = gemPathDirs.join(File::PATH_SEPARATOR)

    $logger.info "Setting GEM_PATH to #{gemPathDirs}"
    ENV['GEM_PATH'] = gemDirs
  end

  # Operate on the gem_home option to set GEM_HOME
  if !gemHomeDir.empty?
    $logger.info "Setting GEM_HOME to #{gemHomeDir}"
    ENV['GEM_HOME'] = gemHome
  end

  # Operate on the bundle option to set BUNDLE_GEMFILE
  use_bundler = false
  if !bundleGemFilePath.empty?
    $logger.info "Setting BUNDLE_GEMFILE to #{bundleGemFilePath}"
    ENV['BUNDLE_GEMFILE'] = bundleGemFilePath
    use_bundler = true

  end

  if !bundleGemDirPath.empty?
    $logger.info "Setting BUNDLE_PATH to #{bundleGemDirPath}"
    ENV['BUNDLE_PATH'] = bundleGemDirPath

  elsif use_bundler
    # bundle was requested but bundle_path was not provided
    $logger.warn "Bundle activated but ENV['BUNDLE_PATH'] is not set"

    $logger.info "Setting BUNDLE_PATH to ':/ruby/2.7.0/'"
    ENV['BUNDLE_PATH'] = ':/ruby/2.7.0/'

  end

  if !bundleWithoutGroups.empty?
    $logger.info "Setting BUNDLE_WITHOUT to #{bundleWithoutGroups}"
    ENV['BUNDLE_WITHOUT'] = bundleWithoutGroups

  elsif use_bundler
    # bundle was requested but bundle_path was not provided
    $logger.info "Bundle activated but ENV['BUNDLE_WITHOUT'] is not set"

    # match configuration in build_openstudio_gems
    $logger.info "Setting BUNDLE_WITHOUT to 'test'"
    ENV['BUNDLE_WITHOUT'] = 'test'

    # ignore any local config on disk
    #DLM: this would be correct if the bundle was created here
    #it would not be correct if the bundle was transfered from another computer
    #ENV['BUNDLE_IGNORE_CONFIG'] = 'true'

  end

  Gem.paths.path << ':/ruby/2.7.0/gems/'
  Gem.paths.path << ':/ruby/2.7.0/bundler/gems/'
  Gem::Deprecate.skip = true

  # find all the embedded gems
  original_embedded_gems = {}
  begin
    EmbeddedScripting::fileNames.each do |f|
      if md = /specifications\/.*\.gemspec$/.match(f) ||
         md = /bundler\/.*\.gemspec$/.match(f)
        begin
          spec = EmbeddedScripting::getFileAsString(f)
          s = eval(spec)
          s.loaded_from = f
          # This is shenanigans because otherwise rubygems will think extensions are missing
          # But we are initing them manually so they are not missing
          # Here the missing_extensions? method is redefined for only this instance "s"
          class << s
            define_method(:missing_extensions?) { false }
          end
          original_embedded_gems[s.name] = s

          init_count = 0
          Gem::Specification.each {|x| init_count += 1}

          # if already have an equivalent spec this will be a no-op
          Gem::Specification.add_spec(s)

          post_count = 0
          Gem::Specification.each {|x| post_count += 1}

          if post_count == init_count
            $logger.debug "Found system gem #{s.name} #{s.version}, overrides embedded gem"
          end

        rescue LoadError => e
          safe_puts e.message
        rescue => e
          safe_puts e.message
        end
      end
    end
  rescue NameError => e
    # EmbeddedScripting not available
  end

  original_load_path = $LOAD_PATH.clone
  embedded_gems_to_activate = []

  # Identify the embedded gems (don't activate them yet)
  current_dir = Dir.pwd
  begin
    # get a list of all the embedded gems
    dependencies = []
    original_embedded_gems.each_value do |spec|
      $logger.debug "Adding dependency on #{spec.name} '~> #{spec.version}'"
      dependencies << Gem::Dependency.new(spec.name, "~> #{spec.version}")
    end

    # resolve dependencies
    activation_errors = false
    resolver = Gem::Resolver.for_current_gems(dependencies)
    activation_requests = resolver.resolve
    $logger.debug "Processing #{activation_requests.size} activation requests"
    activation_requests.each do |request|
      do_activate = true
      spec = request.spec

      # check if this is one of our embedded gems
      if original_embedded_gems[spec.name]
        # check if gem can be loaded from RUBYLIB, this supports developer use case
        original_load_path.each do |lp|
          if File.exists?(File.join(lp, spec.name)) || File.exists?(File.join(lp, spec.name + '.rb')) || File.exists?(File.join(lp, spec.name + '.so'))
            $logger.debug "Found #{spec.name} in '#{lp}', overrides gem #{spec.spec_file}"
            do_activate = false
            break
          end
        end
      end
      if do_activate
        embedded_gems_to_activate << spec
      end

    end

    if activation_errors
      return false
    end
  ensure
    Dir.chdir(current_dir)
  end

  # Load the bundle before activating any embedded gems
  if use_bundler

    embedded_gems_to_activate.each do |spec|
      if spec.name == "bundler"
        $logger.debug "Activating gem #{spec.spec_file}"
        begin
          # Activate will manipulate the $LOAD_PATH to include the gem
          spec.activate
        rescue Gem::LoadError
          # There may be conflicts between the bundle and the embedded gems,
          # those will be logged here
          $logger.error "Error activating gem #{spec.spec_file}"
          activation_errors = true
        end
      end
    end

    current_dir = Dir.pwd

    original_arch = nil
    if RbConfig::CONFIG['arch'] =~ /x64-mswin64/
      # assume that system ruby of 'x64-mingw32' architecture was used to create bundle
      original_arch = RbConfig::CONFIG['arch']
      $logger.info "Temporarily replacing arch '#{original_arch}' with 'x64-mingw32' for Bundle"
      RbConfig::CONFIG['arch'] = 'x64-mingw32'
    end

    # require bundler
    # have to do some forward declaration and pre-require to get around autoload cycles
    require 'bundler/errors'
    #require 'bundler/environment_preserver'
    require 'bundler/plugin'
    #require 'bundler/rubygems_ext'
    require 'bundler/rubygems_integration'
    require 'bundler/version'
    require 'bundler/ruby_version'
    #require 'bundler/constants'
    #require 'bundler/current_ruby'
    require 'bundler/gem_helpers'
    #require 'bundler/plugin'
    require 'bundler/source'
    require 'bundler/definition'
    require 'bundler/dsl'
    require 'bundler/uri_credentials_filter'
    require 'bundler'

    begin
      # activate bundled gems
      # bundler will look in:
      # 1) ENV["BUNDLE_GEMFILE"]
      # 2) find_file("Gemfile", "gems.rb")
      #require 'bundler/setup'

      groups = Bundler.definition.groups
      keep_groups = []
      without_groups = ENV['BUNDLE_WITHOUT']
      $logger.info "without_groups = #{without_groups}"
      groups.each do |g|
        $logger.info "g = #{g}"
        if without_groups.include?(g.to_s)
          $logger.info "Bundling without group '#{g}'"
        else
          keep_groups << g
        end
      end

      $logger.info "Bundling with groups [#{keep_groups.join(',')}]"

      remaining_specs = []
      Bundler.definition.specs_for(keep_groups).each {|s| remaining_specs << s.name}

      $logger.info "Specs to be included [#{remaining_specs.join(',')}]"


      Bundler.setup(*keep_groups)
    ensure

      if original_arch
        $logger.info "Restoring arch '#{original_arch}'"
        RbConfig::CONFIG['arch'] = original_arch
      end

      Dir.chdir(current_dir)
    end
  end

  original_load_path = $LOAD_PATH.clone

  embedded_gems_to_activate.each do |spec|
    $logger.debug "Activating gem #{spec.spec_file}"
    begin
      # Activate will manipulate the $LOAD_PATH to include the gem
      spec.activate
    rescue Gem::LoadError
      # There may be conflicts between the bundle and the embedded gems,
      # those will be logged here
      $logger.error "Error activating gem #{spec.spec_file}"
      activation_errors = true
    end
  end

  # Get all of the embedded gem paths which were added by activating the embedded gems
  # This is used by embedded_help::require
  $EMBEDDED_GEM_PATH = $LOAD_PATH - original_load_path
  # Make sure no non embedded paths snuck in
  $EMBEDDED_GEM_PATH = $EMBEDDED_GEM_PATH.select {|p| p.to_s.chars.first == ':'}
  # Restore LOAD_PATH
  $LOAD_PATH.reject! {|p| not original_load_path.any? p}
end
