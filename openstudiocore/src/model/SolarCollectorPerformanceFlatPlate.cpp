/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

