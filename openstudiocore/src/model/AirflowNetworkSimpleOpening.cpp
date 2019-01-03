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

#include "AirflowNetworkSimpleOpening.hpp"
#include "AirflowNetworkSimpleOpening_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkSimpleOpening_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkSimpleOpening_Impl::AirflowNetworkSimpleOpening_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : AirflowNetworkComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkSimpleOpening::iddObjectType());
  }

  AirflowNetworkSimpleOpening_Impl::AirflowNetworkSimpleOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model, bool keepHandle)
    : AirflowNetworkComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkSimpleOpening::iddObjectType());
  }

  AirflowNetworkSimpleOpening_Impl::AirflowNetworkSimpleOpening_Impl(const AirflowNetworkSimpleOpening_Impl& other,
                                                                     Model_Impl* model, bool keepHandle)
    : AirflowNetworkComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkSimpleOpening_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType AirflowNetworkSimpleOpening_Impl::iddObjectType() const
  {
    return AirflowNetworkSimpleOpening::iddObjectType();
  }

  double AirflowNetworkSimpleOpening_Impl::airMassFlowCoefficientWhenOpeningisClosed() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkSimpleOpening_Impl::airMassFlowExponentWhenOpeningisClosed() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkSimpleOpening_Impl::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed);
  }

  double AirflowNetworkSimpleOpening_Impl::minimumDensityDifferenceforTwoWayFlow() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::MinimumDensityDifferenceforTwoWayFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkSimpleOpening_Impl::dischargeCoefficient() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::DischargeCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkSimpleOpening_Impl::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed)
  {
    return setDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, airMassFlowCoefficientWhenOpeningisClosed);
  }

  bool AirflowNetworkSimpleOpening_Impl::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed)
  {
    return setDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed, airMassFlowExponentWhenOpeningisClosed);
  }

  void AirflowNetworkSimpleOpening_Impl::resetAirMassFlowExponentWhenOpeningisClosed()
  {
    bool result = setString(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkSimpleOpening_Impl::setMinimumDensityDifferenceforTwoWayFlow(double minimumDensityDifferenceforTwoWayFlow)
  {
    return setDouble(OS_AirflowNetworkSimpleOpeningFields::MinimumDensityDifferenceforTwoWayFlow, minimumDensityDifferenceforTwoWayFlow);
  }

  bool AirflowNetworkSimpleOpening_Impl::setDischargeCoefficient(double dischargeCoefficient)
  {
    bool result = setDouble(OS_AirflowNetworkSimpleOpeningFields::DischargeCoefficient, dischargeCoefficient);
    return result;
  }

} // detail

AirflowNetworkSimpleOpening::AirflowNetworkSimpleOpening(const Model& model,
  double massFlowCoefficientWhenOpeningisClosed,
  double minimumDensityDifferenceforTwoWayFlow,
  double dischargeCoefficient)
  : AirflowNetworkComponent(AirflowNetworkSimpleOpening::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSimpleOpening_Impl>());

  bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
  OS_ASSERT(ok);
  ok = setMinimumDensityDifferenceforTwoWayFlow(minimumDensityDifferenceforTwoWayFlow);
  OS_ASSERT(ok);
  ok = setDischargeCoefficient(dischargeCoefficient);
  OS_ASSERT(ok);
}

AirflowNetworkSimpleOpening::AirflowNetworkSimpleOpening(const Model& model,
  double massFlowCoefficientWhenOpeningisClosed,
  double massFlowExponentWhenOpeningisClosed,
  double minimumDensityDifferenceforTwoWayFlow,
  double dischargeCoefficient)
  : AirflowNetworkComponent(AirflowNetworkSimpleOpening::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSimpleOpening_Impl>());

  bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
  OS_ASSERT(ok);
  ok = setAirMassFlowExponentWhenOpeningisClosed(massFlowExponentWhenOpeningisClosed);
  OS_ASSERT(ok);
  ok = setMinimumDensityDifferenceforTwoWayFlow(minimumDensityDifferenceforTwoWayFlow);
  OS_ASSERT(ok);
  ok = setDischargeCoefficient(dischargeCoefficient);
  OS_ASSERT(ok);
}


IddObjectType AirflowNetworkSimpleOpening::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkSimpleOpening);
}

double AirflowNetworkSimpleOpening::airMassFlowCoefficientWhenOpeningisClosed() const
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->airMassFlowCoefficientWhenOpeningisClosed();
}

double AirflowNetworkSimpleOpening::airMassFlowExponentWhenOpeningisClosed() const
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->airMassFlowExponentWhenOpeningisClosed();
}

bool AirflowNetworkSimpleOpening::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->isAirMassFlowExponentWhenOpeningisClosedDefaulted();
}

double AirflowNetworkSimpleOpening::minimumDensityDifferenceforTwoWayFlow() const
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->minimumDensityDifferenceforTwoWayFlow();
}

double AirflowNetworkSimpleOpening::dischargeCoefficient() const
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->dischargeCoefficient();
}

bool AirflowNetworkSimpleOpening::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed)
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setAirMassFlowCoefficientWhenOpeningisClosed(airMassFlowCoefficientWhenOpeningisClosed);
}

bool AirflowNetworkSimpleOpening::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setAirMassFlowExponentWhenOpeningisClosed(airMassFlowExponentWhenOpeningisClosed);
}

void AirflowNetworkSimpleOpening::resetAirMassFlowExponentWhenOpeningisClosed()
{
  getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->resetAirMassFlowExponentWhenOpeningisClosed();
}

bool AirflowNetworkSimpleOpening::setMinimumDensityDifferenceforTwoWayFlow(double minimumDensityDifferenceforTwoWayFlow)
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setMinimumDensityDifferenceforTwoWayFlow(minimumDensityDifferenceforTwoWayFlow);
}

bool AirflowNetworkSimpleOpening::setDischargeCoefficient(double dischargeCoefficient)
{
  return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setDischargeCoefficient(dischargeCoefficient);
}

/// @cond
AirflowNetworkSimpleOpening::AirflowNetworkSimpleOpening(std::shared_ptr<detail::AirflowNetworkSimpleOpening_Impl> impl)
  : AirflowNetworkComponent(impl)
{}
/// @endcond

} // model
} // openstudio

