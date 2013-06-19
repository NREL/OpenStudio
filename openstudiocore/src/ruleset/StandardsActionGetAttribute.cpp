/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <ruleset/StandardsActionGetAttribute.hpp>
#include <ruleset/StandardsActionGetAttribute_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {
namespace detail {

  // IMPL

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::BooleanAttribute& attribute)
    : StandardsActionClause_Impl(),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Boolean),
      m_booleanAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::BooleanAttribute& attribute,
      const UUID& uuid, const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Boolean),
      m_booleanAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::DoubleAttribute& attribute)
    : StandardsActionClause_Impl(),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Double),
      m_doubleAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::DoubleAttribute& attribute,
      const UUID& uuid, const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Double),
      m_doubleAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::EnumAttribute& attribute)
    : StandardsActionClause_Impl(),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Enum),
      m_enumAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::EnumAttribute& attribute,
      const UUID& uuid, const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Enum),
      m_enumAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,const standardsinterface::IntegerAttribute& attribute)
    : StandardsActionClause_Impl(),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Integer),
      m_integerAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::IntegerAttribute& attribute,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID),
      m_object(object),
      m_attributeType(standardsinterface::PartsOfSpeech::Integer),
      m_integerAttribute(attribute)
  {
    this->connectChild(m_object);
  }

  StandardsActionGetAttribute_Impl::StandardsActionGetAttribute_Impl(const QDomElement& element)
    : StandardsActionClause_Impl(element),
      m_object(StandardsFilterAvailableObject(element.firstChildElement(QString::fromStdString("Object"))))
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
    this->connectChild(m_object);
  }

  RulesetObject StandardsActionGetAttribute_Impl::clone() const {
    boost::shared_ptr<StandardsActionGetAttribute_Impl> impl(new StandardsActionGetAttribute_Impl(*this));
    return StandardsActionGetAttribute(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsActionGetAttribute_Impl::deepClone() const {
    StandardsActionGetAttribute result = clone().cast<StandardsActionGetAttribute>();
    result.mf_setObject(m_object.deepClone().cast<StandardsFilterAvailableObject>());
    return result.cast<RulesetObject>();
  }

  std::vector<StandardsFilterClause> StandardsActionGetAttribute_Impl::referencedFilters() const {
    StandardsFilterClauseVector result;
    result.push_back(m_object);
    return result;
  }

  void StandardsActionGetAttribute_Impl::setObject(const StandardsFilterAvailableObject& object) {
    this->disconnectChild(m_object);
    m_object = object;
    this->connectChild(m_object);
    this->onChange();
  }

  std::string StandardsActionGetAttribute_Impl::print() const {
    std::stringstream ss;
    ss << "    (assert (GetAttribute (modelId " << m_object.modelId() << ")" << std::endl;
    ss << "                          (objectType ?" << m_object.factName() << "_ObjectType)" << std::endl;
    ss << "                          (objectId ?" << m_object.factName() << "_ObjectId)" << std::endl;
    ss << "                          (attributeType " << m_attributeType.valueName() << ")" << std::endl;
    switch (m_attributeType.value()) {
      case (standardsinterface::PartsOfSpeech::Boolean) :
        ss << "                          (attributeName \"" << m_booleanAttribute->valueName() << "\")" << std::endl;
       break;
      case (standardsinterface::PartsOfSpeech::Double) :
        ss << "                          (attributeName \"" << m_doubleAttribute->valueName() << "\")" << std::endl;
       break;
      case (standardsinterface::PartsOfSpeech::Enum) :
        ss << "                          (attributeName \"" << m_enumAttribute->valueName() << "\")" << std::endl;
       break;
      case (standardsinterface::PartsOfSpeech::Integer) :
        ss << "                          (attributeName \"" << m_integerAttribute->valueName() << "\")" << std::endl;
       break;
      default:
        BOOST_ASSERT(false);
    };
    ss << "                          (checked FALSE)" << std::endl;
    ss << "                          (message \"\")))" << std::endl;
    return ss.str();
  }

  std::string StandardsActionGetAttribute_Impl::xmlElementName() const
  {
    return StandardsActionGetAttribute::xmlElementName();
  }

  void StandardsActionGetAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsActionClause_Impl::writeValues(doc, element);
  }

  bool StandardsActionGetAttribute_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && StandardsActionClause_Impl::equivalentData(other);
    if (result) {
      OptionalStandardsActionGetAttribute oOther = other.optionalCast<StandardsActionGetAttribute>();
      if (oOther) {
        boost::shared_ptr<StandardsActionGetAttribute_Impl> otherImpl = oOther->getImpl<StandardsActionGetAttribute_Impl>();
        if (m_object.equivalentData(otherImpl->m_object)) {
          return ((m_attributeType == otherImpl->m_attributeType) &&
                  (m_booleanAttribute == otherImpl->m_booleanAttribute) &&
                  (m_doubleAttribute == otherImpl->m_doubleAttribute) &&
                  (m_enumAttribute == otherImpl->m_enumAttribute) &&
                  (m_integerAttribute == otherImpl->m_integerAttribute));
        }
      }
    }
    return false;
  }

  // OBJECT

  std::string StandardsActionGetAttribute::xmlElementName()
  {
    return "StandardsActionGetAttribute";
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::BooleanAttribute& attribute)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::BooleanAttribute& attribute,
                                                           const UUID& uuid,
                                                           const UUID& versionUUID)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute,uuid,versionUUID)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::DoubleAttribute& attribute)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::DoubleAttribute& attribute,
                                                           const UUID& uuid,
                                                           const UUID& versionUUID)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute,uuid,versionUUID)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::EnumAttribute& attribute)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::EnumAttribute& attribute,
                                                           const UUID& uuid,
                                                           const UUID& versionUUID)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute,uuid,versionUUID)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::IntegerAttribute& attribute)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                                           const standardsinterface::IntegerAttribute& attribute,
                                                           const UUID& uuid,
                                                           const UUID& versionUUID)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetAttribute_Impl>(new StandardsActionGetAttribute_Impl(object,attribute,uuid,versionUUID)))
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

  void StandardsActionGetAttribute::mf_setObject(const StandardsFilterAvailableObject& object) {
    getImpl<StandardsActionGetAttribute_Impl>()->setObject(object);
  }

  StandardsActionGetAttribute::StandardsActionGetAttribute(boost::shared_ptr<detail::StandardsActionGetAttribute_Impl> impl)
    : StandardsActionClause(impl)
  {
    BOOST_ASSERT(getImpl<StandardsActionGetAttribute_Impl>());
  }

} // detail
} // ruleset
} // openstudio
