########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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
