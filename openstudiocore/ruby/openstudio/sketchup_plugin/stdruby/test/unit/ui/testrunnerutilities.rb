######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

module Test
  module Unit
    module UI

      SILENT = 0
      PROGRESS_ONLY = 1
      NORMAL = 2
      VERBOSE = 3

      # Provides some utilities common to most, if not all,
      # TestRunners.
      #
      #--
      #
      # Perhaps there ought to be a TestRunner superclass? There
      # seems to be a decent amount of shared code between test
      # runners.

      module TestRunnerUtilities

        # Creates a new TestRunner and runs the suite.
        def run(suite, output_level=NORMAL)
          return new(suite, output_level).start
        end

        # Takes care of the ARGV parsing and suite
        # determination necessary for running one of the
        # TestRunners from the command line.
        def start_command_line_test
          if ARGV.empty?
            puts "You should supply the name of a test suite file to the runner"
            exit
          end
          require ARGV[0].gsub(/.+::/, '')
          new(eval(ARGV[0])).start
        end
      end
    end
  end
end
