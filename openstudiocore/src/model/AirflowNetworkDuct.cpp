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

#include "AirflowNetworkDuct.hpp"
#include "AirflowNetworkDuct_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkDuct_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkDuct_Impl::AirflowNetworkDuct_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : AirflowNetworkComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDuct::iddObjectType());
  }

  AirflowNetworkDuct_Impl::AirflowNetworkDuct_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : AirflowNetworkComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkDuct::iddObjectType());
  }

  AirflowNetworkDuct_Impl::AirflowNetworkDuct_Impl(const AirflowNetworkDuct_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : AirflowNetworkComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkDuct_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType AirflowNetworkDuct_Impl::iddObjectType() const
  {
    return AirflowNetworkDuct::iddObjectType();
  }

  double AirflowNetworkDuct_Impl::ductLength() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::DuctLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkDuct_Impl::hydraulicDiameter() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::HydraulicDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkDuct_Impl::crossSectionArea() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::CrossSectionArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirflowNetworkDuct_Impl::surfaceRoughness() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::SurfaceRoughness,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDuct_Impl::isSurfaceRoughnessDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkDuctFields::SurfaceRoughness);
  }

  double AirflowNetworkDuct_Impl::coefficientforLocalDynamicLossDuetoFitting() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::CoefficientforLocalDynamicLossDuetoFitting,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDuct_Impl::isCoefficientforLocalDynamicLossDuetoFittingDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkDuctFields::CoefficientforLocalDynamicLossDuetoFitting);
  }

  double AirflowNetworkDuct_Impl::ductWallHeatTransmittanceCoefficient() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::HeatTransmittanceCoefficient_UFactor_forDuctWallConstruction, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDuct_Impl::isDuctWallHeatTransmittanceCoefficientDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkDuctFields::HeatTransmittanceCoefficient_UFactor_forDuctWallConstruction);
  }

  double AirflowNetworkDuct_Impl::outsideConvectionCoefficient() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::OutsideConvectionCoefficient, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDuct_Impl::isOutsideConvectionCoefficientDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkDuctFields::OutsideConvectionCoefficient);
  }

  double AirflowNetworkDuct_Impl::insideConvectionCoefficient() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::InsideConvectionCoefficient, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDuct_Impl::isInsideConvectionCoefficientDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkDuctFields::InsideConvectionCoefficient);
  }

  double AirflowNetworkDuct_Impl::overallMoistureTransmittanceCoefficientfromAirtoAir() const
  {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDuctFields::OverallMoistureTransmittanceCoefficientfromAirtoAir,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDuct_Impl::isOverallMoistureTransmittanceCoefficientfromAirtoAirDefaulted() const
  {
    return isEmpty(OS_AirflowNetworkDuctFields::OverallMoistureTransmittanceCoefficientfromAirtoAir);
  }

  bool AirflowNetworkDuct_Impl::setDuctLength(double ductLength)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::DuctLength, ductLength);
    return result;
  }

  bool AirflowNetworkDuct_Impl::setHydraulicDiameter(double hydraulicDiameter)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::HydraulicDiameter, hydraulicDiameter);
    return result;
  }

  bool AirflowNetworkDuct_Impl::setCrossSectionArea(double crossSectionArea)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::CrossSectionArea, crossSectionArea);
    return result;
  }

  bool AirflowNetworkDuct_Impl::setSurfaceRoughness(double surfaceRoughness)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::SurfaceRoughness, surfaceRoughness);
    return result;
  }

  void AirflowNetworkDuct_Impl::resetSurfaceRoughness()
  {
    bool result = setString(OS_AirflowNetworkDuctFields::SurfaceRoughness, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkDuct_Impl::setCoefficientforLocalDynamicLossDuetoFitting(double coefficientforLocalDynamicLossDuetoFitting)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::CoefficientforLocalDynamicLossDuetoFitting, coefficientforLocalDynamicLossDuetoFitting);
    return result;
  }

  void AirflowNetworkDuct_Impl::resetCoefficientforLocalDynamicLossDuetoFitting()
  {
    bool result = setString(OS_AirflowNetworkDuctFields::CoefficientforLocalDynamicLossDuetoFitting, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkDuct_Impl::setDuctWallHeatTransmittanceCoefficient(double coefficient)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::HeatTransmittanceCoefficient_UFactor_forDuctWallConstruction, coefficient);
    return result;
  }

  void AirflowNetworkDuct_Impl::resetDuctWallHeatTransmittanceCoefficient()
  {
    bool result = setString(OS_AirflowNetworkDuctFields::HeatTransmittanceCoefficient_UFactor_forDuctWallConstruction, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkDuct_Impl::setOutsideConvectionCoefficient(double coefficient)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::OutsideConvectionCoefficient, coefficient);
    return result;
  }

  void AirflowNetworkDuct_Impl::resetOutsideConvectionCoefficient()
  {
    bool result = setString(OS_AirflowNetworkDuctFields::OutsideConvectionCoefficient, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkDuct_Impl::setInsideConvectionCoefficient(double coefficient)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::InsideConvectionCoefficient, coefficient);
    return result;
  }

  void AirflowNetworkDuct_Impl::resetInsideConvectionCoefficient()
  {
    bool result = setString(OS_AirflowNetworkDuctFields::InsideConvectionCoefficient, "");
    OS_ASSERT(result);
  }

  bool AirflowNetworkDuct_Impl::setOverallMoistureTransmittanceCoefficientfromAirtoAir(double overallMoistureTransmittanceCoefficientfromAirtoAir)
  {
    bool result = setDouble(OS_AirflowNetworkDuctFields::OverallMoistureTransmittanceCoefficientfromAirtoAir, overallMoistureTransmittanceCoefficientfromAirtoAir);
    return result;
  }

  void AirflowNetworkDuct_Impl::resetOverallMoistureTransmittanceCoefficientfromAirtoAir()
  {
    bool result = setString(OS_AirflowNetworkDuctFields::OverallMoistureTransmittanceCoefficientfromAirtoAir, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkDuct::AirflowNetworkDuct(const Model& model)
  : AirflowNetworkComponent(AirflowNetworkDuct::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkDuct_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setDuctLength();
  OS_ASSERT(ok);
  // ok = setHydraulicDiameter();
  OS_ASSERT(ok);
  // ok = setCrossSectionArea();
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkDuct::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkDuct);
}

double AirflowNetworkDuct::ductLength() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->ductLength();
}

double AirflowNetworkDuct::hydraulicDiameter() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->hydraulicDiameter();
}

double AirflowNetworkDuct::crossSectionArea() const {
  return getImpl<detail::AirflowNetworkDuct_Impl>()->crossSectionArea();
}

double AirflowNetworkDuct::surfaceRoughness() const {
  return getImpl<detail::AirflowNetworkDuct_Impl>()->surfaceRoughness();
}

bool AirflowNetworkDuct::isSurfaceRoughnessDefaulted() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->isSurfaceRoughnessDefaulted();
}

double AirflowNetworkDuct::coefficientforLocalDynamicLossDuetoFitting() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->coefficientforLocalDynamicLossDuetoFitting();
}

bool AirflowNetworkDuct::isCoefficientforLocalDynamicLossDuetoFittingDefaulted() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->isCoefficientforLocalDynamicLossDuetoFittingDefaulted();
}

double AirflowNetworkDuct::ductWallHeatTransmittanceCoefficient() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->ductWallHeatTransmittanceCoefficient();
}

bool AirflowNetworkDuct::isDuctWallHeatTransmittanceCoefficientDefaulted() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->isDuctWallHeatTransmittanceCoefficientDefaulted();
}

double AirflowNetworkDuct::insideConvectionCoefficient() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->insideConvectionCoefficient();
}

bool AirflowNetworkDuct::isInsideConvectionCoefficientDefaulted() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->isInsideConvectionCoefficientDefaulted();
}

double AirflowNetworkDuct::outsideConvectionCoefficient() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->outsideConvectionCoefficient();
}

bool AirflowNetworkDuct::isOutsideConvectionCoefficientDefaulted() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->isOutsideConvectionCoefficientDefaulted();
}

double AirflowNetworkDuct::overallMoistureTransmittanceCoefficientfromAirtoAir() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->overallMoistureTransmittanceCoefficientfromAirtoAir();
}

bool AirflowNetworkDuct::isOverallMoistureTransmittanceCoefficientfromAirtoAirDefaulted() const
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->isOverallMoistureTransmittanceCoefficientfromAirtoAirDefaulted();
}

bool AirflowNetworkDuct::setDuctLength(double ductLength)
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setDuctLength(ductLength);
}

bool AirflowNetworkDuct::setHydraulicDiameter(double hydraulicDiameter)
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setHydraulicDiameter(hydraulicDiameter);
}

bool AirflowNetworkDuct::setCrossSectionArea(double crossSectionArea)
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setCrossSectionArea(crossSectionArea);
}

bool AirflowNetworkDuct::setSurfaceRoughness(double surfaceRoughness)
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setSurfaceRoughness(surfaceRoughness);
}

void AirflowNetworkDuct::resetSurfaceRoughness()
{
  getImpl<detail::AirflowNetworkDuct_Impl>()->resetSurfaceRoughness();
}

bool AirflowNetworkDuct::setCoefficientforLocalDynamicLossDuetoFitting(double coefficientforLocalDynamicLossDuetoFitting)
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setCoefficientforLocalDynamicLossDuetoFitting(coefficientforLocalDynamicLossDuetoFitting);
}

void AirflowNetworkDuct::resetCoefficientforLocalDynamicLossDuetoFitting()
{
  getImpl<detail::AirflowNetworkDuct_Impl>()->resetCoefficientforLocalDynamicLossDuetoFitting();
}

bool AirflowNetworkDuct::setDuctWallHeatTransmittanceCoefficient(double coefficient) {
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setDuctWallHeatTransmittanceCoefficient(coefficient);
}

void AirflowNetworkDuct::resetDuctWallHeatTransmittanceCoefficient()
{
  getImpl<detail::AirflowNetworkDuct_Impl>()->resetDuctWallHeatTransmittanceCoefficient();
}

bool AirflowNetworkDuct::setInsideConvectionCoefficient(double coefficient) {
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setInsideConvectionCoefficient(coefficient);
}

void AirflowNetworkDuct::resetInsideConvectionCoefficient()
{
  getImpl<detail::AirflowNetworkDuct_Impl>()->resetInsideConvectionCoefficient();
}

bool AirflowNetworkDuct::setOutsideConvectionCoefficient(double coefficient) {
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setOutsideConvectionCoefficient(coefficient);
}

void AirflowNetworkDuct::resetOutsideConvectionCoefficient()
{
  getImpl<detail::AirflowNetworkDuct_Impl>()->resetOutsideConvectionCoefficient();
}

bool AirflowNetworkDuct::setOverallMoistureTransmittanceCoefficientfromAirtoAir(double overallMoistureTransmittanceCoefficientfromAirtoAir)
{
  return getImpl<detail::AirflowNetworkDuct_Impl>()->setOverallMoistureTransmittanceCoefficientfromAirtoAir(overallMoistureTransmittanceCoefficientfromAirtoAir);
}

void AirflowNetworkDuct::resetOverallMoistureTransmittanceCoefficientfromAirtoAir()
{
  getImpl<detail::AirflowNetworkDuct_Impl>()->resetOverallMoistureTransmittanceCoefficientfromAirtoAir();
}

/// @cond
AirflowNetworkDuct::AirflowNetworkDuct(std::shared_ptr<detail::AirflowNetworkDuct_Impl> impl)
  : AirflowNetworkComponent(impl)
{}
/// @endcond

} // model
} // openstudio

