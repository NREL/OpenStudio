/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
