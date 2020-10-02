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

#include "ThermalStoragePcmSimple.hpp"
#include "ThermalStoragePcmSimple_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ThermalStorage_Pcm_Simple_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermalStoragePcmSimple_Impl::ThermalStoragePcmSimple_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermalStoragePcmSimple::iddObjectType());
  }

  ThermalStoragePcmSimple_Impl::ThermalStoragePcmSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermalStoragePcmSimple::iddObjectType());
  }

  ThermalStoragePcmSimple_Impl::ThermalStoragePcmSimple_Impl(const ThermalStoragePcmSimple_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ThermalStoragePcmSimple_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result{
      // TODO
    };
    return result;
  }

  IddObjectType ThermalStoragePcmSimple_Impl::iddObjectType() const {
    return ThermalStoragePcmSimple::iddObjectType();
  }

  std::string ThermalStoragePcmSimple_Impl::iceStorageType() const {
    boost::optional<std::string> value = getString(OS_ThermalStorage_Pcm_SimpleFields::IceStorageType, true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStoragePcmSimple_Impl::capacity() const {
    boost::optional<double> value = getString(OS_ThermalStorage_Pcm_SimpleFields::Capacity, true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStoragePcmSimple_Impl::onsetTemperatureOfPhaseChange() const {
    boost::optional<double> value = getString(OS_ThermalStorage_Pcm_SimpleFields::OnsetTemperatureOfPhaseChange, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStoragePcmSimple_Impl::isOnsetTemperatureOfPhaseChangeDefaulted() const {
    return isEmpty(OS_ThermalStorage_Pcm_SimpleFields::OnsetTemperatureOfPhaseChange);
  }

  double ThermalStoragePcmSimple_Impl::finishTemperatureOfPhaseChange() const {
    boost::optional<double> value = getString(OS_ThermalStorage_Pcm_SimpleFields::FinishTemperatureOfPhaseChange, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStoragePcmSimple_Impl::isFinishTemperatureOfPhaseChangeDefaulted() const {
    return isEmpty(OS_ThermalStorage_Pcm_SimpleFields::FinishTemperatureOfPhaseChange);
  }

  double ThermalStoragePcmSimple_Impl::uaAtSolidPhaseOfPhaseChangeMaterial() const {
    boost::optional<double> value = getString(OS_ThermalStorage_Pcm_SimpleFields::UAAtSolidPhaseOfPhaseChangeMaterial, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStoragePcmSimple_Impl::isUAAtSolidPhaseOfPhaseChangeMaterialDefaulted() const {
    return isEmpty(OS_ThermalStorage_Pcm_SimpleFields::UAAtSolidPhaseOfPhaseChangeMaterial);
  }

  double ThermalStoragePcmSimple_Impl::uaAtLiquidPhaseOfPhaseChangeMaterial() const {
    boost::optional<double> value = getString(OS_ThermalStorage_Pcm_SimpleFields::UAAtLiquidPhaseOfPhaseChangeMaterial, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStoragePcmSimple_Impl::isUAAtLiquidPhaseOfPhaseChangeMaterialDefaulted() const {
    return isEmpty(OS_ThermalStorage_Pcm_SimpleFields::UAAtLiquidPhaseOfPhaseChangeMaterial);
  }

  bool ThermalStoragePcmSimple_Impl::setIceStorageType(std::string iceStorageType) {
    bool result = setString(OS_ThermalStorage_Ice_DetailedFields::IceStorageType, iceStorageType);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalStoragePcmSimple_Impl::setCapacity(double capacity) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::Capacity, capacity);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalStoragePcmSimple_Impl::setOnsetTemperatureOfPhaseChange(double onsetTemperatureOfPhaseChange) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::OnsetTemperatureOfPhaseChange, onsetTemperatureOfPhaseChange);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStoragePcmSimple_Impl::resetOnsetTemperatureOfPhaseChange() {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::OnsetTemperatureOfPhaseChange, "");
    OS_ASSERT(result);
  }

  bool ThermalStoragePcmSimple_Impl::setFinishTemperatureOfPhaseChange(double finishTemperatureOfPhaseChange) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::FinishTemperatureOfPhaseChange, finishTemperatureOfPhaseChange);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStoragePcmSimple_Impl::resetFinishTemperatureOfPhaseChange() {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::FinishTemperatureOfPhaseChange, "");
    OS_ASSERT(result);
  }

  bool ThermalStoragePcmSimple_Impl::setUAAtSolidPhaseOfPhaseChangeMaterial(double uaAtSolidPhaseOfPhaseChangeMaterial) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::UAAtSolidPhaseOfPhaseChangeMaterial, uaAtSolidPhaseOfPhaseChangeMaterial);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStoragePcmSimple_Impl::resetUAAtSolidPhaseOfPhaseChangeMaterial() {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::UAAtSolidPhaseOfPhaseChangeMaterial, "");
    OS_ASSERT(result);
  }

  bool ThermalStoragePcmSimple_Impl::setUAAtLiquidPhaseOfPhaseChangeMaterial(double uaAtLiquidPhaseofPhaseChangeMaterial) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::UAAtLiquidPhaseOfPhaseChangeMaterial, uaAtLiquidPhaseofPhaseChangeMaterial);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStoragePcmSimple_Impl::resetUAAtLiquidPhaseOfPhaseChangeMaterial() {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::UAAtLiquidPhaseOfPhaseChangeMaterial, "");
    OS_ASSERT(result);
  }

  unsigned ThermalStoragePcmSimple_Impl::inletPort() const {
    return OS_ThermalStorage_Pcm_SimpleFields::InletNodeName;
  }

  unsigned ThermalStoragePcmSimple_Impl::outletPort() const {
    return OS_ThermalStorage_Pcm_SimpleFields::OutletNodeName;
  }

  bool ThermalStoragePcmSimple_Impl::addToNode(Node & node)
  {
    if( auto plant = node.plantLoop() ) {
      if( ! plant->demandComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

  ModelObject ThermalStoragePcmSimple_Impl::clone(Model model) const
  {
    auto newMo = StraightComponent_Impl::clone(model).cast<ThermalStoragePcmSimple>();

    return newMo;
  }

} // detail

ThermalStoragePcmSimple::ThermalStoragePcmSimple(const Model& model)
  : StraightComponent(ThermalStoragePcmSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermalStoragePcmSimple_Impl>());

  setIceStorageType("IceOnCoilInternal"); // TODO
  setCapacity(0.0); // TODO
}

IddObjectType ThermalStoragePcmSimple::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ThermalStorage_Pcm_Simple);
}

std::string ThermalStoragePcmSimple::iceStorageType() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->iceStorageType();
}

double ThermalStoragePcmSimple::capacity() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->capacity();
}

double ThermalStoragePcmSimple::onsetTemperatureOfPhaseChange() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->onsetTemperatureOfPhaseChange();
}

bool ThermalStoragePcmSimple::isOnsetTemperatureOfPhaseChangeDefaulted() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->isOnsetTemperatureOfPhaseChangeDefaulted();
}

double ThermalStoragePcmSimple::finishTemperatureOfPhaseChange() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->finishTemperatureOfPhaseChange();
}

bool ThermalStoragePcmSimple::isFinishTemperatureOfPhaseChangeDefaulted() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->isFinishTemperatureOfPhaseChangeDefaulted();
}

double ThermalStoragePcmSimple::uaAtSolidPhaseOfPhaseChangeMaterial() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->uaAtSolidPhaseOfPhaseChangeMaterial();
}

bool ThermalStoragePcmSimple::isUAAtSolidPhaseOfPhaseChangeMaterialDefaulted() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->isUAAtSolidPhaseOfPhaseChangeMaterialDefaulted();
}

double ThermalStoragePcmSimple::uaAtLiquidPhaseOfPhaseChangeMaterial() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->uaAtLiquidPhaseOfPhaseChangeMaterial();
}

bool ThermalStoragePcmSimple::isUAAtLiquidPhaseOfPhaseChangeMaterialDefaulted() const {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->isUAAtLiquidPhaseOfPhaseChangeMaterialDefaulted();
}

bool ThermalStoragePcmSimple::setIceStorageType(std::string iceStorageType) {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->setIceStorageType(iceStorageType);
}

bool ThermalStoragePcmSimple::setCapacity(double capacity) {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->setCapacity(capacity);
}

bool ThermalStoragePcmSimple::setOnsetTemperatureOfPhaseChange(double onsetTemperatureOfPhaseChange) {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->setOnsetTemperatureOfPhaseChange(onsetTemperatureOfPhaseChange);
}

void ThermalStoragePcmSimple::resetOnsetTemperatureOfPhaseChange() {
  getImpl<detail::ThermalStoragePcmSimple_Impl>()->resetOnsetTemperatureOfPhaseChange();
}

bool ThermalStoragePcmSimple::setFinishTemperatureOfPhaseChange(double finishTemperatureOfPhaseChange) {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->setFinishTemperatureOfPhaseChange(finishTemperatureOfPhaseChange);
}

void ThermalStoragePcmSimple::resetFinishTemperatureOfPhaseChange() {
  getImpl<detail::ThermalStoragePcmSimple_Impl>()->resetFinishTemperatureOfPhaseChange();
}

bool ThermalStoragePcmSimple::setUAAtSolidPhaseOfPhaseChangeMaterial(double uaAtSolidPhaseOfPhaseChangeMaterial) {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->setUAAtSolidPhaseOfPhaseChangeMaterial(uaAtSolidPhaseOfPhaseChangeMaterial);
}

void ThermalStoragePcmSimple::resetUAAtSolidPhaseOfPhaseChangeMaterial() {
  getImpl<detail::ThermalStoragePcmSimple_Impl>()->resetUAAtSolidPhaseOfPhaseChangeMaterial();
}

bool ThermalStoragePcmSimple::setUAAtLiquidPhaseOfPhaseChangeMaterial(double uaAtLiquidPhaseofPhaseChangeMaterial) {
  return getImpl<detail::ThermalStoragePcmSimple_Impl>()->setUAAtLiquidPhaseOfPhaseChangeMaterial(uaAtLiquidPhaseofPhaseChangeMaterial);
}

void ThermalStoragePcmSimple::resetUAAtLiquidPhaseOfPhaseChangeMaterial() {
  getImpl<detail::ThermalStoragePcmSimple_Impl>()->resetUAAtLiquidPhaseOfPhaseChangeMaterial();
}

/// @cond
ThermalStoragePcmSimple::ThermalStoragePcmSimple(std::shared_ptr<detail::ThermalStoragePcmSimple_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
