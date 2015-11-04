######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

######################################################################
# == Synopsis
#
#   Base class for holding continuous optimization problems.
#
# == Useage
#
#   
#
# == Example
#
#   
######################################################################

class OptimizationProblem

  attr_accessor :nx, :ng, :nh, :x0, :verbose

  def initialize(nx,nh=0,ng=0,x0=[],verbose=false)
    @nx = nx
    @nh = nh
    @ng = ng
    @x0 = x0
    if (not x0.empty?) and (x0.size() != @nx)
      warn "Provided starting point ignored because its length is " + x0.size().to_s + 
           "while the number of variables is " + @nx.to_s + "."
      @x0 = []
    elsif (not x0.empty?)
      check_input(x0)
    end    
    @verbose = verbose    
  end
  
  def check_input(x)
    if (not x.is_a?(Array)) or (not (x.size() == @nx)) 
      raise "Expected an input array of size " + @nx.to_s + "." 
    end
  
    for i in 0..(@nx-1)
      if not x[i].is_a?(Float)
        raise "Expected an Array of Floats."
      end
    end
  end
  
  def f_eval(x)
    raise "Undefined method in abstract class."
  end

  def h_eval(x)
    if @nh > 0
      raise "Undefined method in abstract class."
    end
    return []
  end
  
  def g_eval(x)
    if @ng > 0
      raise "Undefined method in abstract class."
    end
    return []
  end 
  
  def to_s
    fstr = f_to_s
    hstr = h_to_s
    gstr = g_to_s
    if not fstr.empty?
      puts "min " + fstr
    end
    sep = "s.t. "
    if not hstr.empty?
      hstr.each_line { |ln| 
        puts sep + ln
        sep = "     "
      }
    end
    if not gstr.empty?
      gstr.each_line { |ln| 
        puts sep + ln
        sep = "     "
      }
    end
  end
  
  def f_to_s
    return ""
  end
  
  def h_to_s
    return ""
  end
  
  def g_to_s
    return ""
  end
  
end
  
  
