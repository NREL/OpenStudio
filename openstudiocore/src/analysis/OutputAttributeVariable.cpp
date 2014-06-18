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

#include "OutputAttributeVariable.hpp"
#include "OutputAttributeVariable_Impl.hpp"

#include "DataPoint.hpp"

#include "../runmanager/lib/Job.hpp"

#include "../utilities/core/Exception.hpp"
#include "../utilities/data/Attribute.hpp"
#include "../utilities/units/Quantity.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  OutputAttributeVariable_Impl::OutputAttributeVariable_Impl(const std::string& name,
                                                             const std::string& attributeName)
    : OutputVariable_Impl(name),
      m_attributeName(attributeName)
  {}

  /** Constructor provided for deserialization; not for general use. */
  OutputAttributeVariable_Impl::OutputAttributeVariable_Impl(const UUID& uuid,
                                                             const UUID& versionUUID,
                                                             const std::string& name,
                                                             const std::string& displayName,
                                                             const std::string& description,
                                                             const std::string& attributeName)
    : OutputVariable_Impl(uuid,versionUUID,name,displayName,description),
      m_attributeName(attributeName)
  {}

  OutputAttributeVariable_Impl::OutputAttributeVariable_Impl(const OutputAttributeVariable_Impl &other)
    : OutputVariable_Impl(other),
      m_attributeName(other.attributeName())
  {}

  AnalysisObject OutputAttributeVariable_Impl::clone() const {
    std::shared_ptr<OutputAttributeVariable_Impl> impl(new OutputAttributeVariable_Impl(*this));
    return OutputAttributeVariable(impl);
  }

  double OutputAttributeVariable_Impl::getValue(const DataPoint& dataPoint) const {
    OptionalAttribute oa = dataPoint.getOutputAttribute(m_attributeName);
    if (!oa) {
      LOG_AND_THROW("Unable to retrieve attribute '" << m_attributeName << "' for DataPoint at "
                    << toString(dataPoint.directory()) << ".");
    }
    Attribute attribute = oa.get();
    OptionalDouble result;
    AttributeValueType valueType = attribute.valueType();
    if (valueType == AttributeValueType::Double) {
      result = attribute.valueAsDouble();
    }
    if (valueType == AttributeValueType::Quantity) {
      result = attribute.valueAsQuantity().value();
    }
    if (valueType == AttributeValueType::Integer) {
      result = double(attribute.valueAsInteger());
    }
    if (valueType == AttributeValueType::Unsigned) {
      result = double(attribute.valueAsUnsigned());
    }
    if (!result) {
      LOG_AND_THROW("Unable to extract double value from attribute '" << m_attributeName
                    << "' of type " << valueType.valueDescription() << ".");
    }
    return *result;
  }

  std::string OutputAttributeVariable_Impl::attributeName() const {
    return m_attributeName;
  }

  void OutputAttributeVariable_Impl::setAttributeName(const std::string& attributeName) {
    m_attributeName = attributeName;
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  QVariant OutputAttributeVariable_Impl::toVariant() const {
    QVariantMap variableData = AnalysisObject_Impl::toVariant().toMap();

    variableData["variable_type"] = QString("OutputAttributeVariable");
    variableData["attribute_name"] = toQString(attributeName());

    return QVariant(variableData);
  }

  OutputAttributeVariable OutputAttributeVariable_Impl::fromVariant(const QVariant& variant,
                                                                    const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    return OutputAttributeVariable(toUUID(map["uuid"].toString().toStdString()),
                                   toUUID(map["version_uuid"].toString().toStdString()),
                                   map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                                   map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                                   map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                                   map["attribute_name"].toString().toStdString());
  }

} // detail

OutputAttributeVariable::OutputAttributeVariable(
    const std::string& name,
    const std::string& attributeName)
  : OutputVariable(std::shared_ptr<detail::OutputAttributeVariable_Impl>(
                           new detail::OutputAttributeVariable_Impl(name,attributeName)))
{}

OutputAttributeVariable::OutputAttributeVariable(
    const UUID& uuid,
    const UUID& versionUUID,
    const std::string& name,
    const std::string& displayName,
    const std::string& description,
    const std::string& attributeName)
  : OutputVariable(std::shared_ptr<detail::OutputAttributeVariable_Impl>(
                           new detail::OutputAttributeVariable_Impl(uuid,
                                                                    versionUUID,
                                                                    name,
                                                                    displayName,
                                                                    description,
                                                                    attributeName)))
{}

std::string OutputAttributeVariable::attributeName() const {
  return getImpl<detail::OutputAttributeVariable_Impl>()->attributeName();
}

void OutputAttributeVariable::setAttributeName(const std::string& attributeName) {
  getImpl<detail::OutputAttributeVariable_Impl>()->setAttributeName(attributeName);
}

/// @cond
OutputAttributeVariable::OutputAttributeVariable(std::shared_ptr<detail::OutputAttributeVariable_Impl> impl)
  : OutputVariable(impl)
{}
/// @endcond

} // analysis
} // openstudio

