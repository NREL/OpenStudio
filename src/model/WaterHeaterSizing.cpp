/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "WaterHeaterSizing.hpp"
#include "WaterHeaterSizing_Impl.hpp"

#include "Model.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_WaterHeater_Sizing_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    WaterHeaterSizing_Impl::WaterHeaterSizing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WaterHeaterSizing::iddObjectType());
    }

    WaterHeaterSizing_Impl::WaterHeaterSizing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WaterHeaterSizing::iddObjectType());
    }

    WaterHeaterSizing_Impl::WaterHeaterSizing_Impl(const WaterHeaterSizing_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& WaterHeaterSizing_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType WaterHeaterSizing_Impl::iddObjectType() const {
      return WaterHeaterSizing::iddObjectType();
    }

    WaterToWaterComponent WaterHeaterSizing_Impl::waterHeater() const {
      boost::optional<WaterToWaterComponent> value = optionalWaterHeater();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Water Heater attached.");
      }
      return value.get();
    }

    boost::optional<WaterToWaterComponent> WaterHeaterSizing_Impl::optionalWaterHeater() const {
      return getObject<ModelObject>().getModelObjectTarget<WaterToWaterComponent>(OS_WaterHeater_SizingFields::WaterHeaterName);
    }

    bool WaterHeaterSizing_Impl::setWaterHeater(const WaterToWaterComponent& waterHeater) {
      bool result = setPointer(OS_WaterHeater_SizingFields::WaterHeaterName, waterHeater.handle());
      return result;
    }

    boost::optional<std::string> WaterHeaterSizing_Impl::designMode() const {
      return getString(OS_WaterHeater_SizingFields::DesignMode, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::timeStorageCanMeetPeakDraw() const {
      return getDouble(OS_WaterHeater_SizingFields::TimeStorageCanMeetPeakDraw, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::timeforTankRecovery() const {
      return getDouble(OS_WaterHeater_SizingFields::TimeforTankRecovery, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::nominalTankVolumeforAutosizingPlantConnections() const {
      return getDouble(OS_WaterHeater_SizingFields::NominalTankVolumeforAutosizingPlantConnections, true);
    }

    boost::optional<int> WaterHeaterSizing_Impl::numberofBedrooms() const {
      return getInt(OS_WaterHeater_SizingFields::NumberofBedrooms, true);
    }

    boost::optional<int> WaterHeaterSizing_Impl::numberofBathrooms() const {
      return getInt(OS_WaterHeater_SizingFields::NumberofBathrooms, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::storageCapacityperPerson() const {
      return getDouble(OS_WaterHeater_SizingFields::StorageCapacityperPerson, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::recoveryCapacityperPerson() const {
      return getDouble(OS_WaterHeater_SizingFields::RecoveryCapacityperPerson, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::storageCapacityperFloorArea() const {
      return getDouble(OS_WaterHeater_SizingFields::StorageCapacityperFloorArea, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::recoveryCapacityperFloorArea() const {
      return getDouble(OS_WaterHeater_SizingFields::RecoveryCapacityperFloorArea, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::numberofUnits() const {
      return getDouble(OS_WaterHeater_SizingFields::NumberofUnits, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::storageCapacityperUnit() const {
      return getDouble(OS_WaterHeater_SizingFields::StorageCapacityperUnit, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::recoveryCapacityPerUnit() const {
      return getDouble(OS_WaterHeater_SizingFields::RecoveryCapacityPerUnit, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::storageCapacityperCollectorArea() const {
      return getDouble(OS_WaterHeater_SizingFields::StorageCapacityperCollectorArea, true);
    }

    boost::optional<double> WaterHeaterSizing_Impl::heightAspectRatio() const {
      return getDouble(OS_WaterHeater_SizingFields::HeightAspectRatio, true);
    }

    bool WaterHeaterSizing_Impl::setDesignMode(const std::string& designMode) {
      bool result = setString(OS_WaterHeater_SizingFields::DesignMode, designMode);
      return result;
    }

    void WaterHeaterSizing_Impl::resetDesignMode() {
      bool result = setString(OS_WaterHeater_SizingFields::DesignMode, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setTimeStorageCanMeetPeakDraw(double timeStorageCanMeetPeakDraw) {
      bool result = setDouble(OS_WaterHeater_SizingFields::TimeStorageCanMeetPeakDraw, timeStorageCanMeetPeakDraw);
      return result;
    }

    void WaterHeaterSizing_Impl::resetTimeStorageCanMeetPeakDraw() {
      bool result = setString(OS_WaterHeater_SizingFields::TimeStorageCanMeetPeakDraw, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setTimeforTankRecovery(double timeforTankRecovery) {
      bool result = setDouble(OS_WaterHeater_SizingFields::TimeforTankRecovery, timeforTankRecovery);
      return result;
    }

    void WaterHeaterSizing_Impl::resetTimeforTankRecovery() {
      bool result = setString(OS_WaterHeater_SizingFields::TimeforTankRecovery, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setNominalTankVolumeforAutosizingPlantConnections(double nominalTankVolumeforAutosizingPlantConnections) {
      bool result =
        setDouble(OS_WaterHeater_SizingFields::NominalTankVolumeforAutosizingPlantConnections, nominalTankVolumeforAutosizingPlantConnections);
      OS_ASSERT(result);
      return result;
    }

    void WaterHeaterSizing_Impl::resetNominalTankVolumeforAutosizingPlantConnections() {
      bool result = setString(OS_WaterHeater_SizingFields::NominalTankVolumeforAutosizingPlantConnections, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setNumberofBedrooms(int numberofBedrooms) {
      bool result = setInt(OS_WaterHeater_SizingFields::NumberofBedrooms, numberofBedrooms);
      return result;
    }

    void WaterHeaterSizing_Impl::resetNumberofBedrooms() {
      bool result = setString(OS_WaterHeater_SizingFields::NumberofBedrooms, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setNumberofBathrooms(int numberofBathrooms) {
      bool result = setInt(OS_WaterHeater_SizingFields::NumberofBathrooms, numberofBathrooms);
      return result;
    }

    void WaterHeaterSizing_Impl::resetNumberofBathrooms() {
      bool result = setString(OS_WaterHeater_SizingFields::NumberofBathrooms, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setStorageCapacityperPerson(double storageCapacityperPerson) {
      bool result = setDouble(OS_WaterHeater_SizingFields::StorageCapacityperPerson, storageCapacityperPerson);
      return result;
    }

    void WaterHeaterSizing_Impl::resetStorageCapacityperPerson() {
      bool result = setString(OS_WaterHeater_SizingFields::StorageCapacityperPerson, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setRecoveryCapacityperPerson(double recoveryCapacityperPerson) {
      bool result = setDouble(OS_WaterHeater_SizingFields::RecoveryCapacityperPerson, recoveryCapacityperPerson);
      return result;
    }

    void WaterHeaterSizing_Impl::resetRecoveryCapacityperPerson() {
      bool result = setString(OS_WaterHeater_SizingFields::RecoveryCapacityperPerson, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setStorageCapacityperFloorArea(double storageCapacityperFloorArea) {
      bool result = setDouble(OS_WaterHeater_SizingFields::StorageCapacityperFloorArea, storageCapacityperFloorArea);
      return result;
    }

    void WaterHeaterSizing_Impl::resetStorageCapacityperFloorArea() {
      bool result = setString(OS_WaterHeater_SizingFields::StorageCapacityperFloorArea, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setRecoveryCapacityperFloorArea(double recoveryCapacityperFloorArea) {
      bool result = setDouble(OS_WaterHeater_SizingFields::RecoveryCapacityperFloorArea, recoveryCapacityperFloorArea);
      return result;
    }

    void WaterHeaterSizing_Impl::resetRecoveryCapacityperFloorArea() {
      bool result = setString(OS_WaterHeater_SizingFields::RecoveryCapacityperFloorArea, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setNumberofUnits(double numberofUnits) {
      bool result = setDouble(OS_WaterHeater_SizingFields::NumberofUnits, numberofUnits);
      OS_ASSERT(result);
      return result;
    }

    void WaterHeaterSizing_Impl::resetNumberofUnits() {
      bool result = setString(OS_WaterHeater_SizingFields::NumberofUnits, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setStorageCapacityperUnit(double storageCapacityperUnit) {
      bool result = setDouble(OS_WaterHeater_SizingFields::StorageCapacityperUnit, storageCapacityperUnit);
      return result;
    }

    void WaterHeaterSizing_Impl::resetStorageCapacityperUnit() {
      bool result = setString(OS_WaterHeater_SizingFields::StorageCapacityperUnit, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setRecoveryCapacityPerUnit(double recoveryCapacityPerUnit) {
      bool result = setDouble(OS_WaterHeater_SizingFields::RecoveryCapacityPerUnit, recoveryCapacityPerUnit);
      return result;
    }

    void WaterHeaterSizing_Impl::resetRecoveryCapacityPerUnit() {
      bool result = setString(OS_WaterHeater_SizingFields::RecoveryCapacityPerUnit, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setStorageCapacityperCollectorArea(double storageCapacityperCollectorArea) {
      bool result = setDouble(OS_WaterHeater_SizingFields::StorageCapacityperCollectorArea, storageCapacityperCollectorArea);
      return result;
    }

    void WaterHeaterSizing_Impl::resetStorageCapacityperCollectorArea() {
      bool result = setString(OS_WaterHeater_SizingFields::StorageCapacityperCollectorArea, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterSizing_Impl::setHeightAspectRatio(double heightAspectRatio) {
      bool result = setDouble(OS_WaterHeater_SizingFields::HeightAspectRatio, heightAspectRatio);
      return result;
    }

    void WaterHeaterSizing_Impl::resetHeightAspectRatio() {
      bool result = setString(OS_WaterHeater_SizingFields::HeightAspectRatio, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  WaterHeaterSizing::WaterHeaterSizing(const WaterToWaterComponent& waterHeater)
    : ModelObject(WaterHeaterSizing::iddObjectType(), waterHeater.model()) {
    OS_ASSERT(getImpl<detail::WaterHeaterSizing_Impl>());

    bool ok = setWaterHeater(waterHeater);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s WaterHeater " << waterHeater.briefDescription() << ".");
    }

    // Defaults from 5ZoneWaterSystems.idf
    setDesignMode("PeakDraw");
    setTimeStorageCanMeetPeakDraw(0.538503);
    setTimeforTankRecovery(0.0);
    setNominalTankVolumeforAutosizingPlantConnections(1.0);
  }

  IddObjectType WaterHeaterSizing::iddObjectType() {
    return {IddObjectType::OS_WaterHeater_Sizing};
  }

  std::vector<std::string> WaterHeaterSizing::designModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_SizingFields::DesignMode);
  }

  WaterToWaterComponent WaterHeaterSizing::waterHeater() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->waterHeater();
  }

  boost::optional<std::string> WaterHeaterSizing::designMode() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->designMode();
  }

  boost::optional<double> WaterHeaterSizing::timeStorageCanMeetPeakDraw() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->timeStorageCanMeetPeakDraw();
  }

  boost::optional<double> WaterHeaterSizing::timeforTankRecovery() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->timeforTankRecovery();
  }

  boost::optional<double> WaterHeaterSizing::nominalTankVolumeforAutosizingPlantConnections() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->nominalTankVolumeforAutosizingPlantConnections();
  }

  boost::optional<int> WaterHeaterSizing::numberofBedrooms() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->numberofBedrooms();
  }

  boost::optional<int> WaterHeaterSizing::numberofBathrooms() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->numberofBathrooms();
  }

  boost::optional<double> WaterHeaterSizing::storageCapacityperPerson() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->storageCapacityperPerson();
  }

  boost::optional<double> WaterHeaterSizing::recoveryCapacityperPerson() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->recoveryCapacityperPerson();
  }

  boost::optional<double> WaterHeaterSizing::storageCapacityperFloorArea() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->storageCapacityperFloorArea();
  }

  boost::optional<double> WaterHeaterSizing::recoveryCapacityperFloorArea() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->recoveryCapacityperFloorArea();
  }

  boost::optional<double> WaterHeaterSizing::numberofUnits() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->numberofUnits();
  }

  boost::optional<double> WaterHeaterSizing::storageCapacityperUnit() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->storageCapacityperUnit();
  }

  boost::optional<double> WaterHeaterSizing::recoveryCapacityPerUnit() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->recoveryCapacityPerUnit();
  }

  boost::optional<double> WaterHeaterSizing::storageCapacityperCollectorArea() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->storageCapacityperCollectorArea();
  }

  boost::optional<double> WaterHeaterSizing::heightAspectRatio() const {
    return getImpl<detail::WaterHeaterSizing_Impl>()->heightAspectRatio();
  }

  bool WaterHeaterSizing::setWaterHeater(const WaterToWaterComponent& waterHeater) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setWaterHeater(waterHeater);
  }

  bool WaterHeaterSizing::setDesignMode(const std::string& designMode) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setDesignMode(designMode);
  }

  void WaterHeaterSizing::resetDesignMode() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetDesignMode();
  }

  bool WaterHeaterSizing::setTimeStorageCanMeetPeakDraw(double timeStorageCanMeetPeakDraw) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setTimeStorageCanMeetPeakDraw(timeStorageCanMeetPeakDraw);
  }

  void WaterHeaterSizing::resetTimeStorageCanMeetPeakDraw() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetTimeStorageCanMeetPeakDraw();
  }

  bool WaterHeaterSizing::setTimeforTankRecovery(double timeforTankRecovery) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setTimeforTankRecovery(timeforTankRecovery);
  }

  void WaterHeaterSizing::resetTimeforTankRecovery() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetTimeforTankRecovery();
  }

  bool WaterHeaterSizing::setNominalTankVolumeforAutosizingPlantConnections(double nominalTankVolumeforAutosizingPlantConnections) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setNominalTankVolumeforAutosizingPlantConnections(
      nominalTankVolumeforAutosizingPlantConnections);
  }

  void WaterHeaterSizing::resetNominalTankVolumeforAutosizingPlantConnections() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetNominalTankVolumeforAutosizingPlantConnections();
  }

  bool WaterHeaterSizing::setNumberofBedrooms(int numberofBedrooms) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setNumberofBedrooms(numberofBedrooms);
  }

  void WaterHeaterSizing::resetNumberofBedrooms() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetNumberofBedrooms();
  }

  bool WaterHeaterSizing::setNumberofBathrooms(int numberofBathrooms) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setNumberofBathrooms(numberofBathrooms);
  }

  void WaterHeaterSizing::resetNumberofBathrooms() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetNumberofBathrooms();
  }

  bool WaterHeaterSizing::setStorageCapacityperPerson(double storageCapacityperPerson) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setStorageCapacityperPerson(storageCapacityperPerson);
  }

  void WaterHeaterSizing::resetStorageCapacityperPerson() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetStorageCapacityperPerson();
  }

  bool WaterHeaterSizing::setRecoveryCapacityperPerson(double recoveryCapacityperPerson) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setRecoveryCapacityperPerson(recoveryCapacityperPerson);
  }

  void WaterHeaterSizing::resetRecoveryCapacityperPerson() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetRecoveryCapacityperPerson();
  }

  bool WaterHeaterSizing::setStorageCapacityperFloorArea(double storageCapacityperFloorArea) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setStorageCapacityperFloorArea(storageCapacityperFloorArea);
  }

  void WaterHeaterSizing::resetStorageCapacityperFloorArea() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetStorageCapacityperFloorArea();
  }

  bool WaterHeaterSizing::setRecoveryCapacityperFloorArea(double recoveryCapacityperFloorArea) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setRecoveryCapacityperFloorArea(recoveryCapacityperFloorArea);
  }

  void WaterHeaterSizing::resetRecoveryCapacityperFloorArea() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetRecoveryCapacityperFloorArea();
  }

  bool WaterHeaterSizing::setNumberofUnits(double numberofUnits) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setNumberofUnits(numberofUnits);
  }

  void WaterHeaterSizing::resetNumberofUnits() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetNumberofUnits();
  }

  bool WaterHeaterSizing::setStorageCapacityperUnit(double storageCapacityperUnit) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setStorageCapacityperUnit(storageCapacityperUnit);
  }

  void WaterHeaterSizing::resetStorageCapacityperUnit() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetStorageCapacityperUnit();
  }

  bool WaterHeaterSizing::setRecoveryCapacityPerUnit(double recoveryCapacityPerUnit) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setRecoveryCapacityPerUnit(recoveryCapacityPerUnit);
  }

  void WaterHeaterSizing::resetRecoveryCapacityPerUnit() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetRecoveryCapacityPerUnit();
  }

  bool WaterHeaterSizing::setStorageCapacityperCollectorArea(double storageCapacityperCollectorArea) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setStorageCapacityperCollectorArea(storageCapacityperCollectorArea);
  }

  void WaterHeaterSizing::resetStorageCapacityperCollectorArea() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetStorageCapacityperCollectorArea();
  }

  bool WaterHeaterSizing::setHeightAspectRatio(double heightAspectRatio) {
    return getImpl<detail::WaterHeaterSizing_Impl>()->setHeightAspectRatio(heightAspectRatio);
  }

  void WaterHeaterSizing::resetHeightAspectRatio() {
    getImpl<detail::WaterHeaterSizing_Impl>()->resetHeightAspectRatio();
  }

  /// @cond
  WaterHeaterSizing::WaterHeaterSizing(std::shared_ptr<detail::WaterHeaterSizing_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
