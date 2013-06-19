
class SetWindowToWallRatioByFacade < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Window to Wall Ratio by Facade"
  end
  
  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    wwr = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("wwr")
    wwr.setDisplayName("Window to Wall Ratio (fraction)")
    result << wwr
    
    sillHeight = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("sillHeight")
    sillHeight.setDisplayName("Sill Height (m)")
    result << sillHeight
    
    choices = OpenStudio::StringVector.new
    choices << "North"
    choices << "East"
    choices << "South"
    choices << "West"
    facade = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("facade", choices)
    facade.setDisplayName("Facade")
    result << facade
    
    return result
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
    
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
  
    wwr = runner.getDoubleArgumentValue("wwr",user_arguments)
    sillHeight = runner.getDoubleArgumentValue("sillHeight",user_arguments)
    facade = runner.getStringArgumentValue("facade",user_arguments)

    if wwr < 0 or wwr >= 1
      runner.registerError("Window to Wall Ratio must be greater than or equal to 0 and less than 1.")
      return false
    end

    if sillHeight <= 0
      runner.registerError("Sill height must be > 0.")
      return false
    end
   
    model.getSurfaces.each do |s|

      next if not s.surfaceType == "Wall"
      next if not s.outsideBoundaryCondition == "Outdoors"
      
      azimuth = OpenStudio::Quantity.new(s.azimuth,OpenStudio::createSIAngle)
      azimuth = OpenStudio::convert(azimuth,OpenStudio::createIPAngle).get.value
      
      if facade == "North"
        next if not (azimuth >= 315.0 or azimuth < 45.0)
      elsif facade == "East"
        next if not (azimuth >= 45.0 and azimuth < 135.0)
      elsif facade == "South"
        next if not (azimuth >= 135.0 and azimuth < 225.0)
      elsif facade == "West"
        next if not (azimuth >= 225.0 and azimuth < 315.0)
      else
        runner.registerError("Unexpected value of facade: " + facade + ".")
        return false
      end

      new_window = s.setWindowToWallRatio(wwr, sillHeight, true)
    end
       
    return true
  end

end

SetWindowToWallRatioByFacade.new.registerWithApplication
