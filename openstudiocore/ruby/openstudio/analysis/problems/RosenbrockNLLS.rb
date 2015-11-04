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
#   Standard unconstrained optimization problem in two variables. 
#   f(x) = (1-x_1)^2 + 100*(x_2 - x_1^2)^2
#
# == Useage
#
# == Example
#
######################################################################

require "#{File.dirname(__FILE__)}/../OptimizationProblem.rb"

class RosenbrockNLLS < OptimizationProblem

  def initialize(x0 = Array.new(2,0.0), verbose = false)
    super(2,0,0,x0,verbose)
  end

  def f_eval(x)
    check_input(x)
    value2 = (1.0 - x[0])
    value1 = 100.0*(x[1] - x[0]**2)
    value = []
    value.push(value1.to_f, value2.to_f)
    if @verbose
      puts "In RosenbrockNLLS.rb, x[0] = " + x[0].to_s
      puts "In RosenbrockNLLS.rb, x[1] = " + x[1].to_s
      puts "In RosenbrockNLLS.rb, f = " + value.to_s
    end
    return value
  end
  
  def f_to_s
    return "(1 - x_1)^2 + 100(x_2 - x_1^2)^2"
  end
  
end
  
# create an instance so RunDakotaOnTestProblem can find this class
RosenbrockNLLS.new

