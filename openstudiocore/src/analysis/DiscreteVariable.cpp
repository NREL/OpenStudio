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

#include "DiscreteVariable.hpp"
#include "DiscreteVariable_Impl.hpp"

#include "DataPoint.hpp"
#include "Problem.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  DiscreteVariable_Impl::DiscreteVariable_Impl(const std::string& name)
    : InputVariable_Impl(name)
  {}

  DiscreteVariable_Impl::DiscreteVariable_Impl(const UUID& uuid,
                                               const UUID& versionUUID,
                                               const std::string& name,
                                               const std::string& displayName,
                                               const std::string& description,
                                               const boost::optional<UncertaintyDescription>& udesc)
    : InputVariable_Impl(uuid,versionUUID,name,displayName,description,udesc)
  {}

  DiscreteVariable_Impl::DiscreteVariable_Impl(const DiscreteVariable_Impl& other)
    : InputVariable_Impl(other)
  {}

  double DiscreteVariable_Impl::getValue(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    OS_ASSERT(index);
    return dataPoint.variableValues()[*index].toDouble();
  }

  bool DiscreteVariable_Impl::isValid(const UncertaintyDescription& udesc) const {
    UncertaintyDescriptionTypeVector validTypes = UncertaintyDescription::validTypes(VariableValueType::Discrete);
    return (std::find(validTypes.begin(),validTypes.end(),udesc.type()) != validTypes.end());
  }

  int DiscreteVariable_Impl::numValidValues(bool selectedOnly) const {
    return this->validValues(selectedOnly).size();
  }

  QVariant DiscreteVariable_Impl::toServerFormulationVariant() const {
    QVariantMap map;

    map["uuid"] = toQString(removeBraces(uuid()));
    map["version_uuid"] = toQString(removeBraces(uuid()));
    map["name"] = toQString(name());
    map["display_name"] = toQString(displayName());
    map["type"] = QString("Integer"); // could be Discrete instead

    // determine minimum, maximum from all values
    IntVector allValues = validValues(false);
    // right now, only have MeasureGroup, this will always be contiguous listing
    map["minimum"] = 0;
    map["maximum"] = int(allValues.size()) - 1;
    // no initial_value implemented right now

    // if selected values are subset, list them separately
    IntVector selectedValues = validValues(true);
    if (selectedValues.size() < allValues.size()) {
      QVariantList selectedList;
      for (int val : selectedValues) {
        selectedList.push_back(QVariant(val));
      }
      map["selected_values"] = selectedList;
    }

    return QVariant(map);
  }

} // detail

std::vector<int> DiscreteVariable::validValues(bool selectedOnly) const {
  return getImpl<detail::DiscreteVariable_Impl>()->validValues(selectedOnly);
}

int DiscreteVariable::numValidValues(bool selectedOnly) const {
  return getImpl<detail::DiscreteVariable_Impl>()->numValidValues(selectedOnly);
}

/// @cond
DiscreteVariable::DiscreteVariable(std::shared_ptr<detail::DiscreteVariable_Impl> impl)
  : InputVariable(impl)
{}
/// @endcond

} // analysis
} // openstudio

