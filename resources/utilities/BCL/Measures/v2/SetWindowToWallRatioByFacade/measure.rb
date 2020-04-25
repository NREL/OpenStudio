
class SetWindowToWallRatioByFacade < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Window to Wall Ratio by Facade"
  end

  # return a vector of arguments
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make double argument for wwr
    wwr = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("wwr",true)
    wwr.setDisplayName("Window to Wall Ratio (fraction).")
    wwr.setDefaultValue(0.4)
    args << wwr

    #make double argument for sillHeight
    sillHeight = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("sillHeight",true)
    sillHeight.setDisplayName("Sill Height (in).")
    sillHeight.setDefaultValue(30.0)
    args << sillHeight

    #make choice argument for facade
    choices = OpenStudio::StringVector.new
    choices << "North"
    choices << "East"
    choices << "South"
    choices << "West"
    facade = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("facade", choices,true)
    facade.setDisplayName("Cardinal Direction.")
    facade.setDefaultValue("South")
    args << facade

    return args
  end  #end the arguments method

  #define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    #use the built-in error checking
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    #assign the user inputs to variables
    wwr = runner.getDoubleArgumentValue("wwr",user_arguments)
    sillHeight = runner.getDoubleArgumentValue("sillHeight",user_arguments)
    facade = runner.getStringArgumentValue("facade",user_arguments)

    #check reasonableness of fraction
    if wwr <= 0 or wwr >= 1
      runner.registerError("Window to Wall Ratio must be greater than 0 and less than 1.")
      return false
    end

    #check reasonableness of fraction
    if sillHeight <= 0
      runner.registerError("Sill height must be > 0.")
      return false
    elsif sillHeight > 360
      runner.registerWarning("#{sillHeight} inches seems like an unusually high sill height.")
    elsif sillHeight > 9999
      runner.registerError("#{sillHeight} inches is above the measure limit for sill height.")
      return false
    end

    #setup OpenStudio units that we will need
    unit_sillHeight_ip = OpenStudio::createUnit("ft").get
    unit_sillHeight_si = OpenStudio::createUnit("m").get
    unit_area_ip = OpenStudio::createUnit("ft^2").get
    unit_area_si = OpenStudio::createUnit("m^2").get
    unit_cost_per_area_ip = OpenStudio::createUnit("1/ft^2").get #$/ft^2 does not work
    unit_cost_per_area_si = OpenStudio::createUnit("1/m^2").get

    #define starting units
    sillHeight_ip = OpenStudio::Quantity.new(sillHeight/12, unit_sillHeight_ip)

    #unit conversion
    sillHeight_si = OpenStudio::convert(sillHeight_ip, unit_sillHeight_si).get

    #hold data for initial condition
    starting_gross_ext_wall_area = 0.0 # includes windows and doors
    starting_ext_window_area = 0.0

    #hold data for final condition
    final_gross_ext_wall_area = 0.0 # includes windows and doors
    final_ext_window_area = 0.0

    #flag for not applicable
    exterior_walls = false
    windows_added = false

    #flag to track notifications of zone multipliers
    space_warning_issued = []

    #flag to track warning for new windows without construction
    empty_const_warning = false

    #calculate initial envelope cost as negative value
    envelope_cost = 0
    constructions = model.getConstructions
    constructions.each do |construction|
      const_llcs = construction.lifeCycleCosts
      const_llcs.each do |const_llc|
        if const_llc.category == "Construction"
          envelope_cost += const_llc.totalCost*-1
        end
      end
    end #end of constructions.each do

    #loop through surfaces finding exterior walls with proper orientation
    surfaces = model.getSurfaces
    surfaces.each do |s|

      next if not s.surfaceType == "Wall"
      next if not s.outsideBoundaryCondition == "Outdoors"
      if s.space.empty?
        runner.registerWarning("#{s.name} doesn't have a parent space and won't be included in the measure reporting or modifications.")
        next
      end

      # get the absoluteAzimuth for the surface so we can categorize it
      absoluteAzimuth =  OpenStudio::convert(s.azimuth,"rad","deg").get + s.space.get.directionofRelativeNorth + model.getBuilding.northAxis
      until absoluteAzimuth < 360.0
        absoluteAzimuth = absoluteAzimuth - 360.0
      end

      if facade == "North"
        next if not (absoluteAzimuth >= 315.0 or absoluteAzimuth < 45.0)
      elsif facade == "East"
        next if not (absoluteAzimuth >= 45.0 and absoluteAzimuth < 135.0)
      elsif facade == "South"
        next if not (absoluteAzimuth >= 135.0 and absoluteAzimuth < 225.0)
      elsif facade == "West"
        next if not (absoluteAzimuth >= 225.0 and absoluteAzimuth < 315.0)
      else
        runner.registerError("Unexpected value of facade: " + facade + ".")
        return false
      end
      exterior_walls = true

      #get surface area adjusting for zone multiplier
      space = s.space
      if not space.empty?
        zone = space.get.thermalZone
      end
      if not zone.empty?
        zone_multiplier = zone.get.multiplier
        if zone_multiplier > 1 and not space_warning_issued.include?(space.get.name.to_s)
          runner.registerInfo("Space #{space.get.name.to_s} in thermal zone #{zone.get.name.to_s} has a zone multiplier of #{zone_multiplier}. Adjusting area calculations.")
          space_warning_issued << space.get.name.to_s
        end
      else
        zone_multiplier = 1 #space is not in a thermal zone
        runner.registerWarning("Space #{space.get.name.to_s} is not in a thermal zone and won't be included in in the simulation. Windows will still be altered with an assumed zone multiplier of 1")
      end
      surface_gross_area = s.grossArea * zone_multiplier

      #loop through sub surfaces and add area including multiplier
      ext_window_area = 0
      s.subSurfaces.each do |subSurface|
        ext_window_area = ext_window_area + subSurface.grossArea * subSurface.multiplier * zone_multiplier
        if subSurface.multiplier > 1
          runner.registerInfo("Sub-surface #{subSurface.name.to_s} in space #{space.get.name.to_s} has a sub-surface multiplier of #{subSurface.multiplier}. Adjusting area calculations.")
        end
      end

      starting_gross_ext_wall_area += surface_gross_area
      starting_ext_window_area += ext_window_area

      new_window = s.setWindowToWallRatio(wwr, sillHeight_si.value, true)
      if new_window.empty?
        runner.registerWarning("The requested window to wall ratio for surface '#{s.name}' was too large. Fenestration was not altered for this surface.")
      else
        windows_added = true
        #warn user if resulting window doesn't have a construction, as it will result in failed simulation. In the future may use logic from starting windows to apply construction to new window.
        if new_window.get.construction.empty? and empty_const_warning == false
          runner.registerWarning("one or more resulting windows do not have constructions. This script is intended to be used with models using construction sets versus hard assigned constructions.")
          empty_const_warning = true
        end
      end

    end #end of surfaces.each do

    #report initial condition wwr
    #the initial and final ratios does not currently account for either sub-surface or zone multipliers.
    starting_wwr = sprintf("%.02f",(starting_ext_window_area/starting_gross_ext_wall_area))
    runner.registerInitialCondition("The model's initial window to wall ratio for #{facade} facing exterior walls was #{starting_wwr}.")

    if not exterior_walls
      runner.registerAsNotApplicable("The model has no exterior #{facade.downcase} walls and was not altered")
      return true
    elsif not windows_added
      runner.registerAsNotApplicable("The model has exterior #{facade.downcase} walls, but no windows could be added with the requested window to wall ratio")
      return true
    end

    #data for final condition wwr
    surfaces = model.getSurfaces
    surfaces.each do |s|
      next if not s.surfaceType == "Wall"
      next if not s.outsideBoundaryCondition == "Outdoors"
      if s.space.empty?
        runner.registerWarning("#{s.name} doesn't have a parent space and won't be included in the measure reporting or modifications.")
        next
      end

      # get the absoluteAzimuth for the surface so we can categorize it
      absoluteAzimuth =  OpenStudio::convert(s.azimuth,"rad","deg").get + s.space.get.directionofRelativeNorth + model.getBuilding.northAxis
      until absoluteAzimuth < 360.0
        absoluteAzimuth = absoluteAzimuth - 360.0
      end

      if facade == "North"
        next if not (absoluteAzimuth >= 315.0 or absoluteAzimuth < 45.0)
      elsif facade == "East"
        next if not (absoluteAzimuth >= 45.0 and absoluteAzimuth < 135.0)
      elsif facade == "South"
        next if not (absoluteAzimuth >= 135.0 and absoluteAzimuth < 225.0)
      elsif facade == "West"
        next if not (absoluteAzimuth >= 225.0 and absoluteAzimuth < 315.0)
      else
        runner.registerError("Unexpected value of facade: " + facade + ".")
        return false
      end

      #get surface area adjusting for zone multiplier
      space = s.space
      if not space.empty?
        zone = space.get.thermalZone
      end
      if not zone.empty?
        zone_multiplier = zone.get.multiplier
        if zone_multiplier > 1
        end
      else
        zone_multiplier = 1 #space is not in a thermal zone
      end
      surface_gross_area = s.grossArea * zone_multiplier

      #loop through sub surfaces and add area including multiplier
      ext_window_area = 0
      s.subSurfaces.each do |subSurface| #onlky one and should have multiplier of 1
        ext_window_area = ext_window_area + subSurface.grossArea * subSurface.multiplier * zone_multiplier
      end

      final_gross_ext_wall_area += surface_gross_area
      final_ext_window_area += ext_window_area
    end #end of surfaces.each do

    #short def to make numbers pretty (converts 4125001.25641 to 4,125,001.26 or 4,125,001). The definition be called through this measure
    def neat_numbers(number, roundto = 2) #round to 0 or 2)
      # round to zero or two decimals
      if roundto == 2
        number = sprintf "%.2f", number
      else
        number = number.round
      end
      #regex to add commas
      number.to_s.reverse.gsub(%r{([0-9]{3}(?=([0-9])))}, "\\1,").reverse
    end #end def pretty_numbers

    #get delta in ft^2 for final - starting window area
    increase_window_area_si = OpenStudio::Quantity.new(final_ext_window_area - starting_ext_window_area, unit_area_si)
    increase_window_area_ip = OpenStudio::convert(increase_window_area_si, unit_area_ip).get

    #calculate final envelope cost as positive value
    constructions = model.getConstructions
    constructions.each do |construction|
      const_llcs = construction.lifeCycleCosts
      const_llcs.each do |const_llc|
        if const_llc.category == "Construction"
          envelope_cost += const_llc.totalCost
        end
      end
    end #end of constructions.each do

    #report final condition
    final_wwr = sprintf("%.02f",(final_ext_window_area/final_gross_ext_wall_area))
    runner.registerFinalCondition("The model's final window to wall ratio for #{facade} facing exterior walls is #{final_wwr}. Window area increased by #{neat_numbers(increase_window_area_ip.value,0)} (ft^2). The material and construction costs increased by $#{neat_numbers(envelope_cost,0)}.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be used by the application
SetWindowToWallRatioByFacade.new.registerWithApplication
