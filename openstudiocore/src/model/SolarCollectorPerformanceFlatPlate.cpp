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

#include "SolarCollectorPerformanceFlatPlate.hpp"
#include "SolarCollectorPerformanceFlatPlate_Impl.hpp"


#include "SolarCollectorFlatPlateWater.hpp"
#include "SolarCollectorFlatPlateWater_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SolarCollectorPerformance_FlatPlate_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SolarCollectorPerformanceFlatPlate_Impl::SolarCollectorPerformanceFlatPlate_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SolarCollectorPerformanceFlatPlate::iddObjectType());
  }

  SolarCollectorPerformanceFlatPlate_Impl::SolarCollectorPerformanceFlatPlate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SolarCollectorPerformanceFlatPlate::iddObjectType());
  }

  SolarCollectorPerformanceFlatPlate_Impl::SolarCollectorPerformanceFlatPlate_Impl(const SolarCollectorPerformanceFlatPlate_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SolarCollectorPerformanceFlatPlate_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SolarCollectorPerformanceFlatPlate_Impl::iddObjectType() const {
    return SolarCollectorPerformanceFlatPlate::iddObjectType();
  }

  double SolarCollectorPerformanceFlatPlate_Impl::grossArea() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_FlatPlateFields::GrossArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string SolarCollectorPerformanceFlatPlate_Impl::testFluid() const {
    boost::optional<std::string> value = getString(OS_SolarCollectorPerformance_FlatPlateFields::TestFluid,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceFlatPlate_Impl::isTestFluidDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_FlatPlateFields::TestFluid);
  }

  double SolarCollectorPerformanceFlatPlate_Impl::testFlowRate() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_FlatPlateFields::TestFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string SolarCollectorPerformanceFlatPlate_Impl::testCorrelationType() const {
    boost::optional<std::string> value = getString(OS_SolarCollectorPerformance_FlatPlateFields::TestCorrelationType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformanceFlatPlate_Impl::coefficient1ofEfficiencyEquation() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient1ofEfficiencyEquation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformanceFlatPlate_Impl::coefficient2ofEfficiencyEquation() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient2ofEfficiencyEquation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> SolarCollectorPerformanceFlatPlate_Impl::coefficient3ofEfficiencyEquation() const {
    return getDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient3ofEfficiencyEquation,true);
  }

  boost::optional<double> SolarCollectorPerformanceFlatPlate_Impl::coefficient2ofIncidentAngleModifier() const {
    return getDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient2ofIncidentAngleModifier,true);
  }

  boost::optional<double> SolarCollectorPerformanceFlatPlate_Impl::coefficient3ofIncidentAngleModifier() const {
    return getDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient3ofIncidentAngleModifier,true);
  }

  bool SolarCollectorPerformanceFlatPlate_Impl::setGrossArea(double grossArea) {
    bool result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::GrossArea, grossArea);
    return result;
  }

  bool SolarCollectorPerformanceFlatPlate_Impl::setTestFluid(std::string testFluid) {
    bool result = setString(OS_SolarCollectorPerformance_FlatPlateFields::TestFluid, testFluid);
    return result;
  }

  void SolarCollectorPerformanceFlatPlate_Impl::resetTestFluid() {
    bool result = setString(OS_SolarCollectorPerformance_FlatPlateFields::TestFluid, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceFlatPlate_Impl::setTestFlowRate(double testFlowRate) {
    bool result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::TestFlowRate, testFlowRate);
    return result;
  }

  bool SolarCollectorPerformanceFlatPlate_Impl::setTestCorrelationType(std::string testCorrelationType) {
    bool result = setString(OS_SolarCollectorPerformance_FlatPlateFields::TestCorrelationType, testCorrelationType);
    return result;
  }

  void SolarCollectorPerformanceFlatPlate_Impl::setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation) {
    bool result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient1ofEfficiencyEquation, coefficient1ofEfficiencyEquation);
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation) {
    bool result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient2ofEfficiencyEquation, coefficient2ofEfficiencyEquation);
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::setCoefficient3ofEfficiencyEquation(boost::optional<double> coefficient3ofEfficiencyEquation) {
    bool result(false);
    if (coefficient3ofEfficiencyEquation) {
      result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient3ofEfficiencyEquation, coefficient3ofEfficiencyEquation.get());
    }
    else {
      resetCoefficient3ofEfficiencyEquation();
      result = true;
    }
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::resetCoefficient3ofEfficiencyEquation() {
    bool result = setString(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient3ofEfficiencyEquation, "");
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::setCoefficient2ofIncidentAngleModifier(boost::optional<double> coefficient2ofIncidentAngleModifier) {
    bool result(false);
    if (coefficient2ofIncidentAngleModifier) {
      result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient2ofIncidentAngleModifier, coefficient2ofIncidentAngleModifier.get());
    }
    else {
      resetCoefficient2ofIncidentAngleModifier();
      result = true;
    }
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::resetCoefficient2ofIncidentAngleModifier() {
    bool result = setString(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient2ofIncidentAngleModifier, "");
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::setCoefficient3ofIncidentAngleModifier(boost::optional<double> coefficient3ofIncidentAngleModifier) {
    bool result(false);
    if (coefficient3ofIncidentAngleModifier) {
      result = setDouble(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient3ofIncidentAngleModifier, coefficient3ofIncidentAngleModifier.get());
    }
    else {
      resetCoefficient3ofIncidentAngleModifier();
      result = true;
    }
    OS_ASSERT(result);
  }

  void SolarCollectorPerformanceFlatPlate_Impl::resetCoefficient3ofIncidentAngleModifier() {
    bool result = setString(OS_SolarCollectorPerformance_FlatPlateFields::Coefficient3ofIncidentAngleModifier, "");
    OS_ASSERT(result);
  }

} // detail

SolarCollectorPerformanceFlatPlate::SolarCollectorPerformanceFlatPlate(const Model& model)
  : ModelObject(SolarCollectorPerformanceFlatPlate::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>());

  // Defaults from "Alternate Energy Technologies AE - 32" in SolarCollectors.imf in E+ data sets.
  bool ok = true;;
  ok = setGrossArea(2.9646);
  OS_ASSERT(ok);
  ok = setTestFluid("Water");
  OS_ASSERT(ok);
  ok = setTestFlowRate(0.0000388);
  OS_ASSERT(ok);
  ok = setTestCorrelationType("Inlet");
  OS_ASSERT(ok);
  setCoefficient1ofEfficiencyEquation(0.691);
  setCoefficient2ofEfficiencyEquation(-3.396);
  setCoefficient2ofEfficiencyEquation(-0.00193);
  setCoefficient2ofIncidentAngleModifier(-0.1939);
  setCoefficient3ofIncidentAngleModifier(-0.0055);
}

IddObjectType SolarCollectorPerformanceFlatPlate::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollectorPerformance_FlatPlate);
}

std::vector<std::string> SolarCollectorPerformanceFlatPlate::testFluidValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SolarCollectorPerformance_FlatPlateFields::TestFluid);
}

std::vector<std::string> SolarCollectorPerformanceFlatPlate::testCorrelationTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SolarCollectorPerformance_FlatPlateFields::TestCorrelationType);
}

double SolarCollectorPerformanceFlatPlate::grossArea() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->grossArea();
}

std::string SolarCollectorPerformanceFlatPlate::testFluid() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->testFluid();
}

bool SolarCollectorPerformanceFlatPlate::isTestFluidDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->isTestFluidDefaulted();
}

double SolarCollectorPerformanceFlatPlate::testFlowRate() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->testFlowRate();
}

std::string SolarCollectorPerformanceFlatPlate::testCorrelationType() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->testCorrelationType();
}

double SolarCollectorPerformanceFlatPlate::coefficient1ofEfficiencyEquation() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->coefficient1ofEfficiencyEquation();
}

double SolarCollectorPerformanceFlatPlate::coefficient2ofEfficiencyEquation() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->coefficient2ofEfficiencyEquation();
}

boost::optional<double> SolarCollectorPerformanceFlatPlate::coefficient3ofEfficiencyEquation() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->coefficient3ofEfficiencyEquation();
}

boost::optional<double> SolarCollectorPerformanceFlatPlate::coefficient2ofIncidentAngleModifier() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->coefficient2ofIncidentAngleModifier();
}

boost::optional<double> SolarCollectorPerformanceFlatPlate::coefficient3ofIncidentAngleModifier() const {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->coefficient3ofIncidentAngleModifier();
}

bool SolarCollectorPerformanceFlatPlate::setGrossArea(double grossArea) {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setGrossArea(grossArea);
}

bool SolarCollectorPerformanceFlatPlate::setTestFluid(std::string testFluid) {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setTestFluid(testFluid);
}

void SolarCollectorPerformanceFlatPlate::resetTestFluid() {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->resetTestFluid();
}

bool SolarCollectorPerformanceFlatPlate::setTestFlowRate(double testFlowRate) {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setTestFlowRate(testFlowRate);
}

bool SolarCollectorPerformanceFlatPlate::setTestCorrelationType(std::string testCorrelationType) {
  return getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setTestCorrelationType(testCorrelationType);
}

void SolarCollectorPerformanceFlatPlate::setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation) {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setCoefficient1ofEfficiencyEquation(coefficient1ofEfficiencyEquation);
}

void SolarCollectorPerformanceFlatPlate::setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation) {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setCoefficient2ofEfficiencyEquation(coefficient2ofEfficiencyEquation);
}

void SolarCollectorPerformanceFlatPlate::setCoefficient3ofEfficiencyEquation(double coefficient3ofEfficiencyEquation) {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setCoefficient3ofEfficiencyEquation(coefficient3ofEfficiencyEquation);
}

void SolarCollectorPerformanceFlatPlate::resetCoefficient3ofEfficiencyEquation() {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->resetCoefficient3ofEfficiencyEquation();
}

void SolarCollectorPerformanceFlatPlate::setCoefficient2ofIncidentAngleModifier(double coefficient2ofIncidentAngleModifier) {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setCoefficient2ofIncidentAngleModifier(coefficient2ofIncidentAngleModifier);
}

void SolarCollectorPerformanceFlatPlate::resetCoefficient2ofIncidentAngleModifier() {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->resetCoefficient2ofIncidentAngleModifier();
}

void SolarCollectorPerformanceFlatPlate::setCoefficient3ofIncidentAngleModifier(double coefficient3ofIncidentAngleModifier) {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->setCoefficient3ofIncidentAngleModifier(coefficient3ofIncidentAngleModifier);
}

void SolarCollectorPerformanceFlatPlate::resetCoefficient3ofIncidentAngleModifier() {
  getImpl<detail::SolarCollectorPerformanceFlatPlate_Impl>()->resetCoefficient3ofIncidentAngleModifier();
}

/// @cond
SolarCollectorPerformanceFlatPlate::SolarCollectorPerformanceFlatPlate(std::shared_ptr<detail::SolarCollectorPerformanceFlatPlate_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

