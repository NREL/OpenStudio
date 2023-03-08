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

#include "SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"
#include "SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl.hpp"

#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const IdfObject& idfObject,
                                                                                                                   Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType());
    }

    SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType());
    }

    SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(
      const SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::iddObjectType() const {
      return SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(),
                    OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("SolarCollectorPerformancePhotovoltaicThermalBIPVT", "Availability");
      }
      return result;
    }

    SurfacePropertyOtherSideConditionsModel SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::boundaryConditionsModel() const {
      boost::optional<SurfacePropertyOtherSideConditionsModel> value = optionalBoundaryConditionsModel();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Boundary Conditions Model attached.");
      }
      return value.get();
    }

    Schedule SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::effectivePlenumGapThicknessBehindPVModules() const {
      boost::optional<double> value =
        getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVCellNormalTransmittanceAbsorptanceProduct() const {
      boost::optional<double> value =
        getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::backingMaterialNormalTransmittanceAbsorptanceProduct() const {
      boost::optional<double> value =
        getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::claddingNormalTransmittanceAbsorptanceProduct() const {
      boost::optional<double> value =
        getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::fractionofCollectorGrossAreaCoveredbyPVModule() const {
      boost::optional<double> value =
        getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::fractionofPVCellAreatoPVModuleArea() const {
      boost::optional<double> value =
        getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleTopThermalResistance() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleBottomThermalResistance() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleFrontLongwaveEmissivity() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleBackLongwaveEmissivity() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::glassThickness() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::glassRefractionIndex() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::glassExtinctionCoefficient() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setBoundaryConditionsModel(
      const SurfacePropertyOtherSideConditionsModel& surfacePropertyOtherSideConditionsModel) {
      const bool result = setPointer(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName,
                                     surfacePropertyOtherSideConditionsModel.handle());
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setAvailabilitySchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName,
                                      "SolarCollectorPerformancePhotovoltaicThermalBIPVT", "Availability", schedule);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setEffectivePlenumGapThicknessBehindPVModules(
      double effectivePlenumGapThicknessBehindPVModules) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules,
                                    effectivePlenumGapThicknessBehindPVModules);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVCellNormalTransmittanceAbsorptanceProduct(
      double pVCellNormalTransmittanceAbsorptanceProduct) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct,
                                    pVCellNormalTransmittanceAbsorptanceProduct);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setBackingMaterialNormalTransmittanceAbsorptanceProduct(
      double backingMaterialNormalTransmittanceAbsorptanceProduct) {
      const bool result =
        setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct,
                  backingMaterialNormalTransmittanceAbsorptanceProduct);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setCladdingNormalTransmittanceAbsorptanceProduct(
      double claddingNormalTransmittanceAbsorptanceProduct) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct,
                                    claddingNormalTransmittanceAbsorptanceProduct);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setFractionofCollectorGrossAreaCoveredbyPVModule(
      double fractionofCollectorGrossAreaCoveredbyPVModule) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule,
                                    fractionofCollectorGrossAreaCoveredbyPVModule);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setFractionofPVCellAreatoPVModuleArea(double fractionofPVCellAreatoPVModuleArea) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea,
                                    fractionofPVCellAreatoPVModuleArea);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleTopThermalResistance(double pVModuleTopThermalResistance) {
      const bool result =
        setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance, pVModuleTopThermalResistance);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleBottomThermalResistance(double pVModuleBottomThermalResistance) {
      const bool result =
        setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance, pVModuleBottomThermalResistance);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleFrontLongwaveEmissivity(double pVModuleFrontLongwaveEmissivity) {
      const bool result =
        setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity, pVModuleFrontLongwaveEmissivity);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleBackLongwaveEmissivity(double pVModuleBackLongwaveEmissivity) {
      const bool result =
        setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity, pVModuleBackLongwaveEmissivity);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setGlassThickness(double glassThickness) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness, glassThickness);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setGlassRefractionIndex(double glassRefractionIndex) {
      const bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex, glassRefractionIndex);
      return result;
    }

    bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setGlassExtinctionCoefficient(double glassExtinctionCoefficient) {
      const bool result =
        setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient, glassExtinctionCoefficient);
      return result;
    }

    boost::optional<SurfacePropertyOtherSideConditionsModel>
      SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::optionalBoundaryConditionsModel() const {
      return getObject<ModelObject>().getModelObjectTarget<SurfacePropertyOtherSideConditionsModel>(
        OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName);
    }

    boost::optional<Schedule> SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName);
    }

  }  // namespace detail

  SolarCollectorPerformancePhotovoltaicThermalBIPVT::SolarCollectorPerformancePhotovoltaicThermalBIPVT(const Model& model)
    : ModelObject(SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>());

    SurfacePropertyOtherSideConditionsModel oscm(model);
    oscm.setName("BIPVT Surface Property Other Side Conditions Model");
    bool ok = setBoundaryConditionsModel(oscm);
    OS_ASSERT(ok);
    ok = oscm.setTypeOfModeling("GapConvectionRadiation");  // Explicitly set it, even if it's the default
    OS_ASSERT(ok);

    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);

    ok = setEffectivePlenumGapThicknessBehindPVModules(0.1);  // 10cm. Taken from ShopWithBIPVT.idf
    OS_ASSERT(ok);

    // IDD Defaults
    ok = setPVCellNormalTransmittanceAbsorptanceProduct(0.957);
    OS_ASSERT(ok);
    ok = setBackingMaterialNormalTransmittanceAbsorptanceProduct(0.87);
    OS_ASSERT(ok);
    ok = setCladdingNormalTransmittanceAbsorptanceProduct(0.85);
    OS_ASSERT(ok);
    ok = setFractionofCollectorGrossAreaCoveredbyPVModule(0.85);
    OS_ASSERT(ok);
    ok = setFractionofPVCellAreatoPVModuleArea(0.9);
    OS_ASSERT(ok);
    ok = setPVModuleTopThermalResistance(0.0044);
    OS_ASSERT(ok);
    ok = setPVModuleBottomThermalResistance(0.0039);
    OS_ASSERT(ok);
    ok = setPVModuleFrontLongwaveEmissivity(0.85);
    OS_ASSERT(ok);
    ok = setPVModuleBackLongwaveEmissivity(0.9);
    OS_ASSERT(ok);
    ok = setGlassThickness(0.002);
    OS_ASSERT(ok);
    ok = setGlassRefractionIndex(1.526);
    OS_ASSERT(ok);
    ok = setGlassExtinctionCoefficient(4.0);
    OS_ASSERT(ok);
  }

  SolarCollectorPerformancePhotovoltaicThermalBIPVT::SolarCollectorPerformancePhotovoltaicThermalBIPVT(
    const SurfacePropertyOtherSideConditionsModel& oscm)
    : ModelObject(SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType(), oscm.model()) {
    OS_ASSERT(getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>());

    bool ok = setBoundaryConditionsModel(oscm);
    OS_ASSERT(ok);

    auto alwaysOn = oscm.model().alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);

    ok = setEffectivePlenumGapThicknessBehindPVModules(0.1);  // 10cm. Taken from ShopWithBIPVT.idf
    OS_ASSERT(ok);

    // IDD Defaults
    ok = setPVCellNormalTransmittanceAbsorptanceProduct(0.957);
    OS_ASSERT(ok);
    ok = setBackingMaterialNormalTransmittanceAbsorptanceProduct(0.87);
    OS_ASSERT(ok);
    ok = setCladdingNormalTransmittanceAbsorptanceProduct(0.85);
    OS_ASSERT(ok);
    ok = setFractionofCollectorGrossAreaCoveredbyPVModule(0.85);
    OS_ASSERT(ok);
    ok = setFractionofPVCellAreatoPVModuleArea(0.9);
    OS_ASSERT(ok);
    ok = setPVModuleTopThermalResistance(0.0044);
    OS_ASSERT(ok);
    ok = setPVModuleBottomThermalResistance(0.0039);
    OS_ASSERT(ok);
    ok = setPVModuleFrontLongwaveEmissivity(0.85);
    OS_ASSERT(ok);
    ok = setPVModuleBackLongwaveEmissivity(0.9);
    OS_ASSERT(ok);
    ok = setGlassThickness(0.002);
    OS_ASSERT(ok);
    ok = setGlassRefractionIndex(1.526);
    OS_ASSERT(ok);
    ok = setGlassExtinctionCoefficient(4.0);
    OS_ASSERT(ok);
  }

  IddObjectType SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType() {
    return {IddObjectType::OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVT};
  }

  SurfacePropertyOtherSideConditionsModel SolarCollectorPerformancePhotovoltaicThermalBIPVT::boundaryConditionsModel() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->boundaryConditionsModel();
  }

  Schedule SolarCollectorPerformancePhotovoltaicThermalBIPVT::availabilitySchedule() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->availabilitySchedule();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::effectivePlenumGapThicknessBehindPVModules() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->effectivePlenumGapThicknessBehindPVModules();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::pVCellNormalTransmittanceAbsorptanceProduct() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->pVCellNormalTransmittanceAbsorptanceProduct();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::backingMaterialNormalTransmittanceAbsorptanceProduct() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->backingMaterialNormalTransmittanceAbsorptanceProduct();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::claddingNormalTransmittanceAbsorptanceProduct() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->claddingNormalTransmittanceAbsorptanceProduct();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::fractionofCollectorGrossAreaCoveredbyPVModule() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->fractionofCollectorGrossAreaCoveredbyPVModule();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::fractionofPVCellAreatoPVModuleArea() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->fractionofPVCellAreatoPVModuleArea();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::pVModuleTopThermalResistance() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->pVModuleTopThermalResistance();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::pVModuleBottomThermalResistance() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->pVModuleBottomThermalResistance();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::pVModuleFrontLongwaveEmissivity() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->pVModuleFrontLongwaveEmissivity();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::pVModuleBackLongwaveEmissivity() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->pVModuleBackLongwaveEmissivity();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::glassThickness() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->glassThickness();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::glassRefractionIndex() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->glassRefractionIndex();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT::glassExtinctionCoefficient() const {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->glassExtinctionCoefficient();
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setBoundaryConditionsModel(
    const SurfacePropertyOtherSideConditionsModel& surfacePropertyOtherSideConditionsModel) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setBoundaryConditionsModel(
      surfacePropertyOtherSideConditionsModel);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setEffectivePlenumGapThicknessBehindPVModules(
    double effectivePlenumGapThicknessBehindPVModules) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setEffectivePlenumGapThicknessBehindPVModules(
      effectivePlenumGapThicknessBehindPVModules);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVCellNormalTransmittanceAbsorptanceProduct(
    double pVCellNormalTransmittanceAbsorptanceProduct) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVCellNormalTransmittanceAbsorptanceProduct(
      pVCellNormalTransmittanceAbsorptanceProduct);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setBackingMaterialNormalTransmittanceAbsorptanceProduct(
    double backingMaterialNormalTransmittanceAbsorptanceProduct) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setBackingMaterialNormalTransmittanceAbsorptanceProduct(
      backingMaterialNormalTransmittanceAbsorptanceProduct);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setCladdingNormalTransmittanceAbsorptanceProduct(
    double claddingNormalTransmittanceAbsorptanceProduct) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setCladdingNormalTransmittanceAbsorptanceProduct(
      claddingNormalTransmittanceAbsorptanceProduct);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setFractionofCollectorGrossAreaCoveredbyPVModule(
    double fractionofCollectorGrossAreaCoveredbyPVModule) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setFractionofCollectorGrossAreaCoveredbyPVModule(
      fractionofCollectorGrossAreaCoveredbyPVModule);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setFractionofPVCellAreatoPVModuleArea(double fractionofPVCellAreatoPVModuleArea) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setFractionofPVCellAreatoPVModuleArea(
      fractionofPVCellAreatoPVModuleArea);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleTopThermalResistance(double pVModuleTopThermalResistance) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleTopThermalResistance(pVModuleTopThermalResistance);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleBottomThermalResistance(double pVModuleBottomThermalResistance) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleBottomThermalResistance(
      pVModuleBottomThermalResistance);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleFrontLongwaveEmissivity(double pVModuleFrontLongwaveEmissivity) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleFrontLongwaveEmissivity(
      pVModuleFrontLongwaveEmissivity);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleBackLongwaveEmissivity(double pVModuleBackLongwaveEmissivity) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleBackLongwaveEmissivity(
      pVModuleBackLongwaveEmissivity);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setGlassThickness(double glassThickness) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setGlassThickness(glassThickness);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setGlassRefractionIndex(double glassRefractionIndex) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setGlassRefractionIndex(glassRefractionIndex);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setGlassExtinctionCoefficient(double glassExtinctionCoefficient) {
    return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setGlassExtinctionCoefficient(glassExtinctionCoefficient);
  }

  /// @cond
  SolarCollectorPerformancePhotovoltaicThermalBIPVT::SolarCollectorPerformancePhotovoltaicThermalBIPVT(
    std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
