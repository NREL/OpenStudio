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

      # Allows the storage of a Proc passed through '&' in a
      # hash.
      #
      # Note: this may be inefficient, since the hash being
      # used is not necessarily very good. In Observable,
      # efficiency is not too important, since the hash is
      # only accessed when adding and removing listeners,
      # not when notifying.

      class ProcWrapper

        # Creates a new wrapper for a_proc.
        def initialize(a_proc)
          @a_proc = a_proc
          @hash = a_proc.inspect.sub(/^(#<#{a_proc.class}:)/){''}.sub(/(>)$/){''}.hex
        end

        def hash
          return @hash
        end

        def ==(other)
          case(other)
            when ProcWrapper
              return @a_proc == other.to_proc
            else
              return super
          end
        end
        alias :eql? :==

        def to_proc
          return @a_proc
        end
      end
    end
  end
end
