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

#include "MasslessOpaqueMaterial.hpp"
#include "MasslessOpaqueMaterial_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Material_NoMass_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    MasslessOpaqueMaterial_Impl::MasslessOpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == MasslessOpaqueMaterial::iddObjectType());
    }

    MasslessOpaqueMaterial_Impl::MasslessOpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == MasslessOpaqueMaterial::iddObjectType());
    }

    MasslessOpaqueMaterial_Impl::MasslessOpaqueMaterial_Impl(const MasslessOpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {}

    std::string MasslessOpaqueMaterial_Impl::roughness() const {
      boost::optional<std::string> value = getString(OS_Material_NoMassFields::Roughness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double MasslessOpaqueMaterial_Impl::thermalConductivity() const {
      LOG_AND_THROW("Unable to convert thermal resistance to thermal conductivity for "
                    << "MasslessOpaqueMaterial " << briefDescription() << ".");
      return 0.0;
    }

    double MasslessOpaqueMaterial_Impl::thermalConductance() const {
      return 1.0 / thermalResistance();
    }

    double MasslessOpaqueMaterial_Impl::thermalResistivity() const {
      LOG_AND_THROW("Unable to convert thermal resistance to thermal resistivity for "
                    << "MasslessOpaqueMaterial " << briefDescription() << ".");
      return 0.0;
    }

    double MasslessOpaqueMaterial_Impl::thermalAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::ThermalAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    OptionalDouble MasslessOpaqueMaterial_Impl::thermalReflectance() const {
      OptionalDouble result;
      OptionalDouble od = thermalAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    OptionalDouble MasslessOpaqueMaterial_Impl::solarReflectance() const {
      OptionalDouble result;
      OptionalDouble od = solarAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    double MasslessOpaqueMaterial_Impl::visibleAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::VisibleAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    OptionalDouble MasslessOpaqueMaterial_Impl::visibleReflectance() const {
      OptionalDouble result;
      OptionalDouble od = visibleAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    const std::vector<std::string>& MasslessOpaqueMaterial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType MasslessOpaqueMaterial_Impl::iddObjectType() const {
      return MasslessOpaqueMaterial::iddObjectType();
    }

    bool MasslessOpaqueMaterial_Impl::setThickness(double value) {
      return false;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalConductivity(double value) {
      return false;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalConductance(double value) {
      return setThermalResistance(1.0 / value);
    }

    bool MasslessOpaqueMaterial_Impl::setThermalResistivity(double value) {
      return false;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_NoMassFields::ThermalAbsorptance, "");
      }
      return setDouble(OS_Material_NoMassFields::ThermalAbsorptance, *value);
    }

    bool MasslessOpaqueMaterial_Impl::setThermalReflectance(OptionalDouble value) {
      if (!value) {
        return setThermalAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setThermalAbsorptance(od);
    }

    bool MasslessOpaqueMaterial_Impl::setSolarAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_NoMassFields::SolarAbsorptance, "");
      }
      return setDouble(OS_Material_NoMassFields::SolarAbsorptance, *value);
    }

    bool MasslessOpaqueMaterial_Impl::setSolarReflectance(OptionalDouble value) {
      if (!value) {
        return setSolarAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setSolarAbsorptance(od);
    }

    bool MasslessOpaqueMaterial_Impl::setVisibleAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_NoMassFields::VisibleAbsorptance, "");
      }
      return setDouble(OS_Material_NoMassFields::VisibleAbsorptance, *value);
    }

    bool MasslessOpaqueMaterial_Impl::setVisibleReflectance(OptionalDouble value) {
      if (!value) {
        return setVisibleAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setVisibleAbsorptance(od);
    }

    double MasslessOpaqueMaterial_Impl::thermalResistance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::ThermalResistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool MasslessOpaqueMaterial_Impl::isThermalAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_NoMassFields::ThermalAbsorptance);
    }

    double MasslessOpaqueMaterial_Impl::solarAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::SolarAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool MasslessOpaqueMaterial_Impl::isSolarAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_NoMassFields::SolarAbsorptance);
    }

    bool MasslessOpaqueMaterial_Impl::isVisibleAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_NoMassFields::VisibleAbsorptance);
    }

    bool MasslessOpaqueMaterial_Impl::setRoughness(const std::string& roughness) {
      bool result = setString(OS_Material_NoMassFields::Roughness, roughness);
      return result;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalResistance(double thermalResistance) {
      bool result = setDouble(OS_Material_NoMassFields::ThermalResistance, thermalResistance);
      return result;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalAbsorptance(double thermalAbsorptance) {
      bool result = setDouble(OS_Material_NoMassFields::ThermalAbsorptance, thermalAbsorptance);
      return result;
    }

    void MasslessOpaqueMaterial_Impl::resetThermalAbsorptance() {
      bool result = setString(OS_Material_NoMassFields::ThermalAbsorptance, "");
      OS_ASSERT(result);
    }

    bool MasslessOpaqueMaterial_Impl::setSolarAbsorptance(double solarAbsorptance) {
      bool result = setDouble(OS_Material_NoMassFields::SolarAbsorptance, solarAbsorptance);
      return result;
    }

    void MasslessOpaqueMaterial_Impl::resetSolarAbsorptance() {
      bool result = setString(OS_Material_NoMassFields::SolarAbsorptance, "");
      OS_ASSERT(result);
    }

    bool MasslessOpaqueMaterial_Impl::setVisibleAbsorptance(double visibleAbsorptance) {
      bool result = setDouble(OS_Material_NoMassFields::VisibleAbsorptance, visibleAbsorptance);
      return result;
    }

    void MasslessOpaqueMaterial_Impl::resetVisibleAbsorptance() {
      bool result = setString(OS_Material_NoMassFields::VisibleAbsorptance, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> MasslessOpaqueMaterial_Impl::roughnessValues() const {
      return MasslessOpaqueMaterial::roughnessValues();
    }

  }  // namespace detail

  MasslessOpaqueMaterial::MasslessOpaqueMaterial(const Model& model, const std::string& roughness, double thermalResistance)
    : OpaqueMaterial(MasslessOpaqueMaterial::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::MasslessOpaqueMaterial_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    //ok = setHandle();
    OS_ASSERT(ok);
    ok = setRoughness(roughness);
    OS_ASSERT(ok);
    ok = setThermalResistance(thermalResistance);
    OS_ASSERT(ok);
  }

  IddObjectType MasslessOpaqueMaterial::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Material_NoMass);
  }

  std::vector<std::string> MasslessOpaqueMaterial::roughnessValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Material_NoMassFields::Roughness);
  }

  std::string MasslessOpaqueMaterial::roughness() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->roughness();
  }

  boost::optional<double> MasslessOpaqueMaterial::thermalAbsorptance() const {
    OptionalDouble result = getImpl<detail::MasslessOpaqueMaterial_Impl>()->thermalAbsorptance();
    return result;
  }

  boost::optional<double> MasslessOpaqueMaterial::solarAbsorptance() const {
    OptionalDouble result = getImpl<detail::MasslessOpaqueMaterial_Impl>()->solarAbsorptance();
    return result;
  }

  boost::optional<double> MasslessOpaqueMaterial::visibleAbsorptance() const {
    OptionalDouble result = getImpl<detail::MasslessOpaqueMaterial_Impl>()->visibleAbsorptance();
    return result;
  }

  double MasslessOpaqueMaterial::thermalResistance() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->thermalResistance();
  }

  bool MasslessOpaqueMaterial::isThermalAbsorptanceDefaulted() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->isThermalAbsorptanceDefaulted();
  }

  bool MasslessOpaqueMaterial::isSolarAbsorptanceDefaulted() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->isSolarAbsorptanceDefaulted();
  }

  bool MasslessOpaqueMaterial::isVisibleAbsorptanceDefaulted() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->isVisibleAbsorptanceDefaulted();
  }

  bool MasslessOpaqueMaterial::setRoughness(const std::string& roughness) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setRoughness(roughness);
  }

  bool MasslessOpaqueMaterial::setThermalResistance(double thermalResistance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setThermalResistance(thermalResistance);
  }

  bool MasslessOpaqueMaterial::setThermalAbsorptance(double thermalAbsorptance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setThermalAbsorptance(thermalAbsorptance);
  }

  void MasslessOpaqueMaterial::resetThermalAbsorptance() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetThermalAbsorptance();
  }

  bool MasslessOpaqueMaterial::setSolarAbsorptance(double solarAbsorptance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setSolarAbsorptance(solarAbsorptance);
  }

  void MasslessOpaqueMaterial::resetSolarAbsorptance() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetSolarAbsorptance();
  }

  bool MasslessOpaqueMaterial::setVisibleAbsorptance(double visibleAbsorptance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setVisibleAbsorptance(visibleAbsorptance);
  }

  void MasslessOpaqueMaterial::resetVisibleAbsorptance() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetVisibleAbsorptance();
  }

  /// @cond
  MasslessOpaqueMaterial::MasslessOpaqueMaterial(std::shared_ptr<detail::MasslessOpaqueMaterial_Impl> impl) : OpaqueMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
