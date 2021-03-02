/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ElectricEquipmentITEAirCooled.hpp"
#include "ElectricEquipmentITEAirCooled_Impl.hpp"

#include "ElectricEquipmentITEAirCooledDefinition.hpp"
#include "ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "Model.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricEquipment_ITE_AirCooled_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ElectricEquipmentITEAirCooled_Impl::ElectricEquipmentITEAirCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ElectricEquipmentITEAirCooled::iddObjectType());
    }

    ElectricEquipmentITEAirCooled_Impl::ElectricEquipmentITEAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ElectricEquipmentITEAirCooled::iddObjectType());
    }

    ElectricEquipmentITEAirCooled_Impl::ElectricEquipmentITEAirCooled_Impl(const ElectricEquipmentITEAirCooled_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ElectricEquipmentITEAirCooled_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // IT Equipment
        "ITE CPU Electricity Rate ",
        "ITE Fan Electricity Rate ",
        "ITE UPS Electricity Rate ",
        "ITE CPU Electricity Rate at Design Inlet Conditions ",
        "ITE Fan Electricity Rate at Design Inlet Conditions ",
        "ITE UPS Heat Gain to Zone Rate ",
        "ITE Total Heat Gain to Zone Rate ",
        "ITE CPU Electricity Energy ",
        "ITE Fan Electricity Energy ",
        "ITE UPS Electricity Energy ",
        "ITE CPU Electricity Energy at Design Inlet Conditions ",
        "ITE Fan Electricity Energy at Design Inlet Conditions ",
        "ITE UPS Heat Gain to Zone Energy ",
        "ITE Total Heat Gain to Zone Energy ",
        "ITE Standard Density Air Volume Flow Rate ",
        "ITE Current Density Air Volume Flow Rate ",
        "ITE Air Mass Flow Rate ",
        "ITE Air Inlet Dry-Bulb Temperature ",
        "ITE Air Inlet Dewpoint Temperature ",
        "ITE Air Inlet Relative Humidity ",
        "ITE Air Outlet Dry-Bulb Temperature ",
        "ITE Supply Heat Index ",
        "ITE Air Inlet Operating Range Exceeded Time ",
        "ITE Air Inlet Dry-Bulb Temperature Above Operating Range Time ",
        "ITE Air Inlet Dry-Bulb Temperature Below Operating Range Time ",
        "ITE Air Inlet Dewpoint Temperature Above Operating Range Time ",
        "ITE Air Inlet Dewpoint Temperature Below Operating Range Time ",
        "ITE Air Inlet Relative Humidity Above Operating Range Time ",
        "ITE Air Inlet Relative Humidity Below Operating Range Time ",
        "ITE Air Inlet Dry-Bulb Temperature Difference Above Operating Range ",
        "ITE Air Inlet Dry-Bulb Temperature Difference Below Operating Range ",
        "ITE Air Inlet Dewpoint Temperature Difference Above Operating Range ",
        "ITE Air Inlet Dewpoint Temperature Difference Below Operating Range ",
        "ITE Air Inlet Relative Humidity Difference Above Operating Range ",
        "ITE Air Inlet Relative Humidity Difference Below Operating Range "

        // Reported in ThermalZone
        //"Zone ITE Adjusted Return Air Temperature ",
        //"Zone ITE CPU Electricity Rate ",
        //"Zone ITE Fan Electricity Rate ",
        //"Zone ITE UPS Electricity Rate ",
        //"Zone ITE CPU Electricity Rate at Design Inlet Conditions ",
        //"Zone ITE Fan Electricity Rate at Design Inlet Conditions ",
        //"Zone ITE UPS Heat Gain to Zone Rate ",
        //"Zone ITE Total Heat Gain to Zone Rate ",
        //"Zone ITE CPU Electricity Energy ",
        //"Zone ITE Fan Electricity Energy ",
        //"Zone ITE UPS Electricity Energy ",
        //"Zone ITE CPU Electricity Energy at Design Inlet Conditions ",
        //"Zone ITE Fan Electricity Energy at Design Inlet Conditions ",
        //"Zone ITE UPS Heat Gain to Zone Energy ",
        //"Zone ITE Total Heat Gain to Zone Energy ",
        //"Zone ITE Standard Density Air Volume Flow Rate ",
        //"Zone ITE Air Mass Flow Rate ",
        //"Zone ITE Average Supply Heat Index ",
        //"Zone ITE Any Air Inlet Operating Range Exceeded Time ",
        //"Zone ITE Any Air Inlet Dry-Bulb Temperature Above Operating Range Time ",
        //"Zone ITE Any Air Inlet Dry-Bulb Temperature Below Operating Range Time ",
        //"Zone ITE Any Air Inlet Dewpoint Temperature Above Operating Range Time ",
        //"Zone ITE Any Air Inlet Dewpoint Temperature Below Operating Range Time ",
        //"Zone ITE Any Air Inlet Relative Humidity Above Operating Range Time ",
        //"Zone ITE Any Air Inlet Relative Humidity Below Operating Range Time "
      };
      return result;
    }

    IddObjectType ElectricEquipmentITEAirCooled_Impl::iddObjectType() const {
      return ElectricEquipmentITEAirCooled::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ElectricEquipmentITEAirCooled_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ElectricEquipmentITEAirCooled", "Design Power Input"));
      }
      if (std::find(b, e, OS_ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ElectricEquipmentITEAirCooled", "CPU Loading"));
      }
      return result;
    }

    bool ElectricEquipmentITEAirCooled_Impl::hardSize() {
      boost::optional<Space> space = this->space();
      if (!space) {
        return false;
      }

      this->makeUnique();

      ElectricEquipmentITEAirCooledDefinition electricEquipmentITEAirCooledDefinition = this->electricEquipmentITEAirCooledDefinition();
      for (LifeCycleCost cost : electricEquipmentITEAirCooledDefinition.lifeCycleCosts()) {
        cost.convertToCostPerEach();
      }

      boost::optional<double> wattsperUnit = electricEquipmentITEAirCooledDefinition.wattsperUnit();
      if (wattsperUnit) {
        return true;
      }

      boost::optional<double> wattsperZoneFloorArea = electricEquipmentITEAirCooledDefinition.wattsperZoneFloorArea();
      if (wattsperZoneFloorArea) {
        return electricEquipmentITEAirCooledDefinition.setWattsperUnit(*wattsperZoneFloorArea * space->floorArea());
      }

      return false;
    }

    bool ElectricEquipmentITEAirCooled_Impl::hardApplySchedules() {
      bool result = true;

      // optional in EnergyPlus
      boost::optional<Schedule> designPowerInputSchedule = this->designPowerInputSchedule();
      if (designPowerInputSchedule) {
        result = result && this->setDesignPowerInputSchedule(*designPowerInputSchedule);
      }

      // optional in EnergyPlus
      boost::optional<Schedule> cPULoadingSchedule = this->cPULoadingSchedule();
      if (cPULoadingSchedule) {
        result = result && this->setCPULoadingSchedule(*cPULoadingSchedule);
      }

      return result;
    }

    double ElectricEquipmentITEAirCooled_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooledFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricEquipmentITEAirCooled_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_ElectricEquipment_ITE_AirCooledFields::Multiplier);
    }

    bool ElectricEquipmentITEAirCooled_Impl::isAbsolute() const {
      ElectricEquipmentITEAirCooledDefinition definition = electricEquipmentITEAirCooledDefinition();
      if (definition.wattsperUnit()) {
        return true;
      }
      return false;
    }

    ElectricEquipmentITEAirCooledDefinition ElectricEquipmentITEAirCooled_Impl::electricEquipmentITEAirCooledDefinition() const {
      return this->definition().cast<ElectricEquipmentITEAirCooledDefinition>();
    }

    boost::optional<Schedule> ElectricEquipmentITEAirCooled_Impl::designPowerInputSchedule() const {
      boost::optional<Schedule> result =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName);
      if (!result) {
        // search upwards
        OptionalSpace space = this->space();
        OptionalSpaceType spaceType = this->spaceType();
        if (space) {
          result = space->getDefaultSchedule(DefaultScheduleType::DesignPowerInputScheduleName);
        } else if (spaceType) {
          result = spaceType->getDefaultSchedule(DefaultScheduleType::DesignPowerInputScheduleName);
        }
      }
      return result;
    }

    bool ElectricEquipmentITEAirCooled_Impl::isDesignPowerInputScheduleDefaulted() const {
      return isEmpty(OS_ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName);
    }

    boost::optional<Schedule> ElectricEquipmentITEAirCooled_Impl::cPULoadingSchedule() const {
      boost::optional<Schedule> result =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName);
      if (!result) {
        // search upwards
        OptionalSpace space = this->space();
        OptionalSpaceType spaceType = this->spaceType();
        if (space) {
          result = space->getDefaultSchedule(DefaultScheduleType::CPULoadingScheduleName);
        } else if (spaceType) {
          result = spaceType->getDefaultSchedule(DefaultScheduleType::CPULoadingScheduleName);
        }
      }
      return result;
    }

    bool ElectricEquipmentITEAirCooled_Impl::isCPULoadingScheduleDefaulted() const {
      return isEmpty(OS_ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName);
    }

    std::string ElectricEquipmentITEAirCooled_Impl::cPUEndUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooledFields::CPUEndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricEquipmentITEAirCooled_Impl::isCPUEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_ElectricEquipment_ITE_AirCooledFields::CPUEndUseSubcategory);
    }

    std::string ElectricEquipmentITEAirCooled_Impl::fanEndUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooledFields::FanEndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricEquipmentITEAirCooled_Impl::isFanEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_ElectricEquipment_ITE_AirCooledFields::FanEndUseSubcategory);
    }

    std::string ElectricEquipmentITEAirCooled_Impl::electricPowerSupplyEndUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricEquipmentITEAirCooled_Impl::isElectricPowerSupplyEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEndUseSubcategory);
    }

    bool ElectricEquipmentITEAirCooled_Impl::setElectricEquipmentITEAirCooledDefinition(
      const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition) {
      return this->setPointer(this->definitionIndex(), electricEquipmentITEAirCooledDefinition.handle());
    }

    bool ElectricEquipmentITEAirCooled_Impl::setDefinition(const SpaceLoadDefinition& definition) {
      bool result = false;
      boost::optional<ElectricEquipmentITEAirCooledDefinition> electricEquipmentITEAirCooledDefinition =
        definition.optionalCast<ElectricEquipmentITEAirCooledDefinition>();
      if (electricEquipmentITEAirCooledDefinition) {
        result = setElectricEquipmentITEAirCooledDefinition(*electricEquipmentITEAirCooledDefinition);
      }
      return result;
    }

    bool ElectricEquipmentITEAirCooled_Impl::setDesignPowerInputSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName, "ElectricEquipmentITEAirCooled",
                                "Design Power Input", schedule);
      return result;
    }

    void ElectricEquipmentITEAirCooled_Impl::resetDesignPowerInputSchedule() {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName, "");
      OS_ASSERT(result);
    }

    bool ElectricEquipmentITEAirCooled_Impl::setCPULoadingSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName, "ElectricEquipmentITEAirCooled", "CPU Loading", schedule);
      return result;
    }

    void ElectricEquipmentITEAirCooled_Impl::resetCPULoadingSchedule() {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName, "");
      OS_ASSERT(result);
    }

    bool ElectricEquipmentITEAirCooled_Impl::setMultiplier(double multiplier) {
      bool result = setDouble(OS_ElectricEquipment_ITE_AirCooledFields::Multiplier, multiplier);
      return result;
    }

    void ElectricEquipmentITEAirCooled_Impl::resetMultiplier() {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::Multiplier, "");
      OS_ASSERT(result);
    }

    bool ElectricEquipmentITEAirCooled_Impl::setCPUEndUseSubcategory(const std::string& cPUEndUseSubcategory) {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::CPUEndUseSubcategory, cPUEndUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void ElectricEquipmentITEAirCooled_Impl::resetCPUEndUseSubcategory() {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::CPUEndUseSubcategory, "");
      OS_ASSERT(result);
    }

    bool ElectricEquipmentITEAirCooled_Impl::setFanEndUseSubcategory(const std::string& fanEndUseSubcategory) {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::FanEndUseSubcategory, fanEndUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void ElectricEquipmentITEAirCooled_Impl::resetFanEndUseSubcategory() {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::FanEndUseSubcategory, "");
      OS_ASSERT(result);
    }

    bool ElectricEquipmentITEAirCooled_Impl::setElectricPowerSupplyEndUseSubcategory(const std::string& electricPowerSupplyEndUseSubcategory) {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEndUseSubcategory, electricPowerSupplyEndUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void ElectricEquipmentITEAirCooled_Impl::resetElectricPowerSupplyEndUseSubcategory() {
      bool result = setString(OS_ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEndUseSubcategory, "");
      OS_ASSERT(result);
    }

    int ElectricEquipmentITEAirCooled_Impl::spaceIndex() const {
      return OS_ElectricEquipment_ITE_AirCooledFields::SpaceorSpaceTypeName;
    }

    int ElectricEquipmentITEAirCooled_Impl::definitionIndex() const {
      return OS_ElectricEquipment_ITE_AirCooledFields::ElectricEquipmentITEAirCooledDefinitionName;
    }

    // Other functions

    boost::optional<double> ElectricEquipmentITEAirCooled_Impl::wattsperUnit() const {
      OptionalDouble result = electricEquipmentITEAirCooledDefinition().wattsperUnit();
      if (result) {
        return result.get() * multiplier();
      }
      return result;
    }

    boost::optional<double> ElectricEquipmentITEAirCooled_Impl::wattsperZoneFloorArea() const {
      OptionalDouble result = electricEquipmentITEAirCooledDefinition().wattsperZoneFloorArea();
      if (result) {
        return result.get() * multiplier();
      }
      return result;
    }

    double ElectricEquipmentITEAirCooled_Impl::getWattsperUnit(double floorArea) const {
      return electricEquipmentITEAirCooledDefinition().getWattsperUnit(floorArea) * multiplier();
    }

    double ElectricEquipmentITEAirCooled_Impl::getWattsperZoneFloorArea(double floorArea) const {
      return electricEquipmentITEAirCooledDefinition().getWattsperZoneFloorArea(floorArea) * multiplier();
    }

  }  // namespace detail

  ElectricEquipmentITEAirCooled::ElectricEquipmentITEAirCooled(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition)
    : SpaceLoadInstance(ElectricEquipmentITEAirCooled::iddObjectType(), electricEquipmentITEAirCooledDefinition) {
    OS_ASSERT(getImpl<detail::ElectricEquipmentITEAirCooled_Impl>());

    //     OS_ElectricEquipment_ITE_AirCooledFields::ElectricEquipmentITEAirCooledDefinitionName
    bool ok = this->setElectricEquipmentITEAirCooledDefinition(electricEquipmentITEAirCooledDefinition);
    OS_ASSERT(ok);
  }

  IddObjectType ElectricEquipmentITEAirCooled::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ElectricEquipment_ITE_AirCooled);
  }

  ElectricEquipmentITEAirCooledDefinition ElectricEquipmentITEAirCooled::electricEquipmentITEAirCooledDefinition() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->electricEquipmentITEAirCooledDefinition();
  }

  boost::optional<Schedule> ElectricEquipmentITEAirCooled::designPowerInputSchedule() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->designPowerInputSchedule();
  }

  bool ElectricEquipmentITEAirCooled::isDesignPowerInputScheduleDefaulted() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->isDesignPowerInputScheduleDefaulted();
  }

  boost::optional<Schedule> ElectricEquipmentITEAirCooled::cPULoadingSchedule() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->cPULoadingSchedule();
  }

  bool ElectricEquipmentITEAirCooled::isCPULoadingScheduleDefaulted() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->isCPULoadingScheduleDefaulted();
  }

  std::string ElectricEquipmentITEAirCooled::cPUEndUseSubcategory() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->cPUEndUseSubcategory();
  }

  bool ElectricEquipmentITEAirCooled::isCPUEndUseSubcategoryDefaulted() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->isCPUEndUseSubcategoryDefaulted();
  }

  std::string ElectricEquipmentITEAirCooled::fanEndUseSubcategory() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->fanEndUseSubcategory();
  }

  bool ElectricEquipmentITEAirCooled::isFanEndUseSubcategoryDefaulted() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->isFanEndUseSubcategoryDefaulted();
  }

  std::string ElectricEquipmentITEAirCooled::electricPowerSupplyEndUseSubcategory() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->electricPowerSupplyEndUseSubcategory();
  }

  bool ElectricEquipmentITEAirCooled::isElectricPowerSupplyEndUseSubcategoryDefaulted() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->isElectricPowerSupplyEndUseSubcategoryDefaulted();
  }

  bool ElectricEquipmentITEAirCooled::setElectricEquipmentITEAirCooledDefinition(
    const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setElectricEquipmentITEAirCooledDefinition(electricEquipmentITEAirCooledDefinition);
  }

  bool ElectricEquipmentITEAirCooled::setDesignPowerInputSchedule(Schedule& schedule) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setDesignPowerInputSchedule(schedule);
  }

  void ElectricEquipmentITEAirCooled::resetDesignPowerInputSchedule() {
    getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->resetDesignPowerInputSchedule();
  }

  bool ElectricEquipmentITEAirCooled::setCPULoadingSchedule(Schedule& schedule) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setCPULoadingSchedule(schedule);
  }

  void ElectricEquipmentITEAirCooled::resetCPULoadingSchedule() {
    getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->resetCPULoadingSchedule();
  }

  bool ElectricEquipmentITEAirCooled::setMultiplier(double multiplier) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setMultiplier(multiplier);
  }

  void ElectricEquipmentITEAirCooled::resetMultiplier() {
    getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->resetMultiplier();
  }

  bool ElectricEquipmentITEAirCooled::setCPUEndUseSubcategory(const std::string& cPUEndUseSubcategory) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setCPUEndUseSubcategory(cPUEndUseSubcategory);
  }

  void ElectricEquipmentITEAirCooled::resetCPUEndUseSubcategory() {
    getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->resetCPUEndUseSubcategory();
  }

  bool ElectricEquipmentITEAirCooled::setFanEndUseSubcategory(const std::string& fanEndUseSubcategory) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setFanEndUseSubcategory(fanEndUseSubcategory);
  }

  void ElectricEquipmentITEAirCooled::resetFanEndUseSubcategory() {
    getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->resetFanEndUseSubcategory();
  }

  bool ElectricEquipmentITEAirCooled::setElectricPowerSupplyEndUseSubcategory(const std::string& electricPowerSupplyEndUseSubcategory) {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->setElectricPowerSupplyEndUseSubcategory(electricPowerSupplyEndUseSubcategory);
  }

  void ElectricEquipmentITEAirCooled::resetElectricPowerSupplyEndUseSubcategory() {
    getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->resetElectricPowerSupplyEndUseSubcategory();
  }

  boost::optional<double> ElectricEquipmentITEAirCooled::wattsperUnit() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->wattsperUnit();
  }

  boost::optional<double> ElectricEquipmentITEAirCooled::wattsperZoneFloorArea() const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->wattsperZoneFloorArea();
  }

  double ElectricEquipmentITEAirCooled::getWattsperUnit(double floorArea) const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->getWattsperUnit(floorArea);
  }

  double ElectricEquipmentITEAirCooled::getWattsperZoneFloorArea(double floorArea) const {
    return getImpl<detail::ElectricEquipmentITEAirCooled_Impl>()->getWattsperZoneFloorArea(floorArea);
  }

  /// @cond
  ElectricEquipmentITEAirCooled::ElectricEquipmentITEAirCooled(std::shared_ptr<detail::ElectricEquipmentITEAirCooled_Impl> impl)
    : SpaceLoadInstance(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
