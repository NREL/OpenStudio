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
    module Util
      module BacktraceFilter
        TESTUNIT_FILE_SEPARATORS = %r{[\\/:]}
        TESTUNIT_PREFIX = __FILE__.split(TESTUNIT_FILE_SEPARATORS)[0..-3]
        TESTUNIT_RB_FILE = /\.rb\Z/
        
        def filter_backtrace(backtrace, prefix=nil)
          return ["No backtrace"] unless(backtrace)
          split_p = if(prefix)
            prefix.split(TESTUNIT_FILE_SEPARATORS)
          else
            TESTUNIT_PREFIX
          end
          match = proc do |e|
            split_e = e.split(TESTUNIT_FILE_SEPARATORS)[0, split_p.size]
            next false unless(split_e[0..-2] == split_p[0..-2])
            split_e[-1].sub(TESTUNIT_RB_FILE, '') == split_p[-1]
          end
          return backtrace unless(backtrace.detect(&match))
          found_prefix = false
          new_backtrace = backtrace.reverse.reject do |e|
            if(match[e])
              found_prefix = true
              true
            elsif(found_prefix)
              false
            else
              true
            end
          end.reverse
          new_backtrace = (new_backtrace.empty? ? backtrace : new_backtrace)
          new_backtrace = new_backtrace.reject(&match)
          new_backtrace.empty? ? backtrace : new_backtrace
        end
      end
    end
  end
end
