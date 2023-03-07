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

// TODO: Check the following class names against object getters and setters.
#include "OSCM.hpp"
#include "OSCM_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const IdfObject& idfObject,
                                                                                                                 Model_Impl* model,
                                                                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType());
  }

  SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                 Model_Impl* model,
                                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType());
  }

  SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl& other,
                                                                                                                 Model_Impl* model,
                                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::iddObjectType() const {
    return SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SolarCollectorPerformancePhotovoltaicThermalBIPVT","Availability"));
    }
    return result;
  }

  OSCM SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::boundaryConditionsModel() const {
    boost::optional<OSCM> value = optionalBoundaryConditionsModel();
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
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVCellNormalTransmittanceAbsorptanceProduct() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::backingMaterialNormalTransmittanceAbsorptanceProduct() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::claddingNormalTransmittanceAbsorptanceProduct() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::fractionofCollectorGrossAreaCoveredbyPVModule() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::fractionofPVCellAreatoPVModuleArea() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleTopThermalResistance() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleBottomThermalResistance() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleFrontLongwaveEmissivity() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::pVModuleBackLongwaveEmissivity() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::glassThickness() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::glassRefractionIndex() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::glassExtinctionCoefficient() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setBoundaryConditionsModel(const OSCM& oSCM) {
    bool result = setPointer(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName, oSCM.handle());
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName,
                              "SolarCollectorPerformancePhotovoltaicThermalBIPVT",
                              "Availability",
                              schedule);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setEffectivePlenumGapThicknessBehindPVModules(double effectivePlenumGapThicknessBehindPVModules) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules, effectivePlenumGapThicknessBehindPVModules);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVCellNormalTransmittanceAbsorptanceProduct(double pVCellNormalTransmittanceAbsorptanceProduct) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct, pVCellNormalTransmittanceAbsorptanceProduct);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setBackingMaterialNormalTransmittanceAbsorptanceProduct(double backingMaterialNormalTransmittanceAbsorptanceProduct) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct, backingMaterialNormalTransmittanceAbsorptanceProduct);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setCladdingNormalTransmittanceAbsorptanceProduct(double claddingNormalTransmittanceAbsorptanceProduct) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct, claddingNormalTransmittanceAbsorptanceProduct);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setFractionofCollectorGrossAreaCoveredbyPVModule(double fractionofCollectorGrossAreaCoveredbyPVModule) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule, fractionofCollectorGrossAreaCoveredbyPVModule);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setFractionofPVCellAreatoPVModuleArea(double fractionofPVCellAreatoPVModuleArea) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea, fractionofPVCellAreatoPVModuleArea);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleTopThermalResistance(double pVModuleTopThermalResistance) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance, pVModuleTopThermalResistance);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleBottomThermalResistance(double pVModuleBottomThermalResistance) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance, pVModuleBottomThermalResistance);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleFrontLongwaveEmissivity(double pVModuleFrontLongwaveEmissivity) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity, pVModuleFrontLongwaveEmissivity);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setPVModuleBackLongwaveEmissivity(double pVModuleBackLongwaveEmissivity) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity, pVModuleBackLongwaveEmissivity);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setGlassThickness(double glassThickness) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness, glassThickness);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setGlassRefractionIndex(double glassRefractionIndex) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex, glassRefractionIndex);
    return result;
  }

  bool SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::setGlassExtinctionCoefficient(double glassExtinctionCoefficient) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient, glassExtinctionCoefficient);
    return result;
  }

  boost::optional<OSCM> SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::optionalBoundaryConditionsModel() const {
    return getObject<ModelObject>().getModelObjectTarget<OSCM>(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName);
  }

  boost::optional<Schedule> SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName);
  }

} // detail

SolarCollectorPerformancePhotovoltaicThermalBIPVT::SolarCollectorPerformancePhotovoltaicThermalBIPVT(const Model& model)
  : ModelObject(SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName
  //     OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName
  bool ok = true;
  // ok = setBoundaryConditionsModel();
  OS_ASSERT(ok);
  // ok = setAvailabilitySchedule();
  OS_ASSERT(ok);
  // ok = setEffectivePlenumGapThicknessBehindPVModules();
  OS_ASSERT(ok);
  // ok = setPVCellNormalTransmittanceAbsorptanceProduct();
  OS_ASSERT(ok);
  // ok = setBackingMaterialNormalTransmittanceAbsorptanceProduct();
  OS_ASSERT(ok);
  // ok = setCladdingNormalTransmittanceAbsorptanceProduct();
  OS_ASSERT(ok);
  // ok = setFractionofCollectorGrossAreaCoveredbyPVModule();
  OS_ASSERT(ok);
  // ok = setFractionofPVCellAreatoPVModuleArea();
  OS_ASSERT(ok);
  // ok = setPVModuleTopThermalResistance();
  OS_ASSERT(ok);
  // ok = setPVModuleBottomThermalResistance();
  OS_ASSERT(ok);
  // ok = setPVModuleFrontLongwaveEmissivity();
  OS_ASSERT(ok);
  // ok = setPVModuleBackLongwaveEmissivity();
  OS_ASSERT(ok);
  // ok = setGlassThickness();
  OS_ASSERT(ok);
  // ok = setGlassRefractionIndex();
  OS_ASSERT(ok);
  // ok = setGlassExtinctionCoefficient();
  OS_ASSERT(ok);
}

IddObjectType SolarCollectorPerformancePhotovoltaicThermalBIPVT::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVT);
}

OSCM SolarCollectorPerformancePhotovoltaicThermalBIPVT::boundaryConditionsModel() const {
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

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setBoundaryConditionsModel(const OSCM& oSCM) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setBoundaryConditionsModel(oSCM);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setAvailabilitySchedule(schedule);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setEffectivePlenumGapThicknessBehindPVModules(double effectivePlenumGapThicknessBehindPVModules) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setEffectivePlenumGapThicknessBehindPVModules(effectivePlenumGapThicknessBehindPVModules);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVCellNormalTransmittanceAbsorptanceProduct(double pVCellNormalTransmittanceAbsorptanceProduct) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVCellNormalTransmittanceAbsorptanceProduct(pVCellNormalTransmittanceAbsorptanceProduct);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setBackingMaterialNormalTransmittanceAbsorptanceProduct(double backingMaterialNormalTransmittanceAbsorptanceProduct) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setBackingMaterialNormalTransmittanceAbsorptanceProduct(backingMaterialNormalTransmittanceAbsorptanceProduct);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setCladdingNormalTransmittanceAbsorptanceProduct(double claddingNormalTransmittanceAbsorptanceProduct) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setCladdingNormalTransmittanceAbsorptanceProduct(claddingNormalTransmittanceAbsorptanceProduct);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setFractionofCollectorGrossAreaCoveredbyPVModule(double fractionofCollectorGrossAreaCoveredbyPVModule) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setFractionofCollectorGrossAreaCoveredbyPVModule(fractionofCollectorGrossAreaCoveredbyPVModule);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setFractionofPVCellAreatoPVModuleArea(double fractionofPVCellAreatoPVModuleArea) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setFractionofPVCellAreatoPVModuleArea(fractionofPVCellAreatoPVModuleArea);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleTopThermalResistance(double pVModuleTopThermalResistance) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleTopThermalResistance(pVModuleTopThermalResistance);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleBottomThermalResistance(double pVModuleBottomThermalResistance) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleBottomThermalResistance(pVModuleBottomThermalResistance);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleFrontLongwaveEmissivity(double pVModuleFrontLongwaveEmissivity) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleFrontLongwaveEmissivity(pVModuleFrontLongwaveEmissivity);
}

bool SolarCollectorPerformancePhotovoltaicThermalBIPVT::setPVModuleBackLongwaveEmissivity(double pVModuleBackLongwaveEmissivity) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl>()->setPVModuleBackLongwaveEmissivity(pVModuleBackLongwaveEmissivity);
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
SolarCollectorPerformancePhotovoltaicThermalBIPVT::SolarCollectorPerformancePhotovoltaicThermalBIPVT(std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

