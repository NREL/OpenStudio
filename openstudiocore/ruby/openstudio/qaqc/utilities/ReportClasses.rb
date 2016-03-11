#This file defines the classes that the reports will be made from.

class Flag
  attr_accessor :type #Xcel EDA Flag, General Modeling Flag, etc
  attr_accessor :value 
  
  def initialize(type,value)
    @type = type
    @value = value
  end
  
  def to_xml
    xml = "<flag>
            <type>#{@type}</type>
            <value>#{@value}</value>
          </flag>"  
    return xml   
  end
  
end    

class Check    
  attr_accessor :category #comfort, equipment sizing, loads, etc
  attr_accessor :name
  attr_accessor :description
  
  def initialize
    @flags = [] #an array of flags
    @category = "TODO categorize all checks"
  end
  
  def add_flag(flag)
    @flags << flag
    return true
  end
  
  def flags
    return @flags
  end  
  
  def to_xml
    xml = "<check>
            <name>#{@name}</name>
            <category>#{@category}</category>
            <description>#{@description}</description>"            
    @flags.each do |flag|
      xml = xml + flag.to_xml
    end
    xml = xml + "</check>"
    return xml
  end

end

class SingleModelQAQCReport
  attr_accessor :model_id #model name

  def initialize
    @checks = [] #an array of checks
  end

  def add_check(check)
    @checks << check
    return true
  end
  
  def checks
    return @checks
  end

  def to_xml
    xml = "<singlemodelqaqcreport>
            <model_id>#{@model_id}</model_id>"              
    @checks.each do |check|
      xml = xml + check.to_xml
    end
    xml = xml + "</singlemodelqaqcreport>"
    return xml
  end
  
end    

class TwoModelComparisonQAQCReport
  attr_accessor :baseline_model_id
  attr_accessor :proposed_model_id
  
  def initialize
    @checks = [] #an array of checks
  end 
  
  def add_check(check)
    @checks << check
    return TRUE
  end
  
  def checks
    return @checks
  end   

  def to_xml
    xml = "<twomodelcomparisonqaqcreport>
            <baseline_model_id>#{@baseline_model_id}</baseline_model_id>
            <proposed_model_id>#{@proposed_model_id}</proposed_model_id>"
    @checks.each do |check|
      xml = xml + check.to_xml
    end
    xml = xml + "</twomodelcomparisonqaqcreport>"
    return xml
  end
  
end

class ModelingQAQCReport
  attr_accessor :id
  
  def initialize
    @single_model_qaqc_reports = []
    @two_model_comparison_qaqc_reports = []
  end 
  
  def add_single_model_qaqc_report(single_model_qaqc_report)
    @single_model_qaqc_reports << single_model_qaqc_report
    return TRUE
  end
  
  def single_model_qaqc_reports
    return @single_model_qaqc_reports
  end   

  def add_two_model_comparison_qaqc_report(two_model_comparison_qaqc_report)
    @two_model_comparison_qaqc_reports << two_model_comparison_qaqc_report
    return TRUE
  end
  
  def two_model_comparison_qaqc_reports
    return @two_model_comparison_qaqc_reports
  end     

  def to_xml
    xml = "<modelingqaqcreport>"
    @single_model_qaqc_reports.each do |report|
      xml = xml + report.to_xml
    end
    @two_model_comparison_qaqc_reports.each do |report|
      xml = xml + report.to_xml
    end
    xml = xml + "</modelingqaqcreport>"
    return xml
  end  
  
end


