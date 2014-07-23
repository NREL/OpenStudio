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

#include "AttributeRecord.hpp"
#include "AttributeRecord_Impl.hpp"

#include "ProjectDatabase.hpp"
#include "FileReferenceRecord.hpp"
#include "JoinRecord.hpp"
#include "AlgorithmRecord.hpp"
#include "VariableRecord.hpp"
#include "DataPointRecord.hpp"

#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/Quantity.hpp"

#include "../utilities/core/Assert.hpp"

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

int __type__ = qRegisterMetaType< std::vector<openstudio::project::AttributeRecord> >(
    "std::vector<openstudio::project::AttributeRecord>");

namespace openstudio {
namespace project {

namespace detail{

  // constructors
  AttributeRecord_Impl::AttributeRecord_Impl(const Attribute& attribute,
                                             const FileReferenceRecord& fileReferenceRecord)
    : ObjectRecord_Impl(fileReferenceRecord.projectDatabase(),
                        attribute.uuid(),
                        attribute.name(),
                        attribute.displayName() ? attribute.displayName().get() : std::string(),
                        "",
                        attribute.versionUUID()),
      m_fileReferenceRecordId(fileReferenceRecord.id()),
      m_attributeValueType(attribute.valueType()),
      m_attributeValue(attribute.valueAsQVariant()),
      m_attributeUnits(attribute.units()),
      m_source(attribute.source())
  {
    storeAttribute(attribute);
  }

  AttributeRecord_Impl::AttributeRecord_Impl(const Attribute& attribute,
                                             const AttributeRecord& parentAttributeRecord,
                                             int attributeVectorIndex)
    : ObjectRecord_Impl(parentAttributeRecord.projectDatabase(),
                        attribute.uuid(),
                        attribute.name(),
                        attribute.displayName() ? attribute.displayName().get() : std::string(),
                        "",
                        attribute.versionUUID()),
      m_parentAttributeRecordId(parentAttributeRecord.id()),
      m_attributeValueType(attribute.valueType()),
      m_attributeValue(attribute.valueAsQVariant()),
      m_attributeUnits(attribute.units()),
      m_attributeVectorIndex(attributeVectorIndex),
      m_source(attribute.source())
  {
    storeAttribute(attribute);
  }

  AttributeRecord_Impl::AttributeRecord_Impl(const Attribute& attribute,
                                             const AlgorithmRecord& algorithmRecord)
    : ObjectRecord_Impl(algorithmRecord.projectDatabase(),
                        attribute.uuid(),
                        attribute.name(),
                        attribute.displayName() ? attribute.displayName().get() : std::string(),
                        "",
                        attribute.versionUUID()),
      m_attributeValueType(attribute.valueType()),
      m_attributeValue(attribute.valueAsQVariant()),
      m_attributeUnits(attribute.units()),
      m_algorithmRecordId(algorithmRecord.id()),
      m_source(attribute.source())
  {
    storeAttribute(attribute);
  }

  AttributeRecord_Impl::AttributeRecord_Impl(const Attribute& attribute,
                                             const VariableRecord& variableRecord)
    : ObjectRecord_Impl(variableRecord.projectDatabase(),
                        attribute.uuid(),
                        attribute.name(),
                        attribute.displayName() ? attribute.displayName().get() : std::string(),
                        "",
                        attribute.versionUUID()),
      m_attributeValueType(attribute.valueType()),
      m_attributeValue(attribute.valueAsQVariant()),
      m_attributeUnits(attribute.units()),
      m_variableRecordId(variableRecord.id()),
      m_source(attribute.source())
  {
    storeAttribute(attribute);
  }

  AttributeRecord_Impl::AttributeRecord_Impl(const Attribute& attribute,
                                             const DataPointRecord& dataPointRecord)
    : ObjectRecord_Impl(dataPointRecord.projectDatabase(),
                        attribute.uuid(),
                        attribute.name(),
                        attribute.displayName() ? attribute.displayName().get() : std::string(),
                        "",
                        attribute.versionUUID()),
      m_attributeValueType(attribute.valueType()),
      m_attributeValue(attribute.valueAsQVariant()),
      m_attributeUnits(attribute.units()),
      m_dataPointRecordId(dataPointRecord.id()),
      m_source(attribute.source())
  {
    storeAttribute(attribute);
  }

  void AttributeRecord_Impl::storeAttribute(const Attribute& attribute) {
    if (attribute.valueType() == AttributeValueType::AttributeVector) {
      // only keep value for this row
      m_attributeValue.clear();
    }
    if (attribute.valueType() == AttributeValueType::Quantity) {
      Quantity q = attribute.valueAsQuantity();
      m_attributeValue = q.value();
      m_attributeUnits = q.standardUnitsString();
      m_attributeUnitSystem = q.system().valueName();
    }
    if (attribute.valueType() == AttributeValueType::Unit) {
      Unit u = attribute.valueAsUnit();
      m_attributeValue.clear();
      m_attributeUnits = u.standardString();
      m_attributeUnitSystem = u.system().valueName();
    }
  }

  AttributeRecord_Impl::AttributeRecord_Impl(const QSqlQuery& query,
                                             const ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(AttributeRecordColumns::fileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
      m_fileReferenceRecordId = value.toInt();
    }
    else {
      m_fileReferenceRecordId.reset();
    }

    value = query.value(AttributeRecordColumns::parentAttributeRecordId);
    if (value.isValid() && !value.isNull()) {
      m_parentAttributeRecordId = value.toInt();
    }
    else {
      m_parentAttributeRecordId.reset();
    }

    value = query.value(AttributeRecordColumns::attributeValueType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeValueType = AttributeValueType(value.toInt());

    value = query.value(AttributeRecordColumns::attributeValue);
    switch(m_attributeValueType.value()) {
      case AttributeValueType::Boolean:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toBool();
        break;
      case AttributeValueType::Integer:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toInt();
        break;
      case AttributeValueType::Unsigned:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toUInt();
        break;
      case AttributeValueType::Double:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toDouble();
        break;
      case AttributeValueType::Quantity:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toDouble();
        break;
      case AttributeValueType::Unit:
        m_attributeValue.clear();
        break;
      case AttributeValueType::String:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue.setValue(value.toString().toStdString());
        break;
      case AttributeValueType::AttributeVector:
        // empty value
        m_attributeValue.clear();
        break;
      default:
        OS_ASSERT(false);
        break;
    }

    value = query.value(AttributeRecordColumns::attributeUnits);
    if(value.isValid() && !value.isNull()){
      std::string units = value.toString().toStdString();
      m_attributeUnits = units;
    }

    value = query.value(AttributeRecordColumns::attributeUnitSystem);
    if(value.isValid() && !value.isNull()) {
      m_attributeUnitSystem = value.toString().toStdString();
    }

    value = query.value(AttributeRecordColumns::attributeVectorIndex);
    if (value.isValid() && !value.isNull()) {
      m_attributeVectorIndex = value.toInt();
    }

    value = query.value(AttributeRecordColumns::algorithmRecordId);
    if (value.isValid() && !value.isNull()) {
      m_algorithmRecordId = value.toInt();
    }

    value = query.value(AttributeRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_variableRecordId = value.toInt();
    }

    value = query.value(AttributeRecordColumns::source);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_source = value.toString().toStdString();

    value = query.value(AttributeRecordColumns::dataPointRecordId);
    if (value.isValid() && !value.isNull()) {
      m_dataPointRecordId = value.toInt();
    }
  }

  std::string AttributeRecord_Impl::databaseTableName() const {
    return AttributeRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> AttributeRecord_Impl::parent() const {
    OptionalObjectRecord result;
    if (m_fileReferenceRecordId) {
      OS_ASSERT(!m_parentAttributeRecordId);
      OS_ASSERT(!m_algorithmRecordId);
      OS_ASSERT(!m_variableRecordId);
      OS_ASSERT(!m_dataPointRecordId);
      result = fileReferenceRecord().get().cast<ObjectRecord>();
    }
    if (m_parentAttributeRecordId) {
      OS_ASSERT(!m_algorithmRecordId);
      OS_ASSERT(!m_variableRecordId);
      OS_ASSERT(!m_dataPointRecordId);
      result = parentAttributeRecord().get().cast<ObjectRecord>();
    }
    if (m_algorithmRecordId) {
      OS_ASSERT(!m_variableRecordId);
      OS_ASSERT(!m_dataPointRecordId);
      result = algorithmRecord().get().cast<ObjectRecord>();
    }
    if (m_variableRecordId) {
      OS_ASSERT(!m_dataPointRecordId);
      result = variableRecord().get().cast<ObjectRecord>();
    }
    if (m_dataPointRecordId) {
      result = dataPointRecord().get().cast<ObjectRecord>();
    }
    // ETH@20110809 Should be able to assert result here, but don't want to add unnecessary
    // asserts at this time.
    return result;
  }

  std::vector<ObjectRecord> AttributeRecord_Impl::children() const {
    ObjectRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() +
        " WHERE parentAttributeRecordId=:id ORDER BY attributeVectorIndex"));
    query.bindValue(":id", this->id());
    assertExec(query);
    while (query.next()) {
      result.push_back(database.getFromQuery<AttributeRecord>(query));
    }

    return result;
  }

  std::vector<ObjectRecord> AttributeRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> AttributeRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void AttributeRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<AttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  boost::optional<FileReferenceRecord> AttributeRecord_Impl::fileReferenceRecord() const {
    OptionalFileReferenceRecord result;
    if (m_fileReferenceRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_fileReferenceRecordId,database);
    }
    return result;
  }

  boost::optional<AttributeRecord> AttributeRecord_Impl::parentAttributeRecord() const {
    OptionalAttributeRecord result;
    if (m_parentAttributeRecordId) {
      ProjectDatabase database = projectDatabase();
      result = AttributeRecord::getAttributeRecord(*m_parentAttributeRecordId,database);
    }
    return result;
  }

  boost::optional<int> AttributeRecord_Impl::parentAttributeRecordId() const {
    return m_parentAttributeRecordId;
  }

  boost::optional<AlgorithmRecord> AttributeRecord_Impl::algorithmRecord() const {
    OptionalAlgorithmRecord result;
    if (m_algorithmRecordId) {
      ProjectDatabase database = projectDatabase();
      result = AlgorithmRecord::getAlgorithmRecord(*m_algorithmRecordId,database);
    }
    return result;
  }

  boost::optional<VariableRecord> AttributeRecord_Impl::variableRecord() const {
    OptionalVariableRecord result;
    if (m_variableRecordId) {
      ProjectDatabase database = projectDatabase();
      result = VariableRecord::getVariableRecord(*m_variableRecordId,database);
    }
    return result;
  }

  boost::optional<DataPointRecord> AttributeRecord_Impl::dataPointRecord() const {
    OptionalDataPointRecord result;
    if (m_dataPointRecordId) {
      ProjectDatabase database = projectDatabase();
      result = DataPointRecord::getDataPointRecord(*m_dataPointRecordId,database);
    }
    return result;
  }

  bool AttributeRecord_Impl::setParentAttributeRecordId(int parentAttributeRecordId) {
    m_parentAttributeRecordId = parentAttributeRecordId;
    m_fileReferenceRecordId.reset();
    this->onChange(false);
    return true;
  }

  openstudio::AttributeValueType AttributeRecord_Impl::attributeValueType() const {
    return m_attributeValueType;
  }

  bool AttributeRecord_Impl::attributeValueAsBoolean() const {
    OS_ASSERT(m_attributeValue.isValid() && !m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::Boolean);
    return m_attributeValue.value<bool>();
  }

  int AttributeRecord_Impl::attributeValueAsInteger() const {
    OS_ASSERT(m_attributeValue.isValid() && !m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::Integer);
    return m_attributeValue.value<int>();
  }

  unsigned AttributeRecord_Impl::attributeValueAsUnsigned() const {
    OS_ASSERT(m_attributeValue.isValid() && !m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::Unsigned);
    return m_attributeValue.value<unsigned>();
  }

  double AttributeRecord_Impl::attributeValueAsDouble() const {
    OS_ASSERT(m_attributeValue.isValid() && !m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::Double);
    return m_attributeValue.value<double>();
  }

  Quantity AttributeRecord_Impl::attributeValueAsQuantity() const {
    OS_ASSERT(m_attributeValue.isValid() && !m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::Quantity);
    OS_ASSERT(m_attributeUnits);
    OS_ASSERT(m_attributeUnitSystem);
    double value = m_attributeValue.value<double>();
    Unit unit = createUnit(*m_attributeUnits,UnitSystem(*m_attributeUnitSystem)).get();
    return Quantity(value,unit);
  }

  Unit AttributeRecord_Impl::attributeValueAsUnit() const {
    OS_ASSERT(m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::Unit);
    OS_ASSERT(m_attributeUnits);
    OS_ASSERT(m_attributeUnitSystem);
    return createUnit(*m_attributeUnits,UnitSystem(*m_attributeUnitSystem)).get();
  }

  std::string AttributeRecord_Impl::attributeValueAsString() const {
    OS_ASSERT(m_attributeValue.isValid() && !m_attributeValue.isNull() && m_attributeValueType == AttributeValueType::String);
    return m_attributeValue.value<std::string>();
  }

  std::vector<Attribute> AttributeRecord_Impl::attributeValueAsAttributeVector() const {
    OS_ASSERT(m_attributeValueType == AttributeValueType::AttributeVector);

    std::vector<Attribute> result;
    for (const Record& child : this->children()){
      boost::optional<AttributeRecord> childAttribute = child.optionalCast<AttributeRecord>();
      if (childAttribute){
        result.push_back(childAttribute->attribute());
      }
    }

    return result;
  }

  boost::optional<std::string> AttributeRecord_Impl::attributeUnits() const {
    return m_attributeUnits;
  }

  openstudio::Attribute AttributeRecord_Impl::attribute() const {
    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsBoolean(), m_attributeUnits, m_source);
        break;
      case AttributeValueType::Integer:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsInteger(), m_attributeUnits, m_source);
        break;
      case AttributeValueType::Unsigned:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsUnsigned(), m_attributeUnits, m_source);
        break;
      case AttributeValueType::Double:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsDouble(), m_attributeUnits, m_source);
        break;
      case AttributeValueType::Quantity:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsQuantity(), m_source);
        break;
      case AttributeValueType::Unit:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsUnit(), m_source);
        break;
      case AttributeValueType::String:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsString(), m_attributeUnits, m_source);
        break;
      case AttributeValueType::AttributeVector:
        return openstudio::Attribute(handle(), uuidLast(), this->name(), this->displayName(), this->attributeValueAsAttributeVector(), m_attributeUnits, m_source);
        break;
      default:
        OS_ASSERT(false);
    }

    return openstudio::Attribute("","");
  }

  void AttributeRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    if (m_fileReferenceRecordId) {
      query.bindValue(AttributeRecordColumns::fileReferenceRecordId, *m_fileReferenceRecordId);
    }
    else {
      query.bindValue(AttributeRecordColumns::fileReferenceRecordId, QVariant(QVariant::Int));
    }

    if (m_parentAttributeRecordId) {
      query.bindValue(AttributeRecordColumns::parentAttributeRecordId, *m_parentAttributeRecordId);
    }
    else {
      query.bindValue(AttributeRecordColumns::parentAttributeRecordId, QVariant(QVariant::Int));
    }

    query.bindValue(AttributeRecordColumns::attributeValueType, m_attributeValueType.value());

    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        if (m_attributeValue.toBool()){
          query.bindValue(AttributeRecordColumns::attributeValue, "true");
        }else{
          query.bindValue(AttributeRecordColumns::attributeValue, "false");
        }
        break;
      case AttributeValueType::Integer:
        query.bindValue(AttributeRecordColumns::attributeValue, m_attributeValue.toInt());
        break;
      case AttributeValueType::Unsigned:
        query.bindValue(AttributeRecordColumns::attributeValue, m_attributeValue.toUInt());
        break;
      case AttributeValueType::Double:
        query.bindValue(AttributeRecordColumns::attributeValue, m_attributeValue.toDouble());
        break;
      case AttributeValueType::Quantity:
        query.bindValue(AttributeRecordColumns::attributeValue, m_attributeValue.toDouble());
        break;
      case AttributeValueType::Unit:
        query.bindValue(AttributeRecordColumns::attributeValue, QVariant());
        break;
      case AttributeValueType::String:
        query.bindValue(AttributeRecordColumns::attributeValue, toQString(m_attributeValue.value<std::string>()));
        break;
      case AttributeValueType::AttributeVector:
        query.bindValue(AttributeRecordColumns::attributeValue, QVariant());
        break;
      default:
        OS_ASSERT(false);
        break;
    }

    if (m_attributeUnits){
      query.bindValue(AttributeRecordColumns::attributeUnits, toQString(*m_attributeUnits));
    }else{
      query.bindValue(AttributeRecordColumns::attributeUnits, QVariant(QVariant::String));
    }

    if (m_attributeUnitSystem) {
      query.bindValue(AttributeRecordColumns::attributeUnitSystem, toQString(*m_attributeUnitSystem));
    }else{
      query.bindValue(AttributeRecordColumns::attributeUnitSystem, QVariant(QVariant::String));
    }

    if (m_attributeVectorIndex) {
      query.bindValue(AttributeRecordColumns::attributeVectorIndex, *m_attributeVectorIndex);
    }else{
      query.bindValue(AttributeRecordColumns::attributeVectorIndex, QVariant(QVariant::Int));
    }

    if (m_algorithmRecordId) {
      query.bindValue(AttributeRecordColumns::algorithmRecordId, *m_algorithmRecordId);
    }else{
      query.bindValue(AttributeRecordColumns::algorithmRecordId, QVariant(QVariant::Int));
    }

    if (m_variableRecordId) {
      query.bindValue(AttributeRecordColumns::variableRecordId, *m_variableRecordId);
    }
    else {
      query.bindValue(AttributeRecordColumns::variableRecordId, QVariant(QVariant::Int));
    }

    query.bindValue(AttributeRecordColumns::source, toQString(m_source));

    if (m_dataPointRecordId) {
      query.bindValue(AttributeRecordColumns::dataPointRecordId, *m_dataPointRecordId);
    }
    else {
      query.bindValue(AttributeRecordColumns::dataPointRecordId, QVariant(QVariant::Int));
    }
  }


  void AttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(AttributeRecordColumns::fileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastFileReferenceRecordId = value.toInt();
    }
    else {
      m_lastFileReferenceRecordId.reset();
    }

    value = query.value(AttributeRecordColumns::parentAttributeRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastParentAttributeRecordId = value.toInt();
    }
    else {
      m_lastParentAttributeRecordId.reset();
    }

    value = query.value(AttributeRecordColumns::attributeValueType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValueType = AttributeValueType(value.toInt());

    value = query.value(AttributeRecordColumns::attributeValue);

    switch(m_attributeValueType.value()) {
      case AttributeValueType::Boolean:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toBool();
        break;
      case AttributeValueType::Integer:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toInt();
        break;
      case AttributeValueType::Unsigned:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toUInt();
        break;
      case AttributeValueType::Double:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toDouble();
        break;
      case AttributeValueType::Quantity:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toDouble();
        break;
      case AttributeValueType::Unit:
        m_lastAttributeValue.clear();
        break;
      case AttributeValueType::String:
        OS_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue.setValue(value.toString().toStdString());
        break;
      case AttributeValueType::AttributeVector:
        m_lastAttributeValue.clear();
        break;
      default:
        OS_ASSERT(false);
        break;
    }

    value = query.value(AttributeRecordColumns::attributeUnits);
    if(value.isValid() && !value.isNull()){
      std::string units = value.toString().toStdString();
      m_lastAttributeUnits = units;
    }
    else {
      m_lastAttributeUnits.reset();
    }

    value = query.value(AttributeRecordColumns::attributeUnitSystem);
    if (value.isValid() && !value.isNull()) {
      m_lastAttributeUnitSystem = value.toString().toStdString();
    }
    else {
      m_lastAttributeUnitSystem.reset();
    }

    value = query.value(AttributeRecordColumns::attributeVectorIndex);
    if (value.isValid() && !value.isNull()) {
      m_lastAttributeVectorIndex = value.toInt();
    }
    else {
      m_lastAttributeVectorIndex.reset();
    }

    value = query.value(AttributeRecordColumns::algorithmRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastAlgorithmRecordId = value.toInt();
    }
    else {
      m_lastAlgorithmRecordId.reset();
    }

    value = query.value(AttributeRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastVariableRecordId = value.toInt();
    }
    else {
      m_lastVariableRecordId.reset();
    }

    value = query.value(AttributeRecordColumns::source);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastSource = value.toString().toStdString();

    value = query.value(AttributeRecordColumns::dataPointRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastDataPointRecordId = value.toInt();
    }
    else {
      m_lastDataPointRecordId.reset();
    }
  }

  bool AttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = true;
    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(AttributeRecordColumns::fileReferenceRecordId);
    if (value.isValid() && !value.isNull()){
      result = result && m_fileReferenceRecordId && (*m_fileReferenceRecordId == value.toInt());
    }else{
      result = result && !m_fileReferenceRecordId;
    }

    value = query.value(AttributeRecordColumns::parentAttributeRecordId);
    if (value.isValid() && !value.isNull()){
      result = result && m_parentAttributeRecordId && (*m_parentAttributeRecordId == value.toInt());
    }else{
      result = result && !m_parentAttributeRecordId;
    }

    value = query.value(AttributeRecordColumns::attributeValueType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValueType == AttributeValueType(value.toInt()));

    value = query.value(AttributeRecordColumns::attributeValue);
    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        OS_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toBool() == value.toBool());
        break;
      case AttributeValueType::Integer:
        OS_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toInt() == value.toInt());
        break;
      case AttributeValueType::Unsigned:
        OS_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toUInt() == value.toUInt());
        break;
      case AttributeValueType::Double:
        OS_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toDouble() == value.toDouble());
        break;
      case AttributeValueType::Quantity:
        OS_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toDouble() == value.toDouble());
        break;
      case AttributeValueType::Unit:
        break;
      case AttributeValueType::String:
        OS_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toString().toStdString() == value.toString().toStdString());
        break;
      case AttributeValueType::AttributeVector:
        break;
      default:
        OS_ASSERT(false);
        break;
    }

    value = query.value(AttributeRecordColumns::attributeUnits);
    if (value.isValid() && !value.isNull()){
      result = result && m_attributeUnits && (*m_attributeUnits == value.toString().toStdString());
    }else{
      result = result && !m_attributeUnits;
    }

    value = query.value(AttributeRecordColumns::attributeUnitSystem);
    if (value.isValid() && !value.isNull()){
      result = result && m_attributeUnitSystem && (*m_attributeUnitSystem == value.toString().toStdString());
    }else{
      result = result && !m_attributeUnitSystem;
    }

    value = query.value(AttributeRecordColumns::attributeVectorIndex);
    if (value.isValid() && !value.isNull()) {
      result = result && m_attributeVectorIndex && (*m_attributeVectorIndex == value.toInt());
    }
    else {
      result = result && !m_attributeVectorIndex;
    }

    value = query.value(AttributeRecordColumns::algorithmRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_algorithmRecordId && (*m_algorithmRecordId == value.toInt());
    }
    else {
      result = result && !m_algorithmRecordId;
    }

    value = query.value(AttributeRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_variableRecordId && (*m_variableRecordId == value.toInt());
    }
    else {
      result = result && !m_variableRecordId;
    }

    value = query.value(AttributeRecordColumns::source);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_source == value.toString().toStdString());

    value = query.value(AttributeRecordColumns::dataPointRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_dataPointRecordId && (*m_dataPointRecordId == value.toInt());
    }
    else {
      result = result && !m_dataPointRecordId;
    }

    return result;
  }

  void AttributeRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastFileReferenceRecordId = m_fileReferenceRecordId;
    m_lastParentAttributeRecordId = m_parentAttributeRecordId;
    m_lastAttributeValueType = m_attributeValueType;
    m_lastAttributeValue = m_attributeValue;
    m_lastAttributeUnits = m_attributeUnits;
    m_lastAttributeUnitSystem = m_attributeUnitSystem;
    m_lastAttributeVectorIndex = m_attributeVectorIndex;
    m_lastAlgorithmRecordId = m_algorithmRecordId;
    m_lastVariableRecordId = m_variableRecordId;
    m_lastSource = m_source;
    m_lastDataPointRecordId = m_dataPointRecordId;
  }

  void AttributeRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_fileReferenceRecordId = m_lastFileReferenceRecordId;
    m_parentAttributeRecordId = m_lastParentAttributeRecordId;
    m_attributeValueType = m_lastAttributeValueType;
    m_attributeValue = m_lastAttributeValue;
    m_attributeUnits = m_lastAttributeUnits;
    m_attributeUnitSystem = m_lastAttributeUnitSystem;
    m_attributeVectorIndex = m_lastAttributeVectorIndex;
    m_algorithmRecordId = m_lastAlgorithmRecordId;
    m_variableRecordId = m_lastVariableRecordId;
    m_source = m_lastSource;
  }

} // detail

AttributeRecord::AttributeRecord(const openstudio::Attribute& attribute,
                                 const FileReferenceRecord& fileReferenceRecord)
  : ObjectRecord(std::shared_ptr<detail::AttributeRecord_Impl>(
        new detail::AttributeRecord_Impl(attribute, fileReferenceRecord)),
        fileReferenceRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::AttributeRecord_Impl>());
  constructRelatedRecords(attribute);
}

AttributeRecord::AttributeRecord(const Attribute& attribute,
                                 const AttributeRecord& parentAttributeRecord,
                                 int attributeVectorIndex)
  : ObjectRecord(std::shared_ptr<detail::AttributeRecord_Impl>(
        new detail::AttributeRecord_Impl(attribute, parentAttributeRecord, attributeVectorIndex)),
        parentAttributeRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::AttributeRecord_Impl>());
  constructRelatedRecords(attribute);
}

AttributeRecord::AttributeRecord(const openstudio::Attribute& attribute,
                                 const AlgorithmRecord& algorithmRecord)
  : ObjectRecord(std::shared_ptr<detail::AttributeRecord_Impl>(
        new detail::AttributeRecord_Impl(attribute, algorithmRecord)),
        algorithmRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::AttributeRecord_Impl>());
  constructRelatedRecords(attribute);
}

AttributeRecord::AttributeRecord(const Attribute& attribute,
                                 const VariableRecord& variableRecord)
  : ObjectRecord(std::shared_ptr<detail::AttributeRecord_Impl>(
        new detail::AttributeRecord_Impl(attribute,variableRecord)),
        variableRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::AttributeRecord_Impl>());
  constructRelatedRecords(attribute);
}

AttributeRecord::AttributeRecord(const Attribute& attribute,
                                 const DataPointRecord& dataPointRecord)
  : ObjectRecord(std::shared_ptr<detail::AttributeRecord_Impl>(
        new detail::AttributeRecord_Impl(attribute,dataPointRecord)),
        dataPointRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::AttributeRecord_Impl>());
  constructRelatedRecords(attribute);
}

void AttributeRecord::constructRelatedRecords(const Attribute& attribute) {
  // save children to database
  if (attribute.valueType() == AttributeValueType::AttributeVector) {
    AttributeRecord copyOfThis(getImpl<detail::AttributeRecord_Impl>());
    int index(0);
    for (const Attribute& childAttribute : attribute.valueAsAttributeVector()) {
      AttributeRecord childRecord(childAttribute, copyOfThis,index);
      ++index;
    }
  }
}

AttributeRecord::AttributeRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::AttributeRecord_Impl>(
        new detail::AttributeRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AttributeRecord_Impl>());
}

AttributeRecord::AttributeRecord(std::shared_ptr<detail::AttributeRecord_Impl> impl, ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(this->getImpl<detail::AttributeRecord_Impl>());
}

AttributeRecord::AttributeRecord(std::shared_ptr<detail::AttributeRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(this->getImpl<detail::AttributeRecord_Impl>());
}

std::string AttributeRecord::databaseTableName()
{
  return "AttributeRecords";
}

UpdateByIdQueryData AttributeRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (auto it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      auto nextIt = it;
      ++nextIt;
      if (nextIt == itend) {
        ss << " ";
      }
      else {
        ss << ", ";
      }
      ++expectedValue;
    }
    ss << "WHERE id=:id";
    result.queryString = ss.str();

    // null values
    for (const auto & columnValue : result.columnValues)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(columnValue), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void AttributeRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  ObjectRecord::createIndices(qSqlDatabase,databaseTableName);

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "fileReferenceRecordIdIndex ON " + databaseTableName + " (fileReferenceRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "parentAttributeRecordIdIndex ON " + databaseTableName + " (parentAttributeRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "algorithmRecordIdIndex ON " + databaseTableName + " (algorithmRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "variableRecordIdIndex ON " + databaseTableName + " (variableRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "dataPointRecordIdIndex ON " + databaseTableName + " (dataPointRecordId)"));
  assertExec(query);
}

void AttributeRecord::updatePathData(ProjectDatabase database,
                                     const openstudio::path& originalBase,
                                     const openstudio::path& newBase)
{
  // At this time, AttributeRecord does not (explicitly) contain any path data.
}


boost::optional<AttributeRecord> AttributeRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalAttributeRecord result;
  try {
    result = AttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct AttributeRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<AttributeRecord> AttributeRecord::getAttributeRecords(ProjectDatabase& database) {
  AttributeRecordVector result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() + " WHERE parentAttributeRecordId IS NULL"));
  assertExec(query);
  while (query.next()) {
    result.push_back(AttributeRecord(query,database));
  }

  return result;
}

boost::optional<AttributeRecord> AttributeRecord::getAttributeRecord(int id, ProjectDatabase& database)
{
  OptionalAttributeRecord result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = AttributeRecord(query, database);
  }

  return result;
}

boost::optional<FileReferenceRecord> AttributeRecord::fileReferenceRecord() const {
  return getImpl<detail::AttributeRecord_Impl>()->fileReferenceRecord();
}

boost::optional<AttributeRecord> AttributeRecord::parentAttributeRecord() const {
  return getImpl<detail::AttributeRecord_Impl>()->parentAttributeRecord();
}

boost::optional<AlgorithmRecord> AttributeRecord::algorithmRecord() const {
  return getImpl<detail::AttributeRecord_Impl>()->algorithmRecord();
}

boost::optional<VariableRecord> AttributeRecord::variableRecord() const {
  return getImpl<detail::AttributeRecord_Impl>()->variableRecord();
}

boost::optional<DataPointRecord> AttributeRecord::dataPointRecord() const {
  return getImpl<detail::AttributeRecord_Impl>()->dataPointRecord();
}

boost::optional<int> AttributeRecord::parentAttributeRecordId() const {
  return this->getImpl<detail::AttributeRecord_Impl>()->parentAttributeRecordId();
}

bool AttributeRecord::setParentAttributeRecordId(int parentAttributeRecordId) {
  return this->getImpl<detail::AttributeRecord_Impl>()->setParentAttributeRecordId(
      parentAttributeRecordId);
}

openstudio::AttributeValueType AttributeRecord::attributeValueType() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueType();
}

bool AttributeRecord::attributeValueAsBoolean() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsBoolean();
}

int AttributeRecord::attributeValueAsInteger() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsInteger();
}

unsigned AttributeRecord::attributeValueAsUnsigned() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsUnsigned();
}

double AttributeRecord::attributeValueAsDouble() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsDouble();
}

Quantity AttributeRecord::attributeValueAsQuantity() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsQuantity();
}

Unit AttributeRecord::attributeValueAsUnit() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsUnit();
}

std::string AttributeRecord::attributeValueAsString() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsString();
}

std::vector<Attribute> AttributeRecord::attributeValueAsAttributeVector() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeValueAsAttributeVector();
}

boost::optional<std::string> AttributeRecord::attributeUnits() const
{
  return this->getImpl<detail::AttributeRecord_Impl>()->attributeUnits();
}

openstudio::Attribute AttributeRecord::attribute() const {
  return this->getImpl<detail::AttributeRecord_Impl>()->attribute();
}

} // project
} // openstudio

