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

require 'test/unit/collector'

module Test
  module Unit
    module Collector
      class ObjectSpace
        include Collector
        
        NAME = 'collected from the ObjectSpace'
        
        def initialize(source=::ObjectSpace)
          super()
          @source = source
        end
        
        def collect(name=NAME)
          suite = TestSuite.new(name)
          sub_suites = []
          @source.each_object(Class) do |klass|
            if(Test::Unit::TestCase > klass)
              add_suite(sub_suites, klass.suite)
            end
          end
          sort(sub_suites).each{|s| suite << s}
          suite
        end
      end
    end
  end
end
