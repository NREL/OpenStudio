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

#include "AirflowNetworkZoneExhaustFan.hpp"
#include "AirflowNetworkZoneExhaustFan_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "FanZoneExhaust.hpp"
#include "FanZoneExhaust_Impl.hpp"
#include "AirflowNetworkReferenceCrackConditions.hpp"
#include "AirflowNetworkReferenceCrackConditions_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkZoneExhaustFan_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkZoneExhaustFan::iddObjectType());
  }

  AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkZoneExhaustFan::iddObjectType());
  }

  AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const AirflowNetworkZoneExhaustFan_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkZoneExhaustFan_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirflowNetworkZoneExhaustFan_Impl::iddObjectType() const
  {
    return AirflowNetworkZoneExhaustFan::iddObjectType();
  }

  boost::optional<FanZoneExhaust> AirflowNetworkZoneExhaustFan_Impl::fanZoneExhaust() const
  {
    boost::optional<FanZoneExhaust> value = getObject<ModelObject>().getModelObjectTarget<FanZoneExhaust>(OS_AirflowNetworkZoneExhaustFanFields::Name);
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a FanZoneExhaust attached.");
    }
    return value; // .get();
  }

  double AirflowNetworkZoneExhaustFan_Impl::airMassFlowCoefficientWhentheZoneExhaustFanisOff() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkZoneExhaustFanFields::AirMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkZoneExhaustFan_Impl::airMassFlowExponentWhentheZoneExhaustFanisOff() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkZoneExhaustFanFields::AirMassFlowExponentWhentheZoneExhaustFanisOff,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkZoneExhaustFan_Impl::isAirMassFlowExponentWhentheZoneExhaustFanisOffDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkZoneExhaustFanFields::AirMassFlowExponentWhentheZoneExhaustFanisOff);
  }

  boost::optional<AirflowNetworkReferenceCrackConditions> AirflowNetworkZoneExhaustFan_Impl::referenceCrackConditions() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkReferenceCrackConditions>(OS_AirflowNetworkZoneExhaustFanFields::ReferenceCrackConditions);
  }

  bool AirflowNetworkZoneExhaustFan_Impl::setFanZoneExhaust(const FanZoneExhaust& fanZoneExhaust)
  {
    return setPointer(OS_AirflowNetworkZoneExhaustFanFields::Name, fanZoneExhaust.handle());
  }

  bool AirflowNetworkZoneExhaustFan_Impl::setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(double airMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions)
  {
    return setDouble(OS_AirflowNetworkZoneExhaustFanFields::AirMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions, airMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions);
  }

  bool AirflowNetworkZoneExhaustFan_Impl::setAirMassFlowExponentWhentheZoneExhaustFanisOff(double airMassFlowExponentWhentheZoneExhaustFanisOff)
  {
    return setDouble(OS_AirflowNetworkZoneExhaustFanFields::AirMassFlowExponentWhentheZoneExhaustFanisOff, airMassFlowExponentWhentheZoneExhaustFanisOff);
  }

  void AirflowNetworkZoneExhaustFan_Impl::resetAirMassFlowExponentWhentheZoneExhaustFanisOff()
  {
    bool result = setString(OS_AirflowNetworkZoneExhaustFanFields::AirMassFlowExponentWhentheZoneExhaustFanisOff, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkZoneExhaustFan_Impl::setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions)
  {
    return setPointer(OS_AirflowNetworkZoneExhaustFanFields::ReferenceCrackConditions, referenceCrackConditions.handle());
  }

  void AirflowNetworkZoneExhaustFan_Impl::resetReferenceCrackConditions()
  {
    bool result = setString(OS_AirflowNetworkZoneExhaustFanFields::ReferenceCrackConditions, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(const Model& model,
  const FanZoneExhaust &fanZoneExhaust, double massFlowCoefficient)
  : ModelObject(AirflowNetworkZoneExhaustFan::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>());
  bool ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setFanZoneExhaust(fanZoneExhaust);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(massFlowCoefficient);
  OS_ASSERT(ok);
}

AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(const Model& model,
  const FanZoneExhaust &fanZoneExhaust, double massFlowCoefficient, double massFlowExponent)
  : ModelObject(AirflowNetworkZoneExhaustFan::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>());
  bool ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setFanZoneExhaust(fanZoneExhaust);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(massFlowCoefficient);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowExponentWhentheZoneExhaustFanisOff(massFlowExponent);
  OS_ASSERT(ok);
}

AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(const Model& model,
  const FanZoneExhaust &fanZoneExhaust, double massFlowCoefficient, double massFlowExponent,
  const AirflowNetworkReferenceCrackConditions &referenceCrackConditions)
  : ModelObject(AirflowNetworkZoneExhaustFan::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>());
  bool ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setFanZoneExhaust(fanZoneExhaust);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(massFlowCoefficient);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowExponentWhentheZoneExhaustFanisOff(massFlowExponent);
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setReferenceCrackConditions(referenceCrackConditions);
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkZoneExhaustFan::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkZoneExhaustFan);
}

boost::optional<FanZoneExhaust> AirflowNetworkZoneExhaustFan::fanZoneExhaust() const
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->fanZoneExhaust();
}

double AirflowNetworkZoneExhaustFan::airMassFlowCoefficientWhentheZoneExhaustFanisOff() const
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->airMassFlowCoefficientWhentheZoneExhaustFanisOff();
}

double AirflowNetworkZoneExhaustFan::airMassFlowExponentWhentheZoneExhaustFanisOff() const
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->airMassFlowExponentWhentheZoneExhaustFanisOff();
}

bool AirflowNetworkZoneExhaustFan::isAirMassFlowExponentWhentheZoneExhaustFanisOffDefaulted() const
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->isAirMassFlowExponentWhentheZoneExhaustFanisOffDefaulted();
}

boost::optional<AirflowNetworkReferenceCrackConditions> AirflowNetworkZoneExhaustFan::referenceCrackConditions() const
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->referenceCrackConditions();
}

bool AirflowNetworkZoneExhaustFan::setFanZoneExhaust(const FanZoneExhaust& fanZoneExhaust)
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setFanZoneExhaust(fanZoneExhaust);
}

bool AirflowNetworkZoneExhaustFan::setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(double airMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions)
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(airMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions);
}

bool AirflowNetworkZoneExhaustFan::setAirMassFlowExponentWhentheZoneExhaustFanisOff(double airMassFlowExponentWhentheZoneExhaustFanisOff)
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setAirMassFlowExponentWhentheZoneExhaustFanisOff(airMassFlowExponentWhentheZoneExhaustFanisOff);
}

void AirflowNetworkZoneExhaustFan::resetAirMassFlowExponentWhentheZoneExhaustFanisOff()
{
  getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->resetAirMassFlowExponentWhentheZoneExhaustFanisOff();
}

bool AirflowNetworkZoneExhaustFan::setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions &referenceCrackConditions)
{
  return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setReferenceCrackConditions(referenceCrackConditions);
}

void AirflowNetworkZoneExhaustFan::resetReferenceCrackConditions()
{
  getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->resetReferenceCrackConditions();
}

/// @cond
AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(std::shared_ptr<detail::AirflowNetworkZoneExhaustFan_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

