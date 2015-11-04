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

require 'pathname'

class DakotaInterface

  attr_accessor :problem, :in_path, :out_path

  def initialize(problem_path,in_path,out_path)
    @problem = instantiate_problem(problem_path)
    @in_path = Pathname.new(in_path)
    @out_path = Pathname.new(out_path)
  end
  
  def instantiate_problem(problem_path)
    require problem_path.to_s
    
    ObjectSpace.each_object(OptimizationProblem) { |obj|
      return obj
    }
    
    raise prob_path.to_s + " does not appear to instantiate an instance of a class derived from OptimizationProblem."
  end  
  
  def eval()
    # parse temp/params.in
    x = parse()
    f_val = problem.f_eval(x)
    puts "In DakotaInterface.rb, f = " + f_val.to_s
    g_vals = []
    if (problem.ng > 0)
      g_vals = problem.g_eval(x)
    end
    h_vals = []
    if (problem.nh > 0)
      h_vals = problem.h_eval(x)
    end
    write(f_val,g_vals,h_vals)
  end
  
  # parse params.in file from in_path
  def parse()
    x = []    
    
    numRegex = /\s(-?\d+\.?\d*(e(\+|-)\d+)?)/
    tagRegex = / ([a-zA-Z]\w+)/
    xRegex = /\s*([a-z]{2,3}v_\d+)\s*/
    
    IO.foreach(@in_path.to_s) { |ln|
    
      # puts ln
    
      number = ln.slice(numRegex,1).to_f
      tag = ln.slice(tagRegex,1)
      
      if (number != nil) and (tag != nil)
      
        puts tag
      
        if xRegex.match(tag)
          x.push(number)
        end
        
      end
    
    }
    
    if x.size() != @problem.nx
      raise "Unsuccessful parsing of " + in_path.to_s + ". Extracted x vector has size " + 
            x.size().to_s + ". Was expecting size " + @problem.nx.to_s + "."
    end
    return x
    
  end
  
  # write results.out file to out_path
  def write(f_val,g_vals,h_vals)
    outFile = File.new(@out_path,'w')
    
    fmtStr = "%23.15e"
    
    outFile.puts(format(fmtStr,f_val) + " f")
    for i in 1..g_vals.size()
      outFile.puts(format(fmtStr,g_vals[i-1]) + " g" + i.to_s)
    end
    for i in 1..h_vals.size()
      outFile.puts(format(fmtStr,h_vals[i-1]) + " h" + i.to_s)
    end
    
    outFile.close  
  end
  
end
