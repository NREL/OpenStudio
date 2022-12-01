/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "SpaceVentilationDesignFlowRate.hpp"
#include "SpaceVentilationDesignFlowRate_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SpaceVentilation_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/QuantityConverter.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SpaceVentilationDesignFlowRate_Impl::SpaceVentilationDesignFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SpaceVentilationDesignFlowRate::iddObjectType());
    }

    SpaceVentilationDesignFlowRate_Impl::SpaceVentilationDesignFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SpaceVentilationDesignFlowRate::iddObjectType());
    }

    SpaceVentilationDesignFlowRate_Impl::SpaceVentilationDesignFlowRate_Impl(const SpaceVentilationDesignFlowRate_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : SpaceLoad_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SpaceVentilationDesignFlowRate_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SpaceVentilationDesignFlowRate_Impl::iddObjectType() const {
      return SpaceVentilationDesignFlowRate::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SpaceVentilationDesignFlowRate_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_SpaceVentilation_DesignFlowRateFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("SpaceVentilationDesignFlowRate", "Ventilation"));
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::hardSize() {
      boost::optional<Space> space = this->space();
      if (!space) {
        return false;
      }

      boost::optional<double> designFlowRate = this->designFlowRate();
      if (designFlowRate) {
        return true;
      }

      boost::optional<double> flowperSpaceFloorArea = this->flowperSpaceFloorArea();
      if (flowperSpaceFloorArea) {
        double floorArea = space->floorArea();

        return this->setDesignFlowRate(*flowperSpaceFloorArea * floorArea);
      }

      boost::optional<double> flowperExteriorSurfaceArea = this->flowperExteriorSurfaceArea();
      if (flowperExteriorSurfaceArea) {
        double exteriorArea = space->exteriorArea();

        return this->setDesignFlowRate(*flowperExteriorSurfaceArea * exteriorArea);
      }

      boost::optional<double> flowperExteriorWallArea = this->flowperExteriorWallArea();
      if (flowperExteriorWallArea) {
        double exteriorWallArea = space->exteriorWallArea();

        return this->setDesignFlowRate(*flowperExteriorWallArea * exteriorWallArea);
      }

      boost::optional<double> airChangesperHour = this->airChangesperHour();
      if (airChangesperHour) {
        return this->setDesignFlowRate(*airChangesperHour * space->volume() / 3600.0);
      }

      return false;
    }

    bool SpaceVentilationDesignFlowRate_Impl::hardApplySchedules() {
      bool result = false;
      boost::optional<Schedule> schedule = this->schedule();
      if (schedule) {
        result = this->setSchedule(*schedule);
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::isAbsolute() const {
      if (designFlowRate()) {
        return true;
      }
      return false;
    }

    std::string SpaceVentilationDesignFlowRate_Impl::designFlowRateCalculationMethod() const {
      boost::optional<std::string> value = getString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SpaceVentilationDesignFlowRate_Impl::designFlowRate() const {
      boost::optional<double> result;
      if (istringEqual("Flow/Space", this->designFlowRateCalculationMethod())) {
        result = getDouble(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, true);
      }
      return result;
    }

    boost::optional<double> SpaceVentilationDesignFlowRate_Impl::flowperSpaceFloorArea() const {
      boost::optional<double> result;
      if (istringEqual("Flow/Area", this->designFlowRateCalculationMethod())) {
        result = getDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, true);
      }
      return result;
    }

    boost::optional<double> SpaceVentilationDesignFlowRate_Impl::flowperExteriorSurfaceArea() const {
      boost::optional<double> result;
      if (istringEqual("Flow/ExteriorArea", this->designFlowRateCalculationMethod())) {
        result = getDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, true);
      }
      return result;
    }

    boost::optional<double> SpaceVentilationDesignFlowRate_Impl::flowperExteriorWallArea() const {
      boost::optional<double> result;
      if (istringEqual("Flow/ExteriorWallArea", this->designFlowRateCalculationMethod())) {
        result = getDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, true);
      }
      return result;
    }

    boost::optional<double> SpaceVentilationDesignFlowRate_Impl::airChangesperHour() const {
      boost::optional<double> result;
      if (istringEqual("AirChanges/Hour", this->designFlowRateCalculationMethod())) {
        result = getDouble(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, true);
      }
      return result;
    }

    double SpaceVentilationDesignFlowRate_Impl::constantTermCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceVentilation_DesignFlowRateFields::ConstantTermCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceVentilationDesignFlowRate_Impl::isConstantTermCoefficientDefaulted() const {
      return isEmpty(OS_SpaceVentilation_DesignFlowRateFields::ConstantTermCoefficient);
    }

    double SpaceVentilationDesignFlowRate_Impl::temperatureTermCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceVentilation_DesignFlowRateFields::TemperatureTermCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceVentilationDesignFlowRate_Impl::isTemperatureTermCoefficientDefaulted() const {
      return isEmpty(OS_SpaceVentilation_DesignFlowRateFields::TemperatureTermCoefficient);
    }

    double SpaceVentilationDesignFlowRate_Impl::velocityTermCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceVentilation_DesignFlowRateFields::VelocityTermCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceVentilationDesignFlowRate_Impl::isVelocityTermCoefficientDefaulted() const {
      return isEmpty(OS_SpaceVentilation_DesignFlowRateFields::VelocityTermCoefficient);
    }

    double SpaceVentilationDesignFlowRate_Impl::velocitySquaredTermCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceVentilationDesignFlowRate_Impl::isVelocitySquaredTermCoefficientDefaulted() const {
      return isEmpty(OS_SpaceVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient);
    }

    bool SpaceVentilationDesignFlowRate_Impl::setDesignFlowRate(boost::optional<double> designFlowRate) {
      bool result = true;
      if (designFlowRate) {
        if (*designFlowRate < 0) {
          result = false;
        } else {
          // This one could return false for good reasons, if NaN of Infinity was passed
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, designFlowRate.get());
          if (result) {
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Space");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, "");
            OS_ASSERT(result);
          }
        }
      } else {
        if (istringEqual("Flow/Space", this->designFlowRateCalculationMethod())) {
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, 0.0);
        }
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::setFlowperSpaceFloorArea(boost::optional<double> flowperSpaceFloorArea) {
      bool result = true;
      if (flowperSpaceFloorArea) {
        if (*flowperSpaceFloorArea < 0) {
          result = false;
        } else {
          // This one could return false for good reasons, if NaN of Infinity was passed
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, flowperSpaceFloorArea.get());
          if (result) {
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Area");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, "");
            OS_ASSERT(result);
          }
        }
      } else {
        if (istringEqual("Flow/Area", this->designFlowRateCalculationMethod())) {
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, 0.0);
        }
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::setFlowperExteriorSurfaceArea(boost::optional<double> flowperExteriorSurfaceArea) {
      bool result = true;
      if (flowperExteriorSurfaceArea) {
        if (*flowperExteriorSurfaceArea < 0) {
          result = false;
        } else {
          // This one could return false for good reasons, if NaN of Infinity was passed
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, flowperExteriorSurfaceArea.get());
          if (result) {
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/ExteriorArea");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, "");
            OS_ASSERT(result);
          }
        }
      } else {
        if (istringEqual("Flow/ExteriorArea", this->designFlowRateCalculationMethod())) {
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, 0.0);
        }
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::setFlowperExteriorWallArea(boost::optional<double> flowperExteriorWallArea) {
      bool result = true;
      if (flowperExteriorWallArea) {
        if (*flowperExteriorWallArea < 0) {
          result = false;
        } else {
          // This one could return false for good reasons, if NaN of Infinity was passed
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, flowperExteriorWallArea.get());
          if (result) {
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/ExteriorWallArea");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, "");
            OS_ASSERT(result);
          }
        }
      } else {
        if (istringEqual("Flow/ExteriorWallArea", this->designFlowRateCalculationMethod())) {
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, 0.0);
        }
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::setAirChangesperHour(boost::optional<double> airChangesperHour) {
      bool result = true;
      if (airChangesperHour) {
        if (*airChangesperHour < 0) {
          result = false;
        } else {
          // This one could return false for good reasons, if NaN of Infinity was passed
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, airChangesperHour.get());
          if (result) {
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRate, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperSpaceFloorArea, "");
            OS_ASSERT(result);
            result = setString(OS_SpaceVentilation_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
            OS_ASSERT(result);
          }
        }
      } else {
        if (istringEqual("AirChanges/Hour", this->designFlowRateCalculationMethod())) {
          result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::AirChangesperHour, 0.0);
        }
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::setConstantTermCoefficient(double constantTermCoefficient) {
      bool result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::ConstantTermCoefficient, constantTermCoefficient);
      return result;
    }

    void SpaceVentilationDesignFlowRate_Impl::resetConstantTermCoefficient() {
      bool result = setString(OS_SpaceVentilation_DesignFlowRateFields::ConstantTermCoefficient, "");
      OS_ASSERT(result);
    }

    bool SpaceVentilationDesignFlowRate_Impl::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
      bool result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::TemperatureTermCoefficient, temperatureTermCoefficient);
      return result;
    }

    void SpaceVentilationDesignFlowRate_Impl::resetTemperatureTermCoefficient() {
      bool result = setString(OS_SpaceVentilation_DesignFlowRateFields::TemperatureTermCoefficient, "");
      OS_ASSERT(result);
    }

    bool SpaceVentilationDesignFlowRate_Impl::setVelocityTermCoefficient(double velocityTermCoefficient) {
      bool result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::VelocityTermCoefficient, velocityTermCoefficient);
      return result;
    }

    void SpaceVentilationDesignFlowRate_Impl::resetVelocityTermCoefficient() {
      bool result = setString(OS_SpaceVentilation_DesignFlowRateFields::VelocityTermCoefficient, "");
      OS_ASSERT(result);
    }

    bool SpaceVentilationDesignFlowRate_Impl::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
      bool result = setDouble(OS_SpaceVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient, velocitySquaredTermCoefficient);
      return result;
    }

    void SpaceVentilationDesignFlowRate_Impl::resetVelocitySquaredTermCoefficient() {
      bool result = setString(OS_SpaceVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient, "");
      OS_ASSERT(result);
    }

    double SpaceVentilationDesignFlowRate_Impl::getDesignFlowRate(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                   double airVolume) const {
      std::string method = designFlowRateCalculationMethod();

      if (method == "Flow/Space") {
        return designFlowRate().get();
      } else if (method == "Flow/Area") {
        return flowperSpaceFloorArea().get() * floorArea;
      } else if (method == "Flow/ExteriorArea") {
        return flowperExteriorSurfaceArea().get() * exteriorSurfaceArea;
      } else if (method == "Flow/ExteriorWallArea") {
        return flowperExteriorWallArea().get() * exteriorWallArea;
      } else if (method == "AirChanges/Hour") {
        return convert(airChangesperHour().get() * airVolume, "m^3/h", "m^3/s").get();
      }

      OS_ASSERT(false);
      return 0.0;
    }

    double SpaceVentilationDesignFlowRate_Impl::getFlowPerSpaceFloorArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                          double airVolume) const {
      std::string method = designFlowRateCalculationMethod();

      if (method == "Flow/Space") {
        return designFlowRate().get() / floorArea;
      } else if (method == "Flow/Area") {
        return flowperSpaceFloorArea().get();
      } else if (method == "Flow/ExteriorArea") {
        return flowperExteriorSurfaceArea().get() * (exteriorSurfaceArea / floorArea);
      } else if (method == "Flow/ExteriorWallArea") {
        return flowperExteriorWallArea().get() * (exteriorWallArea / floorArea);
      } else if (method == "AirChanges/Hour") {
        return convert(airChangesperHour().get() * (airVolume / floorArea), "m/h", "m/s").get();
      }

      OS_ASSERT(false);
      return 0.0;
    }

    double SpaceVentilationDesignFlowRate_Impl::getFlowPerExteriorSurfaceArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                               double airVolume) const {
      std::string method = designFlowRateCalculationMethod();

      if (method == "Flow/Space") {
        return designFlowRate().get() / exteriorSurfaceArea;
      } else if (method == "Flow/Area") {
        return flowperSpaceFloorArea().get() * (floorArea / exteriorSurfaceArea);
      } else if (method == "Flow/ExteriorArea") {
        return flowperExteriorSurfaceArea().get();
      } else if (method == "Flow/ExteriorWallArea") {
        return flowperExteriorWallArea().get() * (exteriorWallArea / exteriorSurfaceArea);
      } else if (method == "AirChanges/Hour") {
        return convert(airChangesperHour().get() * (airVolume / exteriorSurfaceArea), "m/h", "m/s").get();
      }

      OS_ASSERT(false);
      return 0.0;
    }

    double SpaceVentilationDesignFlowRate_Impl::getFlowPerExteriorWallArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                            double airVolume) const {
      std::string method = designFlowRateCalculationMethod();

      if (method == "Flow/Space") {
        return designFlowRate().get() / exteriorWallArea;
      } else if (method == "Flow/Area") {
        return flowperSpaceFloorArea().get() * (floorArea / exteriorWallArea);
      } else if (method == "Flow/ExteriorArea") {
        return flowperExteriorSurfaceArea().get() * (exteriorSurfaceArea / exteriorWallArea);
      } else if (method == "Flow/ExteriorWallArea") {
        return flowperExteriorWallArea().get();
      } else if (method == "AirChanges/Hour") {
        return convert(airChangesperHour().get() * (airVolume / exteriorWallArea), "m/h", "m/s").get();
      }

      OS_ASSERT(false);
      return 0.0;
    }

    double SpaceVentilationDesignFlowRate_Impl::getAirChangesPerHour(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                      double airVolume) const {
      std::string method = designFlowRateCalculationMethod();

      if (method == "Flow/Space") {
        return convert(designFlowRate().get() / airVolume, "1/s", "1/h").get();
      } else if (method == "Flow/Area") {
        return convert(flowperSpaceFloorArea().get() * (floorArea / airVolume), "1/s", "1/h").get();
      } else if (method == "Flow/ExteriorArea") {
        return convert(flowperExteriorSurfaceArea().get() * (exteriorSurfaceArea / airVolume), "1/s", "1/h").get();
      } else if (method == "Flow/ExteriorWallArea") {
        return convert(flowperExteriorWallArea().get() * (exteriorWallArea / airVolume), "1/s", "1/h").get();
      } else if (method == "AirChanges/Hour") {
        return airChangesperHour().get();
      }

      OS_ASSERT(false);
      return 0.0;
    }

    int SpaceVentilationDesignFlowRate_Impl::spaceIndex() const {
      return OS_SpaceVentilation_DesignFlowRateFields::SpaceorSpaceTypeName;
    }

    boost::optional<Schedule> SpaceVentilationDesignFlowRate_Impl::schedule() const {
      boost::optional<Schedule> result =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SpaceVentilation_DesignFlowRateFields::ScheduleName);
      if (!result) {
        // search upwards
        OptionalSpace space = this->space();
        OptionalSpaceType spaceType = this->spaceType();
        if (space) {
          result = space->getDefaultSchedule(DefaultScheduleType::VentilationSchedule);
        } else if (spaceType) {
          result = spaceType->getDefaultSchedule(DefaultScheduleType::VentilationSchedule);
        }
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::isScheduleDefaulted() const {
      return isEmpty(OS_SpaceVentilation_DesignFlowRateFields::ScheduleName);
    }

    bool SpaceVentilationDesignFlowRate_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_SpaceVentilation_DesignFlowRateFields::ScheduleName, "SpaceVentilationDesignFlowRate",
                                                  "Ventilation", schedule);
      return result;
    }

    void SpaceVentilationDesignFlowRate_Impl::resetSchedule() {
      this->setString(OS_SpaceVentilation_DesignFlowRateFields::ScheduleName, "");
    }

    boost::optional<ModelObject> SpaceVentilationDesignFlowRate_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = schedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SpaceVentilationDesignFlowRate_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSchedule();
      }
      return true;
    }

    std::vector<EMSActuatorNames> SpaceVentilationDesignFlowRate_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Zone Ventilation", "Air Exchange Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> SpaceVentilationDesignFlowRate_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  SpaceVentilationDesignFlowRate::SpaceVentilationDesignFlowRate(const Model& model)
    : SpaceLoad(SpaceVentilationDesignFlowRate::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SpaceVentilationDesignFlowRate_Impl>());
    bool test = this->setDesignFlowRate(0);
    OS_ASSERT(test);
  }

  IddObjectType SpaceVentilationDesignFlowRate::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SpaceVentilation_DesignFlowRate);
    return result;
  }

  boost::optional<Schedule> SpaceVentilationDesignFlowRate::schedule() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->schedule();
  }

  bool SpaceVentilationDesignFlowRate::isScheduleDefaulted() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->isScheduleDefaulted();
  }

  bool SpaceVentilationDesignFlowRate::setSchedule(Schedule& schedule) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setSchedule(schedule);
  }

  void SpaceVentilationDesignFlowRate::resetSchedule() {
    getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->resetSchedule();
  }

  std::vector<std::string> SpaceVentilationDesignFlowRate::designFlowRateCalculationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SpaceVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod);
  }

  std::vector<std::string> SpaceVentilationDesignFlowRate::validDesignFlowRateCalculationMethodValues() {
    return SpaceVentilationDesignFlowRate::designFlowRateCalculationMethodValues();
  }

  std::string SpaceVentilationDesignFlowRate::designFlowRateCalculationMethod() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->designFlowRateCalculationMethod();
  }

  boost::optional<double> SpaceVentilationDesignFlowRate::designFlowRate() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->designFlowRate();
  }

  boost::optional<double> SpaceVentilationDesignFlowRate::flowperSpaceFloorArea() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->flowperSpaceFloorArea();
  }

  boost::optional<double> SpaceVentilationDesignFlowRate::flowperExteriorSurfaceArea() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->flowperExteriorSurfaceArea();
  }

  boost::optional<double> SpaceVentilationDesignFlowRate::flowperExteriorWallArea() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->flowperExteriorWallArea();
  }

  boost::optional<double> SpaceVentilationDesignFlowRate::airChangesperHour() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->airChangesperHour();
  }

  double SpaceVentilationDesignFlowRate::constantTermCoefficient() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->constantTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::isConstantTermCoefficientDefaulted() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->isConstantTermCoefficientDefaulted();
  }

  double SpaceVentilationDesignFlowRate::temperatureTermCoefficient() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->temperatureTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::isTemperatureTermCoefficientDefaulted() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->isTemperatureTermCoefficientDefaulted();
  }

  double SpaceVentilationDesignFlowRate::velocityTermCoefficient() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->velocityTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::isVelocityTermCoefficientDefaulted() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->isVelocityTermCoefficientDefaulted();
  }

  double SpaceVentilationDesignFlowRate::velocitySquaredTermCoefficient() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->velocitySquaredTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::isVelocitySquaredTermCoefficientDefaulted() const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->isVelocitySquaredTermCoefficientDefaulted();
  }

  bool SpaceVentilationDesignFlowRate::setDesignFlowRate(double designFlowRate) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setDesignFlowRate(designFlowRate);
  }

  bool SpaceVentilationDesignFlowRate::setFlowperSpaceFloorArea(double flowperSpaceFloorArea) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setFlowperSpaceFloorArea(flowperSpaceFloorArea);
  }

  bool SpaceVentilationDesignFlowRate::setFlowperExteriorSurfaceArea(double flowperExteriorSurfaceArea) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setFlowperExteriorSurfaceArea(flowperExteriorSurfaceArea);
  }

  bool SpaceVentilationDesignFlowRate::setFlowperExteriorWallArea(double flowperExteriorWallArea) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setFlowperExteriorWallArea(flowperExteriorWallArea);
  }

  bool SpaceVentilationDesignFlowRate::setAirChangesperHour(double airChangesperHour) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setAirChangesperHour(airChangesperHour);
  }

  bool SpaceVentilationDesignFlowRate::setConstantTermCoefficient(double constantTermCoefficient) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setConstantTermCoefficient(constantTermCoefficient);
  }

  void SpaceVentilationDesignFlowRate::resetConstantTermCoefficient() {
    getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->resetConstantTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setTemperatureTermCoefficient(temperatureTermCoefficient);
  }

  void SpaceVentilationDesignFlowRate::resetTemperatureTermCoefficient() {
    getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->resetTemperatureTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::setVelocityTermCoefficient(double velocityTermCoefficient) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setVelocityTermCoefficient(velocityTermCoefficient);
  }

  void SpaceVentilationDesignFlowRate::resetVelocityTermCoefficient() {
    getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->resetVelocityTermCoefficient();
  }

  bool SpaceVentilationDesignFlowRate::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->setVelocitySquaredTermCoefficient(velocitySquaredTermCoefficient);
  }

  void SpaceVentilationDesignFlowRate::resetVelocitySquaredTermCoefficient() {
    getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->resetVelocitySquaredTermCoefficient();
  }

  double SpaceVentilationDesignFlowRate::getDesignFlowRate(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                            double airVolume) const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->getDesignFlowRate(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
  }

  double SpaceVentilationDesignFlowRate::getFlowPerSpaceFloorArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                   double airVolume) const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->getFlowPerSpaceFloorArea(floorArea, exteriorSurfaceArea, exteriorWallArea,
                                                                                             airVolume);
  }

  double SpaceVentilationDesignFlowRate::getFlowPerExteriorSurfaceArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                        double airVolume) const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->getFlowPerExteriorSurfaceArea(floorArea, exteriorSurfaceArea, exteriorWallArea,
                                                                                                  airVolume);
  }

  double SpaceVentilationDesignFlowRate::getFlowPerExteriorWallArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                                     double airVolume) const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->getFlowPerExteriorWallArea(floorArea, exteriorSurfaceArea, exteriorWallArea,
                                                                                               airVolume);
  }

  double SpaceVentilationDesignFlowRate::getAirChangesPerHour(double floorArea, double exteriorSurfaceArea, double exteriorWallArea,
                                                               double airVolume) const {
    return getImpl<detail::SpaceVentilationDesignFlowRate_Impl>()->getAirChangesPerHour(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
  }

  /// @cond
  SpaceVentilationDesignFlowRate::SpaceVentilationDesignFlowRate(std::shared_ptr<detail::SpaceVentilationDesignFlowRate_Impl> impl)
    : SpaceLoad(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
