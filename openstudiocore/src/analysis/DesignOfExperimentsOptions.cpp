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

#include "DesignOfExperimentsOptions.hpp"
#include "DesignOfExperimentsOptions_Impl.hpp"

#include "../utilities/core/Json.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  DesignOfExperimentsOptions_Impl::DesignOfExperimentsOptions_Impl(
      const DesignOfExperimentsType& designType)
    : AlgorithmOptions_Impl(), m_designType(designType)
  {}

  DesignOfExperimentsOptions_Impl::DesignOfExperimentsOptions_Impl(
      const DesignOfExperimentsType& designType,
      const std::vector<Attribute>& options)
    : AlgorithmOptions_Impl(options), m_designType(designType)
  {}

  AlgorithmOptions DesignOfExperimentsOptions_Impl::clone() const {
    std::shared_ptr<DesignOfExperimentsOptions_Impl> impl(new DesignOfExperimentsOptions_Impl(*this));
    return DesignOfExperimentsOptions(impl);
  }

  DesignOfExperimentsType DesignOfExperimentsOptions_Impl::designType() const {
    return m_designType;
  }

  void DesignOfExperimentsOptions_Impl::setDesignType(const DesignOfExperimentsType& designType) {
    m_designType = designType;
  }

  QVariant DesignOfExperimentsOptions_Impl::toVariant() const {
    QVariantMap map = AlgorithmOptions_Impl::toVariant().toMap();

    map["design_type"] = toQString(designType().valueName());

    return QVariant(map);
  }

  DesignOfExperimentsOptions DesignOfExperimentsOptions_Impl::fromVariant(const QVariant& variant,
                                                                          const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    return DesignOfExperimentsOptions(DesignOfExperimentsType(map["design_type"].toString().toStdString()),
                                      attributes);
  }

} // detail

DesignOfExperimentsOptions::DesignOfExperimentsOptions(const DesignOfExperimentsType& designType)
  : AlgorithmOptions(std::shared_ptr<detail::DesignOfExperimentsOptions_Impl>(
        new detail::DesignOfExperimentsOptions_Impl(designType)))
{}

DesignOfExperimentsOptions::DesignOfExperimentsOptions(const DesignOfExperimentsType& designType,
                                                       const std::vector<Attribute>& options)
  : AlgorithmOptions(std::shared_ptr<detail::DesignOfExperimentsOptions_Impl>(
        new detail::DesignOfExperimentsOptions_Impl(designType,options)))
{}

DesignOfExperimentsType DesignOfExperimentsOptions::designType() const {
  return getImpl<detail::DesignOfExperimentsOptions_Impl>()->designType();
}

void DesignOfExperimentsOptions::setDesignType(const DesignOfExperimentsType& designType) {
  getImpl<detail::DesignOfExperimentsOptions_Impl>()->setDesignType(designType);
}

/// @cond
DesignOfExperimentsOptions::DesignOfExperimentsOptions(std::shared_ptr<detail::DesignOfExperimentsOptions_Impl> impl)
  : AlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

