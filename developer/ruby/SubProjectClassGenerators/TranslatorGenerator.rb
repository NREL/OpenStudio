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
    # result << "// #include \"../../utilities/idd/IddEnums.hpp\"\n"
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
    result << "\n" << "}   // end namespace energyplus\n"
    result << "\n" << "}   // end namespace openstudio"
    return result
  end

  # Setup the actual reverse translation method
  #
  # @param None
  # @return [String] returns the translateXXX method
  def generateReverseTranslateFunction()
    result = String.new

    # Method name
    result << "  boost::optional<ModelObject> ReverseTranslator::translate" << @className << "(const WorkspaceObject & workspaceObject) {\n"

    # High level variables
    # No longer used, will use unique variable throughout
    # result << "  boost::optional<ModelObject> result;\n"
    # if @hasObjectFields
    #   result << "  boost::optional<WorkspaceObject> _wo;\n"
    #   result << "  boost::optional<ModelObject> _mo;\n"
    # end
    #result << "  boost::optional<double> _d;\n"
    #result << "  boost::optional<int> _i;\n"
    #result << "  boost::optional<std::string> _s;\n"
    #result << "\n"

    # Instantiate the model object
    result << "\n"
    result << "    // Instantiate an object of the class to store the values,\n"
    result << "    // but we don't return it until we know it's ok\n"
    result << "    // TODO: check constructor, it might need other objects\n"
    result << "    openstudio::model::" << @className << " modelObject(m_model);\n\n"


    result << "    // TODO: Note JM 2018-10-17\n"
    result << "    // You are responsible for implementing any additional logic based on choice fields, etc.\n"
    result << "    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way\n\n"


    # Ok, we're ready to start dealing with each field!

    @nonextensibleFields.each do |field|

      if field.isName?
        result << "    // " << field.name << "\n"
        result << "    if (boost::optional<std::string> name_ = workspaceObject.name()) {\n"
        result << "      modelObject.setName(name_.get());\n"
        result << "    }\n\n"

      elsif field.isObjectList? or field.isNode?
        # Comment
        result << "    // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << (field.isNode? ? " Node": " Object") << "\n"
        result << "    if (boost::optional<WorkspaceObject> wo_ = workspaceObject.getTarget(#{field.fieldEnum})) {\n"
        result << "      if (boost::optional<ModelObject> mo_ = translateAndMapWorkspaceObject(wo_.get())) {\n"
        result << "        // TODO: check return types\n"
        result << "        if (" << field.getterReturnType(true) << " "
        result << field.getterName << "_ = mo_->optionalCast<" << field.getterReturnType(false) << ">()) {\n"
        result << "          modelObject." << field.setterName << "(" << field.getterName << "_.get());\n"
        result << "        } else {\n"
        result << "          LOG(Warn, workspaceObject.briefDescription() << \" has a wrong type for '" << field.name << "'\");\n"
        result << "        }\n"
        if field.isRequired?
          result << "      } else {\n"
          result << '        LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot reverse translate required object '" << field.name << "'\");" << "\n"
          result << "        return boost::none;\n"
          result << "      }\n"
          result << "    } else {\n"
          result << '      LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot find required object '" << field.name << "'\");" << "\n"
          result << "      return boost::none;\n"
          result << "    }\n"
        else
          result << "      }\n"
          result << "    }\n"
        end

      elsif field.isBooleanChoice?
        # We have to check is "Yes", in which case we use true
        result << "    // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " Boolean\n"
        result << "    if (" << field.getterReturnType(true) << " " << field.getterName << "_ = "<< field.getterAccessor << "(" << field.fieldEnum << ", true)) {\n"
        result << "      if(istringEqual(\"Yes\", " << field.getterName << "_.get())) {\n"
        result << "        modelObject." << field.setterName << "(true);\n"
        result << "      } else {\n"
        result << "        modelObject." << field.setterName << "(false);\n"
        result << "      }\n"
        if field.isRequired?
          result << "    } else {\n"
          result << '      LOG(Error, "For " << workspaceObject.briefDescription()'
          result << " << \", cannot find required property '" << field.name << "'\");" << "\n"
          result << "      return boost::none;\n"
        end
        result << "    }\n\n"
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

        result << "    // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " " << cat << "\n"

        #result << "    if (" << assignment << " = "<< field.getterAccessor << "(" << field.fieldEnum << ")) {\n"
        #result << "      modelObject." << field.setterName << "(" << assignment << ");\n"
        # Instead, I don't use global optional vars
        result << "    if (" << field.getterReturnType(true) << " " << field.getterName << "_ = " << "workspaceObject." << field.getterAccessor << "(" << field.fieldEnum << ")) {\n"
        result << "      modelObject." << field.setterName << "(" << field.getterName << "_.get());\n"


        if field.isRequired?
          result << "     } else {\n"
          result << '     LOG(Error, "For " << workspaceObject.briefDescription()'
          result << "   << \", cannot find required property '" << field.name << "'\");" << "\n"
          result << "      return boost::none;\n"
        end
        result << "    }\n\n"
      end

    end # End loop on nonextensibleFields

    # Assign result and return
    result << "    return modelObject;\n"

    # Close method
    result << "  }  // End of translate function\n"

    return result
  end


# Setup the actual reverse translation method
  #
  # @param None
  # @return [String] returns the translateXXX method
  def generateForwardTranslateFunction()
    result = String.new

    # Method name
    result << "  boost::optional<IdfObject> ForwardTranslator::translate" << @className << "( model::" << @className << "& modelObject ) {\n"

    # High level variables
    # No longer used, will use unique variable throughout
    # result << "  boost::optional<IdfObject> result;\n"
    # if @hasObjectFields
    #   result << "  boost::optional<WorkspaceObject> _wo;\n"
    #   result << "  boost::optional<ModelObject> _mo;\n"
    # end
    #result << "  boost::optional<double> _d;\n"
    #result << "  boost::optional<int> _i;\n"
    #result << "  boost::optional<std::string> _s;\n"
    #result << "\n"

    # Instantiate the model object
    result << "\n"
    result << "    // Instantiate an IdfObject of the class to store the values\n"
    result << "    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::" << @iddObjectType.valueName << ", modelObject);\n"
    result << "    // If it doesn't have a name, or if you aren't sure you are going to want to return it\n"
    result << "    // IdfObject idfObject(openstudio::IddObjectType::" << @iddObjectType.valueName << ");\n"
    result << "    // m_idfObjects.push_back(idfObject);\n\n"

    result << "    // TODO: Note JM 2018-10-17\n"
    result << "    // You are responsible for implementing any additional logic based on choice fields, etc.\n"
    result << "    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way\n\n"


    # Ok, we're ready to start dealing with each field!

    @nonextensibleFields.each do |field|

      if field.isName?
        result << "    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.\n"
        result << "    // But in some cases, you'll want to handle failure without pushing to the map\n"
        result << "    // Name\n"
        result << "    idfObject.setName(modelObject.nameString());\n"


      elsif field.isObjectList? or field.isNode?
        # Comment
        result << "    // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << (field.isNode? ? " Node": " Object") << "\n"
        if field.optionalGetter?
          result << "    if (" << field.getterReturnType() << " " << field.getterName << "_ = modelObject."<< field.getterName() << "()) {\n"
          result << "      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(" << field.getterName << "_.get()))  {\n"
          result << "        idfObject.setString(" << field.fieldEnum << ", wo_->nameString());\n"
          result << "      }\n"
          result << "    }\n"
        else
          result << "    " << field.getterReturnType << " " << field.getterName << " = modelObject." << field.getterName << "();\n"
          result << "    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(" << field.getterName << "))  {\n"
          result << "      idfObject.setString(" << field.fieldEnum << ", wo_->nameString());\n"
          result << "    }\n"
        end

      else
        prefix = ""
        need_closing = false

        # Comment
        if field.canAutosize?
          result << "    if (modelObject." << field.isAutosizeName << "()) {\n"
          result << "      idfObject.setString(" << field.fieldEnum << ", \"Autosize\");\n"
          result << "    } else {\n"
          prefix = "  "
          need_closing = true
        elsif field.canAutocalculate?
          result << "    if (modelObject." << field.isAutocalculateName << "()) {\n"
          result << "      idfObject.setString(" << field.fieldEnum << ", \"Autocalculate\");\n"
          result << "    } else {\n"
          prefix = "  "
          need_closing = true
        end


        if field.optionalGetter?
          result << prefix << "    // " << field.name << ": " << field.getterReturnType << "\n"

          result << prefix << "    if (" << field.getterReturnType << " " << field.getterName << "_ = modelObject." << field.getterName << "()) {\n"
          result << prefix << "      idfObject." << field.setterAccessor << "(" << field.fieldEnum << ", " << field.getterName << "_.get());\n"
          result << prefix << "    }\n"


        elsif field.isBooleanChoice?
          result << "    // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " Boolean\n"
          result << "    if (modelObject." << field.getterName << "()) {\n"
          result << "      idfObject.setString(" << field.fieldEnum << ", \"Yes\");\n"
          result << "    } else {\n"
          result << "      idfObject.setString(" << field.fieldEnum << ", \"No\");\n"
          result << "    }\n"
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

          result << "    // " << field.name << ": " << (field.isRequired? ? "Required" : "Optional") << " " << cat << "\n"

          result << "    const " << field.getterReturnType << " " << field.getterName << " = modelObject." << field.getterName << "();\n"
          result << "    idfObject." << field.setterAccessor << "(" << field.fieldEnum << ", " << field.getterName << ");\n\n"

        end

        if need_closing
          result << "  }\n"
        end


      end

      result << "\n"
    end # End loop on nonextensibleFields

    # Assign result and return
    result << "    return idfObject;\n"

    # Close method
    result << "  }  // End of translate function\n"


    return result
  end


  def gtestFtRtIncludes()
    result = String.new

    result << "#include \<gtest/gtest.h>\n"
    result << "#include \"EnergyPlusFixture.hpp\"\n\n"

    result << "#include \"../ForwardTranslator.hpp\"\n"
    result << "#include \"../ReverseTranslator.hpp\"\n\n"

    result << "#include \"../../model/" << @className << ".hpp\"\n"
    result << "#include \"../../model/" << @className << "_Impl.hpp\"\n"

    preamble = "// TODO: Check the following class names against object getters and setters.\n"

    # Check for ObjectList fields, to see which we need to include
    @nonextensibleFields.each { |field|
      if field.isObjectList?
        result << preamble
        result << "#include \"../../model/" << field.objectListClassName << ".hpp\"\n"
        result << "#include \"../../model/" << field.objectListClassName << "_Impl.hpp\"\n"
        preamble = ""
      end
    }

    result << "\n"
    result << "#include \"../../utilities/idf/Workspace.hpp\"\n"
    result << "#include \"../../utilities/idf/IdfObject.hpp\"\n"
    result << "#include \"../../utilities/idf/WorkspaceObject.hpp\"\n"
    result << "\n"
    result << "// E+ FieldEnums\n"
    result << "#include <utilities/idd/IddEnums.hxx>\n"
    result << "#include <utilities/idd/IddFactory.hxx>\n"
    result << "#include <utilities/idd/" << @iddObjectType.valueName << "_FieldEnums.hxx>\n"
    result << "\n"
    result << "using namespace openstudio::energyplus;\n"
    result << "using namespace openstudio::model;\n"
    result << "using namespace openstudio;\n\n"

    return result;
  end

  def gtestForwardTranslator

    s_setup = String.new
    s_tests = String.new

    instanceName = OpenStudio::toLowerCamelCase(@className)

    @nonextensibleFields.each_with_index { |field, i|
      next if field.isHandle?

      if field.isName?
        s_setup << "  #{instanceName}.setName(\"My #{@className}\");\n"

      elsif field.isObjectList? or field.isNode?

        # Comment
        s_setup << "  " << field.getterReturnType << " #{field.getterName}(m);\n"
        s_setup << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(#{field.getterName}));\n"

        s_tests << "  EXPECT_EQ(#{field.getterName}.nameString(), idfObject.getString(#{field.fieldEnum}).get());\n"

      else

        if field.isBooleanChoice?
          if field.hasDefault?
            if field.defaultValue == 'Yes'
              s_setup << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(false));  // Opposite from IDD default\n";
              s_tests << "  EXPECT_EQ(\"No\", idfObject.getString(#{field.fieldEnum}).get());\n"
            else
              s_setup << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(true));   // Opposite from IDD default\n";
              s_tests << "  EXPECT_EQ(\"Yes\", idfObject.getString(#{field.fieldEnum}).get());\n"
            end
          else
            s_setup << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(true));   // TODO: check this isnt the same as the Ctor\n";
            s_tests << "  EXPECT_EQ(\"Yes\", idfObject.getString(#{field.fieldEnum}).get());\n"
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

          if field.canAutosize?
            s_setup << "  // Autosize\n"
            s_setup << "  // #{instanceName}." << field.autosizeName << "();\n"
            s_tests << "  // EXPECT_EQ(\"Autosize\"\, idfObject.getString(#{field.fieldEnum}).get());\n"
          elsif field.canAutocalculate?
            s_setup << "  // Autocalculate\n"
            s_setup << "  // #{instanceName}." << field.autocalculateName << "();\n"
            s_tests << "  // EXPECT_EQ(\"Autocalculate\"\, idfObject.getString(#{field.fieldEnum}).get());\n"
          end

          s_setup << "  EXPECT_TRUE(#{instanceName}." << field.setterName << "(#{good_val}));\n";
          s_tests << "  EXPECT_EQ(#{good_val}, idfObject.get#{cat}(#{field.fieldEnum}).get());\n"

        end

      end

    }

    result = String.new
    result << "TEST_F(EnergyPlusFixture, ForwardTranslator_" << @className << ") {\n\n"

    result << "  ForwardTranslator ft;\n\n"

    result << "  Model m;\n"
    result << "  // TODO: Check regular Ctor arguments\n"
    result << "  " << @className << " #{instanceName}(m);\n"
    result << "  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)\n"
    result << "  // " << @className << " #{instanceName} = m.getUniqueModelObject<" << @className << ">();\n\n"

    result << s_setup

    result << "\n\n"
    result << "  // TODO: you're responsible for creating all other objects needed so this object actually gets ForwardTranslated\n\n"

    result << "  const Workspace w = ft.translateModel(m);\n"
    result << "  const auto idfObjs = w.getObjectsByType(IddObjectType::" << @iddObjectType.valueName << ");\n"
    result << "  ASSERT_EQ(1u, idfObjs.size());\n"
    result << "\n"
    result << "  const auto& idfObject = idfObjs.front();\n"

    result << s_tests

    result << "\n"

    result << "}\n"

    return result
  end

  def gtestReverseTranslator

    s_setup = String.new
    s_tests = String.new

    # This is too long
    # instanceName = "wo#{OpenStudio::toLowerCamelCase(@className)}"
    instanceName = "wo#{@className.gsub(/[^[:upper:]]+/, "")}"

    @nonextensibleFields.each_with_index { |field, i|
      next if field.isHandle?

      if field.isName?
        s_setup << "  #{instanceName}.setName(\"My #{@className}\");\n"

      elsif field.isObjectList? or field.isNode?

        # Comment
        s_setup << "  auto wo#{field.getterName} = w.addObject(IdfObject(IddObjectType::" << field.getterReturnType << ")).get();\n"
        s_setup << "  wo#{field.getterName}.setName(\"My #{field.getterName}\"));\n"
        s_setup << "  EXPECT_TRUE(#{instanceName}.setPointer(#{field.fieldEnum}, wo#{field.getterName}.handle()));\n"

        s_tests << "  ASSERT_TRUE(modelObject.#{field.getterName}().optionalCast<#{field.getterReturnType}>());\n"
        s_tests << "  EXPECT_EQ(wo#{field.getterName}.nameString(), modelObject.#{field.getterName}().nameString());\n"

      else

        if field.isBooleanChoice?
          if field.hasDefault?
            if field.defaultValue == 'Yes'
              s_setup << "  EXPECT_TRUE(#{instanceName}.setString(#{field.fieldEnum}, \"No\"));  // Opposite from IDD default\n"
              s_tests << "  EXPECT_FALSE(modelObject.#{field.getterName}());"
            else
              s_setup << "  EXPECT_TRUE(#{instanceName}.setString(#{field.fieldEnum}, \"Yes\"));  // Opposite from IDD default\n"
              s_tests << "  EXPECT_TRUE(modelObject.#{field.getterName}());"

            end
          else
            s_setup << "  EXPECT_TRUE(#{instanceName}.setString(#{field.fieldEnum}, \"Yes\"));  // TODO: check this isnt the same as the Ctor\n";
            s_tests << "  EXPECT_TRUE(modelObject.#{field.getterName}());"
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

          if field.canAutosize?
            s_setup << "  // Autosize\n"
            s_setup << "  // EXPECT_TRUE(#{instanceName}.setString(#{field.fieldEnum}, \"Autosized\"));\n"
            s_tests << "  // EXPECT_TRUE(modelObject.#{field.isAutosizeName}());\n"
          elsif field.canAutocalculate?
            s_setup << "  // Autocalculate\n"
            s_setup << "  // EXPECT_TRUE(#{instanceName}.setString(#{field.fieldEnum}, \"Autocalculated\"));\n"
            s_tests << "  // EXPECT_TRUE(modelObject.#{field.isAutosizeName}())\n;"
          end

          s_setup << "  EXPECT_TRUE(#{instanceName}.set#{cat}(#{field.fieldEnum}, #{good_val}));\n"
          s_tests << "  EXPECT_EQ(#{good_val}, modelObject.#{field.getterName}());"

        end

      end

    }

    result = String.new
    result << "TEST_F(EnergyPlusFixture, ReverseTranslator_" << @className << ") {\n\n"

    result << "  ReverseTranslator rt;\n\n"

    result << "  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);\n\n"

    result << "  auto #{instanceName} = w.addObject(IdfObject(IddObjectType::" << @iddObjectType.valueName << ")).get();\n\n"

    result << s_setup

    result << "\n\n"
    result << "  const Model m = rt.translateWorkspace(w);\n"
    result << "  const auto modelObjects = m.getConcreteModelObjects<" << @className << ">();\n"
    result << "  ASSERT_EQ(1u, modelObjects.size());\n"
    result << "\n"
    result << "  const auto& modelObject = modelObjects.front();\n"

    result << s_tests

    result << "\n"

    result << "}\n"

    return result
  end


  # Create the file, and save it on disk
  # @param None
  # @return [None] creates the file
  def write_reverse_translator
    file_path = File.join(File.dirname(__FILE__), "../../../src/energyplus/ReverseTranslator/ReverseTranslate#{@className}.cpp")
    file_path = File.expand_path(file_path)

    result = String.new

    # Start with the header (copyright)
    result << fileHeader() << "\n"
    # Add includes + open namespaces
    result << reverseTranslatorIncludes()

    result << generateReverseTranslateFunction()

    # Footer
    result << "\n" << "}  // end namespace energyplus"
    result << "\n" << "}  // end namespace openstudio\n"

    File.open(file_path, "w") do |file|
      file.write(result)
    end

    # puts "Saved to #{file_path}"
    return file_path
  end

  # Create the file, and save it on disk
  # @param None
  # @return [None] creates the file
  def write_forward_translator
    file_path = File.join(File.dirname(__FILE__), "../../../src/energyplus/ForwardTranslator/ForwardTranslate#{@className}.cpp")
    file_path = File.expand_path(file_path)

    result = String.new

    # Start with the header (copyright)
    result << fileHeader() << "\n"
    # Add includes + open namespaces
    result << forwardTranslatorIncludes()

    result << generateForwardTranslateFunction()

    # Footer
    result << "\n" << "}  // end namespace energyplus"
    result << "\n" << "}  // end namespace openstudio\n"

    File.open(file_path, "w") do |file|
      file.write(result)
    end

    # puts "Saved to #{file_path}"
    return file_path
  end

  # Create the file, and save it on disk
  # @param None
  # @return [None] creates the file
  def write_ft_rt_tests(has_ft, has_rt)
    file_path = File.join(File.dirname(__FILE__), "../../../src/energyplus/Test/#{@className}_GTest.cpp")
    file_path = File.expand_path(file_path)

    result = String.new

    # Start with the header (copyright)
    result << fileHeader() << "\n"
    # Add includes + open namespaces
    result << gtestFtRtIncludes()

    if has_ft
      result << gtestForwardTranslator()
    end
    if has_rt
      result << gtestReverseTranslator()
    end


    File.open(file_path, "w") do |file|
      file.write(result)
    end

    # puts "Saved to #{file_path}"
    return file_path
  end
end
