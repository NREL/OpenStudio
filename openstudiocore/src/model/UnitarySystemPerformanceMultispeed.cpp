/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "UnitarySystemPerformanceMultispeed.hpp"
#include "UnitarySystemPerformanceMultispeed_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {
  UnitarySystemPerformanceMultispeed_Impl::UnitarySystemPerformanceMultispeed_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == UnitarySystemPerformanceMultispeed::iddObjectType());
  }

  UnitarySystemPerformanceMultispeed_Impl::UnitarySystemPerformanceMultispeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == UnitarySystemPerformanceMultispeed::iddObjectType());
  }

  UnitarySystemPerformanceMultispeed_Impl::UnitarySystemPerformanceMultispeed_Impl(const UnitarySystemPerformanceMultispeed_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {}

  const std::vector<std::string>& UnitarySystemPerformanceMultispeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()) {
    }
    return result;
  }

  IddObjectType UnitarySystemPerformanceMultispeed_Impl::iddObjectType() const
  {
    return UnitarySystemPerformanceMultispeed::iddObjectType();
  }

  bool UnitarySystemPerformanceMultispeed_Impl::singleModeOperation() const
  {
    boost::optional<std::string> value = getString(OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation, true);
    OS_ASSERT(value);
    return openstudio::istringEqual("Yes", value.get());
  }

  bool UnitarySystemPerformanceMultispeed_Impl::setSingleModeOperation(bool singleMode)
  {
    return setString(OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation, singleMode ? "Yes" : "No");
  }

  void UnitarySystemPerformanceMultispeed_Impl::resetSingleModeOperation()
  {
    OS_ASSERT(setString(OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation, ""));
  }

  boost::optional<double> UnitarySystemPerformanceMultispeed_Impl::readDoubleFieldOrAutosize(unsigned fieldNum, const ModelExtensibleGroup& group)
  {
    boost::optional<double> result = group.getDouble(fieldNum);
    if (!result) {
      boost::optional<std::string> resultString = group.getString(fieldNum);
      if (!resultString || !openstudio::istringEqual(resultString.get(), "autosize")) {
        LOG(Error, "Could not read SupplyAirflowRatioField " << group.groupIndex() << " in " << briefDescription() << ".");
      }
    }
    return result;
  }

  std::vector<SupplyAirflowRatioField> UnitarySystemPerformanceMultispeed_Impl::supplyAirflowRatioFields()
  {
    std::vector<SupplyAirflowRatioField> result;

    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups()))
    {
      boost::optional<double> heatingRatio = readDoubleFieldOrAutosize(OS_UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, group);
      boost::optional<double> coolingRatio = readDoubleFieldOrAutosize(OS_UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, group);
      if (heatingRatio && coolingRatio) {
        result.push_back(SupplyAirflowRatioField(*heatingRatio, *coolingRatio));
      } else if (heatingRatio && !coolingRatio) {
        result.push_back(SupplyAirflowRatioField::fromHeatingRatio(*heatingRatio));
      } else if (coolingRatio && !heatingRatio) {
        result.push_back(SupplyAirflowRatioField::fromCoolingRatio(*coolingRatio));
      } else {
        result.push_back(SupplyAirflowRatioField());
      }
    }
    return result;
  }

  bool UnitarySystemPerformanceMultispeed_Impl::setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields)
  {
    std::vector<SupplyAirflowRatioField> originalFields = supplyAirflowRatioFields();

    bool result = true;

    clearExtensibleGroups(false);
    for (const auto& airflowRatioField : airflowRatioFields) {
      result = result && addSupplyAirflowRatioField(airflowRatioField);
    }

    if (!result) {
      clearExtensibleGroups(false);
      for (const auto& airflowRatioField : originalFields) {
        addSupplyAirflowRatioField(airflowRatioField);
      }
    }

    return result;
  }

  bool UnitarySystemPerformanceMultispeed_Impl::addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio)
  {
    std::vector<std::string> values = airflowRatio.getHeatingCoolingRatiosAsStrings();
    ModelExtensibleGroup group = pushExtensibleGroup(values, false).cast<ModelExtensibleGroup>();
    return (!group.empty());
  }

  bool UnitarySystemPerformanceMultispeed_Impl::addSupplyAirflowRatioField(double heatingRatio, double coolingRatio)
  {
    return addSupplyAirflowRatioField(SupplyAirflowRatioField(heatingRatio, coolingRatio));
  }

  void UnitarySystemPerformanceMultispeed_Impl::resetSupplyAirflowRatioFields()
  {
    clearExtensibleGroups(false);
  }

} //detail

SupplyAirflowRatioField::SupplyAirflowRatioField()
  : m_heatingRatio(boost::none), m_coolingRatio(boost::none)
  {}

SupplyAirflowRatioField::SupplyAirflowRatioField(double heatingRatio, double coolingRatio)
  : m_heatingRatio(heatingRatio), m_coolingRatio(coolingRatio)
  {}

SupplyAirflowRatioField::SupplyAirflowRatioField(bool isHeating, double value)
  : m_heatingRatio(boost::none), m_coolingRatio(boost::none)
{
  if (isHeating) {
    m_heatingRatio = value;
  } else {
    m_coolingRatio = value;
  }
}

SupplyAirflowRatioField SupplyAirflowRatioField::fromHeatingRatio(double heatingRatio)
{
  return SupplyAirflowRatioField(true, heatingRatio);
}

SupplyAirflowRatioField SupplyAirflowRatioField::fromCoolingRatio(double coolingRatio)
{
  return SupplyAirflowRatioField(false, coolingRatio);
}

boost::optional<double> SupplyAirflowRatioField::heatingRatio()
{
  return m_heatingRatio;
}

boost::optional<double> SupplyAirflowRatioField::coolingRatio()
{
  return m_coolingRatio;
}

bool SupplyAirflowRatioField::isHeatingRatioAutosized()
{
  return !m_heatingRatio;
}

bool SupplyAirflowRatioField::isCoolingRatioAutosized()
{
  return !m_coolingRatio;
}

std::vector<std::string> SupplyAirflowRatioField::getHeatingCoolingRatiosAsStrings() const
{
  std::vector<std::string> values;
  for (const auto& item : {m_heatingRatio, m_coolingRatio}) {
    if (item) {
      values.push_back(openstudio::toString(*item));
    } else {
      values.push_back("autosize");
    }
  }
  return values;
}

UnitarySystemPerformanceMultispeed::UnitarySystemPerformanceMultispeed(const Model& model)
  : ModelObject(UnitarySystemPerformanceMultispeed::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>());
}

IddObjectType UnitarySystemPerformanceMultispeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_UnitarySystemPerformance_Multispeed);
}

bool UnitarySystemPerformanceMultispeed::singleModeOperation() const {
  return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->singleModeOperation();
}

bool UnitarySystemPerformanceMultispeed::setSingleModeOperation(bool singleMode) {
  return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->setSingleModeOperation(singleMode);
}

void UnitarySystemPerformanceMultispeed::resetSingleModeOperation() {
  getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->resetSingleModeOperation();
}

std::vector<SupplyAirflowRatioField> UnitarySystemPerformanceMultispeed::supplyAirflowRatioFields() {
  return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->supplyAirflowRatioFields();
}

bool UnitarySystemPerformanceMultispeed::setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields) {
  return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->setSupplyAirflowRatioFields(airflowRatioFields);
}

bool UnitarySystemPerformanceMultispeed::addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio) {
  return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->addSupplyAirflowRatioField(airflowRatio);
}

bool UnitarySystemPerformanceMultispeed::addSupplyAirflowRatioField(double heatingRatio, double coolingRatio) {
  return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->addSupplyAirflowRatioField(heatingRatio, coolingRatio);
}

void UnitarySystemPerformanceMultispeed::resetSupplyAirflowRatioFields() {
  getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->resetSupplyAirflowRatioFields();
}


/// @cond
UnitarySystemPerformanceMultispeed::UnitarySystemPerformanceMultispeed(std::shared_ptr<detail::UnitarySystemPerformanceMultispeed_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} //model
} //openstudio
