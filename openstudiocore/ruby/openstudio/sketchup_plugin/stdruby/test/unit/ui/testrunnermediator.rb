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

require 'test/unit'
require 'test/unit/util/observable'
require 'test/unit/testresult'

module Test
  module Unit
    module UI

      # Provides an interface to write any given UI against,
      # hopefully making it easy to write new UIs.
      class TestRunnerMediator
        RESET = name + "::RESET"
        STARTED = name + "::STARTED"
        FINISHED = name + "::FINISHED"
        
        include Util::Observable
        
        # Creates a new TestRunnerMediator initialized to run
        # the passed suite.
        def initialize(suite)
          @suite = suite
        end

        # Runs the suite the TestRunnerMediator was created
        # with.
        def run_suite
          Unit.run = true
          begin_time = Time.now
          notify_listeners(RESET, @suite.size)
          result = create_result
          notify_listeners(STARTED, result)
          result_listener = result.add_listener(TestResult::CHANGED) do |updated_result|
            notify_listeners(TestResult::CHANGED, updated_result)
          end
          
          fault_listener = result.add_listener(TestResult::FAULT) do |fault|
            notify_listeners(TestResult::FAULT, fault)
          end
          
          @suite.run(result) do |channel, value|
            notify_listeners(channel, value)
          end
          
          result.remove_listener(TestResult::FAULT, fault_listener)
          result.remove_listener(TestResult::CHANGED, result_listener)
          end_time = Time.now
          elapsed_time = end_time - begin_time
          notify_listeners(FINISHED, elapsed_time) #"Finished in #{elapsed_time} seconds.")
          return result
        end

        private
        # A factory method to create the result the mediator
        # should run with. Can be overridden by subclasses if
        # one wants to use a different result.
        def create_result
          return TestResult.new
        end
      end
    end
  end
end
