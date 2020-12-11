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

#include "ZoneVentilationWindandStackOpenArea.hpp"
#include "ZoneVentilationWindandStackOpenArea_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "ZoneHVACEquipmentList.hpp"
#include "ZoneHVACEquipmentList_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneVentilation_WindandStackOpenArea_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneVentilationWindandStackOpenArea_Impl::ZoneVentilationWindandStackOpenArea_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneVentilationWindandStackOpenArea::iddObjectType());
    }

    ZoneVentilationWindandStackOpenArea_Impl::ZoneVentilationWindandStackOpenArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneVentilationWindandStackOpenArea::iddObjectType());
    }

    ZoneVentilationWindandStackOpenArea_Impl::ZoneVentilationWindandStackOpenArea_Impl(const ZoneVentilationWindandStackOpenArea_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneVentilationWindandStackOpenArea_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: all variables reported at the zone level
      //result.push_back("Zone Ventilation Sensible Heat Loss Energy");
      //result.push_back("Zone Ventilation Sensible Heat Gain Energy");
      //result.push_back("Zone Ventilation Latent Heat Loss Energy");
      //result.push_back("Zone Ventilation Latent Heat Gain Energy");
      //result.push_back("Zone Ventilation Total Heat Loss Energy");
      //result.push_back("Zone Ventilation Total Heat Gain Energy");
      //result.push_back("Zone Ventilation Current Density Volume Flow Rate");
      //result.push_back("Zone Ventilation Standard Density Volume Flow Rate");
      //result.push_back("Zone Ventilation Current Density Volume");
      //result.push_back("Zone Ventilation Standard Density Volume");
      //result.push_back("Zone Ventilation Mass");
      //result.push_back("Zone Ventilation Mass Flow Rate");
      //result.push_back("Zone Ventilation Air Change Rate");
      //result.push_back("Zone Ventilation Fan Electricity Energy");
      //result.push_back("Zone Ventilation Air Inlet Temperature");
      return result;
    }

    IddObjectType ZoneVentilationWindandStackOpenArea_Impl::iddObjectType() const {
      return ZoneVentilationWindandStackOpenArea::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneVentilationWindandStackOpenArea_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea", "Opening Area Fraction Schedule"));
      }
      if (std::find(b, e, OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea", "Minimum Indoor Temperature"));
      }
      if (std::find(b, e, OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea", "Maximum Indoor Temperature"));
      }
      if (std::find(b, e, OS_ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea", "Delta Temperature"));
      }
      if (std::find(b, e, OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea", "Minimum Outdoor Temperature"));
      }
      if (std::find(b, e, OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea", "Maximum Outdoor Temperature"));
      }
      return result;
    }

    boost::optional<Schedule> ZoneVentilationWindandStackOpenArea_Impl::optionalOpeningAreaFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName);
    }

    Schedule ZoneVentilationWindandStackOpenArea_Impl::openingAreaFractionSchedule() const {
      boost::optional<Schedule> value = optionalOpeningAreaFractionSchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<ZoneVentilationWindandStackOpenArea_Impl*>(this)->setOpeningAreaFractionSchedule(*value);
        value = optionalOpeningAreaFractionSchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setOpeningAreaFractionSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName,
                                                  "ZoneVentilationWindandStackOpenArea", "Opening Area Fraction Schedule", schedule);
      return result;
    }

    double ZoneVentilationWindandStackOpenArea_Impl::openingArea() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setOpeningArea(double openingArea) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningArea, openingArea);
      // OS_ASSERT(result);
      return result;
    }

    boost::optional<double> ZoneVentilationWindandStackOpenArea_Impl::openingEffectiveness() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, false);
      return value;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::isOpeningEffectivenessAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setOpeningEffectiveness(double openingEffectiveness) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, openingEffectiveness);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::autocalculateOpeningEffectiveness() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, "autocalculate");
      OS_ASSERT(result);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::effectiveAngle() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setEffectiveAngle(double effectiveAngle) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle, effectiveAngle);
      // OS_ASSERT(result);
      return result;
    }

    double ZoneVentilationWindandStackOpenArea_Impl::heightDifference() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::HeightDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setHeightDifference(double heightDifference) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::HeightDifference, heightDifference);
      // OS_ASSERT(result);
      return result;
    }

    boost::optional<double> ZoneVentilationWindandStackOpenArea_Impl::dischargeCoefficientforOpening() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, false);
      return value;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::isDischargeCoefficientforOpeningAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setDischargeCoefficientforOpening(double dischargeCoefficientforOpening) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, dischargeCoefficientforOpening);
      // OS_ASSERT(result);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::autocalculateDischargeCoefficientforOpening() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, "autocalculate");
      OS_ASSERT(result);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::minimumIndoorTemperature() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ZoneVentilationWindandStackOpenArea_Impl::minimumIndoorTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::maximumIndoorTemperature() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ZoneVentilationWindandStackOpenArea_Impl::maximumIndoorTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::deltaTemperature() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ZoneVentilationWindandStackOpenArea_Impl::deltaTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::minimumOutdoorTemperature() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ZoneVentilationWindandStackOpenArea_Impl::minimumOutdoorTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::maximumOutdoorTemperature() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ZoneVentilationWindandStackOpenArea_Impl::maximumOutdoorTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName);
    }

    double ZoneVentilationWindandStackOpenArea_Impl::maximumWindSpeed() const {
      boost::optional<double> value = getDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumWindSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMinimumIndoorTemperature(double minimumIndoorTemperature) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature, minimumIndoorTemperature);
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMinimumIndoorTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName,
                                                  "ZoneVentilationWindandStackOpenArea", "Minimum Indoor Temperature", schedule);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::resetMinimumIndoorTemperatureSchedule() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMaximumIndoorTemperature(double maximumIndoorTemperature) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperature, maximumIndoorTemperature);
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMaximumIndoorTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName,
                                                  "ZoneVentilationWindandStackOpenArea", "Maximum Indoor Temperature", schedule);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::resetMaximumIndoorTemperatureSchedule() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setDeltaTemperature(double deltaTemperature) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperature, deltaTemperature);
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setDeltaTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName,
                                                  "ZoneVentilationWindandStackOpenArea", "Delta Temperature", schedule);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::resetDeltaTemperatureSchedule() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMinimumOutdoorTemperature(double minimumOutdoorTemperature) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperature, minimumOutdoorTemperature);
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName,
                                                  "ZoneVentilationWindandStackOpenArea", "Minimum Outdoor Temperature", schedule);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::resetMinimumOutdoorTemperatureSchedule() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMaximumOutdoorTemperature(double maximumOutdoorTemperature) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperature, maximumOutdoorTemperature);
      return result;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName,
                                                  "ZoneVentilationWindandStackOpenArea", "Maximum Outdoor Temperature", schedule);
      return result;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::resetMaximumOutdoorTemperatureSchedule() {
      bool result = setString(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::setMaximumWindSpeed(double maximumWindSpeed) {
      bool result = setDouble(OS_ZoneVentilation_WindandStackOpenAreaFields::MaximumWindSpeed, maximumWindSpeed);
      return result;
    }

    unsigned ZoneVentilationWindandStackOpenArea_Impl::inletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    unsigned ZoneVentilationWindandStackOpenArea_Impl::outletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    boost::optional<ThermalZone> ZoneVentilationWindandStackOpenArea_Impl::thermalZone() const {
      ModelObject thisObject = this->getObject<ModelObject>();
      std::vector<ThermalZone> thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
      for (const auto& thermalZone : thermalZones) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          return thermalZone;
        }
      }
      return boost::none;
    }

    bool ZoneVentilationWindandStackOpenArea_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m) {
        return false;
      }

      if (thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    void ZoneVentilationWindandStackOpenArea_Impl::removeFromThermalZone() {
      if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    std::vector<EMSActuatorNames> ZoneVentilationWindandStackOpenArea_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Zone Ventilation", "Air Exchange Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> ZoneVentilationWindandStackOpenArea_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  ZoneVentilationWindandStackOpenArea::ZoneVentilationWindandStackOpenArea(const Model& model)
    : ZoneHVACComponent(ZoneVentilationWindandStackOpenArea::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>());

    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setOpeningAreaFractionSchedule(schedule);
    }

    setOpeningArea(0.0);
    autocalculateOpeningEffectiveness();
    setEffectiveAngle(0.0);
    setHeightDifference(0.0);
    autocalculateDischargeCoefficientforOpening();
    setMinimumIndoorTemperature(-100.0);
    setMaximumIndoorTemperature(100.0);
    setDeltaTemperature(-100.0);
    setMinimumOutdoorTemperature(-100.0);
    setMaximumOutdoorTemperature(100.0);
    setMaximumWindSpeed(40.0);
  }

  IddObjectType ZoneVentilationWindandStackOpenArea::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ZoneVentilation_WindandStackOpenArea);
  }

  double ZoneVentilationWindandStackOpenArea::openingArea() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->openingArea();
  }

  bool ZoneVentilationWindandStackOpenArea::setOpeningArea(double openingArea) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setOpeningArea(openingArea);
  }

  Schedule ZoneVentilationWindandStackOpenArea::openingAreaFractionSchedule() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->openingAreaFractionSchedule();
  }

  bool ZoneVentilationWindandStackOpenArea::setOpeningAreaFractionSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setOpeningAreaFractionSchedule(schedule);
  }
  boost::optional<double> ZoneVentilationWindandStackOpenArea::openingEffectiveness() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->openingEffectiveness();
  }

  bool ZoneVentilationWindandStackOpenArea::isOpeningEffectivenessAutocalculated() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->isOpeningEffectivenessAutocalculated();
  }

  bool ZoneVentilationWindandStackOpenArea::setOpeningEffectiveness(double openingEffectiveness) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setOpeningEffectiveness(openingEffectiveness);
  }

  void ZoneVentilationWindandStackOpenArea::autocalculateOpeningEffectiveness() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->autocalculateOpeningEffectiveness();
  }

  double ZoneVentilationWindandStackOpenArea::effectiveAngle() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->effectiveAngle();
  }

  bool ZoneVentilationWindandStackOpenArea::setEffectiveAngle(double effectiveAngle) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setEffectiveAngle(effectiveAngle);
  }

  double ZoneVentilationWindandStackOpenArea::heightDifference() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->heightDifference();
  }

  bool ZoneVentilationWindandStackOpenArea::setHeightDifference(double heightDifference) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setHeightDifference(heightDifference);
  }

  boost::optional<double> ZoneVentilationWindandStackOpenArea::dischargeCoefficientforOpening() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->dischargeCoefficientforOpening();
  }

  bool ZoneVentilationWindandStackOpenArea::isDischargeCoefficientforOpeningAutocalculated() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->isDischargeCoefficientforOpeningAutocalculated();
  }

  bool ZoneVentilationWindandStackOpenArea::setDischargeCoefficientforOpening(double dischargeCoefficientforOpening) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setDischargeCoefficientforOpening(dischargeCoefficientforOpening);
  }

  void ZoneVentilationWindandStackOpenArea::autocalculateDischargeCoefficientforOpening() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->autocalculateDischargeCoefficientforOpening();
  }

  double ZoneVentilationWindandStackOpenArea::minimumIndoorTemperature() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->minimumIndoorTemperature();
  }

  boost::optional<Schedule> ZoneVentilationWindandStackOpenArea::minimumIndoorTemperatureSchedule() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->minimumIndoorTemperatureSchedule();
  }

  double ZoneVentilationWindandStackOpenArea::maximumIndoorTemperature() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->maximumIndoorTemperature();
  }

  boost::optional<Schedule> ZoneVentilationWindandStackOpenArea::maximumIndoorTemperatureSchedule() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->maximumIndoorTemperatureSchedule();
  }

  double ZoneVentilationWindandStackOpenArea::deltaTemperature() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->deltaTemperature();
  }

  boost::optional<Schedule> ZoneVentilationWindandStackOpenArea::deltaTemperatureSchedule() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->deltaTemperatureSchedule();
  }

  double ZoneVentilationWindandStackOpenArea::minimumOutdoorTemperature() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->minimumOutdoorTemperature();
  }

  boost::optional<Schedule> ZoneVentilationWindandStackOpenArea::minimumOutdoorTemperatureSchedule() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->minimumOutdoorTemperatureSchedule();
  }

  double ZoneVentilationWindandStackOpenArea::maximumOutdoorTemperature() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->maximumOutdoorTemperature();
  }

  boost::optional<Schedule> ZoneVentilationWindandStackOpenArea::maximumOutdoorTemperatureSchedule() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->maximumOutdoorTemperatureSchedule();
  }

  double ZoneVentilationWindandStackOpenArea::maximumWindSpeed() const {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->maximumWindSpeed();
  }

  bool ZoneVentilationWindandStackOpenArea::setMinimumIndoorTemperature(double minimumIndoorTemperature) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMinimumIndoorTemperature(minimumIndoorTemperature);
  }

  bool ZoneVentilationWindandStackOpenArea::setMinimumIndoorTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMinimumIndoorTemperatureSchedule(schedule);
  }

  void ZoneVentilationWindandStackOpenArea::resetMinimumIndoorTemperatureSchedule() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->resetMinimumIndoorTemperatureSchedule();
  }

  bool ZoneVentilationWindandStackOpenArea::setMaximumIndoorTemperature(double maximumIndoorTemperature) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMaximumIndoorTemperature(maximumIndoorTemperature);
  }

  bool ZoneVentilationWindandStackOpenArea::setMaximumIndoorTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMaximumIndoorTemperatureSchedule(schedule);
  }

  void ZoneVentilationWindandStackOpenArea::resetMaximumIndoorTemperatureSchedule() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->resetMaximumIndoorTemperatureSchedule();
  }

  bool ZoneVentilationWindandStackOpenArea::setDeltaTemperature(double deltaTemperature) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setDeltaTemperature(deltaTemperature);
  }

  bool ZoneVentilationWindandStackOpenArea::setDeltaTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setDeltaTemperatureSchedule(schedule);
  }

  void ZoneVentilationWindandStackOpenArea::resetDeltaTemperatureSchedule() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->resetDeltaTemperatureSchedule();
  }

  bool ZoneVentilationWindandStackOpenArea::setMinimumOutdoorTemperature(double minimumOutdoorTemperature) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMinimumOutdoorTemperature(minimumOutdoorTemperature);
  }

  bool ZoneVentilationWindandStackOpenArea::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMinimumOutdoorTemperatureSchedule(schedule);
  }

  void ZoneVentilationWindandStackOpenArea::resetMinimumOutdoorTemperatureSchedule() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->resetMinimumOutdoorTemperatureSchedule();
  }

  bool ZoneVentilationWindandStackOpenArea::setMaximumOutdoorTemperature(double maximumOutdoorTemperature) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMaximumOutdoorTemperature(maximumOutdoorTemperature);
  }

  bool ZoneVentilationWindandStackOpenArea::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMaximumOutdoorTemperatureSchedule(schedule);
  }

  void ZoneVentilationWindandStackOpenArea::resetMaximumOutdoorTemperatureSchedule() {
    getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->resetMaximumOutdoorTemperatureSchedule();
  }

  bool ZoneVentilationWindandStackOpenArea::setMaximumWindSpeed(double maximumWindSpeed) {
    return getImpl<detail::ZoneVentilationWindandStackOpenArea_Impl>()->setMaximumWindSpeed(maximumWindSpeed);
  }

  /// @cond
  ZoneVentilationWindandStackOpenArea::ZoneVentilationWindandStackOpenArea(std::shared_ptr<detail::ZoneVentilationWindandStackOpenArea_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
