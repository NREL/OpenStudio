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

#include "StandardOpaqueMaterial.hpp"
#include "StandardOpaqueMaterial_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    StandardOpaqueMaterial_Impl::StandardOpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == StandardOpaqueMaterial::iddObjectType());
    }

    StandardOpaqueMaterial_Impl::StandardOpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == StandardOpaqueMaterial::iddObjectType());
    }

    StandardOpaqueMaterial_Impl::StandardOpaqueMaterial_Impl(const StandardOpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {}

    IddObjectType StandardOpaqueMaterial_Impl::iddObjectType() const {
      return StandardOpaqueMaterial::iddObjectType();
    }

    boost::optional<double> StandardOpaqueMaterial_Impl::getVisibleTransmittance() const {
      return 0.0;
    }

    boost::optional<double> StandardOpaqueMaterial_Impl::interiorVisibleAbsorptance() const {
      return this->visibleAbsorptance();
    }

    boost::optional<double> StandardOpaqueMaterial_Impl::exteriorVisibleAbsorptance() const {
      return this->visibleAbsorptance();
    }

    boost::optional<double> StandardOpaqueMaterial_Impl::heatCapacity() const {
      return boost::none;
    }

    std::string StandardOpaqueMaterial_Impl::roughness() const {
      boost::optional<std::string> value = getString(OS_MaterialFields::Roughness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double StandardOpaqueMaterial_Impl::thickness() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::Thickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double StandardOpaqueMaterial_Impl::conductivity() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::Conductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double StandardOpaqueMaterial_Impl::thermalConductivity() const {
      OptionalDouble od = getDouble(OS_MaterialFields::Conductivity, true);
      if (!od) {
        LOG_AND_THROW("Thermal conductivity is not set for StandardOpaqueMaterial " << briefDescription() << ".");
      }
      return *od;
    }

    double StandardOpaqueMaterial_Impl::thermalConductance() const {
      return thermalConductivity() / thickness();
    }

    double StandardOpaqueMaterial_Impl::thermalResistivity() const {
      return 1.0 / thermalConductivity();
    }

    double StandardOpaqueMaterial_Impl::thermalResistance() const {
      return thermalResistivity() * thickness();
    }

    double StandardOpaqueMaterial_Impl::density() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::Density, true);
      OS_ASSERT(value);
      return value.get();
    }

    double StandardOpaqueMaterial_Impl::specificHeat() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::SpecificHeat, true);
      OS_ASSERT(value);
      return value.get();
    }

    double StandardOpaqueMaterial_Impl::thermalAbsorptance() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::ThermalAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> StandardOpaqueMaterial_Impl::thermalReflectance() const {
      OptionalDouble result;
      OptionalDouble od = thermalAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    bool StandardOpaqueMaterial_Impl::isThermalAbsorptanceDefaulted() const {
      return isEmpty(OS_MaterialFields::ThermalAbsorptance);
    }

    double StandardOpaqueMaterial_Impl::solarAbsorptance() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::SolarAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool StandardOpaqueMaterial_Impl::isSolarAbsorptanceDefaulted() const {
      return isEmpty(OS_MaterialFields::SolarAbsorptance);
    }

    boost::optional<double> StandardOpaqueMaterial_Impl::solarReflectance() const {
      OptionalDouble result;
      OptionalDouble od = solarAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    double StandardOpaqueMaterial_Impl::visibleAbsorptance() const {
      boost::optional<double> value = getDouble(OS_MaterialFields::VisibleAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }
    boost::optional<double> StandardOpaqueMaterial_Impl::visibleReflectance() const {
      OptionalDouble result;
      OptionalDouble od = visibleAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    const std::vector<std::string>& StandardOpaqueMaterial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    bool StandardOpaqueMaterial_Impl::isVisibleAbsorptanceDefaulted() const {
      return isEmpty(OS_MaterialFields::VisibleAbsorptance);
    }

    bool StandardOpaqueMaterial_Impl::setRoughness(const std::string& roughness) {
      bool result = setString(OS_MaterialFields::Roughness, roughness);
      return result;
    }

    bool StandardOpaqueMaterial_Impl::setThickness(double thickness) {
      bool result = setDouble(OS_MaterialFields::Thickness, thickness);
      return result;
    }

    bool StandardOpaqueMaterial_Impl::setConductivity(double conductivity) {
      bool result = setDouble(OS_MaterialFields::Conductivity, conductivity);
      return result;
    }

    bool StandardOpaqueMaterial_Impl::setThermalConductivity(double value) {
      return setDouble(OS_MaterialFields::Conductivity, value);
    }

    bool StandardOpaqueMaterial_Impl::setThermalConductance(double value) {
      // set thickness to achieve conductance
      return setThickness(thermalConductivity() / value);
    }

    bool StandardOpaqueMaterial_Impl::setThermalResistivity(double value) {
      return setThermalConductivity(1.0 / value);
    }

    bool StandardOpaqueMaterial_Impl::setThermalResistance(double value) {
      // set thickenss to achieve resistivity
      LOG(Trace, "Attempt to set thermal resistance to " << value << " by setting thickness.");
      return setThickness(value / thermalResistivity());
    }

    bool StandardOpaqueMaterial_Impl::setDensity(double density) {
      bool result = setDouble(OS_MaterialFields::Density, density);
      return result;
    }

    bool StandardOpaqueMaterial_Impl::setSpecificHeat(double specificHeat) {
      bool result = setDouble(OS_MaterialFields::SpecificHeat, specificHeat);
      return result;
    }

    bool StandardOpaqueMaterial_Impl::setThermalReflectance(boost::optional<double> value) {
      if (!value) {
        return setThermalAbsorptance(value.get());
      }
      OptionalDouble od = (1.0 - *value);
      return setThermalAbsorptance(od.get());
    }

    bool StandardOpaqueMaterial_Impl::setSolarAbsorptance(boost::optional<double> value) {
      if (!value) {
        return setString(OS_MaterialFields::SolarAbsorptance, "");
      }
      return setDouble(OS_MaterialFields::SolarAbsorptance, *value);
    }

    bool StandardOpaqueMaterial_Impl::setSolarReflectance(boost::optional<double> value) {
      if (!value) {
        return setSolarAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setSolarAbsorptance(od);
    }

    bool StandardOpaqueMaterial_Impl::setVisibleAbsorptance(boost::optional<double> value) {
      if (!value) {
        return setString(OS_MaterialFields::VisibleAbsorptance, "");
      }
      return setDouble(OS_MaterialFields::VisibleAbsorptance, *value);
    }

    bool StandardOpaqueMaterial_Impl::setVisibleReflectance(boost::optional<double> value) {
      if (!value) {
        return setVisibleAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setVisibleAbsorptance(od);
    }

    bool StandardOpaqueMaterial_Impl::setThermalAbsorptance(double thermalAbsorptance) {
      bool result = setDouble(OS_MaterialFields::ThermalAbsorptance, thermalAbsorptance);
      return result;
    }

    void StandardOpaqueMaterial_Impl::resetThermalAbsorptance() {
      bool result = setString(OS_MaterialFields::ThermalAbsorptance, "");
      OS_ASSERT(result);
    }

    bool StandardOpaqueMaterial_Impl::setSolarAbsorptance(double solarAbsorptance) {
      bool result = setDouble(OS_MaterialFields::SolarAbsorptance, solarAbsorptance);
      return result;
    }

    void StandardOpaqueMaterial_Impl::resetSolarAbsorptance() {
      bool result = setString(OS_MaterialFields::SolarAbsorptance, "");
      OS_ASSERT(result);
    }

    bool StandardOpaqueMaterial_Impl::setVisibleAbsorptance(double visibleAbsorptance) {
      bool result = setDouble(OS_MaterialFields::VisibleAbsorptance, visibleAbsorptance);
      return result;
    }

    void StandardOpaqueMaterial_Impl::resetVisibleAbsorptance() {
      bool result = setString(OS_MaterialFields::VisibleAbsorptance, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> StandardOpaqueMaterial_Impl::roughnessValues() const {
      return StandardOpaqueMaterial::roughnessValues();
    }

    std::vector<EMSActuatorNames> StandardOpaqueMaterial_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Material", "Surface Property Solar Absorptance"},
                                              {"Material", "Surface Property Thermal Absorptance"},
                                              {"Material", "Surface Property Visible Absorptance"}};
      return actuators;
    }

    std::vector<std::string> StandardOpaqueMaterial_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

  }  // namespace detail

  StandardOpaqueMaterial::StandardOpaqueMaterial(const Model& model, const std::string& roughness, double thickness, double conductivity,
                                                 double density, double specificHeat)
    : OpaqueMaterial(StandardOpaqueMaterial::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::StandardOpaqueMaterial_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
    ok = setRoughness(roughness);
    OS_ASSERT(ok);
    ok = setThickness(thickness);
    OS_ASSERT(ok);
    ok = setConductivity(conductivity);
    OS_ASSERT(ok);
    ok = setDensity(density);
    OS_ASSERT(ok);
    ok = setSpecificHeat(specificHeat);
    OS_ASSERT(ok);
  }

  IddObjectType StandardOpaqueMaterial::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Material);
  }

  double StandardOpaqueMaterial::thermalConductivity() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thermalConductivity();
  }

  double StandardOpaqueMaterial::thermalConductance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thermalConductance();
  }

  double StandardOpaqueMaterial::thermalResistivity() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thermalResistivity();
  }

  double StandardOpaqueMaterial::thermalResistance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thermalResistance();
  }

  boost::optional<double> StandardOpaqueMaterial::thermalReflectance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thermalReflectance();
  }

  boost::optional<double> StandardOpaqueMaterial::solarReflectance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->solarReflectance();
  }

  boost::optional<double> StandardOpaqueMaterial::visibleReflectance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->visibleReflectance();
  }

  bool StandardOpaqueMaterial::setThermalConductivity(double value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalConductivity(value);
  }

  bool StandardOpaqueMaterial::setThermalConductance(double value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalConductance(value);
  }

  bool StandardOpaqueMaterial::setThermalResistivity(double value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalResistivity(value);
  }

  bool StandardOpaqueMaterial::setThermalResistance(double value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalResistance(value);
  }

  bool StandardOpaqueMaterial::setThermalAbsorptance(boost::optional<double> value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalAbsorptance(value.get());
  }

  bool StandardOpaqueMaterial::setThermalReflectance(boost::optional<double> value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalReflectance(value);
  }

  bool StandardOpaqueMaterial::setSolarAbsorptance(boost::optional<double> value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setSolarAbsorptance(value);
  }

  bool StandardOpaqueMaterial::setSolarReflectance(boost::optional<double> value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setSolarReflectance(value);
  }

  bool StandardOpaqueMaterial::setVisibleAbsorptance(boost::optional<double> value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setVisibleAbsorptance(value);
  }

  bool StandardOpaqueMaterial::setVisibleReflectance(boost::optional<double> value) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setVisibleReflectance(value);
  }

  boost::optional<double> StandardOpaqueMaterial::heatCapacity() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->heatCapacity();
  }

  std::vector<std::string> StandardOpaqueMaterial::roughnessValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_MaterialFields::Roughness);
  }

  std::string StandardOpaqueMaterial::roughness() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->roughness();
  }

  double StandardOpaqueMaterial::thickness() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thickness();
  }

  double StandardOpaqueMaterial::conductivity() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->conductivity();
  }

  double StandardOpaqueMaterial::density() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->density();
  }

  double StandardOpaqueMaterial::specificHeat() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->specificHeat();
  }

  double StandardOpaqueMaterial::thermalAbsorptance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->thermalAbsorptance();
  }

  bool StandardOpaqueMaterial::isThermalAbsorptanceDefaulted() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->isThermalAbsorptanceDefaulted();
  }

  double StandardOpaqueMaterial::solarAbsorptance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->solarAbsorptance();
  }

  bool StandardOpaqueMaterial::isSolarAbsorptanceDefaulted() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->isSolarAbsorptanceDefaulted();
  }

  double StandardOpaqueMaterial::visibleAbsorptance() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->visibleAbsorptance();
  }

  bool StandardOpaqueMaterial::isVisibleAbsorptanceDefaulted() const {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->isVisibleAbsorptanceDefaulted();
  }

  bool StandardOpaqueMaterial::setRoughness(const std::string& roughness) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setRoughness(roughness);
  }

  bool StandardOpaqueMaterial::setThickness(double thickness) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThickness(thickness);
  }

  bool StandardOpaqueMaterial::setConductivity(double conductivity) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setConductivity(conductivity);
  }

  bool StandardOpaqueMaterial::setDensity(double density) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setDensity(density);
  }

  bool StandardOpaqueMaterial::setSpecificHeat(double specificHeat) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setSpecificHeat(specificHeat);
  }

  bool StandardOpaqueMaterial::setThermalAbsorptance(double thermalAbsorptance) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setThermalAbsorptance(thermalAbsorptance);
  }

  void StandardOpaqueMaterial::resetThermalAbsorptance() {
    getImpl<detail::StandardOpaqueMaterial_Impl>()->resetThermalAbsorptance();
  }

  bool StandardOpaqueMaterial::setSolarAbsorptance(double solarAbsorptance) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setSolarAbsorptance(solarAbsorptance);
  }

  void StandardOpaqueMaterial::resetSolarAbsorptance() {
    getImpl<detail::StandardOpaqueMaterial_Impl>()->resetSolarAbsorptance();
  }

  bool StandardOpaqueMaterial::setVisibleAbsorptance(double visibleAbsorptance) {
    return getImpl<detail::StandardOpaqueMaterial_Impl>()->setVisibleAbsorptance(visibleAbsorptance);
  }

  void StandardOpaqueMaterial::resetVisibleAbsorptance() {
    getImpl<detail::StandardOpaqueMaterial_Impl>()->resetVisibleAbsorptance();
  }

  /// @cond
  StandardOpaqueMaterial::StandardOpaqueMaterial(std::shared_ptr<detail::StandardOpaqueMaterial_Impl> impl) : OpaqueMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
