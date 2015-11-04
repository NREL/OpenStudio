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

class ParamStudy10D < DakotaInitializer

  def initialize(out_dir="",prob_file="",x0=[],
                 driver=true,nodes=1,cores=4,verbose=false,restart_file=OpenStudio::Path.new)
    super(out_dir,prob_file,x0,driver,nodes,cores,verbose,restart_file)
  end
  
  def in_file_method()
    result = String.new
    result << "method,\n"
    result << "        multidim_parameter_study\n"
    result << "          partitions = 3 3 3 2 2 2 2 2 2 2\n\n"
    return result
  end  
  
  def in_file_variables()
    result = super
    result << "          lower_bounds      -2.0  -2.0  -10  10 -100 -4000 -1 300 1 1\n"
    result << "          upper_bounds       2.0   2.0   10 100 -100 -2000  1 500 2 2\n\n" 
    return result
  end  
    
end

ParamStudy10D.new

