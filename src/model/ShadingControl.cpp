/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ShadingControl.hpp"
#include "ShadingControl_Impl.hpp"
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "ShadingMaterial.hpp"
#include "ShadingMaterial_Impl.hpp"
#include "Blind.hpp"
#include "Blind_Impl.hpp"
#include "DaylightRedirectionDevice.hpp"
#include "DaylightRedirectionDevice_Impl.hpp"
#include "Screen.hpp"
#include "Screen_Impl.hpp"
#include "Shade.hpp"
#include "Shade_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ShadingControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>
#include <array>
#include <string_view>

namespace openstudio {
namespace model {

  namespace detail {

    ShadingControl_Impl::ShadingControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ShadingControl::iddObjectType());
    }

    ShadingControl_Impl::ShadingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ShadingControl::iddObjectType());
    }

    ShadingControl_Impl::ShadingControl_Impl(const ShadingControl_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ShadingControl_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    bool ShadingControl_Impl::isControlTypeValueNeedingSetpoint1(const std::string& controlType) {
      static constexpr std::array data{
        //"AlwaysOn",
        //"AlwaysOff",
        //"OnIfScheduleAllows",
        "OnIfHighSolarOnWindow", "OnIfHighHorizontalSolar", "OnIfHighOutdoorAirTemperature", "OnIfHighZoneAirTemperature", "OnIfHighZoneCooling",
        //"OnIfHighGlare",
        //"MeetDaylightIlluminanceSetpoint",
        "OnNightIfLowOutdoorTempAndOffDay", "OnNightIfLowInsideTempAndOffDay", "OnNightIfHeatingAndOffDay",
        "OnNightIfLowOutdoorTempAndOnDayIfCooling", "OnNightIfHeatingAndOnDayIfCooling", "OffNightAndOnDayIfCoolingAndHighSolarOnWindow",
        "OnNightAndOnDayIfCoolingAndHighSolarOnWindow", "OnIfHighOutdoorAirTempAndHighSolarOnWindow", "OnIfHighOutdoorAirTempAndHighHorizontalSolar",
        "OnIfHighZoneAirTempAndHighSolarOnWindow", "OnIfHighZoneAirTempAndHighHorizontalSolar"};
      return std::find_if(data.begin(), data.end(), [&controlType](auto c) { return openstudio::istringEqual(controlType, c); }) != data.end();
    }

    bool ShadingControl_Impl::isControlTypeValueNeedingSetpoint2(const std::string& controlType) {
      static constexpr std::array data{"OnIfHighOutdoorAirTempAndHighSolarOnWindow", "OnIfHighOutdoorAirTempAndHighHorizontalSolar",
                                       "OnIfHighZoneAirTempAndHighSolarOnWindow", "OnIfHighZoneAirTempAndHighHorizontalSolar"};
      return std::find_if(data.begin(), data.end(), [&controlType](auto c) { return openstudio::istringEqual(controlType, c); }) != data.end();
    }

    bool ShadingControl_Impl::isControlTypeValueAllowingSchedule(const std::string& controlType) {
      static constexpr std::array data{//"AlwaysOn",
                                       //"AlwaysOff",
                                       "OnIfScheduleAllows", "OnIfHighSolarOnWindow", "OnIfHighHorizontalSolar", "OnIfHighOutdoorAirTemperature",
                                       "OnIfHighZoneAirTemperature", "OnIfHighZoneCooling",
                                       //"OnIfHighGlare",
                                       //"MeetDaylightIlluminanceSetpoint",
                                       "OnNightIfLowOutdoorTempAndOffDay", "OnNightIfLowInsideTempAndOffDay", "OnNightIfHeatingAndOffDay",
                                       "OnNightIfLowOutdoorTempAndOnDayIfCooling", "OnNightIfHeatingAndOnDayIfCooling",
                                       "OffNightAndOnDayIfCoolingAndHighSolarOnWindow", "OnNightAndOnDayIfCoolingAndHighSolarOnWindow",
                                       "OnIfHighOutdoorAirTempAndHighSolarOnWindow", "OnIfHighOutdoorAirTempAndHighHorizontalSolar",
                                       "OnIfHighZoneAirTempAndHighSolarOnWindow", "OnIfHighZoneAirTempAndHighHorizontalSolar"};
      return std::find_if(data.begin(), data.end(), [&controlType](auto c) { return openstudio::istringEqual(controlType, c); }) != data.end();
    }

    bool ShadingControl_Impl::isControlTypeValueRequiringSchedule(const std::string& controlType) {
      return openstudio::istringEqual("OnIfScheduleAllows", controlType);
    }

    bool ShadingControl_Impl::isTypeValueAllowingSlatAngleControl(const std::string& type) {
      static constexpr std::array data{
        //"InteriorShade",
        //"ExteriorShade",
        //"ExteriorScreen",
        "InteriorBlind", "ExteriorBlind",
        //"BetweenGlassShade",
        "BetweenGlassBlind",
        //"SwitchableGlazing",
        //"InteriorDaylightRedirectionDevice"
      };
      return std::find_if(data.begin(), data.end(), [&type](auto c) { return openstudio::istringEqual(type, c); }) != data.end();
    }

    IddObjectType ShadingControl_Impl::iddObjectType() const {
      return ShadingControl::iddObjectType();
    }

    ModelObject ShadingControl_Impl::clone(Model model) const {
      auto scClone = ResourceObject_Impl::clone(model).cast<ShadingControl>();

      if (model != this->model()) {
        scClone.removeAllSubSurfaces();
      }

      return std::move(scClone);
    }

    std::string ShadingControl_Impl::shadingType() const {
      boost::optional<std::string> result = getString(OS_ShadingControlFields::ShadingType, true);
      OS_ASSERT(result);
      return result.get();
    }

    boost::optional<Construction> ShadingControl_Impl::construction() const {
      return getObject<ShadingControl>().getModelObjectTarget<Construction>(OS_ShadingControlFields::ConstructionwithShadingName);
    }

    boost::optional<ShadingMaterial> ShadingControl_Impl::shadingMaterial() const {
      return getObject<ShadingControl>().getModelObjectTarget<ShadingMaterial>(OS_ShadingControlFields::ShadingDeviceMaterialName);
    }

    std::string ShadingControl_Impl::shadingControlType() const {
      boost::optional<std::string> result = getString(OS_ShadingControlFields::ShadingControlType, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool ShadingControl_Impl::isShadingControlTypeDefaulted() const {
      return isEmpty(OS_ShadingControlFields::ShadingControlType);
    }

    boost::optional<Schedule> ShadingControl_Impl::schedule() const {
      return getObject<ShadingControl>().getModelObjectTarget<Schedule>(OS_ShadingControlFields::ScheduleName);
    }

    boost::optional<double> ShadingControl_Impl::setpoint() const {
      boost::optional<double> result = getDouble(OS_ShadingControlFields::Setpoint);
      if (!result) {
        std::string shadingControlType = this->shadingControlType();
        if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)) {
          result = 27;  // w/m^2
        }
      }
      return result;
    }

    bool ShadingControl_Impl::isSetpointDefaulted() const {
      return isEmpty(OS_ShadingControlFields::Setpoint);
    }

    bool ShadingControl_Impl::glareControlIsActive() const {
      boost::optional<std::string> value = getString(OS_ShadingControlFields::GlareControlIsActive, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    std::string ShadingControl_Impl::typeofSlatAngleControlforBlinds() const {
      boost::optional<std::string> result = getString(OS_ShadingControlFields::TypeofSlatAngleControlforBlinds, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool ShadingControl_Impl::isTypeofSlatAngleControlforBlindsDefaulted() const {
      return isEmpty(OS_ShadingControlFields::TypeofSlatAngleControlforBlinds);
    }

    boost::optional<Schedule> ShadingControl_Impl::slatAngleSchedule() const {
      return getObject<ShadingControl>().getModelObjectTarget<Schedule>(OS_ShadingControlFields::SlatAngleScheduleName);
    }

    boost::optional<double> ShadingControl_Impl::setpoint2() const {
      boost::optional<double> result = getDouble(OS_ShadingControlFields::Setpoint2);
      if (!result) {
        std::string shadingControlType = this->shadingControlType();
        if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)) {
          result = 27;  // w/m^2
        }
      }
      return result;
    }

    std::string ShadingControl_Impl::multipleSurfaceControlType() const {
      boost::optional<std::string> result = getString(OS_ShadingControlFields::MultipleSurfaceControlType, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool ShadingControl_Impl::setShadingType(const std::string& shadingType) {
      return setString(OS_ShadingControlFields::ShadingType, shadingType);
    }

    bool ShadingControl_Impl::setShadingControlType(const std::string& shadingControlType) {
      const bool result = setString(OS_ShadingControlFields::ShadingControlType, shadingControlType);
      if (result) {
        if (!ShadingControl_Impl::isControlTypeValueNeedingSetpoint1(shadingControlType)) {
          // Not calling resetSetpoint to avoid double check on whether it's required
          LOG(Info, briefDescription() << " Shading Control Type was changed to '" << shadingControlType
                                       << " which does not require a Setpoint, resetting it.");
          const bool test = setString(OS_ShadingControlFields::Setpoint, "");
          OS_ASSERT(test);
        }
        if (!ShadingControl_Impl::isControlTypeValueNeedingSetpoint2(shadingControlType)) {
          // Not calling resetSetpoint2 to avoid double check on whether it's required
          LOG(Info, briefDescription() << " Shading Control Type was changed to '" << shadingControlType
                                       << " which does not require a Setpoint2, resetting it.");
          const bool test = setString(OS_ShadingControlFields::Setpoint2, "");
          OS_ASSERT(test);
        }
        if (!ShadingControl_Impl::isControlTypeValueAllowingSchedule(shadingControlType)) {
          LOG(Info, briefDescription() << " Shading Control Type was changed to '" << shadingControlType
                                       << " which does not allow a Schedule, resetting it.");
          bool test = setString(OS_ShadingControlFields::ScheduleName, "");
          OS_ASSERT(test);
          test = setString(OS_ShadingControlFields::ShadingControlIsScheduled, "No");
          OS_ASSERT(test);
        }
      }
      return result;
    }

    void ShadingControl_Impl::resetShadingControlType() {
      std::string oldControlType = this->shadingControlType();
      bool test = setString(OS_ShadingControlFields::ShadingControlType, "");
      OS_ASSERT(test);
      if (!openstudio::istringEqual("OnIfHighSolarOnWindow", oldControlType)) {
        resetSetpoint();
      }
    }

    bool ShadingControl_Impl::setSchedule(const Schedule& schedule) {
      bool result = false;
      std::string shadingControlType = this->shadingControlType();
      if (ShadingControl_Impl::isControlTypeValueAllowingSchedule(shadingControlType)) {
        result = setPointer(OS_ShadingControlFields::ScheduleName, schedule.handle());
        if (result) {
          bool test = setString(OS_ShadingControlFields::ShadingControlIsScheduled, "Yes");
          OS_ASSERT(test);
        }
      } else {
        LOG(Warn, briefDescription() << " has a Shading Control Type '" << shadingControlType << "' which does not allow a Schedule");
      }
      return result;
    }

    void ShadingControl_Impl::resetSchedule() {
      std::string shadingControlType = this->shadingControlType();
      if (ShadingControl_Impl::isControlTypeValueRequiringSchedule(shadingControlType)) {
        LOG(Warn, briefDescription() << " has a Shading Control Type '" << shadingControlType << "' which does require a Schedule, not resetting it");
      } else {
        bool test = setString(OS_ShadingControlFields::ScheduleName, "");
        OS_ASSERT(test);

        test = setString(OS_ShadingControlFields::ShadingControlIsScheduled, "No");
        OS_ASSERT(test);
      }
    }

    bool ShadingControl_Impl::setSetpoint(double setpoint) {
      bool result = false;
      std::string shadingControlType = this->shadingControlType();
      if (ShadingControl_Impl::isControlTypeValueNeedingSetpoint1(shadingControlType)) {
        result = setDouble(OS_ShadingControlFields::Setpoint, setpoint);
      } else {
        LOG(Warn, briefDescription() << " has a Shading Control Type '" << shadingControlType << "' which does not require a Setpoint");
      }
      return result;
    }

    void ShadingControl_Impl::resetSetpoint() {
      std::string shadingControlType = this->shadingControlType();
      if (ShadingControl_Impl::isControlTypeValueNeedingSetpoint1(shadingControlType)
          && !openstudio::istringEqual("OnIfHighSolarOnWindow", shadingControlType)) {
        LOG(Warn, briefDescription() << " has a Shading Control Type '" << shadingControlType << "' which does require a Setpoint, not resetting it");
      } else {
        bool test = setString(OS_ShadingControlFields::Setpoint, "");
        OS_ASSERT(test);
      }
    }

    bool ShadingControl_Impl::setGlareControlIsActive(bool glareControlIsActive) {
      bool result = false;
      if (glareControlIsActive) {
        result = setString(OS_ShadingControlFields::GlareControlIsActive, "Yes");
      } else {
        result = setString(OS_ShadingControlFields::GlareControlIsActive, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void ShadingControl_Impl::resetGlareControlIsActive() {
      bool result = setString(OS_ShadingControlFields::GlareControlIsActive, "");
      OS_ASSERT(result);
    }

    bool ShadingControl_Impl::setTypeofSlatAngleControlforBlinds(const std::string& typeofSlatAngleControlforBlinds) {
      bool result = false;
      std::string shadingType = this->shadingType();
      if (ShadingControl_Impl::isTypeValueAllowingSlatAngleControl(shadingType)) {
        result = setString(OS_ShadingControlFields::TypeofSlatAngleControlforBlinds, typeofSlatAngleControlforBlinds);
      }
      return result;
    }

    void ShadingControl_Impl::resetTypeofSlatAngleControlforBlinds() {
      bool result = setString(OS_ShadingControlFields::TypeofSlatAngleControlforBlinds, "");
      OS_ASSERT(result);
    }

    bool ShadingControl_Impl::setSlatAngleSchedule(const Schedule& slatAngleSchedule) {
      bool result = false;
      std::string shadingType = this->shadingType();
      if (ShadingControl_Impl::isTypeValueAllowingSlatAngleControl(shadingType)) {
        result = setPointer(OS_ShadingControlFields::SlatAngleScheduleName, slatAngleSchedule.handle());
        if (result) {
          bool test = setString(OS_ShadingControlFields::TypeofSlatAngleControlforBlinds, "ScheduledSlatAngle");
          OS_ASSERT(test);
        }
      } else {
        LOG(Warn, briefDescription() << " has a Shading Type '" << shadingType << "' which does not allow a Slat Angle Schedule");
      }
      return result;
    }

    void ShadingControl_Impl::resetSlatAngleSchedule() {
      bool result = false;

      result = setString(OS_ShadingControlFields::SlatAngleScheduleName, "");
      OS_ASSERT(result);

      result = setString(OS_ShadingControlFields::TypeofSlatAngleControlforBlinds, "");
      OS_ASSERT(result);
    }

    bool ShadingControl_Impl::setSetpoint2(double setpoint2) {
      bool result = false;
      std::string shadingControlType = this->shadingControlType();
      if (ShadingControl_Impl::isControlTypeValueNeedingSetpoint2(shadingControlType)) {
        result = setDouble(OS_ShadingControlFields::Setpoint2, setpoint2);
      } else {
        LOG(Warn, briefDescription() << " has a Shading Control Type '" << shadingControlType << "' which does not require a Setpoint2");
      }
      return result;
    }

    void ShadingControl_Impl::resetSetpoint2() {
      std::string shadingControlType = this->shadingControlType();
      if (ShadingControl_Impl::isControlTypeValueNeedingSetpoint2(shadingControlType)) {
        LOG(Warn,
            briefDescription() << " has a Shading Control Type '" << shadingControlType << "' which does require a Setpoint2, not resetting it");
      } else {
        bool test = setString(OS_ShadingControlFields::Setpoint2, "");
        OS_ASSERT(test);
      }
    }

    bool ShadingControl_Impl::setMultipleSurfaceControlType(const std::string& multipleSurfaceControlType) {
      return setString(OS_ShadingControlFields::MultipleSurfaceControlType, multipleSurfaceControlType);
    }

    unsigned int ShadingControl_Impl::numberofSubSurfaces() const {
      return numExtensibleGroups();
    }

    std::vector<SubSurface> ShadingControl_Impl::subSurfaces() const {
      std::vector<SubSurface> result;
      auto groups = extensibleGroups();
      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ShadingControlExtensibleFields::SubSurfaceName);
        if (wo) {
          if (auto subsurface = wo->optionalCast<SubSurface>()) {
            result.push_back(subsurface.get());
          }
        }
      }
      return result;
    }

    boost::optional<unsigned> ShadingControl_Impl::subSurfaceIndex(const SubSurface& subSurface) const {

      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto h = openstudio::toString(subSurface.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_ShadingControlExtensibleFields::SubSurfaceName).get() == h);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it) + 1;
      }

      return result;
    }

    bool ShadingControl_Impl::addSubSurface(const SubSurface& subSurface) {
      // Check if subSurface already exists
      boost::optional<unsigned> _existingIndex = subSurfaceIndex(subSurface);
      if (_existingIndex) {
        LOG(Warn, "For " << briefDescription() << ", SubSurface already exists.");
        return true;
      }

      bool result;

      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool subsurface = eg.setPointer(OS_ShadingControlExtensibleFields::SubSurfaceName, subSurface.handle());
      if (subsurface) {
        result = true;
      } else {
        // Something went wrong
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      return result;
    }

    bool ShadingControl_Impl::setSubSurfaceIndex(const SubSurface& subSurface, unsigned index) {
      boost::optional<unsigned> idx = subSurfaceIndex(subSurface);
      if (!idx) {
        LOG(Warn,
            "For " << briefDescription() << " cannot set the index of SubSurface " << subSurface.nameString() << " since it is not part of it.");
        return false;
      }

      // TODO: we could just set via string instead of doing a ton of typechecking below...
      std::vector<SubSurface> subSurfaceVector = subSurfaces();

      if (index > subSurfaceVector.size()) {
        LOG(Warn, "Requested a subSurface index of " << index << " to be assigned to " << subSurface.nameString() << ", but " << briefDescription()
                                                     << " only has " << subSurfaceVector.size() << " SubSurfaces, resetting to that.");
        index = subSurfaceVector.size();
      } else if (index < 1) {
        LOG(Warn, "Requested a subSurface index of " << index << " < 1 to be assigned to " << subSurface.briefDescription() << ", resetting to 1");
        index = 1;
      }

      subSurfaceVector.erase(subSurfaceVector.begin() + idx.get() - 1);  // subSurfaceIndex is 1-indexed, and vector is 0-indexed

      subSurfaceVector.insert(subSurfaceVector.begin() + (index - 1), subSurface);

      return setSubSurfaces(subSurfaceVector);
    }

    bool ShadingControl_Impl::addSubSurface(const SubSurface& subSurface, unsigned index) {
      bool ok = addSubSurface(subSurface);
      if (!ok) {
        return false;
      }
      ok = setSubSurfaceIndex(subSurface, index);
      return ok;
    }

    bool ShadingControl_Impl::removeSubSurface(const SubSurface& subSurface) {
      boost::optional<unsigned> idx = subSurfaceIndex(subSurface);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot remove SubSurface " << subSurface.nameString() << " since it is not part of it.");
        return false;
      }

      return removeSubSurface(idx.get());
    }

    bool ShadingControl_Impl::removeSubSurface(unsigned index) {
      bool result = false;
      if ((index > 0) && (index <= numberofSubSurfaces())) {
        getObject<ModelObject>().eraseExtensibleGroup(index - 1);
        result = true;
      }
      return result;
    }

    bool ShadingControl_Impl::addSubSurfaces(const std::vector<SubSurface>& subSurfaces) {
      bool ok = true;
      for (const SubSurface& subSurface : subSurfaces) {
        ok &= addSubSurface(subSurface);
      }
      return ok;
    }

    void ShadingControl_Impl::removeAllSubSurfaces() {
      clearExtensibleGroups();
    }

    bool ShadingControl_Impl::setSubSurfaces(const std::vector<SubSurface>& subSurfaces) {
      removeAllSubSurfaces();
      return addSubSurfaces(subSurfaces);
    }

  }  // namespace detail

  ShadingControl::ShadingControl(const Construction& construction) : ResourceObject(ShadingControl::iddObjectType(), construction.model()) {
    OS_ASSERT(getImpl<detail::ShadingControl_Impl>());

    std::vector<Material> layers = construction.layers();

    std::string type;

    // check layers from inside out
    int i = layers.size() - 1;
    for (; i >= 0; --i) {
      if (layers[i].optionalCast<Shade>()) {
        type = "Shade";
        LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
        break;
      } else if (layers[i].optionalCast<Blind>()) {
        type = "Blind";
        LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
        break;
      } else if (layers[i].optionalCast<Screen>()) {
        type = "Screen";
        setShadingControlType("AlwaysOn");
        LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
        break;
      } else if (layers[i].optionalCast<DaylightRedirectionDevice>()) {
        type = "DaylightRedirectionDevice";
        setShadingControlType("AlwaysOn");
        LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
        break;
      }
    }

    std::string position;
    if (i == int(layers.size()) - 1) {
      position = "Interior";
    } else if (i == 0) {
      position = "Exterior";
    } else {
      position = "BetweenGlass";
    }

    if (type.empty()) {
      type = "SwitchableGlazing";
      position = "";
    }

    if (type == "Screen" && position != "Exterior") {
      this->remove();
      LOG_AND_THROW(position << type << " is not an allowable configuration for ShadingControl");
    }

    if (type == "DaylightRedirectionDevice" && position != "Interior") {
      this->remove();
      LOG_AND_THROW(position << type << " is not an allowable configuration for ShadingControl");
    }

    bool test = this->setShadingType(position + type);
    OS_ASSERT(test);

    test = setMultipleSurfaceControlType("Sequential");  // E+ IDD Default
    OS_ASSERT(test);

    test = this->setPointer(OS_ShadingControlFields::ConstructionwithShadingName, construction.handle());
    OS_ASSERT(test);
  }

  ShadingControl::ShadingControl(const ShadingMaterial& shadingMaterial) : ResourceObject(ShadingControl::iddObjectType(), shadingMaterial.model()) {
    OS_ASSERT(getImpl<detail::ShadingControl_Impl>());

    std::string type;
    if (shadingMaterial.optionalCast<Shade>()) {
      type = "InteriorShade";
    } else if (shadingMaterial.optionalCast<Blind>()) {
      type = "InteriorBlind";
    } else if (shadingMaterial.optionalCast<Screen>()) {
      type = "ExteriorScreen";
      setShadingControlType("AlwaysOn");
    } else if (shadingMaterial.optionalCast<DaylightRedirectionDevice>()) {
      type = "InteriorDaylightRedirectionDevice";
      setShadingControlType("AlwaysOn");
    }

    bool test = this->setShadingType(type);
    OS_ASSERT(test);

    test = setMultipleSurfaceControlType("Sequential");  // E+ IDD Default
    OS_ASSERT(test);

    test = this->setPointer(OS_ShadingControlFields::ShadingDeviceMaterialName, shadingMaterial.handle());
    OS_ASSERT(test);
  }

  IddObjectType ShadingControl::iddObjectType() {
    return {IddObjectType::OS_ShadingControl};
  }

  std::vector<std::string> ShadingControl::shadingTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadingControlFields::ShadingType);
  }

  std::vector<std::string> ShadingControl::shadingControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadingControlFields::ShadingControlType);
  }

  std::vector<std::string> ShadingControl::typeofSlatAngleControlforBlindsValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadingControlFields::TypeofSlatAngleControlforBlinds);
  }

  std::vector<std::string> ShadingControl::multipleSurfaceControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadingControlFields::MultipleSurfaceControlType);
  }

  bool ShadingControl::isControlTypeValueNeedingSetpoint1() {
    return getImpl<detail::ShadingControl_Impl>()->isControlTypeValueNeedingSetpoint1(this->shadingControlType());
  }

  bool ShadingControl::isControlTypeValueNeedingSetpoint2() {
    return getImpl<detail::ShadingControl_Impl>()->isControlTypeValueNeedingSetpoint2(this->shadingControlType());
  }

  bool ShadingControl::isControlTypeValueAllowingSchedule() {
    return getImpl<detail::ShadingControl_Impl>()->isControlTypeValueAllowingSchedule(this->shadingControlType());
  }

  bool ShadingControl::isControlTypeValueRequiringSchedule() {
    return getImpl<detail::ShadingControl_Impl>()->isControlTypeValueRequiringSchedule(this->shadingControlType());
  }

  bool ShadingControl::isTypeValueAllowingSlatAngleControl() {
    return getImpl<detail::ShadingControl_Impl>()->isTypeValueAllowingSlatAngleControl(this->shadingType());
  }

  std::string ShadingControl::shadingType() const {
    return getImpl<detail::ShadingControl_Impl>()->shadingType();
  }

  boost::optional<Construction> ShadingControl::construction() const {
    return getImpl<detail::ShadingControl_Impl>()->construction();
  }

  boost::optional<ShadingMaterial> ShadingControl::shadingMaterial() const {
    return getImpl<detail::ShadingControl_Impl>()->shadingMaterial();
  }

  std::string ShadingControl::shadingControlType() const {
    return getImpl<detail::ShadingControl_Impl>()->shadingControlType();
  }

  bool ShadingControl::isShadingControlTypeDefaulted() const {
    return getImpl<detail::ShadingControl_Impl>()->isShadingControlTypeDefaulted();
  }

  boost::optional<Schedule> ShadingControl::schedule() const {
    return getImpl<detail::ShadingControl_Impl>()->schedule();
  }

  boost::optional<double> ShadingControl::setpoint() const {
    return getImpl<detail::ShadingControl_Impl>()->setpoint();
  }

  bool ShadingControl::isSetpointDefaulted() const {
    return getImpl<detail::ShadingControl_Impl>()->isSetpointDefaulted();
  }

  bool ShadingControl::glareControlIsActive() const {
    return getImpl<detail::ShadingControl_Impl>()->glareControlIsActive();
  }

  std::string ShadingControl::typeofSlatAngleControlforBlinds() const {
    return getImpl<detail::ShadingControl_Impl>()->typeofSlatAngleControlforBlinds();
  }

  bool ShadingControl::isTypeofSlatAngleControlforBlindsDefaulted() const {
    return getImpl<detail::ShadingControl_Impl>()->isTypeofSlatAngleControlforBlindsDefaulted();
  }

  boost::optional<Schedule> ShadingControl::slatAngleSchedule() const {
    return getImpl<detail::ShadingControl_Impl>()->slatAngleSchedule();
  }

  boost::optional<double> ShadingControl::setpoint2() const {
    return getImpl<detail::ShadingControl_Impl>()->setpoint2();
  }

  std::string ShadingControl::multipleSurfaceControlType() const {
    return getImpl<detail::ShadingControl_Impl>()->multipleSurfaceControlType();
  }

  bool ShadingControl::setShadingType(const std::string& shadingType) {
    return getImpl<detail::ShadingControl_Impl>()->setShadingType(shadingType);
  }

  bool ShadingControl::setShadingControlType(const std::string& shadingControlType) {
    return getImpl<detail::ShadingControl_Impl>()->setShadingControlType(shadingControlType);
  }

  void ShadingControl::resetShadingControlType() {
    return getImpl<detail::ShadingControl_Impl>()->resetShadingControlType();
  }

  bool ShadingControl::setSchedule(const Schedule& schedule) {
    return getImpl<detail::ShadingControl_Impl>()->setSchedule(schedule);
  }

  void ShadingControl::resetSchedule() {
    getImpl<detail::ShadingControl_Impl>()->resetSchedule();
  }

  bool ShadingControl::setSetpoint(double setpoint) {
    return getImpl<detail::ShadingControl_Impl>()->setSetpoint(setpoint);
  }

  void ShadingControl::resetSetpoint() {
    getImpl<detail::ShadingControl_Impl>()->resetSetpoint();
  }

  bool ShadingControl::setGlareControlIsActive(bool glareControlIsActive) {
    return getImpl<detail::ShadingControl_Impl>()->setGlareControlIsActive(glareControlIsActive);
  }

  void ShadingControl::resetGlareControlIsActive() {
    getImpl<detail::ShadingControl_Impl>()->resetGlareControlIsActive();
  }

  bool ShadingControl::setTypeofSlatAngleControlforBlinds(const std::string& typeofSlatAngleControlforBlinds) {
    return getImpl<detail::ShadingControl_Impl>()->setTypeofSlatAngleControlforBlinds(typeofSlatAngleControlforBlinds);
  }

  void ShadingControl::resetTypeofSlatAngleControlforBlinds() {
    getImpl<detail::ShadingControl_Impl>()->resetTypeofSlatAngleControlforBlinds();
  }

  bool ShadingControl::setSlatAngleSchedule(const Schedule& slatAngleSchedule) {
    return getImpl<detail::ShadingControl_Impl>()->setSlatAngleSchedule(slatAngleSchedule);
  }

  void ShadingControl::resetSlatAngleSchedule() {
    getImpl<detail::ShadingControl_Impl>()->resetSlatAngleSchedule();
  }

  bool ShadingControl::setSetpoint2(double setpoint2) {
    return getImpl<detail::ShadingControl_Impl>()->setSetpoint2(setpoint2);
  }

  bool ShadingControl::setMultipleSurfaceControlType(const std::string& multipleSurfaceControlType) {
    return getImpl<detail::ShadingControl_Impl>()->setMultipleSurfaceControlType(multipleSurfaceControlType);
  }

  // Extensible Fields

  std::vector<SubSurface> ShadingControl::subSurfaces() const {
    return getImpl<detail::ShadingControl_Impl>()->subSurfaces();
  }

  unsigned int ShadingControl::numberofSubSurfaces() const {
    return getImpl<detail::ShadingControl_Impl>()->numberofSubSurfaces();
  }

  boost::optional<unsigned> ShadingControl::subSurfaceIndex(const SubSurface& subSurface) const {
    return getImpl<detail::ShadingControl_Impl>()->subSurfaceIndex(subSurface);
  }

  bool ShadingControl::addSubSurface(const SubSurface& subSurface) {
    return getImpl<detail::ShadingControl_Impl>()->addSubSurface(subSurface);
  }

  bool ShadingControl::addSubSurface(const SubSurface& subSurface, unsigned index) {
    return getImpl<detail::ShadingControl_Impl>()->addSubSurface(subSurface, index);
  }

  bool ShadingControl::setSubSurfaceIndex(const SubSurface& subSurface, unsigned index) {
    return getImpl<detail::ShadingControl_Impl>()->setSubSurfaceIndex(subSurface, index);
  }

  bool ShadingControl::removeSubSurface(unsigned index) {
    return getImpl<detail::ShadingControl_Impl>()->removeSubSurface(index);
  }

  bool ShadingControl::removeSubSurface(const SubSurface& subSurface) {
    return getImpl<detail::ShadingControl_Impl>()->removeSubSurface(subSurface);
  }

  bool ShadingControl::addSubSurfaces(const std::vector<SubSurface>& subSurfaces) {
    return getImpl<detail::ShadingControl_Impl>()->addSubSurfaces(subSurfaces);
  }

  bool ShadingControl::setSubSurfaces(const std::vector<SubSurface>& subSurfaces) {
    return getImpl<detail::ShadingControl_Impl>()->setSubSurfaces(subSurfaces);
  }

  void ShadingControl::removeAllSubSurfaces() {
    getImpl<detail::ShadingControl_Impl>()->removeAllSubSurfaces();
  }

  /// @cond
  ShadingControl::ShadingControl(std::shared_ptr<detail::ShadingControl_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
