module OpenStudio
  module Workflow
    module Util

      # A set of methods to manage directories, specifically the output, and files directories
      #
      module Directory

        # Returns the absolute directory in which to find the OSW with correct precedence
        #
        # @param [String] directory The directory defined by the user
        # @return [String] If the directory was not absolute it is made so by joining it with Dir.pwd
        #
        def get_directory(directory)
          abs_dir = nil
          abs_dir = directory if Pathname.new(directory).absolute?
          abs_dir = File.join(Dir.pwd, directory) unless abs_dir
          File.absolute_path(abs_dir)
        end

        # Returns the run_directory with correct precedence
        #
        # @param [Hash] workflow The OSW hash to parse for the run directory. The order of precedence for paths is as
        #   follows: 1 - an absolute path defined in :run_dir, 2 - the :root_dir joined with :run_dir, 3 - the current
        #   current working directory joined with :run_dir. If :run_dir is not defined directory is used instead
        # @param [String] directory The main directory defined in the workflow instance. If :run_dir is not defined in
        #   the workflow, then the default run directory becomes directory joined with 'run'
        # @return [String] The absolute path of the run directory
        #
        def get_run_dir(workflow, directory)
          run_dir = nil
          if workflow[:run_directory]
            if Pathname(workflow[:run_directory]).absolute?
              run_dir = File.join(workflow[:run_dirdirectory], 'run')
            elsif get_root_dir(workflow, directory)
              run_dir = File.join(get_root_dir(workflow, directory), workflow[:run_directory], 'run')
            else
              run_dir = File.join(Dir.pwd, workflow[:run_dirirectory], 'run')
            end
          end
          run_dir = File.join(get_directory(directory), 'run') unless run_dir
          File.absolute_path(run_dir)
        end

        # Return an compliant absolute root directory
        #
        # @param [Hash] workflow The OSW hash to parse for the root directory. The order of precedence for paths is as
        #   follows: 1 - an absolute path defined in :root_dir, 2 - the current working directory joined with the
        #   relative path defined in :root_dir
        # @return [String] Returns an absolute path as a string if :root_dir is defined, otherwise nil
        #
        def get_root_dir(workflow, directory)
          root_dir = nil
          if workflow[:root_dir]
            if Pathname.new(workflow[:root_dir]).absolute?
              root_dir = workflow[:root_dir]
            else
              root_dir = File.join(Dir.pwd, root_dir)
            end
          else
            root_dir = directory
          end
          File.absolute_path(root_dir)
        end
      end
    end
  end
end