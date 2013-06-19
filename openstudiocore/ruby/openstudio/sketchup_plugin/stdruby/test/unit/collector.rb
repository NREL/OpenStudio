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
    module Collector
      def initialize
        @filters = []
      end

      def filter=(filters)
        @filters = case(filters)
          when Proc
            [filters]
          when Array
            filters
        end
      end

      def add_suite(destination, suite)
        to_delete = suite.tests.find_all{|t| !include?(t)}
        to_delete.each{|t| suite.delete(t)}
        destination << suite unless(suite.size == 0)
      end

      def include?(test)
        return true if(@filters.empty?)
        @filters.each do |filter|
          result = filter[test]
          if(result.nil?)
            next
          elsif(!result)
            return false
          else
            return true
          end
        end
        true
      end

      def sort(suites)
        suites.sort_by{|s| s.name}
      end
    end
  end
end
