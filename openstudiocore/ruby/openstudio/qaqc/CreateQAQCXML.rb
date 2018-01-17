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
#   Create a QAQC report xml file from a simple project
#
# == Usage
#
#  ruby CreateQAQC.rb ARGV[0] ARGV[1]
#
#  ARGV[0] - Path to directory containing .osp
#  ARGV[1] - Path to save the xml
#
# == Examples
#
#   ruby CreateQAQC.rb 'C:\path\to\project\dir\' 'C:\path\to\save\xml\' 
#
######################################################################

require 'openstudio'
require 'openstudio/qaqc/QAQC'

osp_dir = ARGV[0].gsub("\\", "/")
report_save_dir = ARGV[1].gsub("\\", "/")

#check the path to the osp
if not osp_dir or not File.directory?(osp_dir)
  puts "Script requires argument which is path to directory containing .osp"
  exit(false)
end

#check the save path
if not report_save_dir or not File.directory?(report_save_dir)
  puts "Script requires argument which is path to directory to save xml report in"
  exit(false)
end

#make the paths openstudio 
path_to_osp = OpenStudio::Path.new(osp_dir)
report_save_path = OpenStudio::Path.new("#{report_save_dir}qaqc.xml")


#temporary hard-coded paths for testing
# path_to_osp = OpenStudio::Path.new('C:/Users/aparker/Desktop/PAT Project Examples/Tabs 3 and 4/')
# report_save_path = 'C:/Users/aparker/Desktop/PAT Project Examples/Tabs 3 and 4/QAQC.xml'

#load the project
OpenStudio::Application::instance().application()
proj = OpenStudio::AnalysisDriver::SimpleProject::open(path_to_osp)

#fail if the project didn't load correctly
if proj.empty?
  exit(false)
end
  
#debug switch; false to turn off lots of troubleshooting outputs
Debug_Mode = false  
  
#make a new modeling qaqc report
qaqc_report = ModelingQAQCReport.new

#get the uid of the baseline dp
base_uuid = proj.get.baselineDataPoint.uuid
    
#check each model individually for errors     
proj.get.analysis.dataPoints.each do |dp|
  
  #get the optional model; different method for baseline vs. other dps
  if dp.uuid == base_uuid
    o_model = proj.get.seedModel
  else
    o_model = dp.model
  end  
  
  #get the optional sql file
  o_sql = dp.sqlFile

  #perform the check if the model and sql file exist
  if not o_model.empty? and not o_sql.empty?
      
    #get the model
    model = o_model.get
    
    #get the sql file
    sql = o_sql.get
    
    #make a new qaqc report for this model and add it to the overall report
    single_qaqc = SingleModelQAQCReport.new
    single_qaqc.model_id = "#{dp.name.gsub(/\W/,' ')}"
    qaqc_report.add_single_model_qaqc_report(single_qaqc)  
    
    #TODO make the selection of env periods more intelligent
    $env_period = sql.availableEnvPeriods[0]
      
    #EUI for the whole building
    single_qaqc.add_check(eui_check(model,sql))

    #unmet hours
    single_qaqc.add_check(unmet_hrs_check(model,sql))
       
    #energy use for cooling and heating as percentage of total energy
    single_qaqc.add_check(enduse_pcts_check(model,sql))    

    #peak heating and cooling months
    single_qaqc.add_check(peak_heat_cool_mo_check(model,sql))    
    
    #TODO sizing of equipment - requires timeseries data
    #single_qaqc.add_check(equip_sizing_check(model,sql))

    #TODO heating hours and cooling hours(max for all zones) - requires timeseries data
    #single_qaqc.add_check(heat_cool_hrs_check(model,sql))
  
  end

end

#comparison checks between the baseline model and the design alternatives

#get the baseline datapoint, baseline optional model, and baseline optional sql file
base = proj.get.baselineDataPoint
base_o_model = proj.get.seedModel
base_o_sql = base.sqlFile

#skip all comparisons if the baseline model and sql aren't available
if not base_o_model.empty? and not base_o_sql.empty?
  
  #get the model
  base_model = base_o_model.get
  
  #get the sql file
  base_sql = base_o_sql.get

  #perform all the comparisons
  proj.get.analysis.dataPoints.each do |dp|
    
    #don't compare the baseline against itself
    next if base.uuid == dp.uuid
    
    #get the optional model for comparison
    dp_o_model = dp.model
    
    #get the optional sql file for comparison
    dp_o_sql = dp.sqlFile
    
    #skip comparison if the dp model or sql file aren't available
    if not dp_o_model.empty? and not dp_o_sql.empty?
    
      #get the model
      dp_model = dp_o_model.get
      
      #get the sql file
      dp_sql = dp_o_sql.get
    
      #make a new report for this comparison and add it to the overall report
      comparison = TwoModelComparisonQAQCReport.new
      qaqc_report.add_two_model_comparison_qaqc_report(comparison)

      #record uuids of models being compared
      comparison.baseline_model_id = "#{base.name.gsub(/\W/,' ')}"
      comparison.proposed_model_id = "#{dp.name.gsub(/\W/,' ')}"
      
      #heating & cooling primary fuel swap
      comparison.add_check(heat_cool_fuel_swap_check(base_model,base_sql,dp_model,dp_sql))
    
    end
  
  end
  
end  
  
#save the results to file
File.open("#{report_save_path}", 'w') do |file|  
  file.puts qaqc_report.to_xml
end



  
  






