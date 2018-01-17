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
#   Validate all EnergyPlus Idf files in a directory.
#
# == Usage
#
#   ruby ValidateIdfs.rb ARGV[0] 
# 
#   ARGV[0] - Path to directory with EnergyPlus Idf files directory.
#   ARGV[1] - Path to create temporary files in during execution
#
# == Examples
#
#   ruby ValidateIdfs.rb 'C:\path\to\energyplus\dir\'
#
######################################################################

require 'openstudio'
require 'fileutils'

Dir.chdir(ARGV[0])

if not File.exists?(ARGV[1])
  Dir.mkdir(ARGV[1])
end

if File.exists?("#{ARGV[1]}/unfiltered")
  FileUtils.rm_rf("#{ARGV[1]}/unfiltered")
end
Dir.mkdir("#{ARGV[1]}/unfiltered")

if File.exists?("#{ARGV[1]}/filtered")
  FileUtils.rm_rf("#{ARGV[1]}/filtered")
end
Dir.mkdir("#{ARGV[1]}/filtered")

Dir.glob("*.idf").each do |file|

  # we don't handle parametric preprocessor
  next if /1ZoneParameterAspect.idf/.match(file)
  
  FileUtils.cp(file, "#{ARGV[1]}/unfiltered/#{File.basename(file)}")
  
  puts "Processing idf file #{file}"
  STDOUT.flush

  # load IdfFile
  idfPath = OpenStudio::Path.new(file)
  oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType)

  # Construct workspace
  workspace = OpenStudio::Workspace.new(oIdfFile.get(),"None".to_StrictnessLevel)
  STDOUT.flush
  
  File.open("#{ARGV[1]}/filtered/#{File.basename(file)}", 'w') do |f|
    f << workspace.to_s
  end
  
  if not workspace.isValid("Draft".to_StrictnessLevel)
    puts "#{file} is not valid at Draft level"
    result = false
    report = workspace.validityReport("Draft".to_StrictnessLevel)
    puts report.to_s
  end

  if not workspace.isValid("Final".to_StrictnessLevel)
    puts "#{file} is not valid at Final level"
    result = false
    report = workspace.validityReport("Final".to_StrictnessLevel)
    puts report.to_s
  end
  
  puts ""
  STDOUT.flush

end
