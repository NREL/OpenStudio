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
#   Standard unconstrained optimization problem in two variables. 
#   f(x) = (1-x_1)^2 + 100*(x_2 - x_1^2)^2
#
# == Useage
#
# == Example
#
######################################################################

require "#{File.dirname(__FILE__)}/../OptimizationProblem.rb"

class TestFunction10D < OptimizationProblem

  def initialize(x0 = Array.new(10,0.0),verbose = false)
    super(10,0,0,x0,verbose)
  end

  def f_eval(x)
    check_input(x)
    value = (1.0 - x[0])**2 + (1.0 - x[1])**2 + (1.0 - x[2])**2 + (1.0 - x[3])**2 + (1.0 - x[4])**2 + (1.0 - x[5])**2 + (1.0 - x[6])**2 + (1.0 - x[7])**2 + (1.0 - x[8])**2 + (1.0 - x[9])**2
    if @verbose
      puts "In TestFunction10D.rb, x = " + x[0].to_s + ", " + 
                                           x[1].to_s + ", " + 
                                           x[2].to_s + ", " + 
                                           x[3].to_s + ", " + 
                                           x[4].to_s + ", " + 
                                           x[5].to_s + ", " + 
                                           x[6].to_s + ", " + 
                                           x[7].to_s + ", " + 
                                           x[8].to_s + ", " + 
                                           x[9].to_s
      puts "In TestFunction10D.rb, f = " + value.to_s
    end
    return value
  end
  
  def f_to_s
    return "(1 - x_1)^2 + (1 - x_2)^2 + (1 - x_3)^2 + (1 - x_4)^2 + (1 - x_5)^2 + (1 - x_6)^2 + (1 - x_7)^2 + (1 - x_8)^2 + (1 - x_9)^2 + (1 - x_10)^2"
  end
  
end
  
# create an instance so RunDakotaOnTestProblem can find this class
TestFunction10D.new

