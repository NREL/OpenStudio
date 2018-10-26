# Author: Julien Marrec, EffiBEM
# 2018-10-17

require 'openstudio'

require_relative 'ModelClassGenerator.rb'
require_relative 'FileHeader.rb'


class TranslatorGenerator
  attr_accessor :iddObjectType, :idfObject, :iddObject, :isOS
  attr_accessor :requiredObjectListFields, :requiredDataFields
  attr_accessor :hasRealFields, :hasScheduleFields, :autosizedGetterNames

  # Initialize the class
  #
  # @param EPIddName [String] The E+ IDD class, eg 'WaterHeater:Mixed'
  # @return [] returns true if successful, false if not
  def initialize(epIddName)
    @iddObjectType = epIddName.to_IddObjectType
    @idfObject = OpenStudio::IdfObject.new(@iddObjectType)
    @iddObject = @idfObject.iddObject
    @className = @iddObjectType.valueName.gsub(/^OS/,'').gsub(':', '').gsub('_','')

    # Find required non-extensible fields, split out by object-list and data
    # And Determine if the object has any autosizable fields
    @autosizedGetterNames = []
    @nonextensibleFields = []

    @hasRealFields = false
    @hasScheduleFields = false
    @hasObjectFields = false

    @iddObject.nonextensibleFields.each { |iddField|
      modelObjectField = ModelObjectField.new(@iddObject, iddField)
      @nonextensibleFields << modelObjectField
      @hasRealFields = true if modelObjectField.isReal?
      @hasScheduleFields = true if modelObjectField.isSchedule?
      if modelObjectField.canAutosize?
        @autosizedGetterNames << modelObjectField.autosizedName
      end
      @hasObjectFields = true if modelObjectField.isObjectList?
    }
  end


  # Checks the objectList fields to see what we need to include
  # Apply to both FT and RT
  # @return [String] returns the include statements
  def objectListIncludes()

    result = String.new

    preamble = "// TODO: Check the following class names against object getters and setters.\n"

    classDones = []
    # Check for ObjectList fields, to see which we need to include
    @nonextensibleFields.each do |field|
      className = nil
      if field.isObjectList?
        className = field.objectListClassName
      elsif field.isNode?
        className = "Node"
      end

      if not className.nil?
        # puts "#{field.name}: #{className}"
        if not classDones.include?(className)
          classDones << className
          result << preamble
          result << "#include \"../../model/" << className << ".hpp\"\n"
          result << "#include \"../../model/" << className << "_Impl.hpp\"\n\n"
          preamble = ""
        end
      end

    end

    return result

  end

  # Set up the include statements for the ReverseTranslator
  #
  # @param None
  # @return [String] returns the include statements
  def reverseTranslatorIncludes()
    result = String.new

    result << "#include \"../ReverseTranslator.hpp\"\n\n"
    result << "#include \"../../model/" << @className << ".hpp\"\n\n"

    result << objectListIncludes()

    result << "#include <utilities/idd/" << @iddObjectType.valueName << "_FieldEnums.hxx>\n"
    result << "#include <utilities/idd/IddEnums.hxx>\n"

    result << "\n"
    result << "using namespace openstudio::model;\n\n"
    result << "namespace openstudio {\n\n"
    result << "namespace energyplus {\n\n"

    return result
  end


  # Set up the include statements for the ForwardTranslator
  #
  # @param None
  # @return [String] returns the include statements
  def forwardTranslatorIncludes()
    result = String.new

    result << "#include \"../ForwardTranslator.hpp\"\n"
    result << "#include \"../../model/Model.hpp\"\n\n"
    result << "#include \"../../model/" << @className << ".hpp\"\n\n"

    result << objectListIncludes()

    result << "#include <utilities/idd/" << @iddObjectType.valueName << "_FieldEnums.hxx>\n"
    result << "// #include \"../../utilities/idd/IddEnums.hpp\"\n"
    result << "#include <utilities/idd/IddEnums.hxx>\n"

    result << "\n"
    result << "using namespace openstudio::model;\n\n"
    result << "namespace openstudio {\n\n"
    result << "namespace energyplus {\n\n"

    return result
  end

  # Closes the namespaces for the ReverseTranslator
  #
  # @param None
  # @return [String] returns the footer
  def footer()
    result = String.new
    result << "\n" << "} // end namespace energyplus"
    result << "\n"
    result << "\n" << "} // end namespace openstudio"
    return result
  end

  # Setup the actual reverse translation method
  #
  # @param None
  # @return [String] returns the translateXXX method
  def generateReverseTranslateFunction()
    result = String.new

    # Method name
    result << "boost::optional<ModelObject> ReverseTranslator::translate" << @className << "( const WorkspaceObject & workspaceObject )\n"
    result << "{\n"

    # High level variables
    result << "  boost::optional<ModelObject> result;\n"
    if @hasObjectFields
      result << "  boost::optional<WorkspaceObject> _wo;\n"
      result << "  boost::optional<ModelObject> _mo;\n"
    end
    # No longer used, will use unique variable throughout
    #result << "  boost::optional<double> _d;\n"
    #result << "  boost::optional<int> _i;\n"
    #result << "  boost::optional<std::string> _s;\n"
    #result << "\n"

    # Instantiate the model object
    result << "\n"
    result << "  // Instantiate an object of the class to store the values,\n"
    result << "  // but we don't return it until we know it's ok\n"
    result << "  // TODO: check constructor, it might need other objects\n"
    result << "  openstudio::model::" << @className << " modelObject( m_model );\n\n"


    result << "  // TODO: Note JM 2018-10-17\n"
    result << "  // You are responsible for implementing any additional logic based on choice fields, etc.\n"
    result << "  // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way\n\n"


    # Ok, we're ready to start dealing with each field!

    @nonextensibleFields.each do |field|

      if field.isName?
        result << "  // " << field.name << "\n"
        result << "  if (boost::optional<std::string> _name = workspaceObject.name()) {\n"
        result << "    modelObject.setName(_name.get());\n"
        result << "  }\n\n"

      elsif field.isObjectList? or field.isNode?
        # Comment
        result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << (field.isNode? ? " Node": " Object") << "\n"
        result << "  if ( (_wo = workspaceObject.getTarget(#{field.fieldEnum})) ) {\n"
        result << "    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {\n"
        result << "      // TODO: check return types\n"
        result << "      if (" << field.getterReturnType(true) << " _"
        result << field.getterName << " = _mo->optionalCast<" << field.getterReturnType(false) << ">()) {\n"
        result << "        modelObject." << field.setterName << "(_" << field.getterName << ".get());\n"
        result << "      } else {\n"
        result << "        LOG(Warn, workspaceObject.briefDescription() << \" has a wrong type for '" << field.name << "'\");\n"
        result << "      }\n"
        if field.isRequired?
          result << "    } else {\n"
          result << '      LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot reverse translate required object '" << field.name << "'\");" << "\n"
          result << "      return result;\n"
          result << "    }\n"
          result << "  } else {\n"
          result << '    LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot find required object '" << field.name << "'\");" << "\n"
          result << "    return result;\n"
          result << "  }\n"
        else
          result << "    }\n"
          result << "  }\n"
        end

      elsif field.isBooleanChoice?
        # We have to check is "Yes", in which case we use true
        result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " Boolean\n"
        result << "  if (" << field.getterReturnType(true) << " _" << field.getterName << " = "<< field.getterAccessor << "(" << field.fieldEnum << ", true)) {\n"
        result << "    if(istringEqual(\"Yes\", _" << field.getterName << ".get())) {\n"
        result << "      modelObject." << field.setterName << "(true);\n"
        result << "    } else {\n"
        result << "      modelObject." << field.setterName << "(false);\n"
        result << "    }\n"
        if field.isRequired?
          result << "  } else {\n"
          result << '    LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot find required property '" << field.name << "'\");" << "\n"
          result << "    return result;\n"
        end
        result << "  }\n\n"
      else

        # Note, assignment isn't used anymore since I no longer use high level
        # variables
        if field.isInteger?
          # assignment = "_i"
          cat = "Integer"
        elsif field.isReal?
          # assignment = "_d"
          cat = "Double"
        elsif field.isChoice? or field.isAlpha? or field.isExternalList?
          # assignment = "_s"
          cat = "String"
        else
          # Not handled...
          # assignment = "#{field.getterReturnType(true)} #{field.getterName}"
          cat = "Unsure of Category... TODO: Check!"
        end

        result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " " << cat << "\n"

        #result << "  if (" << assignment << " = "<< field.getterAccessor << "(" << field.fieldEnum << ")) {\n"
        #result << "    modelObject." << field.setterName << "(" << assignment << ");\n"
        # Instead, I don't use global optional vars
        result << "  if (" << field.getterReturnType(true) << " _" << field.getterName << " = "<< field.getterAccessor << "(" << field.fieldEnum << ")) {\n"
        result << "    modelObject." << field.setterName << "(_" << field.getterName << ".get());\n"


        if field.isRequired?
          result << " } else {\n"
          result << '   LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot find required property '" << field.name << "'\");" << "\n"
          result << "    return result;\n"
        end
        result << "  }\n\n"
      end

    end # End loop on nonextensibleFields

    # Assign result and return
    result << "  result = modelObject;\n"
    result << "  return result;\n"
    result << "}\n"


    # Close method
    result << "} // End of translate function\n"


    return result
  end


# Setup the actual reverse translation method
  #
  # @param None
  # @return [String] returns the translateXXX method
  def generateForwardTranslateFunction()
    result = String.new

    # Method name
    result << "boost::optional<ModelObject> ForwardTranslator::translate" << @className << "( const " << @className << "& modelObject )\n"
    result << "{\n"

    # High level variables
    result << "  boost::optional<ModelObject> result;\n"
    if @hasObjectFields
      result << "  boost::optional<WorkspaceObject> _wo;\n"
      result << "  boost::optional<ModelObject> _mo;\n"
    end
    # No longer used, will use unique variable throughout
    #result << "  boost::optional<double> _d;\n"
    #result << "  boost::optional<int> _i;\n"
    #result << "  boost::optional<std::string> _s;\n"
    #result << "\n"

    # Instantiate the model object
    result << "\n"
    result << "  // Instantiate an IdfObject of the class to store the values,\n"
    result << "  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::" << @iddObjectType.valueName << ", modelObject);\n\n"

    result << "  // TODO: Note JM 2018-10-17\n"
    result << "  // You are responsible for implementing any additional logic based on choice fields, etc.\n"
    result << "  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way\n\n"


    # Ok, we're ready to start dealing with each field!

    @nonextensibleFields.each do |field|

      if field.isName?
        result << "  // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.\n"
        result << "  // But in some cases, you'll want to handle failure without pushing to the map\n"
        result << "  // Name\n"
        result << "  if (boost::optional<std::string> moName = modelObject.name()) {\n"
        result << "    idfObject.setName(*moName);\n"
        result << "  }\n"


      elsif field.isObjectList? or field.isNode?
        # Comment
        result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << (field.isNode? ? " Node": " Object") << "\n"
        if field.optionalGetter?
          result << "  if (" << field.getterReturnType() << " _" << field.getterName << " = "<< field.getterName() << "()) {\n"
          result << "    if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(_" << field.getterName << ".get()) )  {\n"
          result << "      idfObject.setString(" << field.fieldEnum << ", _owo->nameString());\n"
          result << "    }\n"
          result << "  }\n"
        else
          result << "  " << field.getterReturnType << " " << field.getterName << " = modelObject." << field.getterName << "();"
          result << "  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(" << field.getterName << ") )  {\n"
          result << "    idfObject.setString(" << field.fieldEnum << ", _owo->nameString());\n"
          result << "  }\n"
        end

        result << "  if ( (_wo = workspaceObject.getTarget(#{field.fieldEnum})) ) {\n"
        result << "    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {\n"
        result << "      // TODO: check return types\n"
        result << "      if (" << field.getterReturnType(true) << " _"
        result << field.getterName << " = _mo->optionalCast<" << field.getterReturnType(false) << ">()) {\n"
        result << "        modelObject." << field.setterName << "(_" << field.getterName << ".get());\n"
        result << "      } else {\n"
        result << "        LOG(Warn, workspaceObject.briefDescription() << \" has a wrong type for '" << field.name << "'\");\n"
        result << "      }\n"
        if field.isRequired?
          result << "    } else {\n"
          result << '      LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot reverse translate required object '" << field.name << "'\");" << "\n"
          result << "      return result;\n"
          result << "    }\n"
          result << "  } else {\n"
          result << '    LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot find required object '" << field.name << "'\");" << "\n"
          result << "    return result;\n"
          result << "  }\n"
        else
          result << "    }\n"
          result << "  }\n"
        end
      else
        prefix = ""
        need_closing = false

        # Comment
        if field.canAutosize?
          result << "  if (modelObject." << field.isAutosizeName << "()) {\n"
          result << "    idfObject." << field.setterAccessor << "(" << field.fieldEnum << ", \"Autosize\")\n"
          result << "  } else {\n"
          prefix = "  "
          need_closing = true
        elsif field.canAutocalculate?
          result << "  if (modelObject." << field.isAutocalculateName << "()) {\n"
          result << "    idfObject." << field.setterAccessor << "(" << field.fieldEnum << ", \"Autocalculate\")\n"
          result << "  } else {\n"
          prefix = "  "
          need_closing = true
        end


        if field.optionalGetter?
          result << prefix << "  // " << field.name << ": " << field.getterReturnType << "\n"

          result << prefix << "  if (" << field.getterReturnType << " _" << field.getterName << " = modelObject." << field.getterName << "()) {\n"
          result << prefix << "    idfObject." << field.setterAccessor << "(" << field.fieldEnum << ", _" << field.getterName << ".get())\n"
          result << prefix << "  }\n"


        elsif field.isBooleanChoice?
          # We have to check is "Yes", in which case we use true
          result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " Boolean\n"
          result << "  if (" << field.getterReturnType(true) << " _" << field.getterName << " = "<< field.getterAccessor << "(" << field.fieldEnum << ", true)) {\n"
          result << "    if(istringEqual(\"Yes\", _" << field.getterName << ".get())) {\n"
          result << "      modelObject." << field.setterName << "(true);\n"
          result << "    } else {\n"
          result << "      modelObject." << field.setterName << "(false);\n"
          result << "    }\n"
          if field.isRequired?
            result << "  } else {\n"
            result << '    LOG(Error, "For " << workspaceObject.briefDescription()'
            result << " << \", cannot find required property '" << field.name << "'\");" << "\n"
            result << "    return result;\n"
          end
          result << "  }\n"
        else

          # Note, assignment isn't used anymore since I no longer use high level
          # variables
          if field.isInteger?
            # assignment = "_i"
            cat = "Integer"
          elsif field.isReal?
            # assignment = "_d"
            cat = "Double"
          elsif field.isChoice? or field.isAlpha? or field.isExternalList?
            # assignment = "_s"
            cat = "String"
          else
            # Not handled...
            # assignment = "#{field.getterReturnType(true)} #{field.getterName}"
            cat = "Unsure of Category... TODO: Check!"
          end

          result << "  // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " " << cat << "\n"

          result << "  " << field.getterReturnType << " " << field.getterName << " = modelObject." << field.getterName << "();\n"
          result << "  idfObject." << field.setterAccessor << "(" << field.fieldEnum << ", " << field.getterName << ")\n\n"

        end

        if need_closing
          result << "  }\n"
        end


      end

      result << "\n"
    end # End loop on nonextensibleFields

    # Assign result and return
    result << "  result = modelObject;\n"
    result << "  return result;\n"
    result << "}\n"


    # Close method
    result << "} // End of translate function\n"


    return result
  end

  # Create the file, and save it on disk
  # @param None
  # @return [None] creates the file
  def write_reverse_translator
    file_path = File.join(File.dirname(__FILE__), "../../../openstudiocore/src/energyplus/ReverseTranslator/ReverseTranslate#{@className}.cpp")
    file_path = File.expand_path(file_path)

    result = String.new

    # Start with the header (copyright)
    result << fileHeader() << "\n"
    # Add includes + open namespaces
    result << reverseTranslatorIncludes()

    result << generateReverseTranslateFunction()

    # Footer
    result << "\n" << "} // end namespace energyplus" << "\n"

    result << "\n" << "} // end namespace openstudio"

    File.open(file_path, "w") do |file|
      file.write(result)
    end

    puts "Saved to #{file_path}"

  end

  # Create the file, and save it on disk
  # @param None
  # @return [None] creates the file
  def write_forward_translator
    file_path = File.join(File.dirname(__FILE__), "../../../openstudiocore/src/energyplus/ForwardTranslator/ForwardTranslate#{@className}_test.cpp")
    file_path = File.expand_path(file_path)

    result = String.new

    # Start with the header (copyright)
    result << fileHeader() << "\n"
    # Add includes + open namespaces
    result << forwardTranslatorIncludes()

    result << generateForwardTranslateFunction()

    # Footer
    result << "\n" << "} // end namespace energyplus" << "\n"

    result << "\n" << "} // end namespace openstudio"

    File.open(file_path, "w") do |file|
      file.write(result)
    end

    puts "Saved to #{file_path}"
  end
end
