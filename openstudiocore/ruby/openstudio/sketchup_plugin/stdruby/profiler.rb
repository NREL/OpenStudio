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

module Profiler__
  # internal values
  @@start = @@stack = @@map = nil
  PROFILE_PROC = proc{|event, file, line, id, binding, klass|
    case event
    when "call", "c-call"
      now = Process.times[0]
      @@stack.push [now, 0.0]
    when "return", "c-return"
      now = Process.times[0]
      key = [klass, id]
      if tick = @@stack.pop
        data = (@@map[key] ||= [0, 0.0, 0.0, key])
        data[0] += 1
        cost = now - tick[0]
        data[1] += cost
        data[2] += cost - tick[1]
        @@stack[-1][1] += cost if @@stack[-1]
      end
    end
  }
module_function
  def start_profile
    @@start = Process.times[0]
    @@stack = []
    @@map = {}
    set_trace_func PROFILE_PROC
  end
  def stop_profile
    set_trace_func nil
  end
  def print_profile(f)
    stop_profile
    total = Process.times[0] - @@start
    if total == 0 then total = 0.01 end
    data = @@map.values
    data.sort!{|a,b| b[2] <=> a[2]}
    sum = 0
    f.printf "  %%   cumulative   self              self     total\n"
    f.printf " time   seconds   seconds    calls  ms/call  ms/call  name\n"
    for d in data
      sum += d[2]
      f.printf "%6.2f %8.2f  %8.2f %8d ", d[2]/total*100, sum, d[2], d[0]
      f.printf "%8.2f %8.2f  %s\n", d[2]*1000/d[0], d[1]*1000/d[0], get_name(*d[3])
    end
    f.printf "%6.2f %8.2f  %8.2f %8d ", 0.0, total, 0.0, 1     # ???
    f.printf "%8.2f %8.2f  %s\n", 0.0, total*1000, "#toplevel" # ???
  end
  def get_name(klass, id)
    name = klass.to_s || ""
    if klass.kind_of? Class
      name += "#"
    else
      name += "."
    end
    name + id.id2name
  end
  private :get_name
end
