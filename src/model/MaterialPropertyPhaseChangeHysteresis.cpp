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

#include <vector>
#include "Material.hpp"
#include "Material_Impl.hpp"
#include "StandardOpaqueMaterial.hpp"
#include "StandardOpaqueMaterial_Impl.hpp"
#include "MasslessOpaqueMaterial.hpp"
#include "MasslessOpaqueMaterial_Impl.hpp"
#include "MaterialPropertyPhaseChangeHysteresis.hpp"
#include "MaterialPropertyPhaseChangeHysteresis_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_MaterialProperty_PhaseChangeHysteresis_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    MaterialPropertyPhaseChangeHysteresis_Impl::MaterialPropertyPhaseChangeHysteresis_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                           bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == MaterialPropertyPhaseChangeHysteresis::iddObjectType());
    }

    MaterialPropertyPhaseChangeHysteresis_Impl::MaterialPropertyPhaseChangeHysteresis_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == MaterialPropertyPhaseChangeHysteresis::iddObjectType());
    }

    MaterialPropertyPhaseChangeHysteresis_Impl::MaterialPropertyPhaseChangeHysteresis_Impl(const MaterialPropertyPhaseChangeHysteresis_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& MaterialPropertyPhaseChangeHysteresis_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType MaterialPropertyPhaseChangeHysteresis_Impl::iddObjectType() const {
      return MaterialPropertyPhaseChangeHysteresis::iddObjectType();
    }

    std::string MaterialPropertyPhaseChangeHysteresis_Impl::materialName() const {
      boost::optional<Material> material =
        getObject<ModelObject>().getModelObjectTarget<Material>(OS_MaterialProperty_PhaseChangeHysteresisFields::MaterialName);
      OS_ASSERT(material);
      return material.get().name().get();
    }

    Material MaterialPropertyPhaseChangeHysteresis_Impl::material() const {
      boost::optional<Material> material =
        getObject<ModelObject>().getModelObjectTarget<Material>(OS_MaterialProperty_PhaseChangeHysteresisFields::MaterialName);
      OS_ASSERT(material);
      return material.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::latentHeatduringtheEntirePhaseChangeProcess() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LatentHeatduringtheEntirePhaseChangeProcess);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::liquidStateThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LiquidStateThermalConductivity);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::liquidStateDensity() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LiquidStateDensity);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::liquidStateSpecificHeat() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LiquidStateSpecificHeat);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::highTemperatureDifferenceofMeltingCurve() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofMeltingCurve);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::peakMeltingTemperature() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::PeakMeltingTemperature);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::lowTemperatureDifferenceofMeltingCurve() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofMeltingCurve);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::solidStateThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::SolidStateThermalConductivity);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::solidStateDensity() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::SolidStateDensity);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::solidStateSpecificHeat() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::SolidStateSpecificHeat);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::highTemperatureDifferenceofFreezingCurve() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofFreezingCurve);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::peakFreezingTemperature() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::PeakFreezingTemperature);
      OS_ASSERT(value);
      return value.get();
    }

    double MaterialPropertyPhaseChangeHysteresis_Impl::lowTemperatureDifferenceofFreezingCurve() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofFreezingCurve);
      OS_ASSERT(value);
      return value.get();
    }

    bool
      MaterialPropertyPhaseChangeHysteresis_Impl::setLatentHeatduringtheEntirePhaseChangeProcess(double latentHeatduringtheEntirePhaseChangeProcess) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LatentHeatduringtheEntirePhaseChangeProcess,
                              latentHeatduringtheEntirePhaseChangeProcess);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setLiquidStateThermalConductivity(double liquidStateThermalConductivity) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LiquidStateThermalConductivity, liquidStateThermalConductivity);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setLiquidStateDensity(double liquidStateDensity) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LiquidStateDensity, liquidStateDensity);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setLiquidStateSpecificHeat(double liquidStateSpecificHeat) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LiquidStateSpecificHeat, liquidStateSpecificHeat);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setHighTemperatureDifferenceofMeltingCurve(double highTemperatureDifferenceofMeltingCurve) {
      bool result =
        setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofMeltingCurve, highTemperatureDifferenceofMeltingCurve);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setPeakMeltingTemperature(double peakMeltingTemperature) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::PeakMeltingTemperature, peakMeltingTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setLowTemperatureDifferenceofMeltingCurve(double lowTemperatureDifferenceofMeltingCurve) {
      bool result =
        setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofMeltingCurve, lowTemperatureDifferenceofMeltingCurve);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setSolidStateThermalConductivity(double solidStateThermalConductivity) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::SolidStateThermalConductivity, solidStateThermalConductivity);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setSolidStateDensity(double solidStateDensity) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::SolidStateDensity, solidStateDensity);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setSolidStateSpecificHeat(double solidStateSpecificHeat) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::SolidStateSpecificHeat, solidStateSpecificHeat);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setHighTemperatureDifferenceofFreezingCurve(double highTemperatureDifferenceofFreezingCurve) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofFreezingCurve,
                              highTemperatureDifferenceofFreezingCurve);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setPeakFreezingTemperature(double peakFreezingTemperature) {
      bool result = setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::PeakFreezingTemperature, peakFreezingTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool MaterialPropertyPhaseChangeHysteresis_Impl::setLowTemperatureDifferenceofFreezingCurve(double lowTemperatureDifferenceofFreezingCurve) {
      bool result =
        setDouble(OS_MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofFreezingCurve, lowTemperatureDifferenceofFreezingCurve);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  MaterialPropertyPhaseChangeHysteresis::MaterialPropertyPhaseChangeHysteresis(StandardOpaqueMaterial& material)
    : ModelObject(MaterialPropertyPhaseChangeHysteresis::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>());

    if (material.materialPropertyPhaseChangeHysteresis()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChangeHysteresis object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeHysteresisFields::MaterialName, material.handle());
    OS_ASSERT(ok);

    // These from 1ZoneUncontrolledWithHysteresisPCM.idf for C5 - 4 IN HW CONCRETE.
    // They should be modified by user!
    ok = setLatentHeatduringtheEntirePhaseChangeProcess(10000);
    OS_ASSERT(ok);
    ok = setLiquidStateThermalConductivity(1.5);
    OS_ASSERT(ok);
    ok = setLiquidStateDensity(2200);
    OS_ASSERT(ok);
    ok = setLiquidStateSpecificHeat(2000);
    OS_ASSERT(ok);
    ok = setHighTemperatureDifferenceofMeltingCurve(1);
    OS_ASSERT(ok);
    ok = setPeakMeltingTemperature(23);
    OS_ASSERT(ok);
    ok = setLowTemperatureDifferenceofMeltingCurve(1);
    OS_ASSERT(ok);
    ok = setSolidStateThermalConductivity(1.8);
    OS_ASSERT(ok);
    ok = setSolidStateDensity(2300);
    OS_ASSERT(ok);
    ok = setSolidStateSpecificHeat(2000);
    OS_ASSERT(ok);
    ok = setHighTemperatureDifferenceofFreezingCurve(1);
    OS_ASSERT(ok);
    ok = setPeakFreezingTemperature(20);
    OS_ASSERT(ok);
    ok = setLowTemperatureDifferenceofFreezingCurve(1);
    OS_ASSERT(ok);
  }

  MaterialPropertyPhaseChangeHysteresis::MaterialPropertyPhaseChangeHysteresis(MasslessOpaqueMaterial& material)
    : ModelObject(MaterialPropertyPhaseChangeHysteresis::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>());

    if (material.materialPropertyPhaseChangeHysteresis()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChangeHysteresis object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeHysteresisFields::MaterialName, material.handle());
    OS_ASSERT(ok);

    // These from 1ZoneUncontrolledWithHysteresisPCM.idf for C5 - 4 IN HW CONCRETE.
    // They should be modified by user!
    ok = setLatentHeatduringtheEntirePhaseChangeProcess(10000);
    OS_ASSERT(ok);
    ok = setLiquidStateThermalConductivity(1.5);
    OS_ASSERT(ok);
    ok = setLiquidStateDensity(2200);
    OS_ASSERT(ok);
    ok = setLiquidStateSpecificHeat(2000);
    OS_ASSERT(ok);
    ok = setHighTemperatureDifferenceofMeltingCurve(1);
    OS_ASSERT(ok);
    ok = setPeakMeltingTemperature(23);
    OS_ASSERT(ok);
    ok = setLowTemperatureDifferenceofMeltingCurve(1);
    OS_ASSERT(ok);
    ok = setSolidStateThermalConductivity(1.8);
    OS_ASSERT(ok);
    ok = setSolidStateDensity(2300);
    OS_ASSERT(ok);
    ok = setSolidStateSpecificHeat(2000);
    OS_ASSERT(ok);
    ok = setHighTemperatureDifferenceofFreezingCurve(1);
    OS_ASSERT(ok);
    ok = setPeakFreezingTemperature(20);
    OS_ASSERT(ok);
    ok = setLowTemperatureDifferenceofFreezingCurve(1);
    OS_ASSERT(ok);
  }

  IddObjectType MaterialPropertyPhaseChangeHysteresis::iddObjectType() {
    return IddObjectType(IddObjectType::OS_MaterialProperty_PhaseChangeHysteresis);
  }

  std::string MaterialPropertyPhaseChangeHysteresis::materialName() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->materialName();
  }

  Material MaterialPropertyPhaseChangeHysteresis::material() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->material();
  }

  double MaterialPropertyPhaseChangeHysteresis::latentHeatduringtheEntirePhaseChangeProcess() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->latentHeatduringtheEntirePhaseChangeProcess();
  }

  double MaterialPropertyPhaseChangeHysteresis::liquidStateThermalConductivity() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->liquidStateThermalConductivity();
  }

  double MaterialPropertyPhaseChangeHysteresis::liquidStateDensity() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->liquidStateDensity();
  }

  double MaterialPropertyPhaseChangeHysteresis::liquidStateSpecificHeat() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->liquidStateSpecificHeat();
  }

  double MaterialPropertyPhaseChangeHysteresis::highTemperatureDifferenceofMeltingCurve() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->highTemperatureDifferenceofMeltingCurve();
  }

  double MaterialPropertyPhaseChangeHysteresis::peakMeltingTemperature() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->peakMeltingTemperature();
  }

  double MaterialPropertyPhaseChangeHysteresis::lowTemperatureDifferenceofMeltingCurve() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->lowTemperatureDifferenceofMeltingCurve();
  }

  double MaterialPropertyPhaseChangeHysteresis::solidStateThermalConductivity() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->solidStateThermalConductivity();
  }

  double MaterialPropertyPhaseChangeHysteresis::solidStateDensity() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->solidStateDensity();
  }

  double MaterialPropertyPhaseChangeHysteresis::solidStateSpecificHeat() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->solidStateSpecificHeat();
  }

  double MaterialPropertyPhaseChangeHysteresis::highTemperatureDifferenceofFreezingCurve() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->highTemperatureDifferenceofFreezingCurve();
  }

  double MaterialPropertyPhaseChangeHysteresis::peakFreezingTemperature() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->peakFreezingTemperature();
  }

  double MaterialPropertyPhaseChangeHysteresis::lowTemperatureDifferenceofFreezingCurve() const {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->lowTemperatureDifferenceofFreezingCurve();
  }

  bool MaterialPropertyPhaseChangeHysteresis::setLatentHeatduringtheEntirePhaseChangeProcess(double latentHeatduringtheEntirePhaseChangeProcess) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setLatentHeatduringtheEntirePhaseChangeProcess(
      latentHeatduringtheEntirePhaseChangeProcess);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setLiquidStateThermalConductivity(double liquidStateThermalConductivity) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setLiquidStateThermalConductivity(liquidStateThermalConductivity);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setLiquidStateDensity(double liquidStateDensity) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setLiquidStateDensity(liquidStateDensity);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setLiquidStateSpecificHeat(double liquidStateSpecificHeat) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setLiquidStateSpecificHeat(liquidStateSpecificHeat);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setHighTemperatureDifferenceofMeltingCurve(double highTemperatureDifferenceofMeltingCurve) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setHighTemperatureDifferenceofMeltingCurve(
      highTemperatureDifferenceofMeltingCurve);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setPeakMeltingTemperature(double peakMeltingTemperature) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setPeakMeltingTemperature(peakMeltingTemperature);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setLowTemperatureDifferenceofMeltingCurve(double lowTemperatureDifferenceofMeltingCurve) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setLowTemperatureDifferenceofMeltingCurve(
      lowTemperatureDifferenceofMeltingCurve);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setSolidStateThermalConductivity(double solidStateThermalConductivity) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setSolidStateThermalConductivity(solidStateThermalConductivity);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setSolidStateDensity(double solidStateDensity) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setSolidStateDensity(solidStateDensity);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setSolidStateSpecificHeat(double solidStateSpecificHeat) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setSolidStateSpecificHeat(solidStateSpecificHeat);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setHighTemperatureDifferenceofFreezingCurve(double highTemperatureDifferenceofFreezingCurve) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setHighTemperatureDifferenceofFreezingCurve(
      highTemperatureDifferenceofFreezingCurve);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setPeakFreezingTemperature(double peakFreezingTemperature) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setPeakFreezingTemperature(peakFreezingTemperature);
  }

  bool MaterialPropertyPhaseChangeHysteresis::setLowTemperatureDifferenceofFreezingCurve(double lowTemperatureDifferenceofFreezingCurve) {
    return getImpl<detail::MaterialPropertyPhaseChangeHysteresis_Impl>()->setLowTemperatureDifferenceofFreezingCurve(
      lowTemperatureDifferenceofFreezingCurve);
  }

  /// @cond
  MaterialPropertyPhaseChangeHysteresis::MaterialPropertyPhaseChangeHysteresis(
    std::shared_ptr<detail::MaterialPropertyPhaseChangeHysteresis_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
