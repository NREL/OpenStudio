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

#ifndef PROJECT_ATTRIBUTERECORD_IMPL_HPP
#define PROJECT_ATTRIBUTERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "../utilities/data/Attribute.hpp"

#include <QVariant>

class QSqlQuery;

namespace openstudio {

class Quantity;
class Unit;

namespace project {

class AttributeRecord;
class FileReferenceRecord;
class AlgorithmRecord;
class VariableRecord;

namespace detail {

  /** AttributeRecord_Impl is a ObjectRecord_Impl that is the implementation class for AttributeRecord.*/
  class PROJECT_API AttributeRecord_Impl : public ObjectRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AttributeRecord_Impl(const Attribute& attribute,
                         const FileReferenceRecord& fileReferenceRecord);

    AttributeRecord_Impl(const Attribute& attribute,
                         const AttributeRecord& parentAttributeRecord,
                         int attributeVectorIndex);

    AttributeRecord_Impl(const Attribute& attribute,
                         const AlgorithmRecord& algorithmRecord);

    AttributeRecord_Impl(const Attribute& attribute,
                         const VariableRecord& variableRecord);

    AttributeRecord_Impl(const Attribute& attribute,
                         const DataPointRecord& dataPointRecord);

    /// constructor from sql query, throws if bad query
    AttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database);

    virtual ~AttributeRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the name of the database table for this record. Should be implemented by the base
     *  class for the table. */
    virtual std::string databaseTableName() const override;

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const override;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const override;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const override;

    /** Returns join relationships between this object and others. Such relationships will be
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const override;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database) override;

    //@}
    /** @name Getters */
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
    /** Bind data member values to a query for saving. */
    virtual void bindValues(QSqlQuery& query) const override;

    /** Set the last state of this object from the query (including id). */
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) override;

    /** Check that values (except id) are same as query. */
    virtual bool compareValues(const QSqlQuery& query) const override;

    /** Save values to last state. */
    virtual void saveLastValues() override;

    /** Revert values back to last state. */
    virtual void revertToLastValues() override;

   private:
    REGISTER_LOGGER("openstudio.project.AttributeRecord");

    boost::optional<int> m_fileReferenceRecordId;
    boost::optional<int> m_parentAttributeRecordId;
    AttributeValueType m_attributeValueType;
    QVariant m_attributeValue;                      // value is just for this row
    boost::optional<std::string> m_attributeUnits;
    boost::optional<std::string> m_attributeUnitSystem;
    boost::optional<int> m_attributeVectorIndex;
    boost::optional<int> m_algorithmRecordId;
    boost::optional<int> m_variableRecordId;
    std::string m_source;
    boost::optional<int> m_dataPointRecordId;

    boost::optional<int> m_lastFileReferenceRecordId;
    boost::optional<int> m_lastParentAttributeRecordId;
    AttributeValueType m_lastAttributeValueType;
    QVariant m_lastAttributeValue;
    boost::optional<std::string> m_lastAttributeUnits;
    boost::optional<std::string> m_lastAttributeUnitSystem;
    boost::optional<int> m_lastAttributeVectorIndex;
    boost::optional<int> m_lastAlgorithmRecordId;
    boost::optional<int> m_lastVariableRecordId;
    std::string m_lastSource;
    boost::optional<int> m_lastDataPointRecordId;

    void storeAttribute(const Attribute& attribute);
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_ATTRIBUTERECORD_IMPL_HPP
