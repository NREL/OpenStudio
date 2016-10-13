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

