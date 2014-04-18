######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'

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
  ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
  ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s) 
  idd_path = OpenStudio::Path.new(ep_hash[:energyplus_idd].to_s)
  
  OpenStudio::create_directory(outputDirectory)
  
  # write idf file
  file_path = outputDirectory / OpenStudio::Path.new("in.idf") 
  File.open(file_path.to_s, 'w') do |f|
    f << idfFile.to_s
    f.close()
  end
  
  # run simulation
  job = OpenStudio::Runmanager::JobFactory::createEnergyPlusJob(
            OpenStudio::Runmanager::ToolInfo.new(ep_path),
            idd_path,
            file_path,
            OpenStudio::Path.new(epwFile),
            outputDirectory)
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
