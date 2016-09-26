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

require "#{File.dirname(__FILE__)}/../DakotaInitializer.rb"

require 'pathname'

class PSOpt < DakotaInitializer

  def initialize(out_dir="",prob_file="",x0=[],
                 driver=true,nodes=1,cores=4,verbose=false,restart_file=OpenStudio::Path.new)
    super(out_dir,prob_file,x0,driver,nodes,cores,verbose,restart_file)
  end
  
  def in_file_method()
    result = String.new
    result << "method,\n"
    result << "        coliny_pattern_search\n"
    result << "          max_iterations = 1000\n"
    result << "          max_function_evaluations = 2000\n"
    result << "          solution_accuracy = 1.0e-4\n"
    result << "          initial_delta = 0.5\n"
    result << "          threshold_delta = 1.0e-4\n"
    result << "          exploratory_moves basic_pattern\n"
    result << "          contraction_factor = 0.75\n\n"
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
      result << (-2.0).to_s.rjust(8)
    end
    result << "\n          upper_bounds" 
    for i in 1..@problem.nx
      result << (2.0).to_s.rjust(8)
    end
    result << "\n\n"
    return result
  end  
  
  def in_file_responses()  
    result = String.new
    result << "responses,\n"
    result << "        num_objective_functions = 1\n"
    result << "        num_nonlinear_inequality_constraints = " + @problem.ng.to_s + "\n" if @problem.ng > 0
    result << "        num_nonlinear_equality_constraints = " + @problem.nh.to_s + "\n" if @problem.nh > 0    
    result << "        no_gradients\n"
    result << "        no_hessians\n\n"
    return result  
  end  
      
end

PSOpt.new

