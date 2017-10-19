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

#include "RefrigerationCase.hpp"
#include "RefrigerationCase_Impl.hpp"

#include "RefrigerationSystem_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "RefrigerationDefrostCycleParameters.hpp"
#include "RefrigerationDefrostCycleParameters_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_Case_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/Time.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationCase_Impl::RefrigerationCase_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationCase::iddObjectType());
  }

  RefrigerationCase_Impl::RefrigerationCase_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationCase::iddObjectType());
  }

  RefrigerationCase_Impl::RefrigerationCase_Impl(const RefrigerationCase_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationCase_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationCase_Impl::iddObjectType() const {
    return RefrigerationCase::iddObjectType();
  }

  std::vector<ScheduleTypeKey> RefrigerationCase_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Refrigeration_CaseFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCase","Availability"));
    }
    if (std::find(b,e,OS_Refrigeration_CaseFields::CaseLightingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCase","Case Lighting"));
    }
    if (std::find(b,e,OS_Refrigeration_CaseFields::CaseDefrostScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCase","Case Defrost"));
    }
    if (std::find(b,e,OS_Refrigeration_CaseFields::CaseDefrostDripDownScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCase","Case Defrost Drip-Down"));
    }
    if (std::find(b,e,OS_Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCase","Refrigerated Case Restocking"));
    }
    if (std::find(b,e,OS_Refrigeration_CaseFields::CaseCreditFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCase","Case Credit Fraction"));
    }
    return result;
  }

  ModelObject RefrigerationCase_Impl::clone(Model model) const
  {
    RefrigerationCase modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationCase>();

    if (boost::optional<RefrigerationDefrostCycleParameters> caseDefrostCycleParameters = this->optionalCaseDefrostCycleParameters()) {
      modelObjectClone.getImpl<RefrigerationCase_Impl>()->setCaseDefrostCycleParameters(caseDefrostCycleParameters->clone(model).cast<RefrigerationDefrostCycleParameters>());
    }

    modelObjectClone.resetThermalZone();

    return modelObjectClone;
  }

  std::vector<IdfObject> RefrigerationCase_Impl::remove()
  {
    std::vector<IdfObject> result;

    if (boost::optional<RefrigerationDefrostCycleParameters> caseDefrostCycleParameters = this->optionalCaseDefrostCycleParameters()) {
      std::vector<IdfObject> removedDefrostCycleParameters = caseDefrostCycleParameters->remove();
      result.insert(result.end(), removedDefrostCycleParameters.begin(), removedDefrostCycleParameters.end());
    }

    std::vector<IdfObject> removedRefrigerationCase = ParentObject_Impl::remove();
    result.insert(result.end(), removedRefrigerationCase.begin(), removedRefrigerationCase.end());

    return result;
  }

  std::vector<IddObjectType> RefrigerationCase_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Cubic);
    return result;
  }

  std::vector<ModelObject> RefrigerationCase_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<CurveCubic> intermediate = latentCaseCreditCurve()) {
      result.push_back(*intermediate);
    }

    if( auto curve = defrostEnergyCorrectionCurve() ) {
      result.push_back(curve.get());
    }

    return result;
  }

  boost::optional<Schedule> RefrigerationCase_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_CaseFields::AvailabilityScheduleName);
  }

  boost::optional<ThermalZone> RefrigerationCase_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_CaseFields::ZoneName);
  }

  double RefrigerationCase_Impl::ratedAmbientTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::RatedAmbientTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isRatedAmbientTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::RatedAmbientTemperature);
  }

  double RefrigerationCase_Impl::ratedAmbientRelativeHumidity() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::RatedAmbientRelativeHumidity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isRatedAmbientRelativeHumidityDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::RatedAmbientRelativeHumidity);
  }

  double RefrigerationCase_Impl::ratedTotalCoolingCapacityperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isRatedTotalCoolingCapacityperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength);
  }

  double RefrigerationCase_Impl::ratedLatentHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::RatedLatentHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isRatedLatentHeatRatioDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::RatedLatentHeatRatio);
  }

  double RefrigerationCase_Impl::ratedRuntimeFraction() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::RatedRuntimeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isRatedRuntimeFractionDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::RatedRuntimeFraction);
  }

  double RefrigerationCase_Impl::caseLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::CaseLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isCaseLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::CaseLength);
  }

  double RefrigerationCase_Impl::caseOperatingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::CaseOperatingTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isCaseOperatingTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::CaseOperatingTemperature);
  }

  std::string RefrigerationCase_Impl::latentCaseCreditCurveType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CaseFields::LatentCaseCreditCurveType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isLatentCaseCreditCurveTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::LatentCaseCreditCurveType);
  }

  CurveCubic RefrigerationCase_Impl::latentCaseCreditCurve() const {
    boost::optional<CurveCubic> value = optionalLatentCaseCreditCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Latent Case Credit Curve attached.");
    }
    return value.get();
  }

  double RefrigerationCase_Impl::standardCaseFanPowerperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isStandardCaseFanPowerperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength);
  }

  double RefrigerationCase_Impl::operatingCaseFanPowerperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isOperatingCaseFanPowerperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength);
  }

  double RefrigerationCase_Impl::standardCaseLightingPowerperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isStandardCaseLightingPowerperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength);
  }

  boost::optional<double> RefrigerationCase_Impl::installedCaseLightingPowerperUnitLength() const {
    return getDouble(OS_Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength,true);
  }

  boost::optional<Schedule> RefrigerationCase_Impl::caseLightingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_CaseFields::CaseLightingScheduleName);
  }

  double RefrigerationCase_Impl::fractionofLightingEnergytoCase() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::FractionofLightingEnergytoCase,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isFractionofLightingEnergytoCaseDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::FractionofLightingEnergytoCase);
  }

  double RefrigerationCase_Impl::caseAntiSweatHeaterPowerperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isCaseAntiSweatHeaterPowerperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength);
  }

  double RefrigerationCase_Impl::minimumAntiSweatHeaterPowerperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isMinimumAntiSweatHeaterPowerperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength);
  }

  std::string RefrigerationCase_Impl::antiSweatHeaterControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CaseFields::AntiSweatHeaterControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isAntiSweatHeaterControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::AntiSweatHeaterControlType);
  }

  double RefrigerationCase_Impl::humidityatZeroAntiSweatHeaterEnergy() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isHumidityatZeroAntiSweatHeaterEnergyDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy);
  }

  double RefrigerationCase_Impl::caseHeight() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::CaseHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isCaseHeightDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::CaseHeight);
  }

  double RefrigerationCase_Impl::fractionofAntiSweatHeaterEnergytoCase() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isFractionofAntiSweatHeaterEnergytoCaseDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase);
  }

  double RefrigerationCase_Impl::caseDefrostPowerperUnitLength() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::CaseDefrostPowerperUnitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isCaseDefrostPowerperUnitLengthDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::CaseDefrostPowerperUnitLength);
  }

  std::string RefrigerationCase_Impl::caseDefrostType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CaseFields::CaseDefrostType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isCaseDefrostTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::CaseDefrostType);
  }

  boost::optional<Schedule> RefrigerationCase_Impl::caseDefrostSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_CaseFields::CaseDefrostScheduleName);
  }

  boost::optional<Schedule> RefrigerationCase_Impl::caseDefrostDripDownSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_CaseFields::CaseDefrostDripDownScheduleName);
  }

  std::string RefrigerationCase_Impl::defrostEnergyCorrectionCurveType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isDefrostEnergyCorrectionCurveTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType);
  }

  boost::optional<CurveCubic> RefrigerationCase_Impl::defrostEnergyCorrectionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName);
  }

  double RefrigerationCase_Impl::underCaseHVACReturnAirFraction() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isUnderCaseHVACReturnAirFractionDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction);
  }

  boost::optional<Schedule> RefrigerationCase_Impl::refrigeratedCaseRestockingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName);
  }

  boost::optional<Schedule> RefrigerationCase_Impl::caseCreditFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_CaseFields::CaseCreditFractionScheduleName);
  }

  boost::optional<double> RefrigerationCase_Impl::designEvaporatorTemperatureorBrineInletTemperature() const {
    return getDouble(OS_Refrigeration_CaseFields::DesignEvaporatorTemperatureorBrineInletTemperature,true);
  }

  double RefrigerationCase_Impl::averageRefrigerantChargeInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_CaseFields::AverageRefrigerantChargeInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isAverageRefrigerantChargeInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::AverageRefrigerantChargeInventory);
  }

  boost::optional<int> RefrigerationCase_Impl::numberOfDoors() const {
    return getInt(OS_Refrigeration_CaseFields::NumberOfDoors,true);
  }

  boost::optional<double> RefrigerationCase_Impl::ratedTotalCoolingCapacityperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::standardCaseFanPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::StandardCaseFanPowerperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::operatingCaseFanPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::standardCaseLightingPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::installedCaseLightingPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::InstalledCaseLightingPowerperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::caseAntiSweatHeaterPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::minimumAntiSweatHeaterPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperDoor,true);
  }

  boost::optional<double> RefrigerationCase_Impl::caseDefrostPowerperDoor() const {
    return getDouble(OS_Refrigeration_CaseFields::CaseDefrostPowerperDoor,true);
  }

  std::string RefrigerationCase_Impl::unitType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CaseFields::UnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCase_Impl::isUnitTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_CaseFields::UnitType);
  }

  boost::optional<RefrigerationDefrostCycleParameters> RefrigerationCase_Impl::optionalCaseDefrostCycleParameters() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationDefrostCycleParameters>(OS_Refrigeration_CaseFields::CaseDefrostCycleParametersName);
  }

  RefrigerationDefrostCycleParameters RefrigerationCase_Impl::caseDefrostCycleParameters() {
    boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters();
    if( !defrostCycleParameters ) {
      defrostCycleParameters = RefrigerationDefrostCycleParameters(this->model());
      OS_ASSERT(defrostCycleParameters);
      bool result = setCaseDefrostCycleParameters(*defrostCycleParameters);
      OS_ASSERT(result);
    }
    return *defrostCycleParameters;
  }

  boost::optional<int> RefrigerationCase_Impl::durationofDefrostCycle() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->durationofDefrostCycle();
    }
    return boost::none;
  }

  boost::optional<int> RefrigerationCase_Impl::dripDownTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->dripDownTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost1StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost1StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost2StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost2StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost3StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost3StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost4StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost4StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost5StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost5StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost6StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost6StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost7StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost7StartTime();
    }
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationCase_Impl::defrost8StartTime() const {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      return defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->defrost8StartTime();
    }
    return boost::none;
  }

  std::vector<openstudio::Time> RefrigerationCase_Impl::defrostStartTimes() const {
    std::vector<openstudio::Time> result;
    if( boost::optional<openstudio::Time> defrost1StartTime = this->defrost1StartTime() ) {
      result.push_back(*defrost1StartTime);
    }
    if( boost::optional<openstudio::Time> defrost2StartTime = this->defrost2StartTime() ) {
      result.push_back(*defrost2StartTime);
    }
    if( boost::optional<openstudio::Time> defrost3StartTime = this->defrost3StartTime() ) {
      result.push_back(*defrost3StartTime);
    }
    if( boost::optional<openstudio::Time> defrost4StartTime = this->defrost4StartTime() ) {
      result.push_back(*defrost4StartTime);
    }
    if( boost::optional<openstudio::Time> defrost5StartTime = this->defrost5StartTime() ) {
      result.push_back(*defrost5StartTime);
    }
    if( boost::optional<openstudio::Time> defrost6StartTime = this->defrost6StartTime() ) {
      result.push_back(*defrost6StartTime);
    }
    if( boost::optional<openstudio::Time> defrost7StartTime = this->defrost7StartTime() ) {
      result.push_back(*defrost7StartTime);
    }
    if( boost::optional<openstudio::Time> defrost8StartTime = this->defrost8StartTime() ) {
      result.push_back(*defrost8StartTime);
    }
    return result;
  }

  boost::optional<RefrigerationSystem> RefrigerationCase_Impl::system() const {
    std::vector<RefrigerationSystem> refrigerationSystems = this->model().getConcreteModelObjects<RefrigerationSystem>();
    RefrigerationCase refrigerationCase = this->getObject<RefrigerationCase>();
    for (RefrigerationSystem refrigerationSystem : refrigerationSystems) {
      RefrigerationCaseVector refrigerationCases = refrigerationSystem.cases();
      if ( !refrigerationCases.empty() && std::find(refrigerationCases.begin(), refrigerationCases.end(), refrigerationCase) != refrigerationCases.end() ) {
        return refrigerationSystem;
      }
    }
    return boost::none;
  }

  bool RefrigerationCase_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_CaseFields::AvailabilityScheduleName,
                              "RefrigerationCase",
                              "Availability",
                              schedule);
    return result;
  }

  void RefrigerationCase_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Refrigeration_CaseFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setThermalZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_Refrigeration_CaseFields::ZoneName, thermalZone.handle());
    return result;
  }

  void RefrigerationCase_Impl::resetThermalZone() {
    bool result = setString(OS_Refrigeration_CaseFields::ZoneName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRatedAmbientTemperature(double ratedAmbientTemperature) {
    bool result = setDouble(OS_Refrigeration_CaseFields::RatedAmbientTemperature, ratedAmbientTemperature);
    return result;
  }

  void RefrigerationCase_Impl::resetRatedAmbientTemperature() {
    bool result = setString(OS_Refrigeration_CaseFields::RatedAmbientTemperature, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRatedAmbientRelativeHumidity(double ratedAmbientRelativeHumidity) {
    bool result = setDouble(OS_Refrigeration_CaseFields::RatedAmbientRelativeHumidity, ratedAmbientRelativeHumidity);
    return result;
  }

  void RefrigerationCase_Impl::resetRatedAmbientRelativeHumidity() {
    bool result = setString(OS_Refrigeration_CaseFields::RatedAmbientRelativeHumidity, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRatedTotalCoolingCapacityperUnitLength(double ratedTotalCoolingCapacityperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength, ratedTotalCoolingCapacityperUnitLength);
    return result;
  }

  void RefrigerationCase_Impl::resetRatedTotalCoolingCapacityperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRatedLatentHeatRatio(double ratedLatentHeatRatio) {
    bool result = setDouble(OS_Refrigeration_CaseFields::RatedLatentHeatRatio, ratedLatentHeatRatio);
    return result;
  }

  void RefrigerationCase_Impl::resetRatedLatentHeatRatio() {
    bool result = setString(OS_Refrigeration_CaseFields::RatedLatentHeatRatio, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRatedRuntimeFraction(double ratedRuntimeFraction) {
    bool result = setDouble(OS_Refrigeration_CaseFields::RatedRuntimeFraction, ratedRuntimeFraction);
    return result;
  }

  void RefrigerationCase_Impl::resetRatedRuntimeFraction() {
    bool result = setString(OS_Refrigeration_CaseFields::RatedRuntimeFraction, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseLength(double caseLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::CaseLength, caseLength);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseLength() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseOperatingTemperature(double caseOperatingTemperature) {
    bool result = setDouble(OS_Refrigeration_CaseFields::CaseOperatingTemperature, caseOperatingTemperature);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseOperatingTemperature() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseOperatingTemperature, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setLatentCaseCreditCurveType(std::string latentCaseCreditCurveType) {
    bool result = setString(OS_Refrigeration_CaseFields::LatentCaseCreditCurveType, latentCaseCreditCurveType);
    return result;
  }

  void RefrigerationCase_Impl::resetLatentCaseCreditCurveType() {
    bool result = setString(OS_Refrigeration_CaseFields::LatentCaseCreditCurveType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setLatentCaseCreditCurve(const CurveCubic& curveCubic) {
    bool result = setPointer(OS_Refrigeration_CaseFields::LatentCaseCreditCurveName, curveCubic.handle());
    return result;
  }

  bool RefrigerationCase_Impl::setStandardCaseFanPowerperUnitLength(double standardCaseFanPowerperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength, standardCaseFanPowerperUnitLength);
    return result;
  }

  void RefrigerationCase_Impl::resetStandardCaseFanPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setOperatingCaseFanPowerperUnitLength(double operatingCaseFanPowerperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength, operatingCaseFanPowerperUnitLength);
    return result;
  }

  void RefrigerationCase_Impl::resetOperatingCaseFanPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::setStandardCaseLightingPowerperUnitLength(double standardCaseLightingPowerperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength, standardCaseLightingPowerperUnitLength);
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::resetStandardCaseLightingPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::setInstalledCaseLightingPowerperUnitLength(boost::optional<double> installedCaseLightingPowerperUnitLength) {
    bool result(false);
    if (installedCaseLightingPowerperUnitLength) {
      result = setDouble(OS_Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength, installedCaseLightingPowerperUnitLength.get());
    }
    else {
      resetInstalledCaseLightingPowerperUnitLength();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::resetInstalledCaseLightingPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseLightingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_CaseFields::CaseLightingScheduleName,
                              "RefrigerationCase",
                              "Case Lighting",
                              schedule);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseLightingSchedule() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseLightingScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setFractionofLightingEnergytoCase(double fractionofLightingEnergytoCase) {
    bool result = setDouble(OS_Refrigeration_CaseFields::FractionofLightingEnergytoCase, fractionofLightingEnergytoCase);
    return result;
  }

  void RefrigerationCase_Impl::resetFractionofLightingEnergytoCase() {
    bool result = setString(OS_Refrigeration_CaseFields::FractionofLightingEnergytoCase, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseAntiSweatHeaterPowerperUnitLength(double caseAntiSweatHeaterPowerperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength, caseAntiSweatHeaterPowerperUnitLength);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseAntiSweatHeaterPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setMinimumAntiSweatHeaterPowerperUnitLength(double minimumAntiSweatHeaterPowerperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength, minimumAntiSweatHeaterPowerperUnitLength);
    return result;
  }

  void RefrigerationCase_Impl::resetMinimumAntiSweatHeaterPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setAntiSweatHeaterControlType(std::string antiSweatHeaterControlType) {
    bool result = setString(OS_Refrigeration_CaseFields::AntiSweatHeaterControlType, antiSweatHeaterControlType);
    return result;
  }

  void RefrigerationCase_Impl::resetAntiSweatHeaterControlType() {
    bool result = setString(OS_Refrigeration_CaseFields::AntiSweatHeaterControlType, "");
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::setHumidityatZeroAntiSweatHeaterEnergy(double humidityatZeroAntiSweatHeaterEnergy) {
    bool result = setDouble(OS_Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy, humidityatZeroAntiSweatHeaterEnergy);
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::resetHumidityatZeroAntiSweatHeaterEnergy() {
    bool result = setString(OS_Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseHeight(double caseHeight) {
    bool result = setDouble(OS_Refrigeration_CaseFields::CaseHeight, caseHeight);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseHeight() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseHeight, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setFractionofAntiSweatHeaterEnergytoCase(double fractionofAntiSweatHeaterEnergytoCase) {
    bool result = setDouble(OS_Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase, fractionofAntiSweatHeaterEnergytoCase);
    return result;
  }

  void RefrigerationCase_Impl::resetFractionofAntiSweatHeaterEnergytoCase() {
    bool result = setString(OS_Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseDefrostPowerperUnitLength(double caseDefrostPowerperUnitLength) {
    bool result = setDouble(OS_Refrigeration_CaseFields::CaseDefrostPowerperUnitLength, caseDefrostPowerperUnitLength);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseDefrostPowerperUnitLength() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseDefrostPowerperUnitLength, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseDefrostType(std::string caseDefrostType) {
    bool result = setString(OS_Refrigeration_CaseFields::CaseDefrostType, caseDefrostType);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseDefrostType() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseDefrostType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseDefrostSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_CaseFields::CaseDefrostScheduleName,
                              "RefrigerationCase",
                              "Case Defrost",
                              schedule);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseDefrostSchedule() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseDefrostScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseDefrostDripDownSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_CaseFields::CaseDefrostDripDownScheduleName,
                              "RefrigerationCase",
                              "Case Defrost Drip-Down",
                              schedule);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseDefrostDripDownSchedule() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseDefrostDripDownScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setDefrostEnergyCorrectionCurveType(std::string defrostEnergyCorrectionCurveType) {
    bool result = setString(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType, defrostEnergyCorrectionCurveType);
    return result;
  }

  void RefrigerationCase_Impl::resetDefrostEnergyCorrectionCurveType() {
    bool result = setString(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setDefrostEnergyCorrectionCurve(const boost::optional<CurveCubic>& curveCubic) {
    bool result(false);
    if (curveCubic) {
      result = setPointer(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName, curveCubic.get().handle());
    }
    else {
      resetDefrostEnergyCorrectionCurve();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetDefrostEnergyCorrectionCurve() {
    bool result = setString(OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setUnderCaseHVACReturnAirFraction(double underCaseHVACReturnAirFraction) {
    bool result = setDouble(OS_Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction, underCaseHVACReturnAirFraction);
    return result;
  }

  void RefrigerationCase_Impl::resetUnderCaseHVACReturnAirFraction() {
    bool result = setString(OS_Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRefrigeratedCaseRestockingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName,
                              "RefrigerationCase",
                              "Refrigerated Case Restocking",
                              schedule);
    return result;
  }

  void RefrigerationCase_Impl::resetRefrigeratedCaseRestockingSchedule() {
    bool result = setString(OS_Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseCreditFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_CaseFields::CaseCreditFractionScheduleName,
                              "RefrigerationCase",
                              "Case Credit Fraction",
                              schedule);
    return result;
  }

  void RefrigerationCase_Impl::resetCaseCreditFractionSchedule() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseCreditFractionScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setDesignEvaporatorTemperatureorBrineInletTemperature(boost::optional<double> designEvaporatorTemperatureorBrineInletTemperature) {
    bool result(false);
    if (designEvaporatorTemperatureorBrineInletTemperature) {
      result = setDouble(OS_Refrigeration_CaseFields::DesignEvaporatorTemperatureorBrineInletTemperature, designEvaporatorTemperatureorBrineInletTemperature.get());
    }
    else {
      resetDesignEvaporatorTemperatureorBrineInletTemperature();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetDesignEvaporatorTemperatureorBrineInletTemperature() {
    bool result = setString(OS_Refrigeration_CaseFields::DesignEvaporatorTemperatureorBrineInletTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory) {
    bool result = setDouble(OS_Refrigeration_CaseFields::AverageRefrigerantChargeInventory, averageRefrigerantChargeInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCase_Impl::resetAverageRefrigerantChargeInventory() {
    bool result = setString(OS_Refrigeration_CaseFields::AverageRefrigerantChargeInventory, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setNumberOfDoors(boost::optional<int> numberOfDoors) {
    bool result(false);
    if (numberOfDoors) {
      result = setInt(OS_Refrigeration_CaseFields::NumberOfDoors, numberOfDoors.get());
    }
    else {
      resetNumberOfDoors();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetNumberOfDoors() {
    bool result = setString(OS_Refrigeration_CaseFields::NumberOfDoors, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setRatedTotalCoolingCapacityperDoor(boost::optional<double> ratedTotalCoolingCapacityperDoor) {
    bool result(false);
    if (ratedTotalCoolingCapacityperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperDoor, ratedTotalCoolingCapacityperDoor.get());
    }
    else {
      resetRatedTotalCoolingCapacityperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetRatedTotalCoolingCapacityperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::RatedTotalCoolingCapacityperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setStandardCaseFanPowerperDoor(boost::optional<double> standardCaseFanPowerperDoor) {
    bool result(false);
    if (standardCaseFanPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::StandardCaseFanPowerperDoor, standardCaseFanPowerperDoor.get());
    }
    else {
      resetStandardCaseFanPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetStandardCaseFanPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::StandardCaseFanPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setOperatingCaseFanPowerperDoor(boost::optional<double> operatingCaseFanPowerperDoor) {
    bool result(false);
    if (operatingCaseFanPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperDoor, operatingCaseFanPowerperDoor.get());
    }
    else {
      resetOperatingCaseFanPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetOperatingCaseFanPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::OperatingCaseFanPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setStandardCaseLightingPowerperDoor(boost::optional<double> standardCaseLightingPowerperDoor) {
    bool result(false);
    if (standardCaseLightingPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperDoor, standardCaseLightingPowerperDoor.get());
    }
    else {
      resetStandardCaseLightingPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetStandardCaseLightingPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::StandardCaseLightingPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setInstalledCaseLightingPowerperDoor(boost::optional<double> installedCaseLightingPowerperDoor) {
    bool result(false);
    if (installedCaseLightingPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::InstalledCaseLightingPowerperDoor, installedCaseLightingPowerperDoor.get());
    }
    else {
      resetInstalledCaseLightingPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetInstalledCaseLightingPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::InstalledCaseLightingPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseAntiSweatHeaterPowerperDoor(boost::optional<double> caseAntiSweatHeaterPowerperDoor) {
    bool result(false);
    if (caseAntiSweatHeaterPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperDoor, caseAntiSweatHeaterPowerperDoor.get());
    }
    else {
      resetCaseAntiSweatHeaterPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetCaseAntiSweatHeaterPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setMinimumAntiSweatHeaterPowerperDoor(boost::optional<double> minimumAntiSweatHeaterPowerperDoor) {
    bool result(false);
    if (minimumAntiSweatHeaterPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperDoor, minimumAntiSweatHeaterPowerperDoor.get());
    }
    else {
      resetMinimumAntiSweatHeaterPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetMinimumAntiSweatHeaterPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseDefrostPowerperDoor(boost::optional<double> caseDefrostPowerperDoor) {
    bool result(false);
    if (caseDefrostPowerperDoor) {
      result = setDouble(OS_Refrigeration_CaseFields::CaseDefrostPowerperDoor, caseDefrostPowerperDoor.get());
    }
    else {
      resetCaseDefrostPowerperDoor();
      result = true;
    }
    return result;
  }

  void RefrigerationCase_Impl::resetCaseDefrostPowerperDoor() {
    bool result = setString(OS_Refrigeration_CaseFields::CaseDefrostPowerperDoor, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setUnitType(std::string unitType) {
    bool result = setString(OS_Refrigeration_CaseFields::UnitType, unitType);
    return result;
  }

  void RefrigerationCase_Impl::resetUnitType() {
    bool result = setString(OS_Refrigeration_CaseFields::UnitType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCase_Impl::setCaseDefrostCycleParameters(const RefrigerationDefrostCycleParameters& caseDefrostCycleParameters) {
    return setPointer(OS_Refrigeration_CaseFields::CaseDefrostCycleParametersName, caseDefrostCycleParameters.handle());
  }

  bool RefrigerationCase_Impl::setDurationofDefrostCycle(boost::optional<int> durationofDefrostCycle) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDurationofDefrostCycle(durationofDefrostCycle);
  }

  void RefrigerationCase_Impl::resetDurationofDefrostCycle() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDurationofDefrostCycle();
    }
  }

  bool RefrigerationCase_Impl::setDripDownTime(boost::optional<int> dripDownTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDripDownTime(dripDownTime);
  }

  void RefrigerationCase_Impl::resetDripDownTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDripDownTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost1StartTime(const openstudio::Time& defrost1StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost1StartTime(defrost1StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost1StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost1StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost2StartTime(const openstudio::Time& defrost2StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost2StartTime(defrost2StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost2StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost2StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost3StartTime(const openstudio::Time& defrost3StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost3StartTime(defrost3StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost3StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost3StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost4StartTime(const openstudio::Time& defrost4StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost4StartTime(defrost4StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost4StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost4StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost5StartTime(const openstudio::Time& defrost5StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost5StartTime(defrost5StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost5StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost5StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost6StartTime(const openstudio::Time& defrost6StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost6StartTime(defrost6StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost6StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost6StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost7StartTime(const openstudio::Time& defrost7StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost7StartTime(defrost7StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost7StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost7StartTime();
    }
  }

  bool RefrigerationCase_Impl::setDefrost8StartTime(const openstudio::Time& defrost8StartTime) {
      return this->caseDefrostCycleParameters().getImpl<RefrigerationDefrostCycleParameters_Impl>()->setDefrost8StartTime(defrost8StartTime);
  }

  void RefrigerationCase_Impl::resetDefrost8StartTime() {
    if( boost::optional<RefrigerationDefrostCycleParameters> defrostCycleParameters = this->optionalCaseDefrostCycleParameters() ) {
      defrostCycleParameters->getImpl<RefrigerationDefrostCycleParameters_Impl>()->resetDefrost8StartTime();
    }
  }

  boost::optional<CurveCubic> RefrigerationCase_Impl::optionalLatentCaseCreditCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_Refrigeration_CaseFields::LatentCaseCreditCurveName);
  }

  bool RefrigerationCase_Impl::addToSystem(RefrigerationSystem & system) {
    return system.addCase(this->getObject<RefrigerationCase>());
  }

  void RefrigerationCase_Impl::removeFromSystem() {
    boost::optional<RefrigerationSystem> refrigerationSystem = system();
    if(refrigerationSystem){
      refrigerationSystem.get().removeCase(this->getObject<RefrigerationCase>());
    }
  }

} // detail

RefrigerationCase::RefrigerationCase(const Model& model, Schedule& caseDefrostSchedule)
  : ParentObject(RefrigerationCase::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationCase_Impl>());

  CurveCubic latentCaseCreditCurve = CurveCubic(model);
  latentCaseCreditCurve.setName("Refrigeration Case Latent Credit Curve");
  latentCaseCreditCurve.setCoefficient1Constant(0.026526281);
  latentCaseCreditCurve.setCoefficient2x(0.001078032);
  latentCaseCreditCurve.setCoefficient3xPOW2(-0.0000602558);
  latentCaseCreditCurve.setCoefficient4xPOW3(0.00000123732);
  latentCaseCreditCurve.setMinimumValueofx(-35);
  latentCaseCreditCurve.setMaximumValueofx(20);

  setRatedAmbientTemperature(24.00);
  setRatedAmbientRelativeHumidity(55.00);
  setRatedTotalCoolingCapacityperUnitLength(1432.66);
  setRatedLatentHeatRatio(0.30);
  setRatedRuntimeFraction(0.85);
  setCaseLength(3.66);
  setCaseOperatingTemperature(2.78);
  setLatentCaseCreditCurveType("CaseTemperatureMethod");

  bool ok = setLatentCaseCreditCurve(latentCaseCreditCurve);
  OS_ASSERT(ok);

  setStandardCaseFanPowerperUnitLength(41.01);
  setOperatingCaseFanPowerperUnitLength(41.01);
  setStandardCaseLightingPowerperUnitLength(45.93);
  setInstalledCaseLightingPowerperUnitLength(45.93);
  setFractionofLightingEnergytoCase(1);
  setAntiSweatHeaterControlType("None");
  setHumidityatZeroAntiSweatHeaterEnergy(-10);
  setCaseHeight(1.5);
  setFractionofAntiSweatHeaterEnergytoCase(1.0);
  setCaseDefrostPowerperUnitLength(0.0);
  setCaseDefrostType("OffCycle");
  setCaseDefrostSchedule(caseDefrostSchedule);
  setDefrostEnergyCorrectionCurveType("None");
  setUnderCaseHVACReturnAirFraction(0.0);
  setDesignEvaporatorTemperatureorBrineInletTemperature(-5.56);
}

IddObjectType RefrigerationCase::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Case);
}

std::vector<std::string> RefrigerationCase::latentCaseCreditCurveTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_CaseFields::LatentCaseCreditCurveType);
}

std::vector<std::string> RefrigerationCase::antiSweatHeaterControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_CaseFields::AntiSweatHeaterControlType);
}

std::vector<std::string> RefrigerationCase::caseDefrostTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_CaseFields::CaseDefrostType);
}

std::vector<std::string> RefrigerationCase::defrostEnergyCorrectionCurveTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType);
}

std::vector<std::string> RefrigerationCase::unitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_CaseFields::UnitType);
}

boost::optional<Schedule> RefrigerationCase::availabilitySchedule() const {
  return getImpl<detail::RefrigerationCase_Impl>()->availabilitySchedule();
}

boost::optional<ThermalZone> RefrigerationCase::thermalZone() const {
  return getImpl<detail::RefrigerationCase_Impl>()->thermalZone();
}

double RefrigerationCase::ratedAmbientTemperature() const {
  return getImpl<detail::RefrigerationCase_Impl>()->ratedAmbientTemperature();
}

bool RefrigerationCase::isRatedAmbientTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isRatedAmbientTemperatureDefaulted();
}

double RefrigerationCase::ratedAmbientRelativeHumidity() const {
  return getImpl<detail::RefrigerationCase_Impl>()->ratedAmbientRelativeHumidity();
}

bool RefrigerationCase::isRatedAmbientRelativeHumidityDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isRatedAmbientRelativeHumidityDefaulted();
}

double RefrigerationCase::ratedTotalCoolingCapacityperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->ratedTotalCoolingCapacityperUnitLength();
}

bool RefrigerationCase::isRatedTotalCoolingCapacityperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isRatedTotalCoolingCapacityperUnitLengthDefaulted();
}

double RefrigerationCase::ratedLatentHeatRatio() const {
  return getImpl<detail::RefrigerationCase_Impl>()->ratedLatentHeatRatio();
}

bool RefrigerationCase::isRatedLatentHeatRatioDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isRatedLatentHeatRatioDefaulted();
}

double RefrigerationCase::ratedRuntimeFraction() const {
  return getImpl<detail::RefrigerationCase_Impl>()->ratedRuntimeFraction();
}

bool RefrigerationCase::isRatedRuntimeFractionDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isRatedRuntimeFractionDefaulted();
}

double RefrigerationCase::caseLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseLength();
}

bool RefrigerationCase::isCaseLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isCaseLengthDefaulted();
}

double RefrigerationCase::caseOperatingTemperature() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseOperatingTemperature();
}

bool RefrigerationCase::isCaseOperatingTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isCaseOperatingTemperatureDefaulted();
}

std::string RefrigerationCase::latentCaseCreditCurveType() const {
  return getImpl<detail::RefrigerationCase_Impl>()->latentCaseCreditCurveType();
}

bool RefrigerationCase::isLatentCaseCreditCurveTypeDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isLatentCaseCreditCurveTypeDefaulted();
}

CurveCubic RefrigerationCase::latentCaseCreditCurve() const {
  return getImpl<detail::RefrigerationCase_Impl>()->latentCaseCreditCurve();
}

double RefrigerationCase::standardCaseFanPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->standardCaseFanPowerperUnitLength();
}

bool RefrigerationCase::isStandardCaseFanPowerperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isStandardCaseFanPowerperUnitLengthDefaulted();
}

double RefrigerationCase::operatingCaseFanPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->operatingCaseFanPowerperUnitLength();
}

bool RefrigerationCase::isOperatingCaseFanPowerperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isOperatingCaseFanPowerperUnitLengthDefaulted();
}

double RefrigerationCase::standardCaseLightingPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->standardCaseLightingPowerperUnitLength();
}

bool RefrigerationCase::isStandardCaseLightingPowerperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isStandardCaseLightingPowerperUnitLengthDefaulted();
}

boost::optional<double> RefrigerationCase::installedCaseLightingPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->installedCaseLightingPowerperUnitLength();
}

boost::optional<Schedule> RefrigerationCase::caseLightingSchedule() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseLightingSchedule();
}

double RefrigerationCase::fractionofLightingEnergytoCase() const {
  return getImpl<detail::RefrigerationCase_Impl>()->fractionofLightingEnergytoCase();
}

bool RefrigerationCase::isFractionofLightingEnergytoCaseDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isFractionofLightingEnergytoCaseDefaulted();
}

double RefrigerationCase::caseAntiSweatHeaterPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseAntiSweatHeaterPowerperUnitLength();
}

bool RefrigerationCase::isCaseAntiSweatHeaterPowerperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isCaseAntiSweatHeaterPowerperUnitLengthDefaulted();
}

double RefrigerationCase::minimumAntiSweatHeaterPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->minimumAntiSweatHeaterPowerperUnitLength();
}

bool RefrigerationCase::isMinimumAntiSweatHeaterPowerperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isMinimumAntiSweatHeaterPowerperUnitLengthDefaulted();
}

std::string RefrigerationCase::antiSweatHeaterControlType() const {
  return getImpl<detail::RefrigerationCase_Impl>()->antiSweatHeaterControlType();
}

bool RefrigerationCase::isAntiSweatHeaterControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isAntiSweatHeaterControlTypeDefaulted();
}

double RefrigerationCase::humidityatZeroAntiSweatHeaterEnergy() const {
  return getImpl<detail::RefrigerationCase_Impl>()->humidityatZeroAntiSweatHeaterEnergy();
}

bool RefrigerationCase::isHumidityatZeroAntiSweatHeaterEnergyDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isHumidityatZeroAntiSweatHeaterEnergyDefaulted();
}

double RefrigerationCase::caseHeight() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseHeight();
}

bool RefrigerationCase::isCaseHeightDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isCaseHeightDefaulted();
}

double RefrigerationCase::fractionofAntiSweatHeaterEnergytoCase() const {
  return getImpl<detail::RefrigerationCase_Impl>()->fractionofAntiSweatHeaterEnergytoCase();
}

bool RefrigerationCase::isFractionofAntiSweatHeaterEnergytoCaseDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isFractionofAntiSweatHeaterEnergytoCaseDefaulted();
}

double RefrigerationCase::caseDefrostPowerperUnitLength() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseDefrostPowerperUnitLength();
}

bool RefrigerationCase::isCaseDefrostPowerperUnitLengthDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isCaseDefrostPowerperUnitLengthDefaulted();
}

std::string RefrigerationCase::caseDefrostType() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseDefrostType();
}

bool RefrigerationCase::isCaseDefrostTypeDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isCaseDefrostTypeDefaulted();
}

boost::optional<Schedule> RefrigerationCase::caseDefrostSchedule() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseDefrostSchedule();
}

boost::optional<Schedule> RefrigerationCase::caseDefrostDripDownSchedule() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseDefrostDripDownSchedule();
}

std::string RefrigerationCase::defrostEnergyCorrectionCurveType() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrostEnergyCorrectionCurveType();
}

bool RefrigerationCase::isDefrostEnergyCorrectionCurveTypeDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isDefrostEnergyCorrectionCurveTypeDefaulted();
}

boost::optional<CurveCubic> RefrigerationCase::defrostEnergyCorrectionCurve() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrostEnergyCorrectionCurve();
}

double RefrigerationCase::underCaseHVACReturnAirFraction() const {
  return getImpl<detail::RefrigerationCase_Impl>()->underCaseHVACReturnAirFraction();
}

bool RefrigerationCase::isUnderCaseHVACReturnAirFractionDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isUnderCaseHVACReturnAirFractionDefaulted();
}

boost::optional<Schedule> RefrigerationCase::refrigeratedCaseRestockingSchedule() const {
  return getImpl<detail::RefrigerationCase_Impl>()->refrigeratedCaseRestockingSchedule();
}

boost::optional<Schedule> RefrigerationCase::caseCreditFractionSchedule() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseCreditFractionSchedule();
}

boost::optional<double> RefrigerationCase::designEvaporatorTemperatureorBrineInletTemperature() const {
  return getImpl<detail::RefrigerationCase_Impl>()->designEvaporatorTemperatureorBrineInletTemperature();
}

double RefrigerationCase::averageRefrigerantChargeInventory() const {
  return getImpl<detail::RefrigerationCase_Impl>()->averageRefrigerantChargeInventory();
}

bool RefrigerationCase::isAverageRefrigerantChargeInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isAverageRefrigerantChargeInventoryDefaulted();
}

boost::optional<int> RefrigerationCase::numberOfDoors() const {
  return getImpl<detail::RefrigerationCase_Impl>()->numberOfDoors();
}

boost::optional<double> RefrigerationCase::ratedTotalCoolingCapacityperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->ratedTotalCoolingCapacityperDoor();
}

boost::optional<double> RefrigerationCase::standardCaseFanPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->standardCaseFanPowerperDoor();
}

boost::optional<double> RefrigerationCase::operatingCaseFanPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->operatingCaseFanPowerperDoor();
}

boost::optional<double> RefrigerationCase::standardCaseLightingPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->standardCaseLightingPowerperDoor();
}

boost::optional<double> RefrigerationCase::installedCaseLightingPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->installedCaseLightingPowerperDoor();
}

boost::optional<double> RefrigerationCase::caseAntiSweatHeaterPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseAntiSweatHeaterPowerperDoor();
}

boost::optional<double> RefrigerationCase::minimumAntiSweatHeaterPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->minimumAntiSweatHeaterPowerperDoor();
}

boost::optional<double> RefrigerationCase::caseDefrostPowerperDoor() const {
  return getImpl<detail::RefrigerationCase_Impl>()->caseDefrostPowerperDoor();
}

std::string RefrigerationCase::unitType() const {
  return getImpl<detail::RefrigerationCase_Impl>()->unitType();
}

bool RefrigerationCase::isUnitTypeDefaulted() const {
  return getImpl<detail::RefrigerationCase_Impl>()->isUnitTypeDefaulted();
}

boost::optional<int> RefrigerationCase::durationofDefrostCycle() const {
  return getImpl<detail::RefrigerationCase_Impl>()->durationofDefrostCycle();
}

boost::optional<int> RefrigerationCase::dripDownTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->dripDownTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost1StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost1StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost2StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost2StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost3StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost3StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost4StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost4StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost5StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost5StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost6StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost6StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost7StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost7StartTime();
}

boost::optional<openstudio::Time> RefrigerationCase::defrost8StartTime() const {
  return getImpl<detail::RefrigerationCase_Impl>()->defrost8StartTime();
}

boost::optional<RefrigerationSystem> RefrigerationCase::system() const {
  return getImpl<detail::RefrigerationCase_Impl>()->system();
}

bool RefrigerationCase::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCase_Impl>()->setAvailabilitySchedule(schedule);
}

void RefrigerationCase::resetAvailabilitySchedule() {
  getImpl<detail::RefrigerationCase_Impl>()->resetAvailabilitySchedule();
}

bool RefrigerationCase::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationCase_Impl>()->setThermalZone(thermalZone);
}

void RefrigerationCase::resetThermalZone() {
  getImpl<detail::RefrigerationCase_Impl>()->resetThermalZone();
}

bool RefrigerationCase::setRatedAmbientTemperature(double ratedAmbientTemperature) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRatedAmbientTemperature(ratedAmbientTemperature);
}

void RefrigerationCase::resetRatedAmbientTemperature() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRatedAmbientTemperature();
}

bool RefrigerationCase::setRatedAmbientRelativeHumidity(double ratedAmbientRelativeHumidity) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRatedAmbientRelativeHumidity(ratedAmbientRelativeHumidity);
}

void RefrigerationCase::resetRatedAmbientRelativeHumidity() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRatedAmbientRelativeHumidity();
}

bool RefrigerationCase::setRatedTotalCoolingCapacityperUnitLength(double ratedTotalCoolingCapacityperUnitLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRatedTotalCoolingCapacityperUnitLength(ratedTotalCoolingCapacityperUnitLength);
}

void RefrigerationCase::resetRatedTotalCoolingCapacityperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRatedTotalCoolingCapacityperUnitLength();
}

bool RefrigerationCase::setRatedLatentHeatRatio(double ratedLatentHeatRatio) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRatedLatentHeatRatio(ratedLatentHeatRatio);
}

void RefrigerationCase::resetRatedLatentHeatRatio() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRatedLatentHeatRatio();
}

bool RefrigerationCase::setRatedRuntimeFraction(double ratedRuntimeFraction) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRatedRuntimeFraction(ratedRuntimeFraction);
}

void RefrigerationCase::resetRatedRuntimeFraction() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRatedRuntimeFraction();
}

bool RefrigerationCase::setCaseLength(double caseLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseLength(caseLength);
}

void RefrigerationCase::resetCaseLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseLength();
}

bool RefrigerationCase::setCaseOperatingTemperature(double caseOperatingTemperature) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseOperatingTemperature(caseOperatingTemperature);
}

void RefrigerationCase::resetCaseOperatingTemperature() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseOperatingTemperature();
}

bool RefrigerationCase::setLatentCaseCreditCurveType(std::string latentCaseCreditCurveType) {
  return getImpl<detail::RefrigerationCase_Impl>()->setLatentCaseCreditCurveType(latentCaseCreditCurveType);
}

void RefrigerationCase::resetLatentCaseCreditCurveType() {
  getImpl<detail::RefrigerationCase_Impl>()->resetLatentCaseCreditCurveType();
}

bool RefrigerationCase::setLatentCaseCreditCurve(const CurveCubic& curveCubic) {
  return getImpl<detail::RefrigerationCase_Impl>()->setLatentCaseCreditCurve(curveCubic);
}

bool RefrigerationCase::setStandardCaseFanPowerperUnitLength(double standardCaseFanPowerperUnitLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setStandardCaseFanPowerperUnitLength(standardCaseFanPowerperUnitLength);
}

void RefrigerationCase::resetStandardCaseFanPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetStandardCaseFanPowerperUnitLength();
}

bool RefrigerationCase::setOperatingCaseFanPowerperUnitLength(double operatingCaseFanPowerperUnitLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setOperatingCaseFanPowerperUnitLength(operatingCaseFanPowerperUnitLength);
}

void RefrigerationCase::resetOperatingCaseFanPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetOperatingCaseFanPowerperUnitLength();
}

void RefrigerationCase::setStandardCaseLightingPowerperUnitLength(double standardCaseLightingPowerperUnitLength) {
  getImpl<detail::RefrigerationCase_Impl>()->setStandardCaseLightingPowerperUnitLength(standardCaseLightingPowerperUnitLength);
}

void RefrigerationCase::resetStandardCaseLightingPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetStandardCaseLightingPowerperUnitLength();
}

void RefrigerationCase::setInstalledCaseLightingPowerperUnitLength(double installedCaseLightingPowerperUnitLength) {
  getImpl<detail::RefrigerationCase_Impl>()->setInstalledCaseLightingPowerperUnitLength(installedCaseLightingPowerperUnitLength);
}

void RefrigerationCase::resetInstalledCaseLightingPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetInstalledCaseLightingPowerperUnitLength();
}

bool RefrigerationCase::setCaseLightingSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseLightingSchedule(schedule);
}

void RefrigerationCase::resetCaseLightingSchedule() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseLightingSchedule();
}

bool RefrigerationCase::setFractionofLightingEnergytoCase(double fractionofLightingEnergytoCase) {
  return getImpl<detail::RefrigerationCase_Impl>()->setFractionofLightingEnergytoCase(fractionofLightingEnergytoCase);
}

void RefrigerationCase::resetFractionofLightingEnergytoCase() {
  getImpl<detail::RefrigerationCase_Impl>()->resetFractionofLightingEnergytoCase();
}

bool RefrigerationCase::setCaseAntiSweatHeaterPowerperUnitLength(double caseAntiSweatHeaterPowerperUnitLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseAntiSweatHeaterPowerperUnitLength(caseAntiSweatHeaterPowerperUnitLength);
}

void RefrigerationCase::resetCaseAntiSweatHeaterPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseAntiSweatHeaterPowerperUnitLength();
}

bool RefrigerationCase::setMinimumAntiSweatHeaterPowerperUnitLength(double minimumAntiSweatHeaterPowerperUnitLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setMinimumAntiSweatHeaterPowerperUnitLength(minimumAntiSweatHeaterPowerperUnitLength);
}

void RefrigerationCase::resetMinimumAntiSweatHeaterPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetMinimumAntiSweatHeaterPowerperUnitLength();
}

bool RefrigerationCase::setAntiSweatHeaterControlType(std::string antiSweatHeaterControlType) {
  return getImpl<detail::RefrigerationCase_Impl>()->setAntiSweatHeaterControlType(antiSweatHeaterControlType);
}

void RefrigerationCase::resetAntiSweatHeaterControlType() {
  getImpl<detail::RefrigerationCase_Impl>()->resetAntiSweatHeaterControlType();
}

void RefrigerationCase::setHumidityatZeroAntiSweatHeaterEnergy(double humidityatZeroAntiSweatHeaterEnergy) {
  getImpl<detail::RefrigerationCase_Impl>()->setHumidityatZeroAntiSweatHeaterEnergy(humidityatZeroAntiSweatHeaterEnergy);
}

void RefrigerationCase::resetHumidityatZeroAntiSweatHeaterEnergy() {
  getImpl<detail::RefrigerationCase_Impl>()->resetHumidityatZeroAntiSweatHeaterEnergy();
}

bool RefrigerationCase::setCaseHeight(double caseHeight) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseHeight(caseHeight);
}

void RefrigerationCase::resetCaseHeight() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseHeight();
}

bool RefrigerationCase::setFractionofAntiSweatHeaterEnergytoCase(double fractionofAntiSweatHeaterEnergytoCase) {
  return getImpl<detail::RefrigerationCase_Impl>()->setFractionofAntiSweatHeaterEnergytoCase(fractionofAntiSweatHeaterEnergytoCase);
}

void RefrigerationCase::resetFractionofAntiSweatHeaterEnergytoCase() {
  getImpl<detail::RefrigerationCase_Impl>()->resetFractionofAntiSweatHeaterEnergytoCase();
}

bool RefrigerationCase::setCaseDefrostPowerperUnitLength(double caseDefrostPowerperUnitLength) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseDefrostPowerperUnitLength(caseDefrostPowerperUnitLength);
}

void RefrigerationCase::resetCaseDefrostPowerperUnitLength() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseDefrostPowerperUnitLength();
}

bool RefrigerationCase::setCaseDefrostType(std::string caseDefrostType) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseDefrostType(caseDefrostType);
}

void RefrigerationCase::resetCaseDefrostType() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseDefrostType();
}

bool RefrigerationCase::setCaseDefrostSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseDefrostSchedule(schedule);
}

void RefrigerationCase::resetCaseDefrostSchedule() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseDefrostSchedule();
}

bool RefrigerationCase::setCaseDefrostDripDownSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseDefrostDripDownSchedule(schedule);
}

void RefrigerationCase::resetCaseDefrostDripDownSchedule() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseDefrostDripDownSchedule();
}

bool RefrigerationCase::setDefrostEnergyCorrectionCurveType(std::string defrostEnergyCorrectionCurveType) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrostEnergyCorrectionCurveType(defrostEnergyCorrectionCurveType);
}

void RefrigerationCase::resetDefrostEnergyCorrectionCurveType() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrostEnergyCorrectionCurveType();
}

bool RefrigerationCase::setDefrostEnergyCorrectionCurve(const CurveCubic& curveCubic) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrostEnergyCorrectionCurve(curveCubic);
}

void RefrigerationCase::resetDefrostEnergyCorrectionCurve() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrostEnergyCorrectionCurve();
}

bool RefrigerationCase::setUnderCaseHVACReturnAirFraction(double underCaseHVACReturnAirFraction) {
  return getImpl<detail::RefrigerationCase_Impl>()->setUnderCaseHVACReturnAirFraction(underCaseHVACReturnAirFraction);
}

void RefrigerationCase::resetUnderCaseHVACReturnAirFraction() {
  getImpl<detail::RefrigerationCase_Impl>()->resetUnderCaseHVACReturnAirFraction();
}

bool RefrigerationCase::setRefrigeratedCaseRestockingSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRefrigeratedCaseRestockingSchedule(schedule);
}

void RefrigerationCase::resetRefrigeratedCaseRestockingSchedule() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRefrigeratedCaseRestockingSchedule();
}

bool RefrigerationCase::setCaseCreditFractionSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseCreditFractionSchedule(schedule);
}

void RefrigerationCase::resetCaseCreditFractionSchedule() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseCreditFractionSchedule();
}

bool RefrigerationCase::setDesignEvaporatorTemperatureorBrineInletTemperature(double designEvaporatorTemperatureorBrineInletTemperature) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDesignEvaporatorTemperatureorBrineInletTemperature(designEvaporatorTemperatureorBrineInletTemperature);
}

void RefrigerationCase::resetDesignEvaporatorTemperatureorBrineInletTemperature() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDesignEvaporatorTemperatureorBrineInletTemperature();
}

void RefrigerationCase::setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory) {
  getImpl<detail::RefrigerationCase_Impl>()->setAverageRefrigerantChargeInventory(averageRefrigerantChargeInventory);
}

void RefrigerationCase::resetAverageRefrigerantChargeInventory() {
  getImpl<detail::RefrigerationCase_Impl>()->resetAverageRefrigerantChargeInventory();
}

bool RefrigerationCase::setNumberOfDoors(int numberOfDoors) {
  return getImpl<detail::RefrigerationCase_Impl>()->setNumberOfDoors(numberOfDoors);
}

void RefrigerationCase::resetNumberOfDoors() {
  getImpl<detail::RefrigerationCase_Impl>()->resetNumberOfDoors();
}

bool RefrigerationCase::setRatedTotalCoolingCapacityperDoor(double ratedTotalCoolingCapacityperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setRatedTotalCoolingCapacityperDoor(ratedTotalCoolingCapacityperDoor);
}

void RefrigerationCase::resetRatedTotalCoolingCapacityperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetRatedTotalCoolingCapacityperDoor();
}

bool RefrigerationCase::setStandardCaseFanPowerperDoor(double standardCaseFanPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setStandardCaseFanPowerperDoor(standardCaseFanPowerperDoor);
}

void RefrigerationCase::resetStandardCaseFanPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetStandardCaseFanPowerperDoor();
}

bool RefrigerationCase::setOperatingCaseFanPowerperDoor(double operatingCaseFanPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setOperatingCaseFanPowerperDoor(operatingCaseFanPowerperDoor);
}

void RefrigerationCase::resetOperatingCaseFanPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetOperatingCaseFanPowerperDoor();
}

bool RefrigerationCase::setStandardCaseLightingPowerperDoor(double standardCaseLightingPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setStandardCaseLightingPowerperDoor(standardCaseLightingPowerperDoor);
}

void RefrigerationCase::resetStandardCaseLightingPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetStandardCaseLightingPowerperDoor();
}

bool RefrigerationCase::setInstalledCaseLightingPowerperDoor(double installedCaseLightingPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setInstalledCaseLightingPowerperDoor(installedCaseLightingPowerperDoor);
}

void RefrigerationCase::resetInstalledCaseLightingPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetInstalledCaseLightingPowerperDoor();
}

bool RefrigerationCase::setCaseAntiSweatHeaterPowerperDoor(double caseAntiSweatHeaterPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseAntiSweatHeaterPowerperDoor(caseAntiSweatHeaterPowerperDoor);
}

void RefrigerationCase::resetCaseAntiSweatHeaterPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseAntiSweatHeaterPowerperDoor();
}

bool RefrigerationCase::setMinimumAntiSweatHeaterPowerperDoor(double minimumAntiSweatHeaterPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setMinimumAntiSweatHeaterPowerperDoor(minimumAntiSweatHeaterPowerperDoor);
}

void RefrigerationCase::resetMinimumAntiSweatHeaterPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetMinimumAntiSweatHeaterPowerperDoor();
}

bool RefrigerationCase::setCaseDefrostPowerperDoor(double caseDefrostPowerperDoor) {
  return getImpl<detail::RefrigerationCase_Impl>()->setCaseDefrostPowerperDoor(caseDefrostPowerperDoor);
}

void RefrigerationCase::resetCaseDefrostPowerperDoor() {
  getImpl<detail::RefrigerationCase_Impl>()->resetCaseDefrostPowerperDoor();
}

bool RefrigerationCase::setUnitType(std::string unitType) {
  return getImpl<detail::RefrigerationCase_Impl>()->setUnitType(unitType);
}

void RefrigerationCase::resetUnitType() {
  getImpl<detail::RefrigerationCase_Impl>()->resetUnitType();
}

bool RefrigerationCase::setDurationofDefrostCycle(int durationofDefrostCycle) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDurationofDefrostCycle(durationofDefrostCycle);
}

void RefrigerationCase::resetDurationofDefrostCycle() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDurationofDefrostCycle();
}

bool RefrigerationCase::setDripDownTime(int dripDownTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDripDownTime(dripDownTime);
}

void RefrigerationCase::resetDripDownTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDripDownTime();
}

bool RefrigerationCase::setDefrost1StartTime(const openstudio::Time& defrost1StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost1StartTime(defrost1StartTime);
}

void RefrigerationCase::resetDefrost1StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost1StartTime();
}

bool RefrigerationCase::setDefrost2StartTime(const openstudio::Time& defrost2StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost2StartTime(defrost2StartTime);
}

void RefrigerationCase::resetDefrost2StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost2StartTime();
}

bool RefrigerationCase::setDefrost3StartTime(const openstudio::Time& defrost3StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost3StartTime(defrost3StartTime);
}

void RefrigerationCase::resetDefrost3StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost3StartTime();
}

bool RefrigerationCase::setDefrost4StartTime(const openstudio::Time& defrost4StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost4StartTime(defrost4StartTime);
}

void RefrigerationCase::resetDefrost4StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost4StartTime();
}

bool RefrigerationCase::setDefrost5StartTime(const openstudio::Time& defrost5StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost5StartTime(defrost5StartTime);
}

void RefrigerationCase::resetDefrost5StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost5StartTime();
}

bool RefrigerationCase::setDefrost6StartTime(const openstudio::Time& defrost6StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost6StartTime(defrost6StartTime);
}

void RefrigerationCase::resetDefrost6StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost6StartTime();
}

bool RefrigerationCase::setDefrost7StartTime(const openstudio::Time& defrost7StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost7StartTime(defrost7StartTime);
}

void RefrigerationCase::resetDefrost7StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost7StartTime();
}

bool RefrigerationCase::setDefrost8StartTime(const openstudio::Time& defrost8StartTime) {
  return getImpl<detail::RefrigerationCase_Impl>()->setDefrost8StartTime(defrost8StartTime);
}

void RefrigerationCase::resetDefrost8StartTime() {
  getImpl<detail::RefrigerationCase_Impl>()->resetDefrost8StartTime();
}

bool RefrigerationCase::addToSystem(RefrigerationSystem & system) {
  return getImpl<detail::RefrigerationCase_Impl>()->addToSystem(system);
}

void RefrigerationCase::removeFromSystem() {
  getImpl<detail::RefrigerationCase_Impl>()->removeFromSystem();
}

/// @cond
RefrigerationCase::RefrigerationCase(std::shared_ptr<detail::RefrigerationCase_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

