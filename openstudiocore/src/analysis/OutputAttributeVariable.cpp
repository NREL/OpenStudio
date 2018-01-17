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

