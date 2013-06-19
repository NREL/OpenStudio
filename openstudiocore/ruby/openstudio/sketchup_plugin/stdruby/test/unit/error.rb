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

require 'test/unit/util/backtracefilter'

module Test
  module Unit

    # Encapsulates an error in a test. Created by
    # Test::Unit::TestCase when it rescues an exception thrown
    # during the processing of a test.
    class Error
      include Util::BacktraceFilter

      attr_reader(:test_name, :exception)

      SINGLE_CHARACTER = 'E'

      # Creates a new Error with the given test_name and
      # exception.
      def initialize(test_name, exception)
        @test_name = test_name
        @exception = exception
      end

      # Returns a single character representation of an error.
      def single_character_display
        SINGLE_CHARACTER
      end

      # Returns the message associated with the error.
      def message
        "#{@exception.class.name}: #{@exception.message}"
      end

      # Returns a brief version of the error description.
      def short_display
        "#@test_name: #{message.split("\n")[0]}"
      end

      # Returns a verbose version of the error description.
      def long_display
        backtrace = filter_backtrace(@exception.backtrace).join("\n    ")
        "Error:\n#@test_name:\n#{message}\n    #{backtrace}"
      end

      # Overridden to return long_display.
      def to_s
        long_display
      end
    end
  end
end
