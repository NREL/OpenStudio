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

#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkCrack_FieldEnums.hxx>

#include "AirflowNetworkReferenceCrackConditions.hpp"
#include "AirflowNetworkReferenceCrackConditions_Impl.hpp"

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

AirflowNetworkCrack_Impl::AirflowNetworkCrack_Impl(const IdfObject& idfObject,
  Model_Impl* model,
  bool keepHandle)
  : AirflowNetworkComponent_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkCrack::iddObjectType());
}

AirflowNetworkCrack_Impl::AirflowNetworkCrack_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
  Model_Impl* model,
  bool keepHandle)
  : AirflowNetworkComponent_Impl(other, model, keepHandle)
{
  OS_ASSERT(other.iddObject().type() == AirflowNetworkCrack::iddObjectType());
}

AirflowNetworkCrack_Impl::AirflowNetworkCrack_Impl(const AirflowNetworkCrack_Impl& other,
  Model_Impl* model,
  bool keepHandle)
  : AirflowNetworkComponent_Impl(other, model, keepHandle)
{}

const std::vector<std::string>& AirflowNetworkCrack_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if(result.empty()){
  }
  return result;
}

IddObjectType AirflowNetworkCrack_Impl::iddObjectType() const
{
  return AirflowNetworkCrack::iddObjectType();
}

double AirflowNetworkCrack_Impl::airMassFlowCoefficient() const
{
  boost::optional<double> value = getDouble(OS_AirflowNetworkCrackFields::AirMassFlowCoefficientatReferenceConditions, true);
  OS_ASSERT(value);
  return value.get();
}

double AirflowNetworkCrack_Impl::airMassFlowExponent() const
{
  boost::optional<double> value = getDouble(OS_AirflowNetworkCrackFields::AirMassFlowExponent, true);
  OS_ASSERT(value);
  return value.get();
}

boost::optional<AirflowNetworkReferenceCrackConditions> AirflowNetworkCrack_Impl::referenceCrackConditions() const {
  return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkReferenceCrackConditions>(OS_AirflowNetworkCrackFields::ReferenceCrackConditions);
}

bool AirflowNetworkCrack_Impl::isAirMassFlowExponentDefaulted() const
{
  return isEmpty(OS_AirflowNetworkCrackFields::AirMassFlowExponent);
}

bool AirflowNetworkCrack_Impl::setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions)
{
  bool result = setDouble(OS_AirflowNetworkCrackFields::AirMassFlowCoefficientatReferenceConditions, airMassFlowCoefficientatReferenceConditions);
  return result;
}

bool AirflowNetworkCrack_Impl::setAirMassFlowExponent(double airMassFlowExponent)
{
  bool result = setDouble(OS_AirflowNetworkCrackFields::AirMassFlowExponent, airMassFlowExponent);
  return result;
}

void AirflowNetworkCrack_Impl::resetAirMassFlowExponent() {
  bool result = setString(OS_AirflowNetworkCrackFields::AirMassFlowExponent, "");
  OS_ASSERT(result);
}

bool AirflowNetworkCrack_Impl::setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions) {
  bool result = setPointer(OS_AirflowNetworkCrackFields::ReferenceCrackConditions, referenceCrackConditions.handle());
  return result;
}

void AirflowNetworkCrack_Impl::resetReferenceCrackConditions() {
  bool result = setString(OS_AirflowNetworkCrackFields::ReferenceCrackConditions, "");
  OS_ASSERT(result);
}

} // detail

AirflowNetworkCrack::AirflowNetworkCrack(const Model& model, double massFlowCoefficient)
  : AirflowNetworkComponent(AirflowNetworkCrack::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkCrack_Impl>());

  bool ok = setAirMassFlowCoefficient(massFlowCoefficient);
  OS_ASSERT(ok);
}

AirflowNetworkCrack::AirflowNetworkCrack(const Model& model, double massFlowCoefficient,
  double massFlowExponent)
  : AirflowNetworkComponent(AirflowNetworkCrack::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkCrack_Impl>());

  bool ok = setAirMassFlowCoefficient(massFlowCoefficient);
  OS_ASSERT(ok);
  ok = setAirMassFlowExponent(massFlowExponent);
  OS_ASSERT(ok);
}

AirflowNetworkCrack::AirflowNetworkCrack(const Model& model, double massFlowCoefficient,
	double massFlowExponent, const AirflowNetworkReferenceCrackConditions &referenceCrackConditions)
  : AirflowNetworkComponent(AirflowNetworkCrack::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkCrack_Impl>());

  bool ok = setAirMassFlowCoefficient(massFlowCoefficient);
  OS_ASSERT(ok);
  ok = setAirMassFlowExponent(massFlowExponent);
  OS_ASSERT(ok);
  ok = setReferenceCrackConditions(referenceCrackConditions);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkCrack::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkCrack);
}

double AirflowNetworkCrack::airMassFlowCoefficient() const
{
  return getImpl<detail::AirflowNetworkCrack_Impl>()->airMassFlowCoefficient();
}

double AirflowNetworkCrack::airMassFlowExponent() const
{
  return getImpl<detail::AirflowNetworkCrack_Impl>()->airMassFlowExponent();
}

bool AirflowNetworkCrack::isAirMassFlowExponentDefaulted() const
{
  return getImpl<detail::AirflowNetworkCrack_Impl>()->isAirMassFlowExponentDefaulted();
}

boost::optional<AirflowNetworkReferenceCrackConditions> AirflowNetworkCrack::referenceCrackConditions() const {
  return getImpl<detail::AirflowNetworkCrack_Impl>()->referenceCrackConditions();
}

bool AirflowNetworkCrack::setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions)
{
  return getImpl<detail::AirflowNetworkCrack_Impl>()->setAirMassFlowCoefficient(airMassFlowCoefficientatReferenceConditions);
}

bool AirflowNetworkCrack::setAirMassFlowExponent(double airMassFlowExponent)
{
  return getImpl<detail::AirflowNetworkCrack_Impl>()->setAirMassFlowExponent(airMassFlowExponent);
}

void AirflowNetworkCrack::resetAirMassFlowExponent()
{
  getImpl<detail::AirflowNetworkCrack_Impl>()->resetAirMassFlowExponent();
}

bool AirflowNetworkCrack::setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions &referenceCrackConditions)
{
  return getImpl<detail::AirflowNetworkCrack_Impl>()->setReferenceCrackConditions(referenceCrackConditions);
}

void AirflowNetworkCrack::resetReferenceCrackConditions()
{
  getImpl<detail::AirflowNetworkCrack_Impl>()->resetReferenceCrackConditions();
}

/// @cond
AirflowNetworkCrack::AirflowNetworkCrack(std::shared_ptr<detail::AirflowNetworkCrack_Impl> impl)
  : AirflowNetworkComponent(impl)
{}
/// @endcond

} // model
} // openstudio

