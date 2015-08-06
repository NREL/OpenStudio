module OsLib_HelperMethods
  # populate choice argument from model objects
  def self.populateChoiceArgFromModelObjects(model, modelObject_args_hash, includeBuilding = nil)
    # populate choice argument for constructions that are applied to surfaces in the model
    modelObject_handles = OpenStudio::StringVector.new
    modelObject_display_names = OpenStudio::StringVector.new

    # looping through sorted hash of constructions
    modelObject_args_hash.sort.map do |key, value|
      modelObject_handles << value.handle.to_s
      modelObject_display_names << key
    end

    unless includeBuilding.nil?
      # add building to string vector with space type
      building = model.getBuilding
      modelObject_handles << building.handle.to_s
      modelObject_display_names << includeBuilding
    end

    result = { 'modelObject_handles' => modelObject_handles, 'modelObject_display_names' => modelObject_display_names }
    return result
  end # end of OsLib_HelperMethods.populateChoiceArgFromModelObjects

  # create variables in run from user arguments
  def self.createRunVariables(runner, model, user_arguments, arguments)
    result = {}

    error = false
    # use the built-in error checking
    unless runner.validateUserArguments(arguments, user_arguments)
      error = true
      runner.registerError('Invalid argument values.')
    end

    user_arguments.each do |argument|
      # get argument info
      arg = user_arguments[argument]
      arg_type = arg.print.lines($/)[1]

      # create argument variable
      if arg_type.include? 'Double, Required'
        eval("result[\"#{arg.name}\"] = runner.getDoubleArgumentValue(\"#{arg.name}\", user_arguments)")
      elsif arg_type.include? 'Integer, Required'
        eval("result[\"#{arg.name}\"] = runner.getIntegerArgumentValue(\"#{arg.name}\", user_arguments)")
      elsif arg_type.include? 'String, Required'
        eval("result[\"#{arg.name}\"] = runner.getStringArgumentValue(\"#{arg.name}\", user_arguments)")
      elsif arg_type.include? 'Boolean, Required'
        eval("result[\"#{arg.name}\"] = runner.getBoolArgumentValue(\"#{arg.name}\", user_arguments)")
      elsif arg_type.include? 'Choice, Required'
        eval("result[\"#{arg.name}\"] = runner.getStringArgumentValue(\"#{arg.name}\", user_arguments)")
      else
        puts 'not setup to handle all argument types yet, or any optional arguments'
      end
    end

    if error
      return false
    else
      return result
    end
  end

  # check choice argument made from model objects
  def self.checkChoiceArgFromModelObjects(object, variableName, to_ObjectType, runner, user_arguments)
    apply_to_building = false
    modelObject = nil
    if object.empty?
      handle = runner.getStringArgumentValue(variableName, user_arguments)
      if handle.empty?
        runner.registerError("No #{variableName} was chosen.") # this logic makes this not work on an optional model object argument
      else
        runner.registerError("The selected #{variableName} with handle '#{handle}' was not found in the model. It may have been removed by another measure.")
      end
      return false
    else
      if !eval("object.get.#{to_ObjectType}").empty?
        modelObject = eval("object.get.#{to_ObjectType}").get
      elsif !object.get.to_Building.empty?
        apply_to_building = true
      else
        runner.registerError("Script Error - argument not showing up as #{variableName}.")
        return false
      end
    end  # end of if construction.empty?

    result = { 'modelObject' => modelObject, 'apply_to_building' => apply_to_building }
  end # end of OsLib_HelperMethods.checkChoiceArgFromModelObjects

  # check choice argument made from model objects
  def self.checkOptionalChoiceArgFromModelObjects(object, variableName, to_ObjectType, runner, user_arguments)
    apply_to_building = false
    modelObject = nil
    if object.empty?
      handle = runner.getOptionalStringArgumentValue(variableName, user_arguments)
      if handle.empty?
        # do nothing, this is a valid option
        puts 'hello'
        modelObject = nil
        apply_to_building = false
      else
        runner.registerError("The selected #{variableName} with handle '#{handle}' was not found in the model. It may have been removed by another measure.")
        return false
      end
    else
      if !eval("object.get.#{to_ObjectType}").empty?
        modelObject = eval("object.get.#{to_ObjectType}").get
      elsif !object.get.to_Building.empty?
        apply_to_building = true
      else
        runner.registerError("Script Error - argument not showing up as #{variableName}.")
        return false
      end
    end  # end of if construction.empty?

    result = { 'modelObject' => modelObject, 'apply_to_building' => apply_to_building }
  end # end of OsLib_HelperMethods.checkChoiceArgFromModelObjects

  # check value of double arguments
  def self.checkDoubleAndIntegerArguments(runner, user_arguments, arg_check_hash)
    error = false

    # get hash values
    min = arg_check_hash['min']
    max = arg_check_hash['max']
    min_eq_bool = arg_check_hash['min_eq_bool']
    max_eq_bool = arg_check_hash['max_eq_bool']

    arg_check_hash['arg_array'].each do |argument|
      argument = user_arguments[argument]

      # get arg values
      arg_value = argument.valueDisplayName.to_f   # instead of valueAsDouble so it allows integer arguments as well
      arg_display = argument.displayName

      unless min.nil?
        if min_eq_bool
          if arg_value < min
            runner.registerError("Please enter value greater than or equal to #{min} for #{arg_display}.") # add in argument display name
            error = true
          end
        else
          if arg_value <= min
            runner.registerError("Please enter value greater than #{min} for #{arg_display}.") # add in argument display name
            error = true
          end
        end
      end
      unless max.nil?
        if max_eq_bool
          if arg_value > max
            runner.registerError("Please enter value less than or equal to #{max} for #{arg_display}.") # add in argument display name
            error = true
          end
        else
          if arg_value >= max
            runner.registerError("Please enter value less than #{max} for #{arg_display}.") # add in argument display name
            error = true
          end
        end
      end
    end

    # check for any errors
    if error
      return false
    else
      return true
    end
  end

  # populate choice argument from model objects. areaType should be string like "floorArea" or "exteriorArea"
  # note: it seems like spaceType.floorArea does account for multiplier, so I don't have to call this method unless I have a custom collection of spaces.
  def self.getAreaOfSpacesInArray(model, spaceArray, areaType = 'floorArea')
    # find selected floor spaces, make array and get floor area.
    totalArea = 0
    spaceAreaHash = {}
    spaceArray.each do |space|
      spaceArea = eval("space.#{areaType}*space.multiplier")
      spaceAreaHash[space] = spaceArea
      totalArea += spaceArea
    end

    result = { 'totalArea' => totalArea, 'spaceAreaHash' => spaceAreaHash }
    return result
  end # end of getAreaOfSpacesInArray

  # runs conversion and neat string, and returns value with units in string, optionally before or after the value
  def self.neatConvertWithUnitDisplay(double, fromString, toString, digits, unitBefore = false, unitAfter = true, space = true, parentheses = true)
    # convert units
    doubleConverted = OpenStudio.convert(double, fromString, toString)
    if !doubleConverted.nil?
      doubleConverted = doubleConverted.get
    else
      puts "Couldn't convert values, check string choices passed in. From: #{fromString}, To: #{toString}"
    end

    # get neat version of converted
    neatConverted = OpenStudio.toNeatString(doubleConverted, digits, true)

    # add prefix
    if unitBefore
      if space == true && parentheses == true
        prefix = "(#{toString}) "
      elsif space == true && parentheses == false
        prefix = "(#{toString})"
      elsif space == false && parentheses == true
        prefix = "#{toString} "
      else
        prefix = "#{toString}"
      end
    else
      prefix = ''
    end

    # add suffix
    if unitAfter
      if space == true && parentheses == true
        suffix = " (#{toString})"
      elsif space == true && parentheses == false
        suffix = "(#{toString})"
      elsif space == false && parentheses == true
        suffix = " #{toString}"
      else
        suffix = "#{toString}"
      end
    else
      suffix = ''
    end

    finalString = "#{prefix}#{neatConverted}#{suffix}"

    return finalString
  end # end of getAreaOfSpacesInArray

  # helper that loops through lifecycle costs getting total costs under "Construction" and add to counter if occurs during year 0
  def self.getTotalCostForObjects(objectArray, category = 'Construction', onlyYearFromStartZero = true)
    counter = 0
    objectArray.each do |object|
      object_LCCs = object.lifeCycleCosts
      object_LCCs.each do |object_LCC|
        if object_LCC.category == category
          if onlyYearFromStartZero == false || object_LCC.yearsFromStart == 0
            counter += object_LCC.totalCost
          end
        end
      end
    end

    return counter
  end # end of def get_total_costs_for_objects(objects)

  # helper that loops through lifecycle costs getting total costs under "Construction" and add to counter if occurs during year 0
  def self.getSpaceTypeStandardsInformation(spaceTypeArray)
    # hash of space types
    spaceTypeStandardsInfoHash = {}

    spaceTypeArray.each do |spaceType|
      # get standards building
      if !spaceType.standardsBuildingType.empty?
        standardsBuilding = spaceType.standardsBuildingType.get
      else
        standardsBuilding = nil
      end

      # get standards space type
      if !spaceType.standardsSpaceType.empty?
        standardsSpaceType = spaceType.standardsSpaceType.get
      else
        standardsSpaceType = nil
      end

      # populate hash
      spaceTypeStandardsInfoHash[spaceType] = [standardsBuilding, standardsSpaceType]
    end # end of spaceTypeArray each do

    return spaceTypeStandardsInfoHash
  end # end of def get_total_costs_for_objects(objects)

  # OpenStudio has built in toNeatString method
  # OpenStudio::toNeatString(double,2,true)# double,decimals, show commas

  # OpenStudio has built in helper for unit conversion. That can be done using OpenStudio::convert() as shown below.
  # OpenStudio::convert(double,"from unit string","to unit string").get
end
