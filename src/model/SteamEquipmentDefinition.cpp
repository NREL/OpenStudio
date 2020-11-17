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

#include "SteamEquipmentDefinition.hpp"
#include "SteamEquipmentDefinition_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SteamEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SteamEquipmentDefinition_Impl::SteamEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoadDefinition_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SteamEquipmentDefinition::iddObjectType());
    }

    SteamEquipmentDefinition_Impl::SteamEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : SpaceLoadDefinition_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SteamEquipmentDefinition::iddObjectType());
    }

    SteamEquipmentDefinition_Impl::SteamEquipmentDefinition_Impl(const SteamEquipmentDefinition_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadDefinition_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SteamEquipmentDefinition_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: output is listed in SteamEquipment instead
      return result;
    }

    IddObjectType SteamEquipmentDefinition_Impl::iddObjectType() const {
      return SteamEquipmentDefinition::iddObjectType();
    }

    std::string SteamEquipmentDefinition_Impl::designLevelCalculationMethod() const {
      boost::optional<std::string> value = getString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SteamEquipmentDefinition_Impl::designLevel() const {
      return getDouble(OS_SteamEquipment_DefinitionFields::DesignLevel, true);
    }

    boost::optional<double> SteamEquipmentDefinition_Impl::wattsperSpaceFloorArea() const {
      return getDouble(OS_SteamEquipment_DefinitionFields::WattsperSpaceFloorArea, true);
    }

    boost::optional<double> SteamEquipmentDefinition_Impl::wattsperPerson() const {
      return getDouble(OS_SteamEquipment_DefinitionFields::WattsperPerson, true);
    }

    double SteamEquipmentDefinition_Impl::fractionLatent() const {
      boost::optional<double> value = getDouble(OS_SteamEquipment_DefinitionFields::FractionLatent, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SteamEquipmentDefinition_Impl::isFractionLatentDefaulted() const {
      return isEmpty(OS_SteamEquipment_DefinitionFields::FractionLatent);
    }

    double SteamEquipmentDefinition_Impl::fractionRadiant() const {
      boost::optional<double> value = getDouble(OS_SteamEquipment_DefinitionFields::FractionRadiant, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SteamEquipmentDefinition_Impl::isFractionRadiantDefaulted() const {
      return isEmpty(OS_SteamEquipment_DefinitionFields::FractionRadiant);
    }

    double SteamEquipmentDefinition_Impl::fractionLost() const {
      boost::optional<double> value = getDouble(OS_SteamEquipment_DefinitionFields::FractionLost, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SteamEquipmentDefinition_Impl::isFractionLostDefaulted() const {
      return isEmpty(OS_SteamEquipment_DefinitionFields::FractionLost);
    }

    bool SteamEquipmentDefinition_Impl::setDesignLevel(boost::optional<double> designLevel) {
      bool result = false;
      if (designLevel) {
        result = setDouble(OS_SteamEquipment_DefinitionFields::DesignLevel, designLevel.get());
        if (result) {
          result = setString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod, "EquipmentLevel");
          OS_ASSERT(result);
          result = setWattsperSpaceFloorArea(boost::none);
          OS_ASSERT(result);
          result = setWattsperPerson(boost::none);
          OS_ASSERT(result);
        }
      } else {
        result = setString(OS_SteamEquipment_DefinitionFields::DesignLevel, "");
      }
      return result;
    }

    bool SteamEquipmentDefinition_Impl::setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea) {
      bool result = false;
      if (wattsperSpaceFloorArea) {
        result = setDouble(OS_SteamEquipment_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
        if (result) {
          result = setString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Area");
          OS_ASSERT(result);
          result = setDesignLevel(boost::none);
          OS_ASSERT(result);
          result = setWattsperPerson(boost::none);
          OS_ASSERT(result);
        }
      } else {
        result = setString(OS_SteamEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
      }
      return result;
    }

    bool SteamEquipmentDefinition_Impl::setWattsperPerson(boost::optional<double> wattsperPerson) {
      bool result = false;
      if (wattsperPerson) {
        result = setDouble(OS_SteamEquipment_DefinitionFields::WattsperPerson, wattsperPerson.get());
        if (result) {
          result = setString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Person");
          OS_ASSERT(result);
          result = setDesignLevel(boost::none);
          OS_ASSERT(result);
          result = setWattsperSpaceFloorArea(boost::none);
          OS_ASSERT(result);
        }
      } else {
        result = setString(OS_SteamEquipment_DefinitionFields::WattsperPerson, "");
      }
      return result;
    }

    bool SteamEquipmentDefinition_Impl::setFractionLatent(double fractionLatent) {

      double fractionRadiantAndLost = fractionRadiant() + fractionLost();
      if ((fractionLatent + fractionRadiantAndLost) > 1.0) {
        LOG(Error, "Radiant Fraction and Lost Fraction sum to " << fractionRadiantAndLost << " and you supplied a Latent Fraction of "
                                                                << fractionLatent << " which would result in a sum greater than 1.0");
        return false;
      }
      bool result = setDouble(OS_SteamEquipment_DefinitionFields::FractionLatent, fractionLatent);
      return result;
    }

    void SteamEquipmentDefinition_Impl::resetFractionLatent() {
      bool result = setString(OS_SteamEquipment_DefinitionFields::FractionLatent, "");
      OS_ASSERT(result);
    }

    bool SteamEquipmentDefinition_Impl::setFractionRadiant(double fractionRadiant) {

      double fractionLatentAndLost = fractionLatent() + fractionLost();
      if ((fractionRadiant + fractionLatentAndLost) > 1.0) {
        LOG(Error, "Latent Fraction and Lost Fraction sum to " << fractionLatentAndLost << " and you supplied a Radiant Fraction of "
                                                               << fractionRadiant << " which would result in a sum greater than 1.0");
        return false;
      }
      bool result = setDouble(OS_SteamEquipment_DefinitionFields::FractionRadiant, fractionRadiant);
      return result;
    }

    void SteamEquipmentDefinition_Impl::resetFractionRadiant() {
      bool result = setString(OS_SteamEquipment_DefinitionFields::FractionRadiant, "");
      OS_ASSERT(result);
    }

    bool SteamEquipmentDefinition_Impl::setFractionLost(double fractionLost) {

      double fractionLatentAndRadiant = fractionLatent() + fractionRadiant();
      if ((fractionLost + fractionLatentAndRadiant) > 1.0) {
        LOG(Error, "Latent Fraction and Radiant Fraction sum to " << fractionLatentAndRadiant << " and you supplied a Lost Fraction of "
                                                                  << fractionLost << " which would result in a sum greater than 1.0");
        return false;
      }
      bool result = setDouble(OS_SteamEquipment_DefinitionFields::FractionLost, fractionLost);
      return result;
    }

    void SteamEquipmentDefinition_Impl::resetFractionLost() {
      bool result = setString(OS_SteamEquipment_DefinitionFields::FractionLost, "");
      OS_ASSERT(result);
    }

    double SteamEquipmentDefinition_Impl::getDesignLevel(double floorArea, double numPeople) const {
      std::string method = designLevelCalculationMethod();

      if (openstudio::istringEqual("EquipmentLevel", method)) {
        return designLevel().get();
      } else if (openstudio::istringEqual("Watts/Area", method)) {
        return wattsperSpaceFloorArea().get() * floorArea;
      } else if (openstudio::istringEqual("Watts/Person", method)) {
        return wattsperPerson().get() * numPeople;
      }

      OS_ASSERT(false);
      return 0.0;
    }

    double SteamEquipmentDefinition_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const {
      std::string method = designLevelCalculationMethod();

      if (openstudio::istringEqual("EquipmentLevel", method)) {
        return designLevel().get() / floorArea;
      } else if (openstudio::istringEqual("Watts/Area", method)) {
        return wattsperSpaceFloorArea().get();
      } else if (openstudio::istringEqual("Watts/Person", method)) {
        return wattsperPerson().get() * numPeople / floorArea;
      }

      OS_ASSERT(false);
      return 0.0;
    }

    double SteamEquipmentDefinition_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
      std::string method = designLevelCalculationMethod();

      if (openstudio::istringEqual("EquipmentLevel", method)) {
        return designLevel().get() / numPeople;
      } else if (openstudio::istringEqual("Watts/Area", method)) {
        return wattsperSpaceFloorArea().get() * floorArea / numPeople;
      } else if (openstudio::istringEqual("Watts/Person", method)) {
        return wattsperPerson().get();
      }

      OS_ASSERT(false);
      return 0.0;
    }

    bool SteamEquipmentDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople) {
      if (openstudio::istringEqual("equipmentlevel", method)) {
        return setDesignLevel(getDesignLevel(floorArea, numPeople));
      } else if (openstudio::istringEqual("watts/area", method)) {
        return setWattsperSpaceFloorArea(getPowerPerFloorArea(floorArea, numPeople));
      } else if (openstudio::istringEqual("watts/person", method)) {
        return setWattsperPerson(getPowerPerPerson(floorArea, numPeople));
      }

      return false;
    }

    std::vector<EMSActuatorNames> SteamEquipmentDefinition_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"SteamEquipment", "District Heating Power Level"}};
      return actuators;
    }

    std::vector<std::string> SteamEquipmentDefinition_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Process Steam District Heat Design Level"};
      return types;
    }

  }  // namespace detail

  SteamEquipmentDefinition::SteamEquipmentDefinition(const Model& model) : SpaceLoadDefinition(SteamEquipmentDefinition::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SteamEquipmentDefinition_Impl>());
    setDesignLevel(0.0);
  }

  IddObjectType SteamEquipmentDefinition::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SteamEquipment_Definition);
    return result;
  }

  std::vector<std::string> SteamEquipmentDefinition::validDesignLevelCalculationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod);
  }

  std::string SteamEquipmentDefinition::designLevelCalculationMethod() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->designLevelCalculationMethod();
  }

  boost::optional<double> SteamEquipmentDefinition::designLevel() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->designLevel();
  }

  boost::optional<double> SteamEquipmentDefinition::wattsperSpaceFloorArea() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->wattsperSpaceFloorArea();
  }

  boost::optional<double> SteamEquipmentDefinition::wattsperPerson() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->wattsperPerson();
  }

  double SteamEquipmentDefinition::fractionLatent() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->fractionLatent();
  }

  bool SteamEquipmentDefinition::isFractionLatentDefaulted() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->isFractionLatentDefaulted();
  }

  double SteamEquipmentDefinition::fractionRadiant() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->fractionRadiant();
  }

  bool SteamEquipmentDefinition::isFractionRadiantDefaulted() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->isFractionRadiantDefaulted();
  }

  double SteamEquipmentDefinition::fractionLost() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->fractionLost();
  }

  bool SteamEquipmentDefinition::isFractionLostDefaulted() const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->isFractionLostDefaulted();
  }

  bool SteamEquipmentDefinition::setDesignLevel(double designLevel) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
  }

  bool SteamEquipmentDefinition::setWattsperSpaceFloorArea(double wattsperSpaceFloorArea) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setWattsperSpaceFloorArea(wattsperSpaceFloorArea);
  }

  bool SteamEquipmentDefinition::setWattsperPerson(double wattsperPerson) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setWattsperPerson(wattsperPerson);
  }

  bool SteamEquipmentDefinition::setFractionLatent(double fractionLatent) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setFractionLatent(fractionLatent);
  }

  void SteamEquipmentDefinition::resetFractionLatent() {
    getImpl<detail::SteamEquipmentDefinition_Impl>()->resetFractionLatent();
  }

  bool SteamEquipmentDefinition::setFractionRadiant(double fractionRadiant) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setFractionRadiant(fractionRadiant);
  }

  void SteamEquipmentDefinition::resetFractionRadiant() {
    getImpl<detail::SteamEquipmentDefinition_Impl>()->resetFractionRadiant();
  }

  bool SteamEquipmentDefinition::setFractionLost(double fractionLost) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setFractionLost(fractionLost);
  }

  void SteamEquipmentDefinition::resetFractionLost() {
    getImpl<detail::SteamEquipmentDefinition_Impl>()->resetFractionLost();
  }

  double SteamEquipmentDefinition::getDesignLevel(double floorArea, double numPeople) const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->getDesignLevel(floorArea, numPeople);
  }

  double SteamEquipmentDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->getPowerPerFloorArea(floorArea, numPeople);
  }

  double SteamEquipmentDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->getPowerPerPerson(floorArea, numPeople);
  }

  bool SteamEquipmentDefinition::setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople) {
    return getImpl<detail::SteamEquipmentDefinition_Impl>()->setDesignLevelCalculationMethod(method, floorArea, numPeople);
  }

  /// @cond
  SteamEquipmentDefinition::SteamEquipmentDefinition(std::shared_ptr<detail::SteamEquipmentDefinition_Impl> impl)
    : SpaceLoadDefinition(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
