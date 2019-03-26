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

#include "AirflowNetworkSimulationControl.hpp"
#include "AirflowNetworkSimulationControl_Impl.hpp"

#include "Model.hpp"
#include "ConvergenceLimits.hpp"
#include "ConvergenceLimits_Impl.hpp"
#include "HeatBalanceAlgorithm.hpp"
#include "HeatBalanceAlgorithm_Impl.hpp"
#include "InsideSurfaceConvectionAlgorithm.hpp"
#include "InsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "OutsideSurfaceConvectionAlgorithm.hpp"
#include "OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "ShadowCalculation.hpp"
#include "ShadowCalculation_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"
#include "SizingParameters.hpp"
#include "SizingParameters_Impl.hpp"
#include "Timestep.hpp"
#include "Timestep_Impl.hpp"
#include "WeatherFile.hpp"
#include "WeatherFile_Impl.hpp"
#include "ZoneAirContaminantBalance.hpp"
#include "ZoneAirContaminantBalance_Impl.hpp"
#include "ZoneAirHeatBalanceAlgorithm.hpp"
#include "ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirflowNetworkSimulationControl_FieldEnums.hxx>

#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/units/Unit.hpp"

#include <boost/bind.hpp>

namespace openstudio{
namespace model{

namespace detail{

AirflowNetworkSimulationControl_Impl::AirflowNetworkSimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
: ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkSimulationControl::iddObjectType());
}

AirflowNetworkSimulationControl_Impl::AirflowNetworkSimulationControl_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == AirflowNetworkSimulationControl::iddObjectType());
}

AirflowNetworkSimulationControl_Impl::AirflowNetworkSimulationControl_Impl(const AirflowNetworkSimulationControl_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
                                                                           : ParentObject_Impl(other,model,keepHandle)
{
}

AirflowNetworkSimulationControl_Impl::~AirflowNetworkSimulationControl_Impl()
{}

// return the parent object in the hierarchy
boost::optional<ParentObject> AirflowNetworkSimulationControl_Impl::parent() const
{
  // Simulation is highest level, there is no parent
  return boost::optional<ParentObject>();
}

// return any children objects in the hierarchy
std::vector<ModelObject> AirflowNetworkSimulationControl_Impl::children() const
{
  std::vector<ModelObject> result;

  /*
  OptionalConvergenceLimits ocl = convergenceLimits();
  if (ocl) { result.push_back(*ocl); }

  //OptionalDaylightSavingsTime odst = daylightSavingsTime();
  //if (odst) { result.push_back(*odst); }

  OptionalHeatBalanceAlgorithm ohba = heatBalanceAlgorithm();
  if (ohba) { result.push_back(*ohba); }

  OptionalInsideSurfaceConvectionAlgorithm oisca = insideSurfaceConvectionAlgorithm();
  if (oisca) { result.push_back(*oisca); }

  OptionalOutsideSurfaceConvectionAlgorithm oosca = outsideSurfaceConvectionAlgorithm();
  if (oosca) { result.push_back(*oosca); }

  RunPeriodVector rps = runPeriods();
  result.insert(result.end(),rps.begin(),rps.end());

  OptionalShadowCalculation osc = shadowCalculation();
  if (osc) { result.push_back(*osc); }

  //SpecialDaysVector sds = specialDays();
  //result.insert(result.end(),sds.begin(),sds.end());

  OptionalSizingParameters osp = sizingParameters();
  if (osp) { result.push_back(*osp); }

  OptionalTimestep ot = timestep();
  if (ot) { result.push_back(*ot); }

  OptionalZoneAirContaminantBalance ozacb = zoneAirContaminantBalance();
  if (ozacb) { result.push_back(*ozacb); }

  OptionalZoneAirHeatBalanceAlgorithm ozahba = zoneAirHeatBalanceAlgorithm();
  if (ozahba) { result.push_back(*ozahba); }

  OptionalZoneCapacitanceMultiplierResearchSpecial ozcmrs;
  ozcmrs = zoneCapacitanceMultiplierResearchSpecial();
  if (ozcmrs) { result.push_back(*ozcmrs); }
  */

  return result;
}

std::vector<IddObjectType> AirflowNetworkSimulationControl_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  /*
  result.push_back(ConvergenceLimits::iddObjectType());
  result.push_back(HeatBalanceAlgorithm::iddObjectType());
  result.push_back(InsideSurfaceConvectionAlgorithm::iddObjectType());
  result.push_back(OutsideSurfaceConvectionAlgorithm::iddObjectType());
  result.push_back(RunPeriod::iddObjectType());
  result.push_back(ShadowCalculation::iddObjectType());
  result.push_back(SizingParameters::iddObjectType());
  result.push_back(Timestep::iddObjectType());
  result.push_back(ZoneAirContaminantBalance::iddObjectType());
  result.push_back(ZoneAirHeatBalanceAlgorithm::iddObjectType());
  result.push_back(ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
  */
  return result;
}

const std::vector<std::string>& AirflowNetworkSimulationControl_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  return result;
}

IddObjectType AirflowNetworkSimulationControl_Impl::iddObjectType() const {
  return AirflowNetworkSimulationControl::iddObjectType();
}

boost::optional<std::string> AirflowNetworkSimulationControl_Impl::airflowNetworkControl() const
{
  return getString(OS_AirflowNetworkSimulationControlFields::AirflowNetworkControl,true);
}

bool AirflowNetworkSimulationControl_Impl::isAirflowNetworkControlDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::AirflowNetworkControl);
}

bool AirflowNetworkSimulationControl_Impl::setAirflowNetworkControl(std::string control)
{
  return setString(OS_AirflowNetworkSimulationControlFields::AirflowNetworkControl, control);
}

void AirflowNetworkSimulationControl_Impl::resetAirflowNetworkControl() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::AirflowNetworkControl, "");
  OS_ASSERT(result);
}

boost::optional<std::string> AirflowNetworkSimulationControl_Impl::windPressureCoefficientType() const
{
  return getString(OS_AirflowNetworkSimulationControlFields::WindPressureCoefficientType,true);
}

bool AirflowNetworkSimulationControl_Impl::isWindPressureCoefficientTypeDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::WindPressureCoefficientType);
}

bool AirflowNetworkSimulationControl_Impl::setWindPressureCoefficientType(std::string type)
{
  return setString(OS_AirflowNetworkSimulationControlFields::WindPressureCoefficientType, type);
}

void AirflowNetworkSimulationControl_Impl::resetWindPressureCoefficientType() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::WindPressureCoefficientType, "");
  OS_ASSERT(result);
}

boost::optional<std::string> AirflowNetworkSimulationControl_Impl::heightSelectionforLocalWindPressureCalculation() const
{
  return getString(OS_AirflowNetworkSimulationControlFields::HeightSelectionforLocalWindPressureCalculation, true);
}

bool AirflowNetworkSimulationControl_Impl::isHeightSelectionforLocalWindPressureCalculationDefaulted() const
{
  return isEmpty(OS_AirflowNetworkSimulationControlFields::HeightSelectionforLocalWindPressureCalculation);
}

bool AirflowNetworkSimulationControl_Impl::setHeightSelectionforLocalWindPressureCalculation(std::string type)
{
  return setString(OS_AirflowNetworkSimulationControlFields::HeightSelectionforLocalWindPressureCalculation, type);
}

void AirflowNetworkSimulationControl_Impl::resetHeightSelectionforLocalWindPressureCalculation() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::HeightSelectionforLocalWindPressureCalculation, "");
  OS_ASSERT(result);
}

boost::optional<std::string> AirflowNetworkSimulationControl_Impl::buildingType() const
{
  return getString(OS_AirflowNetworkSimulationControlFields::BuildingType,true);
}

bool AirflowNetworkSimulationControl_Impl::isBuildingTypeDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::BuildingType);
}

bool AirflowNetworkSimulationControl_Impl::setBuildingType(std::string type)
{
  return setString(OS_AirflowNetworkSimulationControlFields::BuildingType, type);
}

void AirflowNetworkSimulationControl_Impl::resetBuildingType() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::BuildingType, "");
  OS_ASSERT(result);
}

boost::optional<int> AirflowNetworkSimulationControl_Impl::maximumNumberofIterations() const
{
  return getInt(OS_AirflowNetworkSimulationControlFields::MaximumNumberofIterations,true);
}

bool AirflowNetworkSimulationControl_Impl::isMaximumNumberofIterationsDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::MaximumNumberofIterations);
}

bool AirflowNetworkSimulationControl_Impl::setMaximumNumberofIterations(int number)
{
  return setInt(OS_AirflowNetworkSimulationControlFields::MaximumNumberofIterations, number);
}

void AirflowNetworkSimulationControl_Impl::resetMaximumNumberofIterations() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::MaximumNumberofIterations, "");
  OS_ASSERT(result);
}

boost::optional<std::string> AirflowNetworkSimulationControl_Impl::initializationType() const
{
  return getString(OS_AirflowNetworkSimulationControlFields::InitializationType,true);
}

bool AirflowNetworkSimulationControl_Impl::isInitializationTypeDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::InitializationType);
}

bool AirflowNetworkSimulationControl_Impl::setInitializationType(std::string type)
{
  return setString(OS_AirflowNetworkSimulationControlFields::InitializationType, type);
}

void AirflowNetworkSimulationControl_Impl::resetInitializationType() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::InitializationType, "");
  OS_ASSERT(result);
}

boost::optional<double> AirflowNetworkSimulationControl_Impl::relativeAirflowConvergenceTolerance() const
{
  return getDouble(OS_AirflowNetworkSimulationControlFields::RelativeAirflowConvergenceTolerance,true);
}

bool AirflowNetworkSimulationControl_Impl::isRelativeAirflowConvergenceToleranceDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::RelativeAirflowConvergenceTolerance);
}

bool AirflowNetworkSimulationControl_Impl::setRelativeAirflowConvergenceTolerance(double tolerance)
{
  return setDouble(OS_AirflowNetworkSimulationControlFields::RelativeAirflowConvergenceTolerance, tolerance);
}

void AirflowNetworkSimulationControl_Impl::resetRelativeAirflowConvergenceTolerance() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::RelativeAirflowConvergenceTolerance, "");
  OS_ASSERT(result);
}

boost::optional<double> AirflowNetworkSimulationControl_Impl::absoluteAirflowConvergenceTolerance() const
{
  return getDouble(OS_AirflowNetworkSimulationControlFields::AbsoluteAirflowConvergenceTolerance,true);
}

bool AirflowNetworkSimulationControl_Impl::isAbsoluteAirflowConvergenceToleranceDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::AbsoluteAirflowConvergenceTolerance);
}

bool AirflowNetworkSimulationControl_Impl::setAbsoluteAirflowConvergenceTolerance(double tolerance)
{
  return setDouble(OS_AirflowNetworkSimulationControlFields::AbsoluteAirflowConvergenceTolerance, tolerance);
}

void AirflowNetworkSimulationControl_Impl::resetAbsoluteAirflowConvergenceTolerance() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::AbsoluteAirflowConvergenceTolerance, "");
  OS_ASSERT(result);
}

boost::optional<double> AirflowNetworkSimulationControl_Impl::convergenceAccelerationLimit() const
{
  return getDouble(OS_AirflowNetworkSimulationControlFields::ConvergenceAccelerationLimit,true);
}

bool AirflowNetworkSimulationControl_Impl::isConvergenceAccelerationLimitDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::ConvergenceAccelerationLimit);
}

bool AirflowNetworkSimulationControl_Impl::setConvergenceAccelerationLimit(double limit)
{
  return setDouble(OS_AirflowNetworkSimulationControlFields::ConvergenceAccelerationLimit, limit);
}

void AirflowNetworkSimulationControl_Impl::resetConvergenceAccelerationLimit() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::ConvergenceAccelerationLimit, "");
  OS_ASSERT(result);
}

boost::optional<double> AirflowNetworkSimulationControl_Impl::azimuthAngleofLongAxisofBuilding() const
{
  return getDouble(OS_AirflowNetworkSimulationControlFields::AzimuthAngleofLongAxisofBuilding,true);
}

bool AirflowNetworkSimulationControl_Impl::isAzimuthAngleofLongAxisofBuildingDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::AzimuthAngleofLongAxisofBuilding);
}

bool AirflowNetworkSimulationControl_Impl::setAzimuthAngleofLongAxisofBuilding(double angle)
{
  return setDouble(OS_AirflowNetworkSimulationControlFields::AzimuthAngleofLongAxisofBuilding, angle);
}

void AirflowNetworkSimulationControl_Impl::resetAzimuthAngleofLongAxisofBuilding() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::AzimuthAngleofLongAxisofBuilding, "");
  OS_ASSERT(result);
}

boost::optional<double> AirflowNetworkSimulationControl_Impl::buildingAspectRatio() const
{
  return getDouble(OS_AirflowNetworkSimulationControlFields::RatioofBuildingWidthAlongShortAxistoWidthAlongLongAxis,true);
}

bool AirflowNetworkSimulationControl_Impl::isBuildingAspectRatioDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::RatioofBuildingWidthAlongShortAxistoWidthAlongLongAxis);
}

bool AirflowNetworkSimulationControl_Impl::setBuildingAspectRatio(double ratio)
{
  return setDouble(OS_AirflowNetworkSimulationControlFields::RatioofBuildingWidthAlongShortAxistoWidthAlongLongAxis, ratio);
}

void AirflowNetworkSimulationControl_Impl::resetBuildingAspectRatio() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::RatioofBuildingWidthAlongShortAxistoWidthAlongLongAxis, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSimulationControl_Impl::heightDependenceofExternalNodeTemperature() const
{
  boost::optional<std::string> opt = getString(OS_AirflowNetworkSimulationControlFields::HeightDependenceofExternalNodeTemperature, true);
  if (opt) {
    return openstudio::istringEqual(opt.get(), "Yes");
  }
  return false; // Default is "No"
}

bool AirflowNetworkSimulationControl_Impl::isHeightDependenceofExternalNodeTemperatureDefaulted() const {
  return isEmpty(OS_AirflowNetworkSimulationControlFields::HeightDependenceofExternalNodeTemperature);
}

bool AirflowNetworkSimulationControl_Impl::setHeightDependenceofExternalNodeTemperature(bool tf)
{
  if (tf) {
    return setString(OS_AirflowNetworkSimulationControlFields::HeightDependenceofExternalNodeTemperature, "Yes");
  }
  return setString(OS_AirflowNetworkSimulationControlFields::HeightDependenceofExternalNodeTemperature, "No");
}

void AirflowNetworkSimulationControl_Impl::resetHeightDependenceofExternalNodeTemperature() {
  bool result = setString(OS_AirflowNetworkSimulationControlFields::HeightDependenceofExternalNodeTemperature, "");
  OS_ASSERT(result);
}

} // detail

AirflowNetworkSimulationControl::AirflowNetworkSimulationControl( const Model& model ):
ParentObject(iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSimulationControl_Impl>());
}

AirflowNetworkSimulationControl::AirflowNetworkSimulationControl(std::shared_ptr<detail::AirflowNetworkSimulationControl_Impl> impl)
: ParentObject(impl)
{}

IddObjectType AirflowNetworkSimulationControl::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirflowNetworkSimulationControl);
}

boost::optional<std::string> AirflowNetworkSimulationControl::airflowNetworkControl() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->airflowNetworkControl();
}

bool AirflowNetworkSimulationControl::isAirflowNetworkControlDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isAirflowNetworkControlDefaulted();
}

boost::optional<std::string> AirflowNetworkSimulationControl::windPressureCoefficientType() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->windPressureCoefficientType();
}

bool AirflowNetworkSimulationControl::isWindPressureCoefficientTypeDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isWindPressureCoefficientTypeDefaulted();
}

boost::optional<std::string> AirflowNetworkSimulationControl::heightSelectionforLocalWindPressureCalculation() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->heightSelectionforLocalWindPressureCalculation();
}

bool AirflowNetworkSimulationControl::isHeightSelectionforLocalWindPressureCalculationDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isHeightSelectionforLocalWindPressureCalculationDefaulted();
}

boost::optional<std::string> AirflowNetworkSimulationControl::buildingType() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->buildingType();
}

bool AirflowNetworkSimulationControl::isBuildingTypeDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isBuildingTypeDefaulted();
}

boost::optional<int> AirflowNetworkSimulationControl::maximumNumberofIterations() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->maximumNumberofIterations();
}

bool AirflowNetworkSimulationControl::isMaximumNumberofIterationsDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isMaximumNumberofIterationsDefaulted();
}

boost::optional<std::string> AirflowNetworkSimulationControl::initializationType() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->initializationType();
}

bool AirflowNetworkSimulationControl::isInitializationTypeDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isInitializationTypeDefaulted();
}

boost::optional<double> AirflowNetworkSimulationControl::relativeAirflowConvergenceTolerance() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->relativeAirflowConvergenceTolerance();
}

bool AirflowNetworkSimulationControl::isRelativeAirflowConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isRelativeAirflowConvergenceToleranceDefaulted();
}

boost::optional<double> AirflowNetworkSimulationControl::absoluteAirflowConvergenceTolerance() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->absoluteAirflowConvergenceTolerance();
}

bool AirflowNetworkSimulationControl::isAbsoluteAirflowConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isAbsoluteAirflowConvergenceToleranceDefaulted();
}

boost::optional<double> AirflowNetworkSimulationControl::convergenceAccelerationLimit() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->convergenceAccelerationLimit();
}

bool AirflowNetworkSimulationControl::isConvergenceAccelerationLimitDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isConvergenceAccelerationLimitDefaulted();
}

boost::optional<double> AirflowNetworkSimulationControl::azimuthAngleofLongAxisofBuilding() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->azimuthAngleofLongAxisofBuilding();
}

bool AirflowNetworkSimulationControl::isAzimuthAngleofLongAxisofBuildingDefaulted() const {
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isAzimuthAngleofLongAxisofBuildingDefaulted();
}

boost::optional<double> AirflowNetworkSimulationControl::buildingAspectRatio() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->buildingAspectRatio();
}

bool AirflowNetworkSimulationControl::isBuildingAspectRatioDefaulted() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isBuildingAspectRatioDefaulted();
}

bool AirflowNetworkSimulationControl::heightDependenceofExternalNodeTemperature() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->heightDependenceofExternalNodeTemperature();
}

bool AirflowNetworkSimulationControl::isHeightDependenceofExternalNodeTemperatureDefaulted() const
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->isHeightDependenceofExternalNodeTemperatureDefaulted();
}

bool AirflowNetworkSimulationControl::setAirflowNetworkControl(std::string airflowNetworkControl)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setAirflowNetworkControl(airflowNetworkControl);
}

void AirflowNetworkSimulationControl::resetAirflowNetworkControl()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetAirflowNetworkControl();
}

bool AirflowNetworkSimulationControl::setWindPressureCoefficientType(std::string type)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setWindPressureCoefficientType(type);
}

void AirflowNetworkSimulationControl::resetWindPressureCoefficientType()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetWindPressureCoefficientType();
}

bool AirflowNetworkSimulationControl::setHeightSelectionforLocalWindPressureCalculation(std::string height)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setHeightSelectionforLocalWindPressureCalculation(height);
}

void AirflowNetworkSimulationControl::resetHeightSelectionforLocalWindPressureCalculation()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetHeightSelectionforLocalWindPressureCalculation();
}

bool AirflowNetworkSimulationControl::setBuildingType(std::string type)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setBuildingType(type);
}

void AirflowNetworkSimulationControl::resetBuildingType()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetBuildingType();
}

bool AirflowNetworkSimulationControl::setMaximumNumberofIterations(int number)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setMaximumNumberofIterations(number);
}

void AirflowNetworkSimulationControl::resetMaximumNumberofIterations()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetMaximumNumberofIterations();
}

bool AirflowNetworkSimulationControl::setInitializationType(std::string type)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setInitializationType(type);
}

void AirflowNetworkSimulationControl::resetInitializationType()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetInitializationType();
}

bool AirflowNetworkSimulationControl::setRelativeAirflowConvergenceTolerance(double tolerance)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setRelativeAirflowConvergenceTolerance(tolerance);
}

void AirflowNetworkSimulationControl::resetRelativeAirflowConvergenceTolerance()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetRelativeAirflowConvergenceTolerance();
}

bool AirflowNetworkSimulationControl::setAbsoluteAirflowConvergenceTolerance(double tolerance)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setAbsoluteAirflowConvergenceTolerance(tolerance);
}

void AirflowNetworkSimulationControl::resetAbsoluteAirflowConvergenceTolerance()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetAbsoluteAirflowConvergenceTolerance();
}

bool AirflowNetworkSimulationControl::setConvergenceAccelerationLimit(double limit)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setConvergenceAccelerationLimit(limit);
}

void AirflowNetworkSimulationControl::resetConvergenceAccelerationLimit()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetConvergenceAccelerationLimit();
}

bool AirflowNetworkSimulationControl::setAzimuthAngleofLongAxisofBuilding(double angle)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setAzimuthAngleofLongAxisofBuilding(angle);
}

void AirflowNetworkSimulationControl::resetAzimuthAngleofLongAxisofBuilding()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetAzimuthAngleofLongAxisofBuilding();
}

bool AirflowNetworkSimulationControl::setBuildingAspectRatio(double ratio)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setBuildingAspectRatio(ratio);
}

void AirflowNetworkSimulationControl::resetBuildingAspectRatio()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetBuildingAspectRatio();
}

bool AirflowNetworkSimulationControl::setHeightDependenceofExternalNodeTemperature(bool tf)
{
  return getImpl<detail::AirflowNetworkSimulationControl_Impl>()->setHeightDependenceofExternalNodeTemperature(tf);
}

void AirflowNetworkSimulationControl::resetHeightDependenceofExternalNodeTemperature()
{
  getImpl<detail::AirflowNetworkSimulationControl_Impl>()->resetHeightDependenceofExternalNodeTemperature();
}

} // model
} // openstudio
