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

    # A collection of tests which can be #run.
    #
    # Note: It is easy to confuse a TestSuite instance with
    # something that has a static suite method; I know because _I_
    # have trouble keeping them straight. Think of something that
    # has a suite method as simply providing a way to get a
    # meaningful TestSuite instance.
    class TestSuite
      attr_reader :name, :tests
      
      STARTED = name + "::STARTED"
      FINISHED = name + "::FINISHED"

      # Creates a new TestSuite with the given name.
      def initialize(name="Unnamed TestSuite")
        @name = name
        @tests = []
      end

      # Runs the tests and/or suites contained in this
      # TestSuite.
      def run(result, &progress_block)
        yield(STARTED, name)
        @tests.each do |test|
          test.run(result, &progress_block)
        end
        yield(FINISHED, name)
      end

      # Adds the test to the suite.
      def <<(test)
        @tests << test
        self
      end

      def delete(test)
        @tests.delete(test)
      end

      # Retuns the rolled up number of tests in this suite;
      # i.e. if the suite contains other suites, it counts the
      # tests within those suites, not the suites themselves.
      def size
        total_size = 0
        @tests.each { |test| total_size += test.size }
        total_size
      end
      
      def empty?
        tests.empty?
      end

      # Overridden to return the name given the suite at
      # creation.
      def to_s
        @name
      end
      
      # It's handy to be able to compare TestSuite instances.
      def ==(other)
        return false unless(other.kind_of?(self.class))
        return false unless(@name == other.name)
        @tests == other.tests
      end
    end
  end
end
