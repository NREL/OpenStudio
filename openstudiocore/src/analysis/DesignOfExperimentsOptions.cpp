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

