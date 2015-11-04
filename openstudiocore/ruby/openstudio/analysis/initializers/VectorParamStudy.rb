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

class VectorParamStudy < DakotaInitializer

  def initialize(out_dir="",prob_file="",x0=[],
                 driver=true,nodes=1,cores=4,verbose=false,restart_file=OpenStudio::Path.new)
    super(out_dir,prob_file,x0,driver,nodes,cores,verbose,restart_file)
  end
  
  def in_file_method()
    result = String.new
    result << "method,\n"
    result << "        vector_parameter_study\n"
    result << "          final_point = 1.1 1.3\n"
    result << "          num_steps = 10\n\n"
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
    result << "\n"
    return result
  end  
      
end

VectorParamStudy.new

