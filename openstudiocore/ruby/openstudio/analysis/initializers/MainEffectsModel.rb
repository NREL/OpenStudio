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

class LHSTestSetAndMainEffects < DakotaInitializer

  def initialize(out_dir="",prob_file="",x0=[],
                 driver=true,nodes=1,cores=4,verbose=false,restart_file=OpenStudio::Path.new)
    super(out_dir,prob_file,x0,driver,nodes,cores,verbose,restart_file)
  end
  
  # wanted to have LHS points automatically run through real model and surrogate model, but cannot seem to do that in one shot.
  def in_file_method()
    result = String.new
    result << "method,\n"
    result << "        list_parameter_study\n"
    result << "          model_pointer = 'M1'\n"
    result << "          list_of_points = \n"
    result << "           " << " 0.0" * @problem.nx << "\n"
    for i in 1..@problem.nx
      result << "           " << " 0.0" * (i-1) << " 1.0" << " 0.0" * (@problem.nx-i) << "\n"
    end
    result << "\n"
    return result
  end  
  
  def in_file_model()
    result = String.new
    result << "model,\n"
    result << "        id_model = 'M1'\n"
    result << "        surrogate global,\n"
    result << "          polynomial linear\n"
    result << "          reuse_samples all\n"
#    result << "          points_file='dakota_tabular_1.dat'\n" # also works (without loading restart file)
    result << "          diagnostics = 'sum_squared'\n"
    result << "                        'mean_squared'\n"
    result << "                        'root_mean_squared'\n"
    result << "                        'max_squared'\n"
    result << "                        'sum_scaled'\n"
    result << "                        'max_scaled'\n"
    result << "                        'sum_abs'\n" 
    result << "                        'mean_abs'\n" 
    result << "                        'max_abs'\n"
    result << "                        'press'\n"
    result << "                        'cv'\n"
    result << "                        'rsquared'\n\n"
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
  
  def in_file_responses()  
    result = String.new
    result << "responses,\n"
    result << "          num_response_functions = 1\n"
    result << "          no_gradients\n"
    result << "          no_hessians\n"
    return result
  end    
    
end

LHSTestSetAndMainEffects.new

