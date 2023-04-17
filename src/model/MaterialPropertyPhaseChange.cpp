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

    Material MaterialPropertyPhaseChange_Impl::material() const {
      boost::optional<Material> material =
        getObject<ModelObject>().getModelObjectTarget<Material>(OS_MaterialProperty_PhaseChangeFields::MaterialName);
      OS_ASSERT(material);
      return material.get();
    }

    double MaterialPropertyPhaseChange_Impl::temperatureCoefficientforThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_MaterialProperty_PhaseChangeFields::TemperatureCoefficientforThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool MaterialPropertyPhaseChange_Impl::isTemperatureCoefficientforThermalConductivityDefaulted() const {
      return isEmpty(OS_MaterialProperty_PhaseChangeFields::TemperatureCoefficientforThermalConductivity);
    }

    std::vector<TemperatureEnthalpy> MaterialPropertyPhaseChange_Impl::temperatureEnthalpys() const {
      std::vector<TemperatureEnthalpy> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<double> temperature =
          group.cast<WorkspaceExtensibleGroup>().getDouble(OS_MaterialProperty_PhaseChangeExtensibleFields::Temperature);
        boost::optional<double> enthalpy =
          group.cast<WorkspaceExtensibleGroup>().getDouble(OS_MaterialProperty_PhaseChangeExtensibleFields::Enthalpy);

        if (temperature && enthalpy) {
          TemperatureEnthalpy temperatureEnthalpy(temperature.get(), enthalpy.get());
          result.push_back(temperatureEnthalpy);
        }
      }

      return result;
    }

    unsigned int MaterialPropertyPhaseChange_Impl::numberofTemperatureEnthalpys() const {
      return numExtensibleGroups();
    }

    bool MaterialPropertyPhaseChange_Impl::setTemperatureCoefficientforThermalConductivity(double temperatureCoefficientforThermalConductivity) {
      bool result =
        setDouble(OS_MaterialProperty_PhaseChangeFields::TemperatureCoefficientforThermalConductivity, temperatureCoefficientforThermalConductivity);
      OS_ASSERT(result);
      return result;
    }

    void MaterialPropertyPhaseChange_Impl::resetTemperatureCoefficientforThermalConductivity() {
      bool result = setString(OS_MaterialProperty_PhaseChangeFields::TemperatureCoefficientforThermalConductivity, "");
      OS_ASSERT(result);
    }

    bool MaterialPropertyPhaseChange_Impl::addTemperatureEnthalpy(const TemperatureEnthalpy& temperatureEnthalpy) {
      bool result;

      unsigned int num = numberofTemperatureEnthalpys();
      // Max number of temperature-enthalpys is 16
      if (num >= 16) {
        LOG(Warn, briefDescription() << " already has 16 temperature-enthalpys which is the limit");
        result = false;
      } else {
        // Push an extensible group
        auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
        bool temperature = eg.setDouble(OS_MaterialProperty_PhaseChangeExtensibleFields::Temperature, temperatureEnthalpy.temperature());
        bool enthalpy = eg.setDouble(OS_MaterialProperty_PhaseChangeExtensibleFields::Enthalpy, temperatureEnthalpy.enthalpy());
        if (temperature && enthalpy) {
          result = true;
        } else {
          // Something went wrong
          // So erase the new extensible group
          getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
          result = false;
        }
        result = true;
      }
      return result;
    }

    bool MaterialPropertyPhaseChange_Impl::addTemperatureEnthalpy(double temperature, double enthalpy) {
      // Make a temperature-enthalpy, and then call the above function
      TemperatureEnthalpy temperatureEnthalpy(temperature, enthalpy);
      return addTemperatureEnthalpy(temperatureEnthalpy);
    }

    bool MaterialPropertyPhaseChange_Impl::addTemperatureEnthalpys(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys) {
      unsigned int num = numberofTemperatureEnthalpys();
      if ((num + temperatureEnthalpys.size()) > 16) {
        LOG(Warn, briefDescription() << " would have more than the 16 maximum temperature-enthalpys");
        return false;
      } else {
        for (const TemperatureEnthalpy& temperatureEnthalpy : temperatureEnthalpys) {
          addTemperatureEnthalpy(temperatureEnthalpy);
        }
        return true;
      }
    }

    bool MaterialPropertyPhaseChange_Impl::removeTemperatureEnthalpy(unsigned groupIndex) {
      bool result;

      unsigned int num = numberofTemperatureEnthalpys();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      } else {
        result = false;
      }
      return result;
    }

    void MaterialPropertyPhaseChange_Impl::removeAllTemperatureEnthalpys() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

  }  // namespace detail

  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(StandardOpaqueMaterial& material)
    : ModelObject(MaterialPropertyPhaseChange::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChange_Impl>());

    if (material.materialPropertyPhaseChange()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChange object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeFields::MaterialName, material.handle());
    OS_ASSERT(ok);

    // These from CondFD1ZonePurchAirAutoSizeWithPCM.idf for E1 - 3 / 4 IN PLASTER OR GYP BOARD.
    // They should be modified by user!
    ok = addTemperatureEnthalpy(-20, 0.1);
    OS_ASSERT(ok);
    ok = addTemperatureEnthalpy(22, 18260);
    OS_ASSERT(ok);
    ok = addTemperatureEnthalpy(22.1, 32000);
    OS_ASSERT(ok);
    ok = addTemperatureEnthalpy(60, 71000);
    OS_ASSERT(ok);
  }

  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(MasslessOpaqueMaterial& material)
    : ModelObject(MaterialPropertyPhaseChange::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChange_Impl>());

    if (material.materialPropertyPhaseChange()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChange object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeFields::MaterialName, material.handle());
    OS_ASSERT(ok);

    // These from CondFD1ZonePurchAirAutoSizeWithPCM.idf for E1 - 3 / 4 IN PLASTER OR GYP BOARD.
    // They should be modified by user!
    ok = addTemperatureEnthalpy(-20, 0.1);
    OS_ASSERT(ok);
    ok = addTemperatureEnthalpy(22, 18260);
    OS_ASSERT(ok);
    ok = addTemperatureEnthalpy(22.1, 32000);
    OS_ASSERT(ok);
    ok = addTemperatureEnthalpy(60, 71000);
    OS_ASSERT(ok);
  }

  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(StandardOpaqueMaterial& material,
                                                           const std::vector<TemperatureEnthalpy>& temperatureEnthalpys)
    : ModelObject(MaterialPropertyPhaseChange::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChange_Impl>());

    if (material.materialPropertyPhaseChange()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChange object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeFields::MaterialName, material.handle());
    OS_ASSERT(ok);

    ok = addTemperatureEnthalpys(temperatureEnthalpys);
    OS_ASSERT(ok);
  }

  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(MasslessOpaqueMaterial& material,
                                                           const std::vector<TemperatureEnthalpy>& temperatureEnthalpys)
    : ModelObject(MaterialPropertyPhaseChange::iddObjectType(), material.model()) {
    OS_ASSERT(getImpl<detail::MaterialPropertyPhaseChange_Impl>());

    if (material.materialPropertyPhaseChange()) {
      LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyPhaseChange object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_MaterialProperty_PhaseChangeFields::MaterialName, material.handle());
    OS_ASSERT(ok);

    ok = addTemperatureEnthalpys(temperatureEnthalpys);
    OS_ASSERT(ok);
  }

  IddObjectType MaterialPropertyPhaseChange::iddObjectType() {
    return IddObjectType(IddObjectType::OS_MaterialProperty_PhaseChange);
  }

  std::string MaterialPropertyPhaseChange::materialName() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->materialName();
  }

  Material MaterialPropertyPhaseChange::material() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->material();
  }

  double MaterialPropertyPhaseChange::temperatureCoefficientforThermalConductivity() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->temperatureCoefficientforThermalConductivity();
  }

  bool MaterialPropertyPhaseChange::isTemperatureCoefficientforThermalConductivityDefaulted() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->isTemperatureCoefficientforThermalConductivityDefaulted();
  }

  std::vector<TemperatureEnthalpy> MaterialPropertyPhaseChange::temperatureEnthalpys() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->temperatureEnthalpys();
  }

  unsigned int MaterialPropertyPhaseChange::numberofTemperatureEnthalpys() const {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->numberofTemperatureEnthalpys();
  }

  bool MaterialPropertyPhaseChange::setTemperatureCoefficientforThermalConductivity(double temperatureCoefficientforThermalConductivity) {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->setTemperatureCoefficientforThermalConductivity(
      temperatureCoefficientforThermalConductivity);
  }

  void MaterialPropertyPhaseChange::resetTemperatureCoefficientforThermalConductivity() {
    getImpl<detail::MaterialPropertyPhaseChange_Impl>()->resetTemperatureCoefficientforThermalConductivity();
  }

  bool MaterialPropertyPhaseChange::addTemperatureEnthalpy(const TemperatureEnthalpy& temperatureEnthalpy) {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->addTemperatureEnthalpy(temperatureEnthalpy);
  }

  bool MaterialPropertyPhaseChange::addTemperatureEnthalpy(double temperature, double enthalpy) {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->addTemperatureEnthalpy(temperature, enthalpy);
  }

  bool MaterialPropertyPhaseChange::addTemperatureEnthalpys(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys) {
    return getImpl<detail::MaterialPropertyPhaseChange_Impl>()->addTemperatureEnthalpys(temperatureEnthalpys);
  }

  void MaterialPropertyPhaseChange::removeTemperatureEnthalpy(unsigned groupIndex) {
    getImpl<detail::MaterialPropertyPhaseChange_Impl>()->removeTemperatureEnthalpy(groupIndex);
  }

  void MaterialPropertyPhaseChange::removeAllTemperatureEnthalpys() {
    getImpl<detail::MaterialPropertyPhaseChange_Impl>()->removeAllTemperatureEnthalpys();
  }

  /// @cond
  MaterialPropertyPhaseChange::MaterialPropertyPhaseChange(std::shared_ptr<detail::MaterialPropertyPhaseChange_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
