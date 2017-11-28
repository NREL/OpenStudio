/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "SteamEquipmentDefinition.hpp"
#include "SteamEquipmentDefinition_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SteamEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SteamEquipmentDefinition_Impl::SteamEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SteamEquipmentDefinition::iddObjectType());
  }

  SteamEquipmentDefinition_Impl::SteamEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SteamEquipmentDefinition::iddObjectType());
  }

  SteamEquipmentDefinition_Impl::SteamEquipmentDefinition_Impl(const SteamEquipmentDefinition_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SteamEquipmentDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SteamEquipmentDefinition_Impl::iddObjectType() const {
    return SteamEquipmentDefinition::iddObjectType();
  }

  ATTRIBUTE_IMPLEMENTATION(1,0,0,designLevel,DesignLevel,
                           SteamEquipmentDefinition,0,OS_SteamEquipment_Definition,DesignLevel)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea,
                           SteamEquipmentDefinition,0,OS_SteamEquipment_Definition,WattsperSpaceFloorArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperPerson,WattsperPerson,
                           SteamEquipmentDefinition,0,OS_SteamEquipment_Definition,WattsperPerson)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionRadiant,FractionRadiant,
                           SteamEquipmentDefinition,0,OS_SteamEquipment_Definition,FractionRadiant)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLatent,FractionLatent,
                           SteamEquipmentDefinition,0,OS_SteamEquipment_Definition,FractionLatent)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLost,FractionLost,
                           SteamEquipmentDefinition,0,OS_SteamEquipment_Definition,FractionLost)

  std::string SteamEquipmentDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> SteamEquipmentDefinition_Impl::designLevel() const {
    return getDouble(OS_SteamEquipment_DefinitionFields::DesignLevel,true);
  }

  boost::optional<double> SteamEquipmentDefinition_Impl::wattsperSpaceFloorArea() const {
    return getDouble(OS_SteamEquipment_DefinitionFields::WattsperSpaceFloorArea,true);
  }

  boost::optional<double> SteamEquipmentDefinition_Impl::wattsperPerson() const {
    return getDouble(OS_SteamEquipment_DefinitionFields::WattsperPerson,true);
  }

  double SteamEquipmentDefinition_Impl::fractionLatent() const {
    boost::optional<double> value = getDouble(OS_SteamEquipment_DefinitionFields::FractionLatent,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool SteamEquipmentDefinition_Impl::isFractionLatentDefaulted() const {
  //  return isEmpty(OS_SteamEquipment_DefinitionFields::FractionLatent);
  //}

  double SteamEquipmentDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_SteamEquipment_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool SteamEquipmentDefinition_Impl::isFractionRadiantDefaulted() const {
  //  return isEmpty(OS_SteamEquipment_DefinitionFields::FractionRadiant);
  //}

  double SteamEquipmentDefinition_Impl::fractionLost() const {
    boost::optional<double> value = getDouble(OS_SteamEquipment_DefinitionFields::FractionLost,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool SteamEquipmentDefinition_Impl::isFractionLostDefaulted() const {
  //  return isEmpty(OS_SteamEquipment_DefinitionFields::FractionLost);
  //}

  bool SteamEquipmentDefinition_Impl::setDesignLevel(boost::optional<double> designLevel) {
    bool result = false;
    if (designLevel) {
      result = setDouble(OS_SteamEquipment_DefinitionFields::DesignLevel, designLevel.get());
      if (result) {
        result = setString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod,"EquipmentLevel");
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
        result = setString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod,"Watts/Area");
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
        result = setString(OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod,"Watts/Person");
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
    bool result = setDouble(OS_SteamEquipment_DefinitionFields::FractionLatent, fractionLatent);
    return result;
  }

  //void SteamEquipmentDefinition_Impl::resetFractionLatent() {
  //  bool result = setString(OS_SteamEquipment_DefinitionFields::FractionLatent, "");
  //  OS_ASSERT(result);
  //}

  bool SteamEquipmentDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_SteamEquipment_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  //void SteamEquipmentDefinition_Impl::resetFractionRadiant() {
  //  bool result = setString(OS_SteamEquipment_DefinitionFields::FractionRadiant, "");
  //  OS_ASSERT(result);
  //}

  bool SteamEquipmentDefinition_Impl::setFractionLost(double fractionLost) {
    bool result = setDouble(OS_SteamEquipment_DefinitionFields::FractionLost, fractionLost);
    return result;
  }

  //void SteamEquipmentDefinition_Impl::resetFractionLost() {
  //  bool result = setString(OS_SteamEquipment_DefinitionFields::FractionLost, "");
  //  OS_ASSERT(result);
  //}

  double SteamEquipmentDefinition_Impl::getDesignLevel(double floorArea, double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      return designLevel().get();
    }
    else if (method == "Watts/Area") {
      return wattsperSpaceFloorArea().get() * floorArea;
    }
    else if (method == "Watts/Person") {
      return wattsperPerson().get() * numPeople;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double SteamEquipmentDefinition_Impl::getPowerPerFloorArea(double floorArea,
                                                             double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      return designLevel().get() / floorArea;
    }
    else if (method == "Watts/Area") {
      return wattsperSpaceFloorArea().get();
    }
    else if (method == "Watts/Person") {
      return wattsperPerson().get() * numPeople / floorArea;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double SteamEquipmentDefinition_Impl::getPowerPerPerson(double floorArea,
                                                          double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      return designLevel().get() / numPeople;
    }
    else if (method == "Watts/Area") {
      return wattsperSpaceFloorArea().get() * floorArea / numPeople;
    }
    else if (method == "Watts/Person") {
      return wattsperPerson().get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  bool SteamEquipmentDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
                                                                      double floorArea,
                                                                      double numPeople)
  {
    std::string wmethod(method);
    boost::to_lower(wmethod);

    if (wmethod == "equipmentlevel") {
      return setDesignLevel(getDesignLevel(floorArea,numPeople));
    }
    else if (wmethod == "watts/area") {
      return setWattsperSpaceFloorArea(getPowerPerFloorArea(floorArea,numPeople));
    }
    else if (wmethod == "watts/person") {
      return setWattsperPerson(getPowerPerPerson(floorArea,numPeople));
    }

    return false;
  }

} // detail

SteamEquipmentDefinition::SteamEquipmentDefinition(const Model& model)
  : SpaceLoadDefinition(SteamEquipmentDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SteamEquipmentDefinition_Impl>());
  setDesignLevel(0.0);
}

IddObjectType SteamEquipmentDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SteamEquipment_Definition);
  return result;
}

std::vector<std::string> SteamEquipmentDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SteamEquipment_DefinitionFields::DesignLevelCalculationMethod);
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
  return getImpl<detail::SteamEquipmentDefinition_Impl>()->getDesignLevel(floorArea,numPeople);
}

double SteamEquipmentDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::SteamEquipmentDefinition_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double SteamEquipmentDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::SteamEquipmentDefinition_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

bool SteamEquipmentDefinition::setDesignLevelCalculationMethod(const std::string& method,
                                     double floorArea,
                                     double numPeople)
{
  return getImpl<detail::SteamEquipmentDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
SteamEquipmentDefinition::SteamEquipmentDefinition(std::shared_ptr<detail::SteamEquipmentDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

