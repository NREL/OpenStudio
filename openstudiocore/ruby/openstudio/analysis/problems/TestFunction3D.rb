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

class TestFunction3D < OptimizationProblem

  def initialize(x0 = Array.new(3,0.0),verbose = false)
    super(3,0,0,x0,verbose)
  end

  def f_eval(x)
    check_input(x)
    value = (1.0 - x[0])**2 + (1.0 - x[1])**2 + (1.0 - x[2])**2
    if @verbose
      puts "In TestFunction3D.rb, x = " + x[0].to_s + ", " + 
                                           x[1].to_s + ", " + 
                                           x[2].to_s
      puts "In TestFunction3D.rb, f = " + value.to_s
    end
    return value
  end
  
  def f_to_s
    return "(1 - x_1)^2 + (1 - x_2)^2 + (1 - x_3)^2"
  end
  
end
  
# create an instance so RunDakotaOnTestProblem can find this class
TestFunction3D.new

