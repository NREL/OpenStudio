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

#include <vector>
#include "Material.hpp"
#include "Material_Impl.hpp"
#include "MaterialPropertyPhaseChange.hpp"
#include "MaterialPropertyPhaseChange_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_MaterialProperty_PhaseChange_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  TemperatureEnthalpy::TemperatureEnthalpy(double temperature, double enthalpy) : m_temperature(temperature), m_enthalpy(enthalpy) {}

  double TemperatureEnthalpy::temperature() const {
    return m_temperature;
  }

  double TemperatureEnthalpy::enthalpy() const {
    return m_enthalpy;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::TemperatureEnthalpy& temperatureEnthalpy) {
    out << "temperature=" << temperatureEnthalpy.temperature() << ", enthalpy=" << temperatureEnthalpy.enthalpy();
    return out;
  }

  namespace detail {

    MaterialPropertyPhaseChange_Impl::MaterialPropertyPhaseChange_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == MaterialPropertyPhaseChange::iddObjectType());
    }

    MaterialPropertyPhaseChange_Impl::MaterialPropertyPhaseChange_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == MaterialPropertyPhaseChange::iddObjectType());
    }

    MaterialPropertyPhaseChange_Impl::MaterialPropertyPhaseChange_Impl(const MaterialPropertyPhaseChange_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& MaterialPropertyPhaseChange_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType MaterialPropertyPhaseChange_Impl::iddObjectType() const {
      return MaterialPropertyPhaseChange::iddObjectType();
    }

    std::string MaterialPropertyPhaseChange_Impl::materialName() const {
      boost::optional<Material> material =
        getObject<ModelObject>().getModelObjectTarget<Material>(OS_MaterialProperty_PhaseChangeFields::MaterialName);
      OS_ASSERT(material);
      return material.get().name().get();
    }

    double MaterialPropertyPhaseChange_Impl::temperatureCoefficientforThermalConductivity() const {}

    bool MaterialPropertyPhaseChange_Impl::isTemperatureCoefficientforThermalConductivityDefaulted() const {}

    std::vector<TemperatureEnthalpy> MaterialPropertyPhaseChange_Impl::temperatureEnthalpys() const {}

    unsigned int MaterialPropertyPhaseChange_Impl::numberofTemperatureEnthalpys() const {}

    boost::optional<unsigned> MaterialPropertyPhaseChange_Impl::temperatureEnthalpyIndex(const TemperatureEnthalpy& temperatureEnthalpy) const {}

    boost::optional<TemperatureEnthalpy> MaterialPropertyPhaseChange_Impl::getTemperatureEnthalpy(unsigned groupIndex) const {}

    bool MaterialPropertyPhaseChange_Impl::setTemperatureCoefficientforThermalConductivity(double temperatureCoefficientforThermalConductivity) {}

    void MaterialPropertyPhaseChange_Impl::resetTemperatureCoefficientforThermalConductivity() {}

    bool MaterialPropertyPhaseChange_Impl::addTemperatureEnthalpy(const TemperatureEnthalpy& temperatureEnthalpy) {}

    bool MaterialPropertyPhaseChange_Impl::addTemperatureEnthalpy(double temperature, double enthalpy) {}

    bool MaterialPropertyPhaseChange_Impl::addTemperatureEnthalpys(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys) {}

    bool MaterialPropertyPhaseChange_Impl::removeTemperatureEnthalpy(unsigned groupIndex) {}

    void MaterialPropertyPhaseChange_Impl::removeAllTemperatureEnthalpys() {}

  }  // namespace detail

  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(Material& material)
    : ModelObject(MaterialPropertyPhaseChange::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChange_Impl>());

    if (material.materialPropertyPhaseChange()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChange object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeFields::MaterialName, material.handle());
    OS_ASSERT(ok);
  }

  IddObjectType MaterialPropertyPhaseChange::iddObjectType() {
    return IddObjectType(IddObjectType::OS_MaterialProperty_PhaseChange);
  }

  std::string MaterialPropertyPhaseChange::materialName() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->materialName();
  }

  double MaterialPropertyPhaseChange::temperatureCoefficientforThermalConductivity() const {}

  bool MaterialPropertyPhaseChange::isTemperatureCoefficientforThermalConductivityDefaulted() const {}

  std::vector<TemperatureEnthalpy> MaterialPropertyPhaseChange::temperatureEnthalpys() const {}

  unsigned int MaterialPropertyPhaseChange::numberofTemperatureEnthalpys() const {}

  boost::optional<unsigned> MaterialPropertyPhaseChange::temperatureEnthalpyIndex(const TemperatureEnthalpy& temperatureEnthalpy) const {}

  boost::optional<TemperatureEnthalpy> MaterialPropertyPhaseChange::getTemperatureEnthalpy(unsigned groupIndex) const {}

  bool MaterialPropertyPhaseChange::setTemperatureCoefficientforThermalConductivity(double temperatureCoefficientforThermalConductivity) {}

  void MaterialPropertyPhaseChange::resetTemperatureCoefficientforThermalConductivity() {}

  bool MaterialPropertyPhaseChange::addTemperatureEnthalpy(const TemperatureEnthalpy& temperatureEnthalpy) {}

  bool MaterialPropertyPhaseChange::addTemperatureEnthalpy(double temperature, double enthalpy) {}

  bool MaterialPropertyPhaseChange::addTemperatureEnthalpys(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys) {}

  bool MaterialPropertyPhaseChange::removeTemperatureEnthalpy(unsigned groupIndex) {}

  void MaterialPropertyPhaseChange::removeAllTemperatureEnthalpys() {}

  /// @cond
  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(std::shared_ptr<detail::MaterialPropertyPhaseChange_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
