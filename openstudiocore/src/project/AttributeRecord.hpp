/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef PROJECT_ATTRIBUTERECORD_HPP
#define PROJECT_ATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"

namespace openstudio {

class Attribute;
class AttributeValueType;
class Quantity;
class Unit;

namespace project {

class FileReferenceRecord;
class AlgorithmRecord;
class VariableRecord;
class DataPointRecord;

namespace detail {
  class AttributeRecord_Impl;
}

/** \class AttributeRecordColumns
 *  \brief Column definitions for the AttributeRecords table.
 *
 *  \relates AttributeRecord */
OPENSTUDIO_ENUM( AttributeRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4)) 
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((fileReferenceRecordId)(INTEGER)(8))
  ((parentAttributeRecordId)(INTEGER)(9))
  ((attributeValueType)(INTEGER)(10))
  ((attributeValue)(TEXT)(11))
  ((attributeUnits)(TEXT)(12))
  ((attributeUnitSystem)(TEXT)(13))
  ((attributeVectorIndex)(INTEGER)(14))
  ((algorithmRecordId)(INTEGER)(15))
  ((variableRecordId)(INTEGER)(16))
  ((source)(TEXT)(17))
  ((dataPointRecordId)(INTEGER)(18))
);

/** AttributeRecord is an ObjectRecord that serializes an Attribute. An Attribute is a name-value 
 *  pair where the value can be a bool, double, int, string, or vector of other attributes. The 
 *  ability for an attribute value to be a vector of other attributes allows for hierarchical 
 *  attribute structures. */
class PROJECT_API AttributeRecord : public ObjectRecord {
 public:

  typedef detail::AttributeRecord_Impl ImplType;
  typedef AttributeRecordColumns ColumnsType;
  typedef AttributeRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  AttributeRecord(const Attribute& attribute, 
                  const FileReferenceRecord& fileReferenceRecord);

  AttributeRecord(const Attribute& attribute,
                  const AttributeRecord& parentAttributeRecord,
                  int attributeVectorIndex);

  AttributeRecord(const Attribute& attribute,
                  const AlgorithmRecord& algorithmRecord);

  AttributeRecord(const Attribute& attribute,
                  const VariableRecord& variableRecord);

  AttributeRecord(const Attribute& attribute,
                  const DataPointRecord& dataPointRecord);

  /// constructor from query, throws if bad query
  AttributeRecord(const QSqlQuery& query, ProjectDatabase& database);


  virtual ~AttributeRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<AttributeRecord> factoryFromQuery(const QSqlQuery& query, 
      ProjectDatabase& database);

  static std::vector<AttributeRecord> getAttributeRecords(ProjectDatabase& database);

  static boost::optional<AttributeRecord> getAttributeRecord(int id, ProjectDatabase& database);

  /** Getters */
  //@{

  /** Returns the FileReferenceRecord that parents this AttributeRecord. */
  boost::optional<FileReferenceRecord> fileReferenceRecord() const;

  /** Returns the AttributeRecord that parents this AttributeRecord. */
  boost::optional<AttributeRecord> parentAttributeRecord() const;

  /** Returns the AlgorithmRecord that parents this AttributeRecord. */
  boost::optional<AlgorithmRecord> algorithmRecord() const;

  /** Returns the VariableRecord that parents this AttributeRecord. */
  boost::optional<VariableRecord> variableRecord() const;
  
  /** Returns the DataPointRecord that parents this AttributeRecord. */
  boost::optional<DataPointRecord> dataPointRecord() const;

  // for verifying after construction
  boost::optional<int> parentAttributeRecordId() const;

  /// get the attribute value type
  AttributeValueType attributeValueType() const;           

  /// get value as a bool
  bool attributeValueAsBoolean() const;

  /// get value as int
  int attributeValueAsInteger() const;

  /// get value as unsigned
  unsigned attributeValueAsUnsigned() const;

  /// get value as double
  double attributeValueAsDouble() const;

  /// get value as Quantity
  Quantity attributeValueAsQuantity() const;

  /// get value as Unit
  Unit attributeValueAsUnit() const;

  /// get value as string
  std::string attributeValueAsString() const;

  /// get value as attribute vector
  std::vector<Attribute> attributeValueAsAttributeVector() const;

  /// get the attribute units
  boost::optional<std::string> attributeUnits() const;

  /// get equivalent attribute
  Attribute attribute() const;

  //@}
  /** @name Setters */
  //@{

  // for setting after construction
  bool setParentAttributeRecordId(int parentAttributeRecordId);

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::AttributeRecord_Impl;

  // from impl
  AttributeRecord(std::shared_ptr<detail::AttributeRecord_Impl> impl,
      ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit AttributeRecord(std::shared_ptr<detail::AttributeRecord_Impl> impl);

  /// @endcond

  void constructRelatedRecords(const Attribute& attribute);
 private:
  REGISTER_LOGGER("openstudio.project.AttributeRecord");
};

/** \relates AttributeRecord*/
typedef boost::optional<AttributeRecord> OptionalAttributeRecord;

/** \relates AttributeRecord*/
typedef std::vector<AttributeRecord> AttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_ATTRIBUTERECORD_HPP
