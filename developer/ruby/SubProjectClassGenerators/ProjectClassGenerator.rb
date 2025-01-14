########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

######################################################################
# == Synopsis
#
#   Generator for project::ObjectRecord and project::JoinRecord classes.
#
######################################################################

require File.dirname(__FILE__) + '/SubProjectClassGenerator.rb'

class ProjectClassGenerator < SubProjectClassGenerator

  #              T/F          T/F
  attr_accessor :joinRecord, :objectRecord

  # Classes to be joined, w/o "Record"
  attr_accessor :leftJoinClass, :rightJoinClass

  # Class being serialized to database as objectRecord
  attr_accessor :serializedClass

  # Lower camel case forms
  attr_accessor :classArgument, :baseClassArgument, :serializedClassArgument, :leftArgument, :rightArgument

  def initialize(className, baseClassName, pImpl, qobject)
    raise "ProjectClassMethods assumes that the class to be generated derives from Record." if baseClassName.empty?
    raise "Project classes must follow the pImpl idiom. Please re-run with -p as an argument." if not pImpl
    raise "New project classes should not be QObjects. Please re-run without listing -q ." if qobject

    super(className, baseClassName, pImpl, qobject)

    @classArgument = @className.gsub(/\b\w/){$&.downcase}
    @baseClassArgument = @baseClassName.gsub(/\b\w/){$&.downcase}

    # determine if joinRecord/objectRecord, pull out join information
    re = Regexp.new('(\w*)_(\w*)_JoinRecord')
    m = @className.match(re)
    if not (m == nil)
      @joinRecord = true
      @objectRecord = false
      @leftJoinClass = m[1]
      @rightJoinClass = m[2]

      @leftArgument = String.new
      @rightArgument = String.new
      @leftArgument << @leftJoinClass.gsub(/\b\w/){$&.downcase} << "Record"
      @rightArgument << @rightJoinClass.gsub(/\b\w/){$&.downcase} << "Record"
    else
      @joinRecord = false
      @objectRecord = true
      re = Regexp.new('(\w*)Record')
      m = @className.match(re)
      @serializedClass = m[1]
      @serializedClassArgument = @serializedClass.gsub(/\b\w/){$&.downcase}
    end

  end

  def implHppIncludes()
    result = String.new
    if @objectRecord
      result << "// TODO: Delete this include if no derived classes (and no " << @className << "Type enum).\n"
      result << "#include \"../project/" << @className << ".hpp\"\n\n"
    end
    return result
  end

  def cppIncludes()
    result = String.new
    if @joinRecord
      result << "#include \"../project/" << @leftJoinClass << "Record.hpp\"\n"
      result << "#include \"../project/" << @rightJoinClass << "Record.hpp\"\n"
    else
      result << "#include \"../project/JoinRecord.hpp\"\n"
      result << "// TODO: Add derived class includes for factory methods if this is a base class.\n\n"
      result << "// TODO: Replace with derived class includes if this is a base class.\n"
      result << "#include <NAMESPACE/" << @serializedClass << ".hpp>\n\n"
    end
    result << "#include \"../utilities/core/Assert.hpp\"\n\n"
    return result
  end

  def hppOSForwardDeclarations
    result = String.new
    if @objectRecord
      result << "namespace NAMESPACE {\n"
      result << "  class " << @serializedClass << ";\n"
      result << "}\n"
    end
    return result
  end

  def implHppOSForwardDeclarations
    result = String.new
    if @objectRecord
      result << "namespace NAMESPACE {\n"
      result << "  class " << @serializedClass << ";\n"
      result << "}\n"
    end
    return result
  end

  def hppSubProjectForwardDeclarations
    result = String.new
    if @joinRecord
      result << "class " << @leftJoinClass << "Record;\n"
      result << "class " << @rightJoinClass << "Record;\n\n"
    end
    return result
  end

  def implHppSubProjectForwardDeclarations
    result = String.new
    if @joinRecord
      result << "class " << @leftJoinClass << "Record;\n"
      result << "class " << @rightJoinClass << "Record;\n\n"
    end
    return result
  end

  def hppPreClass()
    result = String.new

    if @objectRecord

      result << "// TODO: Populate or delete this enumeration if there are/are not any derived types, respectively.\n"
      result << "/** \\class " << @className << "Type\n"
      result << " *  \\brief ObjectRecord types that derive from " << @className << ".\n"
      result << " *  \\details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual\n"
      result << " *  macro call is:\n"
      result << " *  \\code\n"
      result << "OPENSTUDIO_ENUM(" << @className << "Type,\n"
      result << "    ((" << @className << "DerivedRecord1))\n"
      result << ");\n"
      result << " *  \\endcode */\n"
      result << "OPENSTUDIO_ENUM(" << @className << "Type,\n"
      result << "    ((" << @className << "DerivedRecord1))\n"
      result << ");\n\n"

    end

    if @baseClassName == "ObjectRecord"

      # Start Enum for Table Columns
      result << "/** \\class " << @className << "Columns\n"
      result << " *  \\brief Column definitions for the " << @className << "s table.\n"
      result << " *\n"
      result << " *  \\relates " << @className << " */\n"
      result << "OPENSTUDIO_ENUM(" << @className << "Columns,\n"
      result << "  ((id)(INTEGER PRIMARY KEY)(0))\n"
      result << "  ((handle)(TEXT)(1))\n"
      result << "  ((name)(TEXT)(2))\n"
      result << "  ((displayName)(TEXT)(3))\n"
      result << "  ((description)(TEXT)(4))\n"
      result << "  ((timestampCreate)(TEXT)(5))\n"
      result << "  ((timestampLast)(TEXT)(6))\n"
      result << "  ((uuidLast)(TEXT)(7))\n"
      result << "  // TODO: Add Columns to Record Class (and Derived-Class)-Specific Data.\n"
      result << "  ((" << @classArgument << "Type)(INTEGER)(8))\n"
      result << ");\n\n"

    end

    result << super

    return result
  end

  def hppPublicTypedefs()
    result = String.new

    result << "\n"
    result << "  using ImplType = detail::" << @className << "_Impl;\n\n"
    if @joinRecord
      result << "  using JoinRecordColumns = ColumnsType;\n\n"
    else
      if @baseClassName == "ObjectRecord"
        result << "  using ColumnsType = " << @className << "Columns;\n"
        result << "  using ObjectRecordType = " << @className << ";\n\n"
      else
        result << "  // TODO: Check this typedef. The generator script assumes that the hierarchy is only two deep "
        result << "  // from ObjectRecord.\n"
        result << "  using ColumnsType = " << @baseClassName << "Columns;\n"
        result << "  // TODO: Check this typedef too.\n"
        result << "  using ObjectRecordType = " << @baseClassName << ";\n\n"
      end
    end

    return result
  end

  def hppConstructors()
    result = String.new

    if @joinRecord

      # construct from leftJoinClass and rightJoinClass records
      result << "  " << @className
      result << "(const " << @leftJoinClass << "Record& " << @leftArgument
      result << ", const " << @rightJoinClass << "Record& " << @rightArgument
      result << ");\n\n"

    else

      # construct from serializedClass
      result << "  // TODO: Delete if " << @serializedClass << " is abstract, make private if "
      result << @serializedClass << " is concrete and has derived classes.\n"
      result << "  // TODO: Replace ProjectDatabase& database (or add another object if it is ok for "
      result << @className << " to be and orphan) with const& to parent Record if the Table contains a parent id.\n"
      result << "  // TODO: Find-replace on 'NAMESPACE'.\n"
      result << "  " << @className << "(const NAMESPACE::" << @serializedClass << "& " << @serializedClassArgument
      result << ", ProjectDatabase& database);\n\n"

      # construct from query
      result << "  // TODO: Delete if " << @serializedClass << " is abstract, make private if "
      result << @serializedClass << " is concrete and has derived classes.\n"
      result << "  " << @className << "(const QSqlQuery& query, ProjectDatabase& database);\n\n"

    end

    return result
  end

  def implHppConstructors()
    result = String.new

    if @joinRecord

      # construct from leftJoinClass and rightJoinClass records
      result << "    " << @className << "_Impl"
      result << "(const " << @leftJoinClass << "Record& " << @leftArgument
      result << ", const " << @rightJoinClass << "Record& " << @rightArgument
      result << ");\n\n"

    else

      implConstructorStart = String.new
      implConstructorStart << "    " << @className << "_Impl("

      # construct from serializedClass
      result << "    // TODO: May need to remove type enum if " << @serializedClass
        result << " is a leaf of the inheritance tree.\n"
      result << "    // TODO: Replace ProjectDatabase& database with parent Record and/or add more \n"
      result << "    // construtors to match public class.\n"
      result << "    // TODO: Find-replace on 'NAMESPACE'.\n"
      result << implConstructorStart << "const NAMESPACE::" << @serializedClass << "& " << @serializedClassArgument << ",\n"
      result << " " * implConstructorStart.size << "const " << @className << "Type& " << @classArgument << "Type,\n"
      result << " " * implConstructorStart.size << "ProjectDatabase& database);\n\n"

    end

    # construct from query
    result << "    /** Constructor from query. Throws if bad query. */\n"
    result << "    " << @className << "_Impl(const QSqlQuery& query, ProjectDatabase& database);\n\n"

    return result
  end

  def cppConstructors()
    result = String.new

    constructorPreamble = String.new
    constructorPreamble << "  " << @className << "_Impl::" << @className << "_Impl("

    if @joinRecord

      # construct from leftJoinClass and rightJoinClass records
      result << constructorPreamble << "const " << @leftJoinClass << "Record& " << @leftArgument << ",\n"
      result << " " * constructorPreamble.size << "const " << @rightJoinClass << "Record& " << @rightArgument << ")\n"
      result << "    : JoinRecord_Impl(" << @leftArgument << ".id(), " << @leftArgument << ".handle(), "
      result << @rightArgument << ".id(), " << @rightArgument << ".handle(), " << @leftArgument << ".projectDatabase())\n"
      result << "  {\n"
      result << "    OS_ASSERT(" << @leftArgument << ".projectDatabase().handle() == "
      result << @rightArgument << ".projectDatabase().handle());\n"
      result << "  }\n\n"

      # construct from query
      result << constructorPreamble << "const QSqlQuery& query, const ProjectDatabase& database)\n"
      result << "    : JoinRecord_Impl(query, database)\n"
      result << "  {}\n\n"

    else

      # construct from serializedClass
      result << constructorPreamble << "const NAMESPACE::" << @serializedClass << "& " << @serializedClassArgument << ",\n"
      result << " " * constructorPreamble.size << "const " << @className << "Type& " << @classArgument << "Type,\n"
      result << " " * constructorPreamble.size << "ProjectDatabase& database)\n"
      result << "    : " << @baseClassName << "_Impl("
      if @baseClassName == "ObjectRecord"
        result << "database," << @serializedClassArgument << ".uuid(),"
        result << @serializedClassArgument << ".name()," << @serializedClassArgument << ".name(),\"\","
        result << @serializedClassArgument << ".versionUUID()),\n"
      else
        result << @serializedClassArgument << ", database),\n"
      end
      result << "  // TODO: Delete member enum initialization if deleted from _Impl.hpp\n"
      result << "      m_" << @classArgument << "Type(" << @classArgument << "Type)\n"
      result << "  {\n"
      result << "    OS_ASSERT(false);\n"
      result << "    // TODO: Initialize data members, check constructor call for base class.\n"
      result << "  }\n\n"


      # construct from query
      result << constructorPreamble << "const QSqlQuery& query, ProjectDatabase& database)\n"
      result << "    : " << @baseClassName << "_Impl"

      if @baseClassName == "ObjectRecord"
        result << "(database, query)\n"
      else
        result << "(query, database)\n"
      end
      result << "  {\n"
      result << "    OS_ASSERT(query.isValid());\n"
      result << "    OS_ASSERT(query.isActive());\n"
      result << "    OS_ASSERT(query.isSelect());\n\n"
      result << "    QVariant value;\n\n"
      result << "    // TODO: Delete deserialization of enum if deleted from _Impl.hpp\n"
      result << "    value = query.value(" << @className << "::ColumnsType::" << @classArgument
          result << "Type);\n"
      result << "    OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    m_" << @classArgument << "Type = " << @className << "Type(value.toInt());\n\n"
      result << "    // TODO: Extract data members from query. Templates follow.\n\n"
      result << "    // Required data member\n"
      result << "    // value = query.value(" << @className << "::ColumnsType::DATAMEMBERNAME);\n"
      result << "    // OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    // m_DATAMEMBERNAME = value.toTYPE();\n\n"
      result << "    // Optional data member\n"
      result << "    // value = query.value(" << @className << "::ColumnsType::DATAMEMBERNAME);\n"
      result << "    // if (value.isValid() && !value.isNull()) {\n"
      result << "    //   m_DATAMEMBERNAME = value.toTYPE();\n"
      result << "    // }\n\n"
      result << "  }\n\n"

    end

    return result
  end

  def cppPublicClassConstructors()
    result = String.new

    constructorPreamble = String.new
    constructorPreamble << @className << "::" << @className << "("

    if @joinRecord

      result << constructorPreamble << "const " << @leftJoinClass << "Record& " << @leftArgument
          result << ",\n"
      result << " " * constructorPreamble.size << "const " << @rightJoinClass << "Record& "
          result << @rightArgument << ")\n"
      result << "  : JoinRecord(std::shared_ptr<detail::" << @className << "_Impl(\n"
      result << "        new detail::" << @className << "_Impl(" << @leftArgument << ", "
          result << @rightArgument << ")), " << @leftArgument << ".projectDatabase())\n"
      result << "{\n"
      result << "  OS_ASSERT(getImpl<detail::" << @className << "_Impl>());\n"
      result << "}\n\n"

    else

      # construct from serializedClass
      result << constructorPreamble << "const NAMESPACE::" << @serializedClass << "& " << @serializedClassArgument
      result << ", ProjectDatabase& database)\n"
      result << "  : " << @baseClassName << "(std::shared_ptr<detail::" << @className << "_Impl>(\n"
      result << "        new detail::" << @className << "_Impl(" << @serializedClassArgument << ", database)),\n"
      result << "        database)\n"
      result << "{\n"
      result << "  OS_ASSERT(getImpl<detail::" << @className << "_Impl>());\n\n"
      result << "  OS_ASSERT(false);\n"
      result << "  // TODO: Align with final public constructors.\n"
      result << "  // TODO: Handle relationships (setting id fields) as needed.\n"
      result << "}\n\n"

      # construct from query
      result << constructorPreamble << "const QSqlQuery& query, ProjectDatabase& database)\n"
      result << "  : " << @baseClassName << "(std::shared_ptr<detail::" << @className << "_Impl>(\n"
      result << "        new detail::" << @className << "_Impl(query, database)),\n"
      result << "        database)\n"
      result << "{\n"
      result << "  OS_ASSERT(getImpl<detail::" << @className << "_Impl>());\n"
      result << "}\n\n"

    end

    return result
  end

  def hppPublicMethods()
    result = String.new

    if ((@baseClassName == "ObjectRecord") or (@baseClassName == "JoinRecord"))

      result << "  // TODO: Add a call to createTable in ProjectDatabase_Impl::initialize().\n"
      result << "  static std::string databaseTableName();\n\n"

      result << "  static UpdateByIdQueryData updateByIdQueryData();\n\n"

      if (@baseClassName == "ObjectRecord")

        result << "  // TODO: Add a call to this updatePathData method in ProjectDatabase_Impl::updatePathData.\n"
        result << "  static void updatePathData(ProjectDatabase database,\n"
        result << "                             const openstudio::path& originalBase,\n"
        result << "                             const openstudio::path& newBase);\n\n"

      end

    end

    if @objectRecord

      result << "  /** Get " << @className << " from query. Returned object will be of the correct\n"
      result << "   *  derived type. */\n"
      result << "  static boost::optional<" << @className << "> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);\n\n"

      result << "  // TODO: Delete if no derived classes.\n"
      result << "  static " << @className << " factoryFrom" << @serializedClass << "(const NAMESPACE::"
      result << @serializedClass << "& " << @serializedClassArgument
      result << ", ProjectDatabase& database);\n\n"

    end

    if @joinRecord

      result << "  static std::vector<" << @rightJoinClass << "Record> get" << @rightJoinClass
      result << "Records(const " << @leftJoinClass << "Record& " << @leftArgument << ");\n\n"

      result << "  static std::vector<" << @leftJoinClass << "Record> get" << @leftJoinClass
      result << "Records(const " << @rightJoinClass << "Record& " << @rightArgument << ");\n\n"

    else

      result << "  static std::vector<" << @className << "> get" << @className
      result << "s(ProjectDatabase& database);\n\n"

      result << "  static boost::optional<" << @className << "> get" << @className
      result << "(int id, ProjectDatabase& database);\n\n"

    end

    result << "  /** @name Getters */\n"
    result << "  //@{\n\n"

    if @joinRecord

      result << "  " << @leftJoinClass << "Record " << @leftArgument << "() const;\n\n"

      result << "  " << @rightJoinClass << "Record " << @rightArgument << "() const;\n\n"

    else

      result << "  // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED\n\n"

      result << "  // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED\n\n"

      result << "  NAMESPACE::" << @serializedClass << " " << @serializedClassArgument << "() const;\n\n"

    end

    result << "  //@}\n"

    return result
  end

  def implHppPublicMethods()
    result = String.new

    result << "    /** @name Virtual Methods */\n"
    result << "    //@{\n\n"

    if ((@baseClassName == "ObjectRecord") or (@baseClassName == "JoinRecord"))
      result << "    /** Get the name of the database table for this record. Should be implemented by the base \n"
      result << "     *  class for the table. */\n"
      result << "    virtual std::string databaseTableName() const;\n\n"
    end

    if @objectRecord

      result << "    /** Returns the direct parent of this object, if it exists. */\n"
      result << "    virtual boost::optional<ObjectRecord> parent() const;\n\n"

      result << "    /** Returns objects directly owned by this Record. Children are removed when this Record \n"
      result << "     *  is removed. */\n"
      result << "    virtual std::vector<ObjectRecord> children() const;\n\n"

      result << "    /** Returns objects referenced, but not owned, by this Record. */\n"
      result << "    virtual std::vector<ObjectRecord> resources() const;\n\n"

      result << "    /** Returns join relationships between this object and others. Such relationships will be \n"
      result << "     *  removed when either record in the relationship is removed. */\n"
      result << "    virtual std::vector<JoinRecord> joinRecords() const;\n\n"

    end

    result << "    /** Save the row that corresponds to this record in projectDatabase. */\n"
    result << "    virtual void saveRow(const std::shared_ptr<QSqlDatabase>& database);\n\n"

    result << "    //@}\n"
    result << "    /** @name Getters */\n"
    result << "    //@{\n\n"

    if @joinRecord

      result << "    virtual ObjectRecord leftObject() const;\n\n"

      result << "    virtual ObjectRecord rightObject() const;\n\n"

      result << "    " << @leftJoinClass << "Record " << @leftArgument << "() const;\n\n"

      result << "    " << @rightJoinClass << "Record " << @rightArgument << "() const;\n\n"

    else

      result << "    // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED\n\n"

      result << "    // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED\n\n"

      result << "    NAMESPACE::" << @serializedClass << " " << @serializedClassArgument << "() const;\n\n"

    end

    result << "    //@}\n"

    return result
  end

  def cppPublicMethods()
    result = String.new

    if ((@baseClassName == "ObjectRecord") or (@baseClassName == "JoinRecord"))
      result << "  std::string " << @className << "_Impl::databaseTableName() const {\n"
      result << "    return " << @className << "::databaseTableName();\n"
      result << "  }\n\n"
    end

    if @objectRecord

      result << "  boost::optional<ObjectRecord> " << @className << "_Impl::parent() const {\n"
      result << "    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl\n"
      result << "    // for an example.\n"
      result << "    OS_ASSERT(false);\n"
      result << "    return boost::none;\n"
      result << "  }\n\n"

      result << "  std::vector<ObjectRecord> " << @className << "_Impl::children() const {\n"
      result << "    // Return this object's children. See ComponentReferenceRecord_Impl for an example.\n"
      result << "    OS_ASSERT(false);\n"
      result << "    ObjectRecordVector result;\n"
      result << "    return result;\n"
      result << "  }\n\n"

      result << "  std::vector<ObjectRecord> " << @className << "_Impl::resources() const {\n"
      result << "    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl\n"
      result << "    // for an example.\n"
      result << "    OS_ASSERT(false);\n"
      result << "    ObjectRecordVector result;\n"
      result << "    return result;\n"
      result << "  }\n\n"

      result << "  std::vector<JoinRecord> " << @className << "_Impl::joinRecords() const {\n"
      result << "    // Return the join relationships between this object and others. See\n"
      result << "    // ModelObjectActionSetRelationshipRecord_Impl for an example.\n"
      result << "    OS_ASSERT(false);\n"
      result << "    JoinRecordVector result;\n"
      result << "    return result;\n"
      result << "  }\n\n"

    end

    result << "  void " << @className << "_Impl::saveRow(const std::shared_ptr<QSqlDatabase>& database) {\n"
    result << "    QSqlQuery query(*database);\n"
    result << "    this->makeUpdateByIdQuery<" << @className << ">(query);\n"
    result << "    this->bindValues(query);\n"
    result << "    assertExec(query);\n"
    result << "  }\n\n"

    if @joinRecord

      result << "  ObjectRecord " << @className << "_Impl::leftObject() const {\n"
      result << "    return this->" << @leftArgument << "();\n"
      result << "  }\n\n"

      result << "  ObjectRecord " << @className << "_Impl::rightObject() const {\n"
      result << "    return this->" << @rightArgument << "();\n"
      result << "  }\n\n"

      result << "  " << @leftJoinClass << "Record " << @className << "_Impl::" << @leftArgument << "() const {\n"
      result << "    ProjectDatabase database = this->projectDatabase();\n"
      result << "    boost::optional<" << @leftJoinClass << "Record> " << @leftArgument << " = " << @leftJoinClass << "Record::get" << @leftJoinClass << "Record(this->leftId(),database);\n"
      result << "    OS_ASSERT(" << @leftArgument << ");\n"
      result << "    return *" << @leftArgument << ";\n"
      result << "  }\n\n"

      result << "  " << @rightJoinClass << "Record " << @className << "_Impl::" << @rightArgument << "() const {\n"
      result << "    ProjectDatabase database = this->projectDatabase();\n"
      result << "    boost::optional<" << @rightJoinClass << "Record> " << @rightArgument << " = " << @rightJoinClass << "Record::get" << @rightJoinClass << "Record(this->rightId(),database);\n"
      result << "    OS_ASSERT(" << @rightArgument << ");\n"
      result << "    return *" << @rightArgument << ";\n"
      result << "  }\n\n"

    else

      result << "  NAMESPACE::" << @serializedClass << " " << @className << "_Impl::" << @serializedClassArgument
      result << "() const {\n"
      result << "    // TODO: De-serialize the object here.\n"
      result << "    OS_ASSERT(false);\n"
      result << "  }\n\n"

    end

    return result
  end

  def cppPublicClassPublicMethods()
    result = String.new

    # static methods

    if ((@baseClassName == "ObjectRecord") or (@baseClassName == "JoinRecord"))
      result << "std::string " << @className << "::databaseTableName() {\n"
      result << "  return \"" << @className << "s\";\n"
      result << "}\n\n"

      result << "UpdateByIdQueryData UrlRecord::updateByIdQueryData() {\n"
      result << "  static UpdateByIdQueryData result;\n"
      result << "  if (result.queryString.empty()) {\n"
      result << "    // numeric column identifiers\n"
      result << "    result.columnValues = ColumnsType::getValues();\n"
      result << "\n"
      result << "    // query string\n"
      result << "    std::stringstream ss;\n"
      result << "    ss << \"UPDATE \" << databaseTableName() << \" SET \";\n"
      result << "    int expectedValue = 0;\n"
      result << "    for (std::set<int>::const_iterator it = result.columnValues.begin(),\n"
      result << "         itend = result.columnValues.end(); it != itend; ++it)\n"
      result << "    {\n"
      result << "      // require 0 based columns, don't skip any\n"
      result << "      OS_ASSERT(*it == expectedValue);\n"
      result << "      // column name is name, type is description\n"
      result << "      ss << ColumnsType::valueName(*it) << \"=:\" << ColumnsType::valueName(*it);\n"
      result << "      // is this the last column?\n"
      result << "      std::set<int>::const_iterator nextIt = it;\n"
      result << "      ++nextIt;\n"
      result << "      if (nextIt == itend) {\n"
      result << "        ss << \" \";\n"
      result << "      }\n"
      result << "      else {\n"
      result << "        ss << \", \";\n"
      result << "      }\n"
      result << "      ++expectedValue;\n"
      result << "    }\n"
      result << "    ss << \"WHERE id=:id\";\n"
      result << "    result.queryString = ss.str();\n"
      result << "\n"
      result << "    // null values\n"
      result << "    for (std::set<int>::const_iterator it = result.columnValues.begin(),\n"
      result << "         itend = result.columnValues.end(); it != itend; ++it)\n"
      result << "    {\n"
      result << "      // bind all values to avoid parameter mismatch error\n"
      result << "      if (istringEqual(ColumnsType::valueDescription(*it), \"INTEGER\")) {\n"
      result << "        result.nulls.push_back(QVariant(QVariant::Int));\n"
      result << "      }\n"
      result << "      else {\n"
      result << "        result.nulls.push_back(QVariant(QVariant::String));\n"
      result << "      }\n"
      result << "    }\n"
      result << "  }\n"
      result << "  return result;\n"
      result << "}\n\n"

    end

    if (@baseClassName == "ObjectRecord")

      result << "void " << @className << "::updatePathData(ProjectDatabase database,\n"
      result << " " * (22 + @className.length) << "const openstudio::path& originalBase,\n"
      result << " " * (22 + @className.length) << "const openstudio::path& newBase)\n"
      result << "{\n"
      result << "  // TODO: Delete implementation if no classes in this hierarchy contain path data.\n"
      result << "  // TODO: Override virtual ObjectRecord_Impl method updatePathData for any classes in this\n"
      result << "  // hierarchy that do contain path data. See ProjectDatabaseRecord_Impl for an example. \n"
      result << "  " << @className << "Vector records = get" << @className << "s(database);\n"
      result << "  BOOST_FOREACH(" << @className << "& record,records) {\n"
      result << "    record.getImpl<detail::" << @className << "_Impl>()->updatePathData(originalBase,newBase);\n"
      result << "  }\n"
      result << "}\n\n"

    end

    if @objectRecord
      result << "boost::optional<" << @className << "> " << @className << "::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)\n"
      result << "{\n"
      result << "  Optional" << @className << " result;\n\n"
      result << "  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.\n"
      result << "  // int " << @classArgument << "Type = query.value("
      result << @className << "Columns::" << @classArgument << "Type).toInt();\n\n"
      result << "  // switch (" << @classArgument << "Type) {\n"
      result << "  //   case " << @className << "Type::FIRSTDERIVEDTYPE : \n"
      result << "  //     result = FIRSTDERIVEDTYPE(query, database).cast<" << @className << ">();\n"
      result << "  //    break;\n"
      result << "  //   default :\n"
      result << "  //     LOG(Error,\"Unknown " << @className << "Type \" << " << @classArgument << "Type);\n"
      result << "  //     return boost::none;\n"
      result << "  // }\n\n"
      result << "  // Template for classes with no derived classes.\n"
      result << "  // try {\n"
      result << "  //   result = " << @className << "(query,database);\n"
      result << "  // }\n"
      result << "  // catch (const std::exception& e) {\n"
      result << "  //   LOG(Error,\"Unable to construct " << @className << " from query, because '\"\n"
      result << "  //       << e.what() << \"'.\");\n"
      result << "  // }\n\n"
      result << "  return result;\n"
      result << "}\n\n"

      result << @className << " " << @className << "::factoryFrom" << @serializedClass << "(const NAMESPACE::"
      result << @serializedClass << "& " << @serializedClassArgument
      result << ", ProjectDatabase& database)\n"
      result << "{\n"
      result << "  // TODO: Delete if no derived classes.\n"
      result << "  OS_ASSERT(false);\n\n"
      result << "  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.\n\n"
      result << "  // if (" << @serializedClassArgument << ".optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {\n"
      result << "  //   return FIRST_DERIVED_CLASSRecord(" << @serializedClassArgument << ".cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);\n"
      result << "  // else if {\n"
      result << "  //   ...\n"
      result << "  // }\n\n"
      result << "  OS_ASSERT(false);\n"
      result << "  return " << @className << "(std::shared_ptr<detail::" << @className << "_Impl>());\n"
      result << "}\n\n"

    end

    if @joinRecord

      result << "std::vector<" << @rightJoinClass << "Record> " << @className << "::get" << @rightJoinClass
      result << "Records(const " << @leftJoinClass << "Record& "
      result << @leftArgument << ") {\n"
      result << "  ProjectDatabase database = " << @leftArgument << ".projectDatabase();\n\n"
      result << "  std::vector<" << @rightJoinClass << "Record> result;\n"
      result << "  std::vector<int> " << @rightArgument << "Ids;\n\n"
      result << "  QSqlQuery query(*database.qSqlDatabase()));\n"
      result << "  query.prepare(toQString(\"SELECT * FROM \" + " << @className << "::databaseTableName() + \" WHERE leftId=:leftId\"));\n"
      result << "  query.bindValue(\":leftId\", " << @leftArgument << ".id());\n"
      result << "  assertExec(query);\n"
      result << "  while (query.next()) {\n"
      result << "    QVariant value;\n"
      result << "    value = query.value(JoinRecordColumns::rightId);\n"
      result << "    OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    " << @rightArgument << "Ids.push_back(value.toInt());\n"
      result << "  }\n\n"
      result << "  BOOST_FOREACH(int id, " << @rightArgument << "Ids) {\n"
      result << "    boost::optional<" << @rightJoinClass << "Record> " << @rightArgument << " = "
      result << @rightJoinClass << "Record::get" << @rightJoinClass << "Record(id,database);\n"
      result << "    OS_ASSERT(" << @rightArgument << ");\n"
      result << "    result.push_back(*" << @rightArgument << ");\n"
      result << "  }\n\n"
      result << "  return result;\n"
      result << "}\n\n"

      result << "std::vector<" << @leftJoinClass << "Record> " << @className << "::get" << @leftJoinClass
      result << "Records(const " << @rightJoinClass << "Record& "
      result << @rightArgument << ") {\n"
      result << "  ProjectDatabase database = " << @rightArgument << ".projectDatabase();\n\n"
      result << "  std::vector<" << @leftJoinClass << "Record> result;\n"
      result << "  std::vector<int> " << @leftArgument << "Ids;\n\n"
      result << "  QSqlQuery query(*database.qSqlDatabase()));\n"
      result << "  query.prepare(toQString(\"SELECT * FROM \" + " << @className << "::databaseTableName() + \" WHERE rightId=:rightId\"));\n"
      result << "  query.bindValue(\":rightId\", " << @rightArgument << ".id());\n"
      result << "  assertExec(query);\n"
      result << "  while (query.next()) {\n"
      result << "    QVariant value;\n"
      result << "    value = query.value(JoinRecordColumns::leftId);\n"
      result << "    OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    " << @leftArgument << "Ids.push_back(value.toInt());\n"
      result << "  }\n\n"
      result << "  BOOST_FOREACH(int id, " << @leftArgument << "Ids) {\n"
      result << "    boost::optional<" << @leftJoinClass << "Record> " << @leftArgument << " = "
      result << @leftJoinClass << "Record::get" << @leftJoinClass << "Record(id,database);\n"
      result << "    OS_ASSERT(" << @leftArgument << ");\n"
      result << "    result.push_back(*" << @leftArgument << ");\n"
      result << "  }\n\n"
      result << "  return result;\n"
      result << "}\n\n"

    else

      result << "std::vector<" << @className << "> " << @className << "::get" << @className
      result << "s(ProjectDatabase& database) {\n"
      result << "  std::vector<" << @className << "> result;\n\n"
      result << "  QSqlQuery query(*(database.qSqlDatabase()));\n"
      result << "  // TODO: Check class used to determine databaseTableName().\n"
      result << "  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.\n"
      result << "  query.prepare(toQString(\"SELECT * FROM \" + "
      if (@baseClassName == "ObjectRecord")
        result << @className
      else
        result << @baseClassName
      end
      result << "::databaseTableName()"
      if (@baseClassName != "ObjectRecord")
        # select records by type
        result << " + \" WHERE " << @baseClassArgument << "Type=:" << @baseClassArgument << "Type\""
      end
      result << "));\n"
      if (@baseClassName != "ObjectRecord")
        result << "  query.bindValue(\":" << @baseClassArgument << "Type\", " << @baseClassName << "Type::" << @className << ");\n"
      end
      result << "  assertExec(query);\n"
      result << "  while (query.next()) {\n"
      result << "    // TODO: Choose appropriate implementation.\n\n"
      result << "    // Optional" << @className << " " << @classArgument << " = " << @className << "::factoryFromQuery(query, database);\n"
      result << "    // if (" << @classArgument << ") {\n"
      result << "    //   result.push_back(*" << @classArgument << ");\n"
      result << "    // }\n\n"
      result << "    // result.push_back(" << @className << "(query, database));\n"
      result << "  }\n\n"
      result << "  return result;\n"
      result << "}\n\n"

      result << "boost::optional<" << @className << "> " << @className << "::get" << @className
      result << "(int id, ProjectDatabase& database) {\n"
      result << "  boost::optional<" << @className << "> result;\n\n"
      result << "  QSqlQuery query(*(database.qSqlDatabase()));\n"
      result << "  // TODO: Check class used to determine databaseTableName().\n"
      result << "  // TODO: Check the WHERE portion of the query.\n"
      result << "  query.prepare(toQString(\"SELECT * FROM \" + "
      if (@baseClassName == "ObjectRecord")
        result << @className
      else
        result << @baseClassName
      end
      result << "::databaseTableName() + \" WHERE "
      if (@baseClassName != "ObjectRecord")
        # select records by type
        result << @baseClassArgument << "Type=:" << @baseClassArgument << "Type AND "
      end
      result << "id=:id\"));\n"
      if (@baseClassName != "ObjectRecord")
        result << "  query.bindValue(\":" << @baseClassArgument << "Type\", " << @baseClassName << "Type::" << @className << ");\n"
      end
      result << "  query.bindValue(\":id\",id);\n"
      result << "  assertExec(query);\n"
      result << "  if (query.first()) {\n"
      result << "    // TODO: Choose appropriate implementation.\n\n"
      result << "    // result = " << @className << "::factoryFromQuery(query, database);\n\n"
      result << "    // result = " << @className << "(query, database);\n"
      result << "  }\n\n"
      result << "  return result;\n"
      result << "}\n\n"

    end

    # public methods on object

    if @joinRecord

      result << @leftJoinClass << "Record " << @className << "::" << @leftArgument << "() const {\n"
      result << "  return getImpl<detail::" << @className << "_Impl>()->" << @leftArgument << "();\n"
      result << "}\n\n"

      result << @rightJoinClass << "Record " << @className << "::" << @rightArgument << "() const {\n"
      result << "  return getImpl<detail::" << @className << "_Impl>()->" << @rightArgument << "();\n"
      result << "}\n\n"

    else

      result << "NAMESPACE::" << @serializedClass << " " << @className << "::" << @serializedClassArgument
      result << "() const {\n"
      result << "  return getImpl<detail::" << @className << "_Impl>()->" << @serializedClassArgument
      result << "();\n"
      result << "}\n\n"

    end

    return result
  end

  def hppProtectedFriends()
    result = String.new

    if pImpl
      result << "  friend class Record;\n"
      result << "  friend class ProjectDatabase;\n"
    end

    return result
  end

  def hppProtectedMethods()
    result = String.new

    if pImpl
      result << "\n"
      result << "  /** Construct from impl. */\n"
      result << "  " << @className << "(std::shared_ptr<detail::" << @className << "_Impl> impl,\n"
      result << "  " << " " * @className.size << " ProjectDatabase database);\n\n"
    end

    return result
  end

  def implHppProtectedMethods()
    result = String.new

    if @objectRecord

      result << "    /** Bind data member values to a query for saving. */\n"
      result << "    virtual void bindValues(QSqlQuery& query) const;\n\n"

      result << "    /** Set the last state of this object from the query (including id). */\n"
      result << "    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);\n\n"

      result << "    /** Check that values (except id) are same as query. */\n"
      result << "    virtual bool compareValues(const QSqlQuery& query) const;\n\n"

      result << "    /** Save values to last state. */\n"
      result << "    virtual void saveLastValues();\n\n"

      result << "    /** Revert values back to last state. */\n"
      result << "    virtual void revertToLastValues();\n\n"

    end

    return result
  end

  def cppProtectedMethods()
    result = String.new

    if @objectRecord

      result << "  void " << @className << "_Impl::bindValues(QSqlQuery& query) const {\n"
      result << "    " << @baseClassName << "_Impl::bindValues(query);\n\n"
      result << "    // TODO: Delete bind for enum if no derived classes.\n"
      result << "    query.bindValue(" << @className << "::ColumnsType::" << @classArgument
          result << "Type,m_" << @classArgument << "Type.value());\n"
      result << "    // Template for required data.\n"
      result << "    // query.bindValue(" << @className << "::ColumnsType::DATAMEMBERNAME,"
          result << "m_DATAMEMBERNAME);\n"
      result << "    // Template for optional data.\n"
      result << "    // if (m_DATAMEMBERNAME) {\n"
      result << "    //   query.bindValue(" << @className << "::ColumnsType::DATAMEMBERNAME,"
          result << "*m_DATAMEMBERNAME);\n"
      result << "    // }\n"
      result << "    // else {\n"
      result << "    //   query.bindValue(" << @className << "::ColumnsType::DATAMEMBERNAME,"
          result << "QVariant(QVariant::TYPE));\n"
      result << "    // }\n"
      result << "  }\n\n"

      result << "  void " << @className << "_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {\n"
      result << "    OS_ASSERT(query.isValid());\n"
      result << "    OS_ASSERT(query.isActive());\n"
      result << "    OS_ASSERT(query.isSelect());\n\n"
      result << "    " << @baseClassName << "_Impl::setLastValues(query,projectDatabase);\n\n"
      result << "    QVariant value;\n\n"
      result << "    // TODO: Delete if no derived classes.\n"
      result << "    value = query.value(" << @className << "::ColumnsType::" << @classArgument
          result << "Type);\n"
      result << "    OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    m_last" << @className << "Type = " << @className << "Type(value.toInt());\n\n"
      result << "    // Template for required data.\n"
      result << "    // value = query.value(" << @className << "::ColumnsType::DATAMEMBERNAME);\n"
      result << "    // OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    // m_lastDATAMEMBERNAME = value.toTYPE();\n\n"
      result << "    // Template for optional data.\n"
      result << "    // value = query.value(" << @className << "::ColumnsType::DATAMEMBERNAME);\n"
      result << "    // if (value.isValid() && !value.isNull()) {\n"
      result << "    //   m_lastDATAMEMBERNAME = value.toTYPE();\n"
      result << "    // }\n"
      result << "    // else {\n"
      result << "    //   m_lastDATAMEMBERNAME.reset();\n"
      result << "    // }\n"
      result << "  }\n\n"

      result << "  bool " << @className << "_Impl::compareValues(const QSqlQuery& query) const {\n"
      result << "    OS_ASSERT(query.isValid());\n"
      result << "    OS_ASSERT(query.isActive());\n"
      result << "    OS_ASSERT(query.isSelect());\n\n"
      result << "    bool result = " << @baseClassName << "_Impl::compareValues(query);\n\n"
      result << "    QVariant value;\n\n"
      result << "    // TODO: Delete if no derived classes.\n"
      result << "    value = query.value(" << @className << "::ColumnsType::" << @classArgument
          result << "Type);\n"
      result << "    OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    result = result && (m_" << @classArgument << "Type == " << @className
          result << "Type(value.toInt()));\n\n"
      result << "    // Template for required data.\n"
      result << "    // value = query.value(" << @className << "::ColumnsType::DATAMEMBERNAME);\n"
      result << "    // OS_ASSERT(value.isValid() && !value.isNull());\n"
      result << "    // result = result && (m_DATAMEMBERNAME == value.toTYPE());\n\n"
      result << "    // Template for optional data.\n"
      result << "    // value = query.value(" << @className << "::ColumnsType::DATAMEMBERNAME);\n"
      result << "    // if (value.isValid() && !value.isNull()) {\n"
      result << "    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());\n"
      result << "    // }\n"
      result << "    // else {\n"
      result << "    //   result = result && !m_DATAMEMBERNAME;\n"
      result << "    // }\n\n"
      result << "    return result;\n"
      result << "  }\n\n"

      result << "  void " << @className << "_Impl::saveLastValues() {\n"
      result << "    " << @baseClassName << "_Impl::saveLastValues();\n\n"
      result << "    // TODO: Delete if no derived types.\n"
      result << "    m_last" << @className << "Type = m_" << @classArgument << "Type;\n"
      result << "    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;\n"
      result << "  }\n\n"

      result << "  void " << @className << "_Impl::revertToLastValues() {\n"
      result << "    " << @baseClassName << "_Impl::revertToLastValues();\n\n"
      result << "    // TODO: Delete if no derived types.\n"
      result << "    m_" << @classArgument << "Type = m_last" << @className << "Type;\n"
      result << "    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;\n"
      result << "  }\n\n"

    end

    return result
  end

  def cppPublicClassProtectedMethods()
    result = String.new

    if pImpl
      constructorPreamble = String.new
      constructorPreamble << @className << "::" << @className << "("

      # construct from impl and database
      result << "\n"
      result << constructorPreamble << "std::shared_ptr<detail::" << @className << "_Impl> impl,\n"
      result << " " * constructorPreamble.size << "ProjectDatabase database)\n"
      result << "  : " << @baseClassName << "(impl, database)\n"
      result << "{\n"
      result << "  OS_ASSERT(getImpl<detail::" << @className << "_Impl>());\n"
      result << "}\n"
    end

    return result
  end

  def implHppPrivateMethods()
    result = String.new

    if @objectRecord

      result << "\n"
      result << "    // TODO: Delete enums if no derived classes.\n"
      result << "    " << @className << "Type m_" << @classArgument << "Type;\n\n"
      result << "    " << @className << "Type m_last" << @className << "Type;\n"

    end

    return result
  end

end
