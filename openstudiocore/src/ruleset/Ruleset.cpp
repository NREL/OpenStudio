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

#include <ruleset/Ruleset.hpp>
#include <ruleset/Ruleset_Impl.hpp>
#include <ruleset/ModelRuleset.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/core/UUID.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

namespace openstudio {
namespace ruleset {

namespace detail {

  Ruleset_Impl::Ruleset_Impl(const std::string& name)
    : RulesetObject_Impl(), m_name(name)
  {}

  Ruleset_Impl::Ruleset_Impl(const std::string& name, const UUID& uuid, const UUID& versionUUID)
    : RulesetObject_Impl(uuid, versionUUID), m_name(name)
  {}

  Ruleset_Impl::Ruleset_Impl(const QDomElement& element)
    : RulesetObject_Impl(element)
  {
    OS_ASSERT(!element.isNull());

    QDomElement nameElement = element.firstChildElement(QString::fromStdString("Name"));

    m_name = nameElement.firstChild().nodeValue().toStdString();
  }

  Ruleset_Impl::~Ruleset_Impl()
  {}

  std::string Ruleset_Impl::name() const
  {
    return m_name;
  }

  bool Ruleset_Impl::setName(const std::string& name)
  {
    m_name = name;
    this->onChange();
    return true;
  }

  Ruleset Ruleset_Impl::ruleset() const {
    return this->getObject<Ruleset>();
  }

  bool Ruleset_Impl::saveToXml(const openstudio::path& path) const
  {
    bool result = false;

    try {
      QFile file(toQString(path));
      file.open(QFile::WriteOnly);
      QTextStream out(&file);
      out << this->toXml();
      file.close();
      result = true;
    }
    catch(...) {}

    return result;
  }

  void Ruleset_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    RulesetObject_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("Name"));
    text = doc.createTextNode(toQString(m_name));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool Ruleset_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    return result;
  }

} // detail

boost::optional<Ruleset> Ruleset::factoryFromXml(const QDomElement& element)
{
  boost::optional<Ruleset> result;

  std::string xmlElementName = toString(element.tagName());
  if (xmlElementName == ModelRuleset::xmlElementName()){
    result = ModelRuleset(element);
  }

  return result;
}

/// @cond
Ruleset::Ruleset(boost::shared_ptr<detail::Ruleset_Impl> impl)
  : RulesetObject(impl)
{
  OS_ASSERT(getImpl<detail::Ruleset_Impl>());
}
/// @endcond

std::string Ruleset::name() const
{
  return this->getImpl<detail::Ruleset_Impl>()->name();
}

bool Ruleset::setName(const std::string& name)
{
  return this->getImpl<detail::Ruleset_Impl>()->setName(name);
}

bool Ruleset::saveToXml(const openstudio::path& path) const
{
  return this->getImpl<detail::Ruleset_Impl>()->saveToXml(path);
}

} // ruleset
} // openstudio
