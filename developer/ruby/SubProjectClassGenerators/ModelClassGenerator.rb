########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

######################################################################
# == Synopsis
#
#   Generator for model::ModelObject classes.
#
######################################################################

require File.dirname(__FILE__) + '/SubProjectClassGenerator.rb'

class ModelObjectField

  def initialize(iddObject, iddField)
    # puts "  " + iddField.name

    @iddObject = iddObject
    @iddField = iddField

    # discover boolean choice strings if they exist
    @booleanChoiceTrue = nil
    @booleanChoiceFalse = nil
    self.isBooleanChoice?
  end

  def name
    return @iddField.name
  end

  def iddField
    return @iddField
  end

  def isHandle?
    return (@iddField.properties.type == "HandleType".to_IddFieldType)
  end

  def isName?
    return @iddField.isNameField
  end

  def isRequired?
    return @iddField.properties.required
  end

  def isInteger?
    return (@iddField.properties.type == "Integer".to_IddFieldType)
  end

  def isReal?
    return (@iddField.properties.type == "Real".to_IddFieldType)
  end

  def isAlpha?
    return (@iddField.properties.type == "Alpha".to_IddFieldType)
  end

  def isExternalList?
    return (@iddField.properties.type == "ExternalListType".to_IddFieldType)
  end

  def isObjectList?
    return (@iddField.properties.type == "ObjectListType".to_IddFieldType)
  end

  def isNode?
    return (@iddField.properties.type == "node".to_IddFieldType)
  end

  def isSchedule?
    result = false
    @iddField.properties.objectLists.each { |objectList|
      if objectList == "ScheduleNames"
        result = true
      end
    }
    return result
  end

  def isChoice?
    return (@iddField.properties.type == "ChoiceType".to_IddFieldType)
  end

  def choices
    return @iddField.keys
  end

  def isBooleanChoice?
    result = false
    if isChoice?
      keys = self.choices
      if (keys.size == 2)
        if((choices[0].name.upcase == "YES") || (choices[1].name.upcase == "NO"))
          result = true
          @booleanChoiceTrue = keys[0].name
          @booleanChoiceFalse = keys[1].name
        elsif((choices[0].name.upcase == "NO") || (choices[1].name.upcase == "YES"))
          result = true
          @booleanChoiceTrue = keys[1].name
          @booleanChoiceFalse = keys[0].name
        elsif((choices[0].name.upcase == "TRUE") || (choices[1].name.upcase == "FALSE"))
          result = true
          @booleanChoiceTrue = keys[0].name
          @booleanChoiceFalse = keys[1].name
        elsif((choices[0].name.upcase == "FALSE") || (choices[1].name.upcase == "TRUE"))
          result = true
          @booleanChoiceTrue = keys[1].name
          @booleanChoiceFalse = keys[0].name
        end
      end
    end
    return result
  end

  def booleanChoiceTrue
    return @booleanChoiceTrue
  end

  def booleanChoiceFalse
    return @booleanChoiceFalse
  end

  def hasReset?
    return (not @iddField.properties.required)
  end

  def hasDefault?
    return (not @iddField.properties.stringDefault.empty?)
  end

  def defaultValue
    return @iddField.properties.stringDefault.get
  end

  def canAutosize?
    return @iddField.properties.autosizable
  end

  def canAutocalculate?
    return @iddField.properties.autocalculatable
  end

  def objectListClassName
    result = "UnknownType"
    @iddField.properties.objectLists.each { |objectList|
      temp = objectList.gsub(/Names/,"")
      if not temp.empty?
        result = temp
      end
      break
    }
    return result
  end

  def candidateObjectListClassNames
    result = String.new
    sep = String.new
    @iddField.properties.objectLists.each { |objectList|
      result = result + sep + objectList.gsub(/Names/,"")
      sep = ", "
    }
    return result
  end

  def scheduleDisplayName
    result = String.new
    if isSchedule?
      result = @iddField.name.gsub(/[sS]chedule [nN]ame/,"")
      result = result.strip
      if result.size == 0
        result = OpenStudio::iddObjectNameToIdfObjectName(@iddObject.name)
      end
    end
    return result
  end

  def fieldEnum
    return OpenStudio::convertIddName(@iddObject.name) + "Fields::" + OpenStudio::convertIddName(@iddField.name)
  end

  def getterName
    result = OpenStudio::toLowerCamelCase(OpenStudio::convertIddName(@iddField.name))
    if (isObjectList?)
      result = result.gsub(/Name/,"")
    end
    return result
  end

  def setterArgumentName
    result = getterName
    if (isObjectList?)
      result = OpenStudio::toLowerCamelCase(objectListClassName)
    end
    return result
  end

  def unitsGetterName
    return getterName + "Units"
  end

  def optionalGetter?
    result = nil
    if isInteger? or isReal?
      if (iddField.properties.required || (not iddField.properties.numericDefault.empty?)) && (not (iddField.properties.autocalculatable || iddField.properties.autosizable))
        result = false
      else
        result = true
      end
    elsif isChoice? or isAlpha? or isExternalList? or isObjectList?
      if (iddField.properties.required || (not iddField.properties.stringDefault.empty?))
        result = false
      else
        result = true
      end
    end
    return result
  end

  # Compute return type
  # @param forceOptional [Bool, or nil]: whether to force optional return type.
  # Useful for the reverse translator
  # if nil, will check optionalGetter
  # @return result [String]: the getter return type
  def getterReturnType(forceOptional=nil)
    result = nil
    if isInteger?
      result = "int"
    elsif isReal?
      result = "double"
    elsif isChoice? or isAlpha? or isExternalList?
      if isBooleanChoice?
        result = "bool"
      else
        result = "std::string"
      end
    elsif isObjectList?
      result = objectListClassName
    elsif isNode?
      result = "Node"
    end

    if result
      if forceOptional.nil?
        if optionalGetter?
          result = "boost::optional<" + result + ">"
        end
      elsif forceOptional
        result = "boost::optional<" + result + ">"
      end
    end

    raise "Unexpected field type " + @iddField.properties.type.valueName + "." if not result

    return result
  end

  def getterAccessor
    result = nil
    if isInteger?
      result = "getInt"
    elsif isReal?
      result = "getDouble"
    elsif isChoice? or isAlpha? or isExternalList?
      result = "getString"
    elsif isObjectList?
      result = "getObject<ModelObject>().getModelObjectTarget<" + objectListClassName + ">"
    elsif isNode?
      result = "getObject<ModelObject>().getModelObjectTarget<Node>"
    end
    return result
  end

  def isDefaultName
    result = nil
    if hasDefault?
      result = "is" + OpenStudio::toUpperCamelCase(getterName) + "Defaulted"
    end
    return result
  end

  def isAutosizeName
    result = nil
    if canAutosize?
      result = "is" + OpenStudio::toUpperCamelCase(getterName) + "Autosized"
    end
    return result
  end

  def isAutocalculateName
    result = nil
    if canAutocalculate?
      result = "is" + OpenStudio::toUpperCamelCase(getterName) + "Autocalculated"
    end
    return result
  end

  def setterName
    return "set" + OpenStudio::toUpperCamelCase(getterName)
  end

  def publicClassSetterType
    result = nil
    if isInteger?
      result = "int"
    elsif isReal?
      result = "double"
    elsif isChoice? or isAlpha? or isExternalList?
      if isBooleanChoice?
        result = "bool"
      else
        result = "const std::string&"
      end
    elsif isObjectList?
      if isSchedule?
        result = objectListClassName + "&"
      else
        result = "const " + objectListClassName + "&"
      end
    end
    return result
  end

  def setCanFail?
    result = true
    if isInteger? or isReal?
      result = (not iddField.properties.maxBoundValue.empty?) || (not iddField.properties.minBoundValue.empty?)
    elsif isChoice?
      if isBooleanChoice?
        result = false
      else
        result = true
      end
    elsif isAlpha?
      result = false
    elsif isExternalList? or isObjectList?
      result = true
    end
    return result
  end

  def setterAccessor
    result = nil
    if isInteger?
      result = "setInt"
    elsif isReal?
      result = "setDouble"
    elsif isBooleanChoice?
      result = "setBooleanFieldValue"
    elsif isChoice? or isAlpha? or isExternalList?
      result = "setString"
    elsif isObjectList?
      result = "setPointer"
    end
    return result
  end

  def resetName
    result = nil
    if hasReset?
      result = "reset" + OpenStudio::toUpperCamelCase(getterName)
    end
    return result
  end

  def autocalculateName
    result = nil
    if canAutocalculate?
      result = "autocalculate" + OpenStudio::toUpperCamelCase(getterName)
    end
    return result
  end

  def autosizeName
    result = nil
    if canAutosize?
      result = "autosize" + OpenStudio::toUpperCamelCase(getterName)
    end
    return result
  end

  def autosizedName
    result = nil
    if canAutosize?
      result = "autosized" + OpenStudio::toUpperCamelCase(getterName)
    end
    return result
  end

  def sqlUnitString
    result = ''
    if @iddField.properties.units
      result = @iddField.properties.units.to_s
    end
    return result
  end

end

class ModelClassGenerator < SubProjectClassGenerator
  attr_accessor :iddObjectType, :idfObject, :iddObject, :isOS
  attr_accessor :requiredObjectListFields, :requiredDataFields
  attr_accessor :hasRealFields, :hasScheduleFields, :autosizedGetterNames, :autosizeSetterNames

  def initialize(className, baseClassName, pImpl, qobject, iddObjectType)
    super(className, baseClassName, pImpl, qobject)
    @iddObjectType = iddObjectType
    @hasRealFields = false
    @hasScheduleFields = false

    @derivesHVACComponent = [
      'AirToAirComponent',
      'HVACComponent',
      'Mixer',
      'SetpointManager',
      'Splitter',
      'StraightComponent',
      'WaterToAirComponent',
      'WaterToWaterComponent',
      'ZoneHVACComponent'].include?(baseClassName)

    if not @iddObjectType.empty?
      require 'openstudio'

      raise "ModelObjects follow the pImpl idiom. Add -p to the command line." if not @pImpl

      @iddObjectType = @iddObjectType.to_IddObjectType
      @idfObject = OpenStudio::IdfObject.new(@iddObjectType)
      @iddObject = @idfObject.iddObject

      # Find required non-extensible fields, split out by object-list and data
      @nonextensibleFields = []
      @iddObject.nonextensibleFields.each { |iddField|
        modelObjectField = ModelObjectField.new(@iddObject, iddField)
        @nonextensibleFields << modelObjectField
        @hasRealFields = true if modelObjectField.isReal?
        @hasScheduleFields = true if modelObjectField.isSchedule?
      }

      @objectListClassNames = @nonextensibleFields.select{|field| field.isObjectList?}.map(&:objectListClassName).uniq

      # Determine if the object has any autosizable fields
      @autosizedGetterNames = []
      @autosizeSetterNames = []
      @iddObject.nonextensibleFields.each do |iddField|
        modelObjectField = ModelObjectField.new(@iddObject, iddField)
        if modelObjectField.canAutosize?
          @autosizedGetterNames << modelObjectField.autosizedName
          @autosizeSetterNames << modelObjectField.autosizeName
        end
      end

    end
  end

  def implHppIncludes()
    result = String.new
    return result
  end

  def cppIncludes()
    result = String.new

    if @idfObject

      # include object list field classes
      # include IddFactory.hxx if there is a non-boolean choice field
      includeIddFactory = false
      @nonextensibleFields.each { |field|
        if field.isChoice? and not field.isBooleanChoice?
          includeIddFactory = true
        end
      }

      preamble = "// TODO: Check the following class names against object getters and setters.\n"
      @objectListClassNames.each { |className|
        result << preamble
        result << "#include \"" << className << ".hpp\"\n"
        result << "#include \"" << className << "_Impl.hpp\"\n"
        preamble = ""
      }

      if @hasScheduleFields
        result << "#include \"ScheduleTypeLimits.hpp\"\n"
        result << "#include \"ScheduleTypeRegistry.hpp\"\n"
      end

      result << "\n" if preamble == ""

      result << "#include \"../utilities/core/Assert.hpp\"\n"
      if @derivesHVACComponent
        result << "#include \"../utilities/data/DataEnums.hpp\"\n"
      end
      result << "\n"

      if includeIddFactory
        result << "#include <utilities/idd/IddFactory.hxx>\n"
      end

      result << "#include <utilities/idd/IddEnums.hxx>\n"
      result << "#include <utilities/idd/" << @iddObjectType.valueName << "_FieldEnums.hxx>\n\n"

      # if @hasRealFields
      #   result << "#include \"../utilities/units/Unit.hpp\"\n\n"
      # end

    else
      result = super
    end

    return result
  end

  def hppOSForwardDeclarations
    result = String.new
    if @hasRealFields
      result << "\n"
    end
    return result
  end

  def implHppOSForwardDeclarations
    result = String.new
    return result
  end

  def hppSubProjectForwardDeclarations
    result = String.new
    if @idfObject
      preamble = "  // TODO: Check the following class names against object getters and setters.\n"
      @objectListClassNames.each { |className|
        result << preamble
        result << "  class " << className << ";\n"
        preamble = ""
      }
      result << "\n" if preamble == ""
    end
    return result
  end

  def implHppSubProjectForwardDeclarations
    result = String.new
    if @idfObject
      result = hppSubProjectForwardDeclarations
    end
    return result
  end

  def hppPreClass()
    result = String.new

    if @idfObject
      result << "  /** " << className << " is a " << baseClassName
      result << " that wraps the OpenStudio IDD object '" << iddObjectType.valueDescription
      result << "'. */\n"
    else
      result = super
    end

    return result
  end

  def hppConstructors()
    result = String.new

    if @idfObject

      if (not @iddObject.properties.unique)
        result << "    explicit " << className << "(const Model& model);\n\n"
      end

    else
      result = super
    end

    return result
  end

  def implHppConstructors()
    result = String.new

    if @idfObject

      implConstructorStart = String.new
      implConstructorStart << "      " << @className << "_Impl("

      result << implConstructorStart << "const IdfObject& idfObject,\n"
      result << " " * implConstructorStart.size << "Model_Impl* model,\n"
      result << " " * implConstructorStart.size << "bool keepHandle);\n"
      result << "\n"
      result << implConstructorStart << "const openstudio::detail::WorkspaceObject_Impl& other,\n"
      result << " " * implConstructorStart.size << "Model_Impl* model,\n"
      result << " " * implConstructorStart.size << "bool keepHandle);\n"
      result << "\n"
      result << implConstructorStart << "const " << @className << "_Impl& other,\n"
      result << " " * implConstructorStart.size << "Model_Impl* model,\n"
      result << " " * implConstructorStart.size << "bool keepHandle);\n\n"

    else
      result = super
    end

    return result
  end

  def cppConstructors()
    result = String.new

    if @idfObject


      implConstructorStart = String.new
      implConstructorStart << "    " << @className << "_Impl::" << @className << "_Impl("

      result << implConstructorStart << "const IdfObject& idfObject,\n"
      result << " " * implConstructorStart.size << "Model_Impl* model, bool keepHandle)" << "\n"
      result << "      : " << @baseClassName << "_Impl(idfObject, model, keepHandle) {\n"
      result << "      OS_ASSERT(idfObject.iddObject().type() == " << @className << "::iddObjectType());" << "\n"
      result << "    }" << "\n"
      result << "\n"
      result << implConstructorStart << "const openstudio::detail::WorkspaceObject_Impl& other," << "\n"
      result << " " * implConstructorStart.size << "Model_Impl* model, bool keepHandle)" << "\n"
      result << "      : " << @baseClassName << "_Impl(other, model, keepHandle) {\n"
      result << "      OS_ASSERT(other.iddObject().type() == " << @className << "::iddObjectType());" << "\n"
      result << "    }" << "\n"
      result << "\n"
      result << implConstructorStart << "const " << @className << "_Impl& other," << "\n"
      result << " " * implConstructorStart.size << "Model_Impl* model, bool keepHandle)" << "\n"
      result << "    : " << @baseClassName << "_Impl(other, model, keepHandle) {}\n"
      result << "\n"

    else
      result = super
    end

    return result
  end

  def cppPublicClassConstructors()
    result = String.new

    if @idfObject

      if (not iddObject.properties.unique)

        result << "  " << @className << "::" << @className << "(const Model& model)\n"
        result << "    : " << @baseClassName << "(" << @className << "::iddObjectType(), model) {\n"
        result << "    OS_ASSERT(getImpl<detail::" << @className << "_Impl>());\n\n"

        result << "    // TODO: consider adding (overloaded or not) explicit ctors taking required objects as argument\n\n"

        result << "    // TODO: Appropriately handle the following required object-list fields.\n"
        @nonextensibleFields.each { |field|
          if field.isRequired? and field.isObjectList?
            result << "    //     " << field.fieldEnum << "\n"
          end
        }

        preamble = "    bool ok = true;\n"
        @nonextensibleFields.each { |field|
          if field.isRequired? and not field.isName? and not field.isHandle?
            if field.setCanFail?
              result << preamble
              result << "    // ok = " << field.setterName << "();\n"
              result << "    OS_ASSERT(ok);\n"
              preamble = String.new
            else
              result << "    // " << field.setterName << "();\n"
            end
          end
        }

        result << "  }\n\n"

      end

    else
      result = super
    end

    return result
  end

  def hppPublicMethods()
    result = String.new

    if @idfObject

      result << "    static IddObjectType iddObjectType();\n\n"


      # Static methods that return choice field valid values
      @nonextensibleFields.each { |field|
        if field.isChoice? and not field.isBooleanChoice?
          result << "    static std::vector<std::string> " << field.getterName << "Values();\n\n"
        end
      }

      result << "    /** @name Getters */\n"
      result << "    //@{" << "\n\n"

      # Non-extensible field getters
      @nonextensibleFields.each { |field|

        # name is in ModelObject
        next if field.isHandle?
        next if field.isName?

        if field.isObjectList?
          result << "    // TODO: Check return type. From object lists, some candidates are: " << field.candidateObjectListClassNames << ".\n"
        end

        result << "    " << field.getterReturnType << " " << field.getterName << "() const;\n\n"

        if field.hasDefault?
          result << "    bool " << field.isDefaultName << "() const;\n\n"
        end

        if field.canAutosize?
          result << "    bool " << field.isAutosizeName << "() const;\n\n"
          # Get the autosized value from the sql file
          result << "    boost::optional <double> " << field.autosizedName << "();\n\n"
        end

        if field.canAutocalculate?
          result << "    bool " << field.isAutocalculateName << "() const;\n\n"
        end
      }

      # Extensible field getters
      if (@iddObject.properties.extensible)
        result << "    // TODO: Handle this object's extensible fields." << "\n"
        result << "  \n"
      end

      result << "    //@}\n"

      result << "    /** @name Setters */\n"
      result << "    //@{\n\n"


      # Non-extensible field setters
      @nonextensibleFields.each { |field|

        # name is in ModelObject
        next if field.isHandle?
        next if field.isName?

        if field.isObjectList?
          result << "    // TODO: Check argument type. From object lists, some candidates are: " << field.candidateObjectListClassNames << ".\n"
          if field.isSchedule?
            result << "    // Note Schedules are passed by reference, not const reference.\n"
          end
        end

        if field.setCanFail?
          result << "    bool " << field.setterName << "(" << field.publicClassSetterType << " " << field.setterArgumentName << ");\n\n"
        else
          # Note: JM 2018-10-17: even if the setter can't fail, we return bool
          result << "    bool " << field.setterName << "(" << field.publicClassSetterType << " " << field.setterArgumentName << ");\n\n"
        end

        if field.hasReset?
          result << "    void " << field.resetName << "();\n\n"
        end

        if field.canAutosize?
          result << "    void " << field.autosizeName << "();\n\n"
        end

        if field.canAutocalculate?
          result << "    void " << field.autocalculateName << "();\n\n"
        end

      }

      # Extensible field setters

      if (@iddObject.properties.extensible)
        result << "    // TODO: Handle this object's extensible fields.\n\n"
      end

      result << "    //@}\n"

      result << "    /** @name Other */\n"
      result << "    //@{\n\n"
      result << "    //@}\n"

    else
      result = super
    end

    return result
  end

  def implHppPublicMethods()
    result = String.new

    if @idfObject

      result << "      /** @name Virtual Methods */\n"


      result << "      //@{\n\n"

      result << "      virtual const std::vector<std::string>& outputVariableNames() const override;\n\n"

      result << "      virtual IddObjectType iddObjectType() const override;\n\n"

      if @hasScheduleFields
        result << "      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;\n\n"
      end

      if @derivesHVACComponent
        result << "      virtual ComponentType componentType() const override;\n"
        result << "      virtual std::vector<FuelType> coolingFuelTypes() const override;\n"
        result << "      virtual std::vector<FuelType> heatingFuelTypes() const override;\n"
        result << "      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;\n\n"
      end

      result << "      //@}\n"
      result << "      /** @name Getters */\n"
      result << "      //@{\n\n"

      # Non-extensible field getters
      @nonextensibleFields.each { |field|

        # name is in ModelObject
        next if field.isHandle?
        next if field.isName?

        if field.isObjectList?
          result << "      // TODO: Check return type. From object lists, some candidates are: " << field.candidateObjectListClassNames << ".\n"
        end

        result << "      " << field.getterReturnType << " " << field.getterName << "() const;\n\n"

        if field.hasDefault?
          result << "      bool " << field.isDefaultName << "() const;\n\n"
        end

        if field.canAutosize?
          result << "      bool " << field.isAutosizeName << "() const;\n\n"
          # Get the autosized value from the sql file
          result << "      boost::optional <double> " << field.autosizedName << "();\n\n"
        end

        if field.canAutocalculate?
          result << "      bool " << field.isAutocalculateName << "() const;\n\n"
        end

      }

      # Extensible field getters
      if (@iddObject.properties.extensible)
        result << "      // TODO: Handle this object's extensible fields." << "\n"
        result << "  \n"
      end

      result << "      //@}" << "\n"

      result << "      /** @name Setters */\n"
      result << "      //@{\n\n"


      # Non-extensible field setters
      @nonextensibleFields.each { |field|

        # name is in ModelObject
        next if field.isHandle?
        next if field.isName?

        if field.isObjectList?
          result << "      // TODO: Check argument type. From object lists, some candidates are: " << field.candidateObjectListClassNames << ".\n"
          if field.isSchedule?
            result << "    // Note Schedules are passed by reference, not const reference.\n"
          end
        end

        if field.setCanFail?
          result << "      bool " << field.setterName << "(" << field.publicClassSetterType << " " << field.setterArgumentName << ");\n\n"
        else
          # Note: JM 2018-10-17: even if the setter can't fail, we return bool
          result << "      bool " << field.setterName << "(" << field.publicClassSetterType << " " << field.setterArgumentName << ");\n\n"
        end

        if field.hasReset?
          result << "      void " << field.resetName << "();\n\n"
        end

        if field.canAutosize?
          result << "      void " << field.autosizeName << "();\n\n"
        end

        if field.canAutocalculate?
          result << "      void " << field.autocalculateName << "();\n\n"
        end
      }

      # If there are any autosizeable fields, need to add bulk autosize
      # and applySizingValues methods.  These methods are assumed
      # to be overrides from the method declared in HVACComponent.
      if @autosizedGetterNames.size > 0
        result << "      virtual void autosize() override;\n\n"
        result << "      virtual void applySizingValues() override;\n\n"
      end

      # Extensible field setters

      if (@iddObject.properties.extensible)
        result << "      // TODO: Handle this object's extensible fields.\n\n"
      end

      result << "      //@}\n"

      result << "      /** @name Other */\n"
      result << "      //@{\n\n"
      result << "      //@}\n"

    else
      result = super
    end

    return result
  end

  def cppPublicMethods()
    result = String.new

    if @idfObject

      result << "    const std::vector<std::string>& " << @className << "_Impl::outputVariableNames() const {\n"
      result << "      static std::vector<std::string> result;\n"
      result << "      if (result.empty()){\n"
      result << "      }\n"
      result << "      return result;\n"
      result << "    }\n\n"

      result << "    IddObjectType " << @className << "_Impl::iddObjectType() const {\n"
      result << "      return " << @className << "::iddObjectType();\n"
      result << "    }\n\n"

      if @hasScheduleFields
        result << "    std::vector<ScheduleTypeKey> " << @className << "_Impl::getScheduleTypeKeys(const Schedule& schedule) const {\n"
        result << "      // TODO: Check schedule display names.\n"
        result << "      std::vector<ScheduleTypeKey> result;\n"
        result << "      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());\n"
        @nonextensibleFields.each { |field|
          next if not field.isSchedule?
          result << "      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(),\n"
          result << "                    " << field.fieldEnum << ")\n"
          result << "          != fieldIndices.cend()) {\n"
          result << "        result.emplace_back(\"" << @className << "\", \"" << field.scheduleDisplayName << "\");\n"
          result << "      }\n"
        }
        result << "      return result;\n"
        result << "    }\n\n"
      end

      if @derivesHVACComponent
        result << "    ComponentType " << @className << "_Impl::componentType() const {\n"
        result << "      // TODO\n"
        result << "      return ComponentType::None;\n"
        result << "    }\n\n"

        result << "    std::vector<FuelType> " << @className << "_Impl::coolingFuelTypes() const {\n"
        result << "      // TODO\n"
        result << "      return {};\n"
        result << "    }\n\n"

        result << "    std::vector<FuelType> " << @className << "_Impl::heatingFuelTypes() const {\n"
        result << "      // TODO\n"
        result << "      return {};\n"
        result << "    }\n\n"

        result << "    std::vector<AppGFuelType> " << @className << "_Impl::appGHeatingFuelTypes() const {\n\n"
        result << "      // TODO\n"
        result << "      return {};\n"
        result << "    }\n\n"
      end

      # Non-extensible field getters
      @nonextensibleFields.each { |field|

        # name is in ModelObject
        next if field.isHandle?
        next if field.isName?

        result << "    " << field.getterReturnType << " " << @className << "_Impl::" << field.getterName << "() const {\n"

        if field.isBooleanChoice?

          if field.optionalGetter?
            result << "      boost::optional<bool> result;\n"
            result << "      boost::optional<std::string> value = " << field.getterAccessor << "(" << field.fieldEnum << ", true);\n"
            result << "      if (value){\n"
            result << "         result = openstudio::istringEqual(value.get(), \"" << field.booleanChoiceTrue << "\");\n"
            result << "      }\n"
            result << "      return result;"
          else
            result << "      return getBooleanFieldValue(" << field.fieldEnum << ");\n"
          end

        elsif field.optionalGetter?

          result << "      return " << field.getterAccessor << "(" << field.fieldEnum
          if not field.isObjectList?
            result << "  , true"
          end
          result << "  );\n"

        else

          if field.isObjectList?

            result << "      boost::optional<" << field.getterReturnType << "> value = optional" <<
                      OpenStudio::toUpperCamelCase(field.getterName) << "();\n"
            result << "      if (!value) {\n"
            result << "        LOG_AND_THROW(briefDescription() << \" does not have an " <<
                      OpenStudio::iddObjectNameToIdfObjectName(OpenStudio::toUpperCamelCase(field.getterName)) <<
                      " attached.\");\n"
            result << "      }\n"

          else

            result << "      boost::optional<" << field.getterReturnType << "> value = " <<
                      field.getterAccessor << "(" << field.fieldEnum << ", true);\n"
            result << "      OS_ASSERT(value);\n"

          end

          result << "      return value.get();\n"

        end

        result << "    }\n\n"

        if field.hasDefault?
          result << "    bool " << @className << "_Impl::" << field.isDefaultName << "() const {\n"
          result << "      return isEmpty(" << field.fieldEnum << ");\n"
          result << "    }\n\n"
        end

        if field.canAutosize?
          result << "    bool " << @className << "_Impl::" << field.isAutosizeName << "() const {\n"
          result << "      bool result = false;\n"
          result << "      boost::optional<std::string> value = getString(" << field.fieldEnum << ", true);\n"
          result << "      if (value) {\n"
          result << "        result = openstudio::istringEqual(value.get(), \"autosize\");\n"
          result << "      }\n"
          result << "      return result;\n"
          result << "    }\n\n"

          # Get the autosized value from the sql file
          result << "    boost::optional <double> " << @className << "_Impl::" << field.autosizedName << "() {\n"
          result << "      return getAutosizedValue(\"TODO_CHECK_SQL #{field.name}\", \"#{field.sqlUnitString}\");\n"
          result << "    }\n\n"

        end

        if field.canAutocalculate?
          result << "    bool " << @className << "_Impl::" << field.isAutocalculateName << "() const {\n"
          result << "      bool result = false;\n"
          result << "      boost::optional<std::string> value = getString(" << field.fieldEnum << ", true);\n"
          result << "      if (value) {\n"
          result << "        result = openstudio::istringEqual(value.get(), \"autocalculate\");\n"
          result << "      }\n"
          result << "      return result;\n"
          result << "    }\n\n"
        end
      }

      # Non-extensible field setters
      @nonextensibleFields.each { |field|

        # name is in ModelObject
        next if field.isHandle?
        next if field.isName?

        next if not field.getterReturnType

        if field.setCanFail?
          result << "    bool "
        else
          # Note: JM 2018-10-17: even if the setter can't fail, we return bool
          result << "    bool "
        end

        result << @className << "_Impl::" << field.setterName << "(" << field.publicClassSetterType << " " << field.setterArgumentName << ") {\n"

        if field.isBooleanChoice?

          if /optional/.match(field.publicClassSetterType)
            result << "      bool result = false;\n"
            result << "      if (" << field.setterArgumentName << ") {\n"
            result << "        if (" << field.setterArgumentName << ".get()) {\n"
            result << "          result = " << field.setterAccessor << "(" << field.fieldEnum << ", \"" << field.booleanChoiceTrue << "\");\n"
            result << "        } else {\n"
            result << "          result = " << field.setterAccessor << "(" << field.fieldEnum << ", \"" << field.booleanChoiceFalse << "\");\n"
            result << "        }\n"
            result << "      }\n"
          else
            result << "      const bool result = setBooleanFieldValue(" << field.fieldEnum << ", " << field.setterArgumentName << ");\n"
          end

        elsif /optional/.match(field.publicClassSetterType)

          #raise "Schedule setters should not take optionals." if field.isSchedule?

          result << "      bool result(false);\n"
          result << "      if (" << field.setterArgumentName << ") {\n"
          result << "        result = " << field.setterAccessor << "(" << field.fieldEnum << ", " << field.setterArgumentName << ".get()"
          if field.isObjectList?
            result << ".handle()"
          end
          result << ");\n"
          result << "      }\n"
          if field.hasReset?
            result << "      else {\n"
            result << "        " << field.resetName << "();\n"
            result << "        result = true;\n"
            result << "      }\n"
          end

        else

          if field.isSchedule?
            if field.setterName == "setSchedule"
              result << "      const bool result = ModelObject_Impl::setSchedule(" << field.fieldEnum << ",\n"
              result << "                                                  \"" << @className << "\",\n"
              result << "                                                  \"" << field.scheduleDisplayName << "\",\n"
              result << "                                                  " << field.setterArgumentName << ");\n"
            else
              result << "      const bool result = setSchedule(" << field.fieldEnum << ",\n"
              result << "                                \"" << @className << "\",\n"
              result << "                                \"" << field.scheduleDisplayName << "\",\n"
              result << "                                " << field.setterArgumentName << ");\n"
            end
          else
            result << "      const bool result = " << field.setterAccessor << "(" << field.fieldEnum << ", " << field.setterArgumentName
            if field.isObjectList?
              result << ".handle()"
            end
            result << ");\n"
          end

        end

        if field.setCanFail?
          result << "      return result;\n"
        else
          result << "      OS_ASSERT(result);\n"
          result << "      return result;\n"
        end

        result << "    }\n\n"

        if field.hasReset?
          result << "    void " << @className << "_Impl::" << field.resetName << "() {\n"
          result << "      const bool result = setString(" << field.fieldEnum << ", \"\");\n"
          result << "      OS_ASSERT(result);\n"
          result << "    }\n\n"
        end

        if field.canAutosize?
          result << "    void " << @className << "_Impl::" << field.autosizeName << "() {\n"
          result << "      const bool result = setString(" << field.fieldEnum << ", \"autosize\");\n"
          result << "      OS_ASSERT(result);\n"
          result << "    }\n\n"
        end

        if field.canAutocalculate?
          result << "    void " << @className << "_Impl::" << field.autocalculateName << "() {\n"
          result << "      const bool result = setString(" << field.fieldEnum << ", \"autocalculate\");\n"
          result << "      OS_ASSERT(result);\n"
          result << "    }\n\n"
        end

      }

      # If there are any autosizeable fields, need to add bulk autosize
      # and applySizingValues methods
      if @autosizedGetterNames.size > 0
        # autosize()
        result << "    void " << @className << "_Impl::autosize() {\n"
        @autosizeSetterNames.each do |name|
          result << "      #{name}();\n"
        end
        result << "    }\n\n"

        # applySizingValues()
        result << "    void " << @className << "_Impl::applySizingValues() {\n"
        @autosizedGetterNames.each do |name|
          setter_name = name.gsub('autosized','set')
          result << "      if (boost::optional<double> val_ = #{name}()) {\n"
          result << "        #{setter_name}(*val_));\n"
          result << "      }\n\n"
        end
        result << "    }\n\n"

      end

    else
      result = super
    end

    return result
  end

  def cppPublicClassPublicMethods()
    result = String.new

    if @idfObject

      result << "  IddObjectType " << @className << "::iddObjectType() {\n"
      result << "    return {IddObjectType::" << @iddObjectType.valueName << "};\n"
      result << "  }\n\n"

      # Static methods that return choice field valid values
      @nonextensibleFields.each { |field|
        if field.isChoice? and not field.isBooleanChoice?
          result << "  std::vector<std::string> " << @className << "::" << field.getterName << "Values() {\n"
          result << "    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),\n"
          result << "                        " << field.fieldEnum << ");\n"
          result << "  }\n\n"
        end
      }

      # Non-extensible field getters
      @nonextensibleFields.each { |field|

        next if field.isHandle?
        next if field.isName?

        next if not field.getterReturnType

        result << "  " << field.getterReturnType << " " << @className << "::" << field.getterName << "() const {\n"
        result << "    return getImpl<detail::" << @className << "_Impl>()->" << field.getterName << "();\n"
        result << "  }\n\n"

        if field.hasDefault?
          result << "  bool " << @className << "::" << field.isDefaultName << "() const {\n"
          result << "    return getImpl<detail::" << @className << "_Impl>()->" << field.isDefaultName << "();\n"
          result << "  }\n\n"
        end

        if field.canAutosize?
          result << "  bool " << @className << "::" << field.isAutosizeName << "() const {\n"
          result << "    return getImpl<detail::" << @className << "_Impl>()->" << field.isAutosizeName << "();\n"
          result << "  }\n\n"

          # Get the autosized value from the sql file
          result << "  boost::optional <double> " << @className << "::" << field.autosizedName << "() {\n"
          result << "    return getImpl<detail::" << @className << "_Impl>()->#{field.autosizedName}();\n"
          result << "  }\n\n"
        end

        if field.canAutocalculate?
          result << "  bool " << @className << "::" << field.isAutocalculateName << "() const {\n"
          result << "    return getImpl<detail::" << @className << "_Impl>()->" << field.isAutocalculateName << "();\n"
          result << "  }\n\n"
        end

      }

      # Non-extensible field setters
      @nonextensibleFields.each { |field|

        next if field.isHandle?
        next if field.isName?

        next if not field.getterReturnType

        result << "  bool " << @className << "::" << field.setterName << "(" << field.publicClassSetterType << " " << field.setterArgumentName << ") {\n"
        result << "    return getImpl<detail::" << @className << "_Impl>()->" << field.setterName << "(" << field.setterArgumentName << ");\n"
        result << "  }\n\n"

        if field.hasReset?
          result << "  void " << @className << "::" << field.resetName << "() {\n"
          result << "    getImpl<detail::" << @className << "_Impl>()->" << field.resetName << "();\n"
          result << "  }\n\n"
        end

        if field.canAutosize?
          result << "  void " << @className << "::" << field.autosizeName << "() {\n"
          result << "    getImpl<detail::" << @className << "_Impl>()->" << field.autosizeName << "();\n"
          result << "  }\n\n"
        end

        if field.canAutocalculate?
          result << "  void " << @className << "::" << field.autocalculateName << "() {\n"
          result << "    getImpl<detail::" << @className << "_Impl>()->" << field.autocalculateName << "();\n"
          result << "  }\n\n"
        end
      }

    else
      result = super
    end

    return result
  end

  def hppProtectedFriends()
    result = String.new

    if @idfObject
      result << "    friend class Model;\n"
      result << "    friend class IdfObject;\n"
      result << "    friend class openstudio::detail::IdfObject_Impl;\n"
    end

    return result
  end

  def hppProtectedMethods()
    result = String.new

    if @idfObject

      if (@iddObject.properties.unique)
        result << "    explicit " << @className << "(Model& model);\n\n"
      end

    else
      result = super
    end

    return result
  end

  def implHppProtectedMethods()
    result = String.new

    if @idfObject

    else
      result = super
    end

    return result
  end

  def cppProtectedMethods()
    result = String.new

    if @idfObject

    else
      result = super
    end

    return result
  end

  def cppPublicClassProtectedMethods()
    result = String.new

    if @idfObject

      if (iddObject.properties.unique)
        result << "  " << @className << "::" << @className << "(Model& model)\n"
        result << "    : " << @baseClassName << "(" << @className << "::iddObjectType(), model) {}\n"
      end

    else
      result = super
    end

    return result
  end

  def implHppPrivateMethods()
    result = String.new

    if @idfObject

      # Optional getters for required objects
      any = false
      @nonextensibleFields.each { |field|
        if field.isObjectList? and (not field.optionalGetter?)
          if not any
            result << "  \n"
            result << "      // TODO: Check the return types of these methods.\n"
            result << "      // Optional getters for use by methods like children() so can remove() if the constructor fails.\n"
            result << "      // There are other ways for the public versions of these getters to fail--perhaps all required\n"
            result << "      // objects should be returned as boost::optionals\n"
            any = true
          end

          result << "      boost::optional<" << field.getterReturnType << "> optional" << OpenStudio::toUpperCamelCase(field.getterName) << "() const;\n"

        end
      }

    end

    return result
  end

  def cppPrivateMethods()
    result = String.new

    if @idfObject

      # Optional getters for required objects
      @nonextensibleFields.each { |field|
        if field.isObjectList? and (not field.optionalGetter?)
          result << "    boost::optional<" << field.getterReturnType << "> " << @className << "_Impl::optional" << OpenStudio::toUpperCamelCase(field.getterName) << "() const {\n"
          result << "      return " << field.getterAccessor << "(" << field.fieldEnum << ");\n"
          result << "    }\n\n"
        end
      }

      return result

    end
  end


  def gtestIncludes()
    result = String.new

    result << "#include \"ModelFixture.hpp\"\n\n"
    result << "#include \"../" << @className << ".hpp\"\n"
    result << "#include \"../" << @className << "_Impl.hpp\"\n\n"

    preamble = "// TODO: Check the following class names against object getters and setters.\n"

    # Check for ObjectList fields, to see which we need to include
    @objectListClassNames.each { |className|
      result << preamble
      result << "#include \"../" << className << ".hpp\"\n"
      result << "#include \"../" << className << "_Impl.hpp\"\n\n"
      preamble = ""
    }

    result << "using namespace openstudio;\n"
    result << "using namespace openstudio::model;\n\n"

    return result;
  end

  def gtestGetterSetters

    result = String.new

    instanceName = OpenStudio::toLowerCamelCase(@className)

    result << "TEST_F(ModelFixture, " << className << "_GettersSetters) {\n"
    result << "  Model m;\n"
    result << "  // TODO: Check regular Ctor arguments\n"
    result << "  " << className << " #{instanceName}(m);\n"
    result << "  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)\n"
    result << "  // " << className << " #{instanceName} = m.getUniqueModelObject<" << className << ">();\n\n"

    @nonextensibleFields.each_with_index { |field, i|
      next if field.isHandle?

      if field.isName?
        result << "  #{instanceName}.setName(\"My #{className}\");\n"

      elsif field.isObjectList? or field.isNode?

        # Comment
        result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << (field.isNode? ? " Node": " Object") << "\n"


        result << "  " << field.getterReturnType << " #{field.getterName}(m);\n"
        result << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(#{field.getterName}));\n"

        if field.optionalGetter?

          result << "  ASSERT_TRUE(#{instanceName}." << field.getterName << "());\n"
          result << "  EXPECT_EQ(#{field.getterName}, #{instanceName}." << field.getterName << "().get());\n"

        else

          result << "  EXPECT_EQ(#{field.getterName}, #{instanceName}." << field.getterName << "());\n"
        end

      else
        prefix = ""
        need_closing = false


        if field.isBooleanChoice?
          result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " Boolean\n"
          if field.hasDefault?
            result << "  // Default value from IDD\n"
            result << "  EXPECT_TRUE(#{instanceName}." << field.isDefaultName << "());\n"
            if field.defaultValue == 'Yes'
              result << "  EXPECT_TRUE(#{instanceName}." << field.getterName << "());\n";
            else
              result << "  EXPECT_FALSE(#{instanceName}." << field.getterName << "());\n";
            end
          end

          result << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(true));\n";
          if field.optionalGetter?
            result << "  EXPECT_TRUE(#{instanceName}." << field.getterName << "().get());\n"
            result << "  EXPECT_FALSE(#{instanceName}." << field.isDefaultName << "());\n"
          else
            result << "  EXPECT_TRUE(#{instanceName}." << field.getterName << "());\n"
          end
          result << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(false));\n";
          if field.optionalGetter?
            result << "  EXPECT_FALSE(#{instanceName}." << field.getterName << "().get());\n"
          else
            result << "  EXPECT_FALSE(#{instanceName}." << field.getterName << "());\n"
          end

        else

          # Note, assignment isn't used any#{instanceName}re since I no longer use high level
          # variables
          isNumber = false;

          if field.isInteger?
            # assignment = "_i"
            cat = "Integer"
            isNumber = true
          elsif field.isReal?
            # assignment = "_d"
            cat = "Double"
            isNumber = true
          elsif field.isChoice? or field.isAlpha? or field.isExternalList?
            # assignment = "_s"
            cat = "String"
          else
            # Not handled...
            # assignment = "#{field.getterReturnType(true)} #{field.getterName}"
            cat = "Unsure of Category... TODO: Check!"
          end

          result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " " << cat << "\n"

          if isNumber
            min_bound = field.iddField.properties.minBoundValue
            max_bound = field.iddField.properties.maxBoundValue
            # The whole shenanigans with `i` here is to avoid setting every
            # field to the same numeric value, which wouldn't catch mistakes
            # such as setting the wrong field (due to copy paste for eg)
            if field.isInteger?
              offset = 1
            else
              offset = 0.1
            end

            if (min_bound.is_initialized && max_bound.is_initialized)
              max = max_bound.get
              min = min_bound.get
              # Break it up in 2 + i segments, take the position of the start
              # of the last segment
              seg_len = (max - min) / (i+2)
              good_val = (max - seg_len)
              bad_val = min_bound.get - 10
            elsif (min_bound.is_initialized)
              good_val = (min_bound.get + offset * (i + 1))
              bad_val = min_bound.get - 10
            elsif (max_bound.is_initialized)
              good_val = (max_bound.get - offset * (i + i))
              bad_val = max_bound.get + 10
            else
              good_val = offset * (i + 1)
              bad_val = nil
            end
            if field.isInteger?
              good_val = good_val.to_i
              if !bad_val.nil?
                bad_val = bad_val.to_i
              end
            else
              good_val = good_val.to_f.round(3)
              if !bad_val.nil?
                bad_val = bad_val.to_f.round(3)
              end
            end
          elsif field.isChoice?
            good_val = "\"#{field.choices[0].name}\""
            bad_val = "\"BADENUM\""
          end

          if field.hasDefault?
            result << "  // Default value from IDD\n"
            result << "  EXPECT_TRUE(#{instanceName}." << field.isDefaultName << "());\n"
            if isNumber
              if field.optionalGetter?
                # Not sure if it's worth checking if optionalGetter?... if it
                # has a default, the getter shouldn't be optional really
                result << "  EXPECT_EQ(#{field.defaultValue}, #{instanceName}." << field.getterName << "()"
                result << (field.optionalGetter? ? ".get()" : "") << ");\n"
              end
            else
              result << "  EXPECT_EQ(\"#{field.defaultValue}\", #{instanceName}." << field.getterName << "()"
              result << (field.optionalGetter? ? ".get()" : "") << ");\n"
            end
          end

          if field.canAutosize?
            result << "  // Autosize\n"
            result << "  #{instanceName}." << field.autosizeName << "();\n"
            result << "  EXPECT_TRUE(#{instanceName}." << field.isAutosizeName << "());\n"

            prefix = "  "
            closing = "  EXPECT_FALSE(#{instanceName}.#{field.isAutosizeName}());\n"

            need_closing = true
          elsif field.canAutocalculate?
            result << "  // Autocalculate\n"
            result << "  #{instanceName}." << field.autocalculateName << "();\n"
            result << "  EXPECT_TRUE(#{instanceName}." << field.isAutocalculateName << "());\n"

            closing = "  EXPECT_FALSE(#{instanceName}.#{field.isAutocalculateName}());\n"
            need_closing = true
          end

          if field.hasDefault? or field.canAutosize? or field.canAutocalculate?
            result << "  // Set\n"
          end

          result << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(#{good_val}));\n";

          if field.optionalGetter?
            result << "  ASSERT_TRUE(#{instanceName}." << field.getterName << "());\n"
            result << "  EXPECT_EQ(#{good_val}, #{instanceName}." << field.getterName << "().get());\n"
          else
            result << "  EXPECT_EQ(#{good_val}, #{instanceName}." << field.getterName << "());\n"
          end

          if field.hasDefault?
            result << "  EXPECT_FALSE(#{instanceName}." << field.isDefaultName << "());\n"
          end

          if !bad_val.nil?
            result << "  // Bad Value\n";
            result << "  EXPECT_FALSE(#{instanceName}." << field.setterName << "(#{bad_val}));\n";

            if field.optionalGetter?
              result << "  ASSERT_TRUE(#{instanceName}." << field.getterName << "());\n"
              result << "  EXPECT_EQ(#{good_val}, #{instanceName}." << field.getterName << "().get());\n"
            else
              result << "  EXPECT_EQ(#{good_val}, #{instanceName}." << field.getterName << "());\n"
            end
          end

          if field.hasDefault?
            result << "  // Reset\n";
            result << "  #{instanceName}." << field.resetName << "();\n"
            result << "  EXPECT_TRUE(#{instanceName}." << field.isDefaultName << "());\n"
          end
        end

        if need_closing
          result << closing
        end

      end

      result << "\n"
    }

    result << "}\n"

    if @derivesHVACComponent
      result << "TEST_F(ModelFixture, " << className << "_HeatCoolFuelTypes) {\n"
      result << "  Model m;\n"
      result << "  // TODO: Check regular Ctor arguments\n"
      result << "  " << className << " #{instanceName}(m);\n"
      result << "  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)\n"
      result << "  // " << className << " #{instanceName} = m.getUniqueModelObject<" << className << ">();\n\n"

      result << "  EXPECT_EQ(ComponentType(ComponentType::Both), #{instanceName}.componentType());\n"
      result << "  testFuelTypeEquality({FuelType::Electricity}, #{instanceName}.coolingFuelTypes());\n"
      result << "  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, #{instanceName}.heatingFuelTypes());\n"
      result << "  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, #{instanceName}.appGHeatingFuelTypes());\n"
      result << "}\n"
    end

    return result
  end

  def aux()
    result = String.new
    if @hasScheduleFields
      result << "// TODO: Copy-paste this starter text into ScheduleTypeRegistry.cpp, in the ScheduleTypeRegistrySingleton\n"
      result << "// constructor. Read the EnergyPlus Input-Output reference and fill out the ScheduleTypeLimits information\n"
      result << "// accordingly.\n"
      @nonextensibleFields.each { |field|
        next if not field.isSchedule?
        if field.scheduleDisplayName.downcase.include?("availability")
          result << "    {\"" << @className << "\", \"" << field.scheduleDisplayName << "\", \"" << field.getterName << "\", false, \"Availability\", 0.0, 1.0},\n"
        else
          result << "    {\"" << @className << "\", \"" << field.scheduleDisplayName << "\", \"" << field.getterName << "\",,,,},\n"
        end
      }
    end
    return result
  end

end
