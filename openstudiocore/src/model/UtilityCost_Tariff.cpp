/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "UtilityCost_Tariff.hpp"
#include "UtilityCost_Tariff_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_UtilityCost_Tariff_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Tariff_Impl::UtilityCost_Tariff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Tariff::iddObjectType());
}

UtilityCost_Tariff_Impl::UtilityCost_Tariff_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Tariff::iddObjectType());
}

UtilityCost_Tariff_Impl::UtilityCost_Tariff_Impl(const UtilityCost_Tariff_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalDouble UtilityCost_Tariff_Impl::energyConversionFactor() const
{
  return getDouble(OS_UtilityCost_TariffFields::EnergyConversionFactor);
}

bool UtilityCost_Tariff_Impl::setEnergyConversionFactor(double num)
{
  return setDouble(OS_UtilityCost_TariffFields::EnergyConversionFactor,num);;
}

OptionalDouble UtilityCost_Tariff_Impl::demandConversionFactor() const
{
  return getDouble(OS_UtilityCost_TariffFields::DemandConversionFactor);
}

bool UtilityCost_Tariff_Impl::setDemandConversionFactor(double num)
{
  return setDouble(OS_UtilityCost_TariffFields::DemandConversionFactor,num);;
}

OptionalString UtilityCost_Tariff_Impl::outputMeterName() const
{
  return getString(OS_UtilityCost_TariffFields::OutputMeterName);
}

bool UtilityCost_Tariff_Impl::setOutputMeterName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::OutputMeterName,str);;
}

OptionalString UtilityCost_Tariff_Impl::conversionFactorChoice() const
{
  return getString(OS_UtilityCost_TariffFields::ConversionFactorChoice);
}

bool UtilityCost_Tariff_Impl::setConversionFactorChoice(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::ConversionFactorChoice,str);;
}

OptionalString UtilityCost_Tariff_Impl::timeOfUsePeriodScheduleName() const
{
  return getString(OS_UtilityCost_TariffFields::TimeofUsePeriodScheduleName);
}

bool UtilityCost_Tariff_Impl::setTimeOfUsePeriodScheduleName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::TimeofUsePeriodScheduleName,str);;
}

OptionalString UtilityCost_Tariff_Impl::seasonScheduleName() const
{
  return getString(OS_UtilityCost_TariffFields::SeasonScheduleName);
}

bool UtilityCost_Tariff_Impl::setSeasonScheduleName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::SeasonScheduleName,str);;
}

OptionalString UtilityCost_Tariff_Impl::monthScheduleName() const
{
  return getString(OS_UtilityCost_TariffFields::MonthScheduleName);
}

bool UtilityCost_Tariff_Impl::setMonthScheduleName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::MonthScheduleName,str);;
}

OptionalString UtilityCost_Tariff_Impl::demandWindowLength() const
{
  return getString(OS_UtilityCost_TariffFields::DemandWindowLength);
}

bool UtilityCost_Tariff_Impl::setDemandWindowLength(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::DemandWindowLength,str);;
}

OptionalString UtilityCost_Tariff_Impl::monthlyChargeOrVariableName() const
{
  return getString(OS_UtilityCost_TariffFields::MonthlyChargeorVariableName);
}

bool UtilityCost_Tariff_Impl::setMonthlyChargeOrVariableName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::MonthlyChargeorVariableName,str);;
}

OptionalString UtilityCost_Tariff_Impl::minimumMonthlyChargeOrVariableName() const
{
  return getString(OS_UtilityCost_TariffFields::MinimumMonthlyChargeorVariableName);
}

bool UtilityCost_Tariff_Impl::setMinimumMonthlyChargeOrVariableName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::MinimumMonthlyChargeorVariableName,str);;
}

OptionalString UtilityCost_Tariff_Impl::realTimePricingChargeScheduleName() const
{
  return getString(OS_UtilityCost_TariffFields::RealTimePricingChargeScheduleName);
}

bool UtilityCost_Tariff_Impl::setRealTimePricingChargeScheduleName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::RealTimePricingChargeScheduleName,str);;
}

OptionalString UtilityCost_Tariff_Impl::customerBaselineLoadScheduleName() const
{
  return getString(OS_UtilityCost_TariffFields::CustomerBaselineLoadScheduleName);
}

bool UtilityCost_Tariff_Impl::setCustomerBaselineLoadScheduleName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::CustomerBaselineLoadScheduleName,str);;
}

OptionalString UtilityCost_Tariff_Impl::groupName() const
{
  return getString(OS_UtilityCost_TariffFields::GroupName);
}

bool UtilityCost_Tariff_Impl::setGroupName(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::GroupName,str);;
}

OptionalString UtilityCost_Tariff_Impl::buyOrSell() const
{
  return getString(OS_UtilityCost_TariffFields::BuyOrSell);
}

bool UtilityCost_Tariff_Impl::setBuyOrSell(const std::string& str)
{
  return setString(OS_UtilityCost_TariffFields::BuyOrSell,str);;
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Tariff_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Tariff_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Tariff_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Tariff_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Tariff_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  return result;
}

} // detail

/// constructor
UtilityCost_Tariff::UtilityCost_Tariff(const Model& model)
  : ParentObject(UtilityCost_Tariff::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Tariff_Impl>());
}

// constructor
UtilityCost_Tariff::UtilityCost_Tariff(std::shared_ptr<detail::UtilityCost_Tariff_Impl> impl)
  : ParentObject(std::move(impl))
{}

OptionalDouble UtilityCost_Tariff::energyConversionFactor() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->energyConversionFactor();
}

bool UtilityCost_Tariff::setEnergyConversionFactor(double num)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setEnergyConversionFactor(num);
}

OptionalDouble UtilityCost_Tariff::demandConversionFactor() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->demandConversionFactor();
}

bool UtilityCost_Tariff::setDemandConversionFactor(double num)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setDemandConversionFactor(num);
}

OptionalString UtilityCost_Tariff::outputMeterName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->outputMeterName();
}

bool UtilityCost_Tariff::setOutputMeterName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setOutputMeterName(str);
}

OptionalString UtilityCost_Tariff::conversionFactorChoice() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->conversionFactorChoice();
}

bool UtilityCost_Tariff::setConversionFactorChoice(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setConversionFactorChoice(str);
}

OptionalString UtilityCost_Tariff::timeOfUsePeriodScheduleName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->timeOfUsePeriodScheduleName();
}

bool UtilityCost_Tariff::setTimeOfUsePeriodScheduleName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setTimeOfUsePeriodScheduleName(str);
}

OptionalString UtilityCost_Tariff::seasonScheduleName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->seasonScheduleName();
}

bool UtilityCost_Tariff::setSeasonScheduleName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setSeasonScheduleName(str);
}

OptionalString UtilityCost_Tariff::monthScheduleName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->monthScheduleName();
}

bool UtilityCost_Tariff::setMonthScheduleName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setMonthScheduleName(str);
}

OptionalString UtilityCost_Tariff::demandWindowLength() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->demandWindowLength();
}

bool UtilityCost_Tariff::setDemandWindowLength(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setDemandWindowLength(str);
}

OptionalString UtilityCost_Tariff::monthlyChargeOrVariableName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->monthlyChargeOrVariableName();
}

bool UtilityCost_Tariff::setMonthlyChargeOrVariableName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setMonthlyChargeOrVariableName(str);
}

OptionalString UtilityCost_Tariff::minimumMonthlyChargeOrVariableName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->minimumMonthlyChargeOrVariableName();
}

bool UtilityCost_Tariff::setMinimumMonthlyChargeOrVariableName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setMinimumMonthlyChargeOrVariableName(str);
}

OptionalString UtilityCost_Tariff::realTimePricingChargeScheduleName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->realTimePricingChargeScheduleName();
}

bool UtilityCost_Tariff::setRealTimePricingChargeScheduleName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setRealTimePricingChargeScheduleName(str);
}

OptionalString UtilityCost_Tariff::customerBaselineLoadScheduleName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->customerBaselineLoadScheduleName();
}

bool UtilityCost_Tariff::setCustomerBaselineLoadScheduleName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setCustomerBaselineLoadScheduleName(str);
}

OptionalString UtilityCost_Tariff::groupName() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->groupName();
}

bool UtilityCost_Tariff::setGroupName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setGroupName(str);
}

OptionalString UtilityCost_Tariff::buyOrSell() const
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->buyOrSell();
}

bool UtilityCost_Tariff::setBuyOrSell(const std::string& str)
{
  return getImpl<detail::UtilityCost_Tariff_Impl>()->setBuyOrSell(str);
}

IddObjectType UtilityCost_Tariff::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Tariff);
  return result;
}

} // model
} // openstudio
