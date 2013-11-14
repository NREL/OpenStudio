#start the measure
class ReportingMeasure < OpenStudio::Ruleset::ReportingUserScript
  
  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "ReportingMeasure"
  end
  
  #define the arguments that the user will input
  def arguments()
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    #use the built-in error checking 
    if not runner.validateUserArguments(arguments(), user_arguments)
      return false
    end

    # get the last model and sql file
    
    model = runner.lastOpenStudioModel
    if model.empty?
      runner.registerError("Cannot find last model.")
      return false
    end
    model = model.get
    
    sqlFile = runner.lastEnergyPlusSqlFile
    if sqlFile.empty?
      runner.registerError("Cannot find last sql file.")
      return false
    end
    sqlFile = sqlFile.get
    
    # perform queries on the sql file
    # look at eplustbl.htm to see how queries in tabulardatawithstrings correspond
    
    query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' AND ReportForString='Entire Facility' AND TableName='General' AND RowName='Program Version and Build' AND ColumnName='Value'"    
    
    s = sqlFile.execAndReturnFirstString(query)
    if s.empty?
      s = "Unknown"
      runner.registerWarning("Cannot read E+ version")
    else
      s = s.get
      runner.registerInfo("E+ version = #{s}")
    end

    query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' AND ReportForString='Entire Facility' AND TableName='Window-Wall Ratio' AND RowName='Gross Wall Area' AND ColumnName='Total' AND Units='m2'"    
    
    d = sqlFile.execAndReturnFirstDouble(query)
    if s.empty?
      d = 0
      runner.registerWarning("Cannot read Gross Wall Area")
    else
      d = d.get
      runner.registerInfo("Gross Wall Area = #{d}")
    end    
    
    # if we want this report could write out a csv, html, or any other file here
    runner.registerInfo("Writing CSV report 'report.csv'")
    File.open("report.csv", 'w') do |file|
      file << "#{s}, #{d}"
    end
    
    # if we write out OpenStudio attributes then these will be imported into PAT database
    # these can then be accessed using ProjectMeasures
    runner.registerInfo("Writing OpenStudio attributes")
    attributes = OpenStudio::AttributeVector.new
    attributes << OpenStudio::Attribute.new("GrossWallArea", d)
    attributes << OpenStudio::Attribute.new("EnergyPlusVersion", s)
    
    result = OpenStudio::Attribute.new("report", attributes)
    
    result.saveToXml(OpenStudio::Path.new("report.xml"))

    #closing the sql file
    sqlFile.close()
    
    #reporting final condition
    runner.registerFinalCondition("Goodbye.")
    
    return true
 
  end #end the run method

end #end the measure

#this allows the measure to be use by the application
ReportingMeasure.new.registerWithApplication