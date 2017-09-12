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

#include "OtherEquipmentDefinition.hpp"
#include "OtherEquipmentDefinition_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_OtherEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ATTRIBUTE_IMPLEMENTATION(1,0,0,designLevel,DesignLevel,
                           OtherEquipmentDefinition,0,OS_OtherEquipment_Definition,DesignLevel)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea,
                           OtherEquipmentDefinition,0,OS_OtherEquipment_Definition,WattsperSpaceFloorArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperPerson,WattsperPerson,
                           OtherEquipmentDefinition,0,OS_OtherEquipment_Definition,WattsperPerson)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionRadiant,FractionRadiant,
                           OtherEquipmentDefinition,0,OS_OtherEquipment_Definition,FractionRadiant)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLatent,FractionLatent,
                           OtherEquipmentDefinition,0,OS_OtherEquipment_Definition,FractionLatent)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLost,FractionLost,
                           OtherEquipmentDefinition,0,OS_OtherEquipment_Definition,FractionLost)


  OtherEquipmentDefinition_Impl::OtherEquipmentDefinition_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OtherEquipmentDefinition::iddObjectType());
  }

  OtherEquipmentDefinition_Impl::OtherEquipmentDefinition_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OtherEquipmentDefinition::iddObjectType());
  }

  OtherEquipmentDefinition_Impl::OtherEquipmentDefinition_Impl(
      const OtherEquipmentDefinition_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& OtherEquipmentDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OtherEquipmentDefinition_Impl::iddObjectType() const {
    return OtherEquipmentDefinition::iddObjectType();
  }

  std::string OtherEquipmentDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_OtherEquipment_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> OtherEquipmentDefinition_Impl::designLevel() const {
    return getDouble(OS_OtherEquipment_DefinitionFields::DesignLevel,true);
  }

  boost::optional<double> OtherEquipmentDefinition_Impl::wattsperSpaceFloorArea() const {
    return getDouble(OS_OtherEquipment_DefinitionFields::WattsperSpaceFloorArea,true);
  }

  boost::optional<double> OtherEquipmentDefinition_Impl::wattsperPerson() const {
    return getDouble(OS_OtherEquipment_DefinitionFields::WattsperPerson,true);
  }

  double OtherEquipmentDefinition_Impl::fractionLatent() const {
    boost::optional<double> value = getDouble(OS_OtherEquipment_DefinitionFields::FractionLatent,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool OtherEquipmentDefinition_Impl::isFractionLatentDefaulted() const {
  //  return isEmpty(OS_OtherEquipment_DefinitionFields::FractionLatent);
  //}

  double OtherEquipmentDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_OtherEquipment_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool OtherEquipmentDefinition_Impl::isFractionRadiantDefaulted() const {
  //  return isEmpty(OS_OtherEquipment_DefinitionFields::FractionRadiant);
  //}

  double OtherEquipmentDefinition_Impl::fractionLost() const {
    boost::optional<double> value = getDouble(OS_OtherEquipment_DefinitionFields::FractionLost,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool OtherEquipmentDefinition_Impl::isFractionLostDefaulted() const {
  //  return isEmpty(OS_OtherEquipment_DefinitionFields::FractionLost);
  //}

  bool OtherEquipmentDefinition_Impl::setDesignLevel(boost::optional<double> designLevel) {
    bool result = false;
    if (designLevel) {
      result = setString(OS_OtherEquipment_DefinitionFields::DesignLevelCalculationMethod,"EquipmentLevel");
      OS_ASSERT(result);
      result = setDouble(OS_OtherEquipment_DefinitionFields::DesignLevel, designLevel.get());
      OS_ASSERT(result);
      result = setWattsperSpaceFloorArea(boost::none);
      OS_ASSERT(result);
      result = setWattsperPerson(boost::none);
    } else {
      result = setString(OS_OtherEquipment_DefinitionFields::DesignLevel, "");
    }

    return result;
  }

  bool OtherEquipmentDefinition_Impl::setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea) {
    bool result = false;
    if (wattsperSpaceFloorArea) {
      result = setString(OS_OtherEquipment_DefinitionFields::DesignLevelCalculationMethod,"Watts/Area");
      OS_ASSERT(result);
      setDesignLevel(boost::none);
      result = setDouble(OS_OtherEquipment_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
      OS_ASSERT(result);
      result = setWattsperPerson(boost::none);
    } else {
      result = setString(OS_OtherEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
    }
    return result;
  }

  bool OtherEquipmentDefinition_Impl::setWattsperPerson(boost::optional<double> wattsperPerson) {
    bool result = false;
    if (wattsperPerson) {
      result = setString(OS_OtherEquipment_DefinitionFields::DesignLevelCalculationMethod,"Watts/Person");
      OS_ASSERT(result);
      setDesignLevel(boost::none);
      result = setWattsperSpaceFloorArea(boost::none);
      OS_ASSERT(result);
      result = setDouble(OS_OtherEquipment_DefinitionFields::WattsperPerson, wattsperPerson.get());
    } else {
      result = setString(OS_OtherEquipment_DefinitionFields::WattsperPerson, "");
    }
    return result;
  }

  bool OtherEquipmentDefinition_Impl::setFractionLatent(double fractionLatent) {
    bool result = setDouble(OS_OtherEquipment_DefinitionFields::FractionLatent, fractionLatent);
    return result;
  }

  //void OtherEquipmentDefinition_Impl::resetFractionLatent() {
  //  bool result = setString(OS_OtherEquipment_DefinitionFields::FractionLatent, "");
  //  OS_ASSERT(result);
  //}

  bool OtherEquipmentDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_OtherEquipment_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  //void OtherEquipmentDefinition_Impl::resetFractionRadiant() {
  //  bool result = setString(OS_OtherEquipment_DefinitionFields::FractionRadiant, "");
  //  OS_ASSERT(result);
  //}

  bool OtherEquipmentDefinition_Impl::setFractionLost(double fractionLost) {
    bool result = setDouble(OS_OtherEquipment_DefinitionFields::FractionLost, fractionLost);
    return result;
  }

  //void OtherEquipmentDefinition_Impl::resetFractionLost() {
  //  bool result = setString(OS_OtherEquipment_DefinitionFields::FractionLost, "");
  //  OS_ASSERT(result);
  //}

  double OtherEquipmentDefinition_Impl::getDesignLevel(double floorArea, double numPeople) const {
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

  double OtherEquipmentDefinition_Impl::getPowerPerFloorArea(double floorArea,
                                                                  double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return designLevel().get() / floorArea;
    }
    else if (method == "Watts/Area") {
      return wattsperSpaceFloorArea().get();
    }
    else if (method == "Watts/Person") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return wattsperPerson().get() * numPeople / floorArea;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double OtherEquipmentDefinition_Impl::getPowerPerPerson(double floorArea, double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return designLevel().get() / numPeople;
    }
    else if (method == "Watts/Area") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return wattsperSpaceFloorArea().get() * floorArea / numPeople;
    }
    else if (method == "Watts/Person") {
      return wattsperPerson().get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  bool OtherEquipmentDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
                                                                      double floorArea,
                                                                      double numPeople)
  {
    std::string wmethod(method);
    boost::to_lower(wmethod);

    if (wmethod == "equipmentlevel") {
      setDesignLevel(getDesignLevel(floorArea,numPeople));
      return true;
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

OtherEquipmentDefinition::OtherEquipmentDefinition(const Model& model)
  : SpaceLoadDefinition(OtherEquipmentDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::OtherEquipmentDefinition_Impl>());
  setDesignLevel(0.0);
}

IddObjectType OtherEquipmentDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_OtherEquipment_Definition);
  return result;
}

std::vector<std::string> OtherEquipmentDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_OtherEquipment_DefinitionFields::DesignLevelCalculationMethod);
}

std::string OtherEquipmentDefinition::designLevelCalculationMethod() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->designLevelCalculationMethod();
}

boost::optional<double> OtherEquipmentDefinition::designLevel() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->designLevel();
}

boost::optional<double> OtherEquipmentDefinition::wattsperSpaceFloorArea() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->wattsperSpaceFloorArea();
}

boost::optional<double> OtherEquipmentDefinition::wattsperPerson() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->wattsperPerson();
}

double OtherEquipmentDefinition::fractionLatent() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->fractionLatent();
}

bool OtherEquipmentDefinition::isFractionLatentDefaulted() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->isFractionLatentDefaulted();
}

double OtherEquipmentDefinition::fractionRadiant() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->fractionRadiant();
}

bool OtherEquipmentDefinition::isFractionRadiantDefaulted() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->isFractionRadiantDefaulted();
}

double OtherEquipmentDefinition::fractionLost() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->fractionLost();
}

bool OtherEquipmentDefinition::isFractionLostDefaulted() const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->isFractionLostDefaulted();
}

void OtherEquipmentDefinition::setDesignLevel(double designLevel) {
  getImpl<detail::OtherEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
}

bool OtherEquipmentDefinition::setWattsperSpaceFloorArea(double wattsperSpaceFloorArea) {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->setWattsperSpaceFloorArea(wattsperSpaceFloorArea);
}

bool OtherEquipmentDefinition::setWattsperPerson(double wattsperPerson) {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->setWattsperPerson(wattsperPerson);
}

bool OtherEquipmentDefinition::setFractionLatent(double fractionLatent) {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->setFractionLatent(fractionLatent);
}

void OtherEquipmentDefinition::resetFractionLatent() {
  getImpl<detail::OtherEquipmentDefinition_Impl>()->resetFractionLatent();
}

bool OtherEquipmentDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

void OtherEquipmentDefinition::resetFractionRadiant() {
  getImpl<detail::OtherEquipmentDefinition_Impl>()->resetFractionRadiant();
}

bool OtherEquipmentDefinition::setFractionLost(double fractionLost) {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->setFractionLost(fractionLost);
}

void OtherEquipmentDefinition::resetFractionLost() {
  getImpl<detail::OtherEquipmentDefinition_Impl>()->resetFractionLost();
}

double OtherEquipmentDefinition::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->getDesignLevel(floorArea,numPeople);
}

double OtherEquipmentDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const
{
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double OtherEquipmentDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

bool OtherEquipmentDefinition::setDesignLevelCalculationMethod(const std::string& method,
                                     double floorArea,
                                     double numPeople)
{
  return getImpl<detail::OtherEquipmentDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
OtherEquipmentDefinition::OtherEquipmentDefinition(std::shared_ptr<detail::OtherEquipmentDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

