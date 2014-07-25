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

#include "RulesetObject.hpp"
#include "RulesetObject_Impl.hpp"

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

namespace openstudio {
namespace ruleset {

namespace detail {

  RulesetObject_Impl::RulesetObject_Impl()
    : m_uuid(createUUID()), m_versionUUID(createUUID())
  {}

  RulesetObject_Impl::RulesetObject_Impl(const UUID& uuid, const UUID& versionUUID)
    : m_uuid(uuid), m_versionUUID(versionUUID)
  {}

  RulesetObject_Impl::RulesetObject_Impl(const QDomElement& element)
  {
    OS_ASSERT(!element.isNull());

    QDomElement uuidElement = element.firstChildElement(QString::fromStdString("UUID"));
    OS_ASSERT(!uuidElement.isNull());
    m_uuid = toUUID(uuidElement.firstChild().nodeValue().toStdString());

    QDomElement versionUUIDElement = element.firstChildElement(QString::fromStdString("VersionUUID"));
    OS_ASSERT(!versionUUIDElement.isNull());
    m_versionUUID = toUUID(versionUUIDElement.firstChild().nodeValue().toStdString());
  }

  RulesetObject_Impl::RulesetObject_Impl(const RulesetObject_Impl& other)
    : m_uuid(createUUID()), m_versionUUID(createUUID())
  {}

  UUID RulesetObject_Impl::uuid() const
  {
    return m_uuid;
  }

  UUID RulesetObject_Impl::versionUUID() const
  {
    return m_versionUUID;
  }

  QDomDocument RulesetObject_Impl::toXml() const
  {
    QDomDocument doc;
    QDomElement element = doc.createElement(toQString(this->xmlElementName()));
    OS_ASSERT(!element.isNull());

    this->writeValues(doc, element);
    doc.appendChild(element);

    return doc;
  }

  void RulesetObject_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("UUID"));
    text = doc.createTextNode(toQString(toString(m_uuid)));
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("VersionUUID"));
    text = doc.createTextNode(toQString(toString(m_versionUUID)));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool RulesetObject_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    // do not compare UUIDs
    //result = result && (m_uuid == other.uuid());
    //result = result && (m_versionUUID == other.versionUUID());
    return result;
  }

  void RulesetObject_Impl::onChildChanged()
  {
    onChange();
  }

  void RulesetObject_Impl::onChange()
  {
    m_versionUUID = createUUID();
    emit changed();
  }

  void RulesetObject_Impl::connectChild(const RulesetObject& child) const
  {
    connect(child.getImpl<detail::RulesetObject_Impl>().get(), SIGNAL(changed()), this, SLOT(onChildChanged()));
  }

  void RulesetObject_Impl::disconnectChild(const RulesetObject& child) const
  {
    disconnect(child.getImpl<detail::RulesetObject_Impl>().get(), SIGNAL(changed()), this, SLOT(onChildChanged()));
  }

  std::string RulesetObject_Impl::printUUID() const {
    return ("_" + toString(m_uuid));
  }

} // detail

UUID RulesetObject::uuid() const {
  return m_impl->uuid();
}

UUID RulesetObject::versionUUID() const {
  return m_impl->versionUUID();
}

QDomDocument RulesetObject::toXml() const
{
  return m_impl->toXml();
}

bool RulesetObject::equivalentData(const RulesetObject& other) const {
  return m_impl->equivalentData(other);
}

bool RulesetObject::operator==(const RulesetObject& other) const {
  return (m_impl == other.m_impl);
}

bool RulesetObject::operator!=(const RulesetObject& other) const {
  return !(operator==(other));
}

bool RulesetObject::operator<(const RulesetObject& other) const {
  return m_impl < other.m_impl;
}

/// @cond
RulesetObject::RulesetObject(std::shared_ptr<detail::RulesetObject_Impl> impl)
  : m_impl(impl)
{
  OS_ASSERT(impl);
}
/// @endcond

std::ostream& operator<<(std::ostream& os, const RulesetObject& rulesetObject)
{
  QString str;
  QTextStream qts(&str);
  rulesetObject.toXml().save(qts, 2);
  os << str.toStdString();
  return os;
}

} // ruleset
} // openstudio
