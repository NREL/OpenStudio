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

require 'openstudio'

def simulateOneWorkspace(workspace,epwFile,outputDirectory,failOnInvalid=true,printValidityReport=true)

  sql_path = OpenStudio::Path.new()

  # check validity of workspace before converting to IdfFile and attempting to simulate
  if failOnInvalid or printValidityReport
    report = workspace.validityReport(OpenStudio::StrictnessLevel.new("Final"))
    if report.numErrors() > 0
      puts "Workspace is not valid at Strictness Final."
      puts report.to_s if printValidityReport 
      return sql_path if failOnInvalid 
    else
      puts "Workspace is valid at Strictness Final."
    end
  end
  
  idfFile = workspace.toIdfFile()
  
  # set up EnergyPlus
  co = OpenStudio::Runmanager::ConfigOptions.new
  co.fastFindEnergyPlus()
  
  OpenStudio::create_directory(outputDirectory)
  
  # write idf file
  file_path = outputDirectory / OpenStudio::Path.new("in.idf") 
  File.open(file_path.to_s, 'w') do |f|
    f << idfFile.to_s
    f.close()
  end
  
  # run simulation
  workflow = OpenStudio::Runmanager::Workflow.new("EnergyPlus")
  workflow.add(co.getTools)
  
  job = workflow.create(outputDirectory, file_path, OpenStudio::Path.new(epwFile))
  
  runManager = OpenStudio::Runmanager::RunManager.new(outputDirectory / OpenStudio::Path.new("runmanager.db"), true)
  runManager.enqueue(job, true)
  print "Running EnergyPlus "
  
  # wait for jobs to complete
  while runManager.workPending()
    sleep 1
    OpenStudio::Application::instance().processEvents()
    print "."
  end
  puts " "
  
  errs = job.errors()
  
  if errs.succeeded 
    puts "EnergyPlus simulation completed successfully. Will launch ResultsViewer."
    sql_path = outputDirectory / OpenStudio::Path.new("EnergyPlus/eplusout.sql")
  else
    puts "EnergyPlus simulation failed."
  end
  
  if errs.errors().size() > 0
    puts "Errors:"
    errs.errors().each do |err_str|
      puts err_str
    end
  end
  
  if errs.warnings().size() > 0
    puts "Warnings:"
    errs.warnings().each do |warn_str|
      puts warn_str
    end
  end
  
  return sql_path

end
