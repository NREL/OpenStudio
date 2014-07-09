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
    virtual std::string databaseTableName() const;

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Returns join relationships between this object and others. Such relationships will be
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

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
    virtual void bindValues(QSqlQuery& query) const;

    /** Set the last state of this object from the query (including id). */
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /** Check that values (except id) are same as query. */
    virtual bool compareValues(const QSqlQuery& query) const;

    /** Save values to last state. */
    virtual void saveLastValues();

    /** Revert values back to last state. */
    virtual void revertToLastValues();

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
