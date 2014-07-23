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

#include "SequentialSearchOptions.hpp"
#include "SequentialSearchOptions_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Json.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  SequentialSearchOptions_Impl::SequentialSearchOptions_Impl(int objectiveToMinimizeFirst)
    : AlgorithmOptions_Impl()
  {
    saveOption(Attribute("objectiveToMinimizeFirst",objectiveToMinimizeFirst));
  }

  SequentialSearchOptions_Impl::SequentialSearchOptions_Impl(const std::vector<Attribute>& options)
    : AlgorithmOptions_Impl(options)
  {}

  AlgorithmOptions SequentialSearchOptions_Impl::clone() const {
    std::shared_ptr<SequentialSearchOptions_Impl> impl(new SequentialSearchOptions_Impl(*this));
    return SequentialSearchOptions(impl);
  }

  int SequentialSearchOptions_Impl::objectiveToMinimizeFirst() const {
    OptionalAttribute option = getOption("objectiveToMinimizeFirst");
    OS_ASSERT(option);
    return option->valueAsInteger();
  }

  SequentialSearchOptions SequentialSearchOptions_Impl::fromVariant(const QVariant& variant,
                                                                const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    return SequentialSearchOptions(attributes);
  }

} // detail

SequentialSearchOptions::SequentialSearchOptions(int objectiveToMinimizeFirst)
  : AlgorithmOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl>(
        new detail::SequentialSearchOptions_Impl(objectiveToMinimizeFirst)))
{}

SequentialSearchOptions::SequentialSearchOptions(const std::vector<Attribute>& options)
  : AlgorithmOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl>(
        new detail::SequentialSearchOptions_Impl(options)))
{}

int SequentialSearchOptions::objectiveToMinimizeFirst() const {
  return getImpl<detail::SequentialSearchOptions_Impl>()->objectiveToMinimizeFirst();
}

/// @cond
SequentialSearchOptions::SequentialSearchOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl> impl)
  : AlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

