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
  
  
