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

require "#{File.dirname(__FILE__)}/../DakotaInitializer.rb"

require 'pathname'

class DaceOAS < DakotaInitializer

  def initialize(out_dir="",prob_file="",x0=[],
                 driver=true,nodes=1,cores=4,verbose=false,restart_file=OpenStudio::Path.new)
    super(out_dir,prob_file,x0,driver,nodes,cores,verbose,restart_file)
  end
  
  def in_file_method()
    result = String.new
    result << "method,\n"
    result << "        id_method = 'DACE'\n"    
    result << "        dace oas\n"
    result << "          samples = 10\n"
    result << "          main_effects\n"
    result << "          max_iterations = 100\n"
    result << "          convergence_tolerance = 1e-4\n\n"
    return result
  end  
  
  def in_file_variables()
    result = super
    if not @problem.x0.empty?
      result << "          initial_point "
      for i in 1..@problem.x0.size()
        result << @problem.x0[i-1].to_s.rjust(8)
      end
      result << "\n"
    end
    result << "          lower_bounds"
    for i in 1..@problem.nx
      result << (-1.0).to_s.rjust(8)
    end
    result << "\n          upper_bounds" 
    for i in 1..@problem.nx
      result << (1.0).to_s.rjust(8)
    end
    result << "\n\n"
    return result
  end    
  
#  def in_file_responses()  
#    result = String.new
#    result << "responses,\n"
#    result << "        num_objective_functions = 1\n"
#    result << "        numerical_gradients\n"
#    result << "          method_source dakota\n"
#    result << "          interval_type forward\n"
#    result << "          fd_gradient_step_size = 1.e-5\n"
#    result << "        no_hessians\n\n"
#    return result
#  end    
    
end

DaceOAS.new

