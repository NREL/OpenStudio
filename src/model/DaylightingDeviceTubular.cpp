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

#include "DaylightingDeviceTubular.hpp"
#include "DaylightingDeviceTubular_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/OS_DaylightingDevice_Tubular_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    DaylightingDeviceTubular_Impl::DaylightingDeviceTubular_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DaylightingDeviceTubular::iddObjectType());
    }

    DaylightingDeviceTubular_Impl::DaylightingDeviceTubular_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DaylightingDeviceTubular::iddObjectType());
    }

    DaylightingDeviceTubular_Impl::DaylightingDeviceTubular_Impl(const DaylightingDeviceTubular_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DaylightingDeviceTubular_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType DaylightingDeviceTubular_Impl::iddObjectType() const {
      return DaylightingDeviceTubular::iddObjectType();
    }

    SubSurface DaylightingDeviceTubular_Impl::subSurfaceDome() const {
      OptionalSubSurface subSurface = getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_DaylightingDevice_TubularFields::DomeName);
      OS_ASSERT(subSurface);
      return subSurface.get();
    }

    SubSurface DaylightingDeviceTubular_Impl::subSurfaceDiffuser() const {
      OptionalSubSurface subSurface = getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_DaylightingDevice_TubularFields::DiffuserName);
      OS_ASSERT(subSurface);
      return subSurface.get();
    }

    ConstructionBase DaylightingDeviceTubular_Impl::construction() const {
      boost::optional<ConstructionBase> construction =
        getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DaylightingDevice_TubularFields::ConstructionName);
      OS_ASSERT(construction);
      return construction.get();
    }

    double DaylightingDeviceTubular_Impl::diameter() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_TubularFields::Diameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    double DaylightingDeviceTubular_Impl::totalLength() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_TubularFields::TotalLength, true);
      OS_ASSERT(value);
      return value.get();
    }

    double DaylightingDeviceTubular_Impl::effectiveThermalResistance() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_TubularFields::EffectiveThermalResistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<TransitionZone> DaylightingDeviceTubular_Impl::transitionZones() const {
      std::vector<TransitionZone> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo =
          group.cast<WorkspaceExtensibleGroup>().getTarget(OS_DaylightingDevice_TubularExtensibleFields::TransitionZoneName);
        boost::optional<ThermalZone> zone = wo->optionalCast<ThermalZone>();
        boost::optional<double> length =
          group.cast<WorkspaceExtensibleGroup>().getDouble(OS_DaylightingDevice_TubularExtensibleFields::TransitionZoneLength);

        if (zone && length) {
          TransitionZone transitionZone(zone.get(), length.get());
          result.push_back(transitionZone);
        }
      }

      return result;
    }

    unsigned int DaylightingDeviceTubular_Impl::numberofTransitionZones() const {
      return numExtensibleGroups();
    }

    bool DaylightingDeviceTubular_Impl::setConstruction(const ConstructionBase& construction) {
      return setPointer(OS_DaylightingDevice_TubularFields::ConstructionName, construction.handle());
    }

    bool DaylightingDeviceTubular_Impl::setDiameter(double diameter) {
      bool result = setDouble(OS_DaylightingDevice_TubularFields::Diameter, diameter);
      OS_ASSERT(result);
      return result;
    }

    bool DaylightingDeviceTubular_Impl::setTotalLength(double totalLength) {
      bool result = setDouble(OS_DaylightingDevice_TubularFields::TotalLength, totalLength);
      OS_ASSERT(result);
      return result;
    }

    bool DaylightingDeviceTubular_Impl::setEffectiveThermalResistance(double effectiveThermalResistance) {
      bool result = setDouble(OS_DaylightingDevice_TubularFields::EffectiveThermalResistance, effectiveThermalResistance);
      OS_ASSERT(result);
      return result;
    }

    bool DaylightingDeviceTubular_Impl::addTransitionZone(const TransitionZone& transitionZone) {
      bool result;

      // Push an extensible group
      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool zone = eg.setPointer(OS_DaylightingDevice_TubularExtensibleFields::TransitionZoneName, transitionZone.thermalZone().handle());
      bool length = eg.setDouble(OS_DaylightingDevice_TubularExtensibleFields::TransitionZoneLength, transitionZone.length());
      if (zone && length) {
        result = true;
      } else {
        // Something went wrong
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      result = true;

      return result;
    }

    bool DaylightingDeviceTubular_Impl::addTransitionZone(const ThermalZone& zone, double length) {
      // Make a transition zone, and then call the above function
      return addTransitionZone(TransitionZone{zone, length});
    }

    bool DaylightingDeviceTubular_Impl::removeTransitionZone(unsigned groupIndex) {
      bool result;

      unsigned int num = numberofTransitionZones();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      } else {
        result = false;
      }
      return result;
    }

    void DaylightingDeviceTubular_Impl::removeAllTransitionZones() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    bool DaylightingDeviceTubular_Impl::addTransitionZones(const std::vector<TransitionZone>& transitionZones) {
      for (const TransitionZone& transitionZone : transitionZones) {
        addTransitionZone(transitionZone);
      }
      return true;
    }

  }  // namespace detail

  DaylightingDeviceTubular::DaylightingDeviceTubular(const SubSurface& dome, const SubSurface& diffuser, const ConstructionBase& construction,
                                                     double diameter, double totalLength, double effectiveThermalResistance)
    : ModelObject(DaylightingDeviceTubular::iddObjectType(), dome.model()) {
    OS_ASSERT(getImpl<detail::DaylightingDeviceTubular_Impl>());

    bool subSurfaceOk = false;
    if (dome.allowDaylightingDeviceTubularDome() && diffuser.allowDaylightingDeviceTubularDiffuser()) {
      if (!dome.daylightingDeviceTubular() && !diffuser.daylightingDeviceTubular()) {
        subSurfaceOk = true;
      }
    }

    if (!subSurfaceOk) {
      this->remove();
      LOG_AND_THROW("Cannot create DaylightingDeviceTubular for SubSurface '" << dome.name().get() << "' and '" << diffuser.name().get() << "'");
    }

    bool test = setPointer(OS_DaylightingDevice_TubularFields::DomeName, dome.handle());
    OS_ASSERT(test);
    test = setPointer(OS_DaylightingDevice_TubularFields::DiffuserName, diffuser.handle());
    OS_ASSERT(test);

    test = setPointer(OS_DaylightingDevice_TubularFields::ConstructionName, construction.handle());
    OS_ASSERT(test);

    setDiameter(diameter);

    setTotalLength(totalLength);

    setEffectiveThermalResistance(effectiveThermalResistance);
  }

  DaylightingDeviceTubular::DaylightingDeviceTubular(const SubSurface& dome, const SubSurface& diffuser, const ConstructionBase& construction)
    : ModelObject(DaylightingDeviceTubular::iddObjectType(), dome.model()) {
    OS_ASSERT(getImpl<detail::DaylightingDeviceTubular_Impl>());

    bool subSurfaceOk = false;
    if (dome.allowDaylightingDeviceTubularDome() && diffuser.allowDaylightingDeviceTubularDiffuser()) {
      if (!dome.daylightingDeviceTubular() && !diffuser.daylightingDeviceTubular()) {
        subSurfaceOk = true;
      }
    }

    if (!subSurfaceOk) {
      this->remove();
      LOG_AND_THROW("Cannot create DaylightingDeviceTubular for SubSurface '" << dome.name().get() << "' and '" << diffuser.name().get() << "'");
    }

    bool test = setPointer(OS_DaylightingDevice_TubularFields::DomeName, dome.handle());
    OS_ASSERT(test);
    test = setPointer(OS_DaylightingDevice_TubularFields::DiffuserName, diffuser.handle());
    OS_ASSERT(test);

    test = setPointer(OS_DaylightingDevice_TubularFields::ConstructionName, construction.handle());
    OS_ASSERT(test);

    setDiameter(0.3556);  // DaylightingDeviceTubular.idf

    setTotalLength(1.4);  // DaylightingDeviceTubular.idf

    setEffectiveThermalResistance(0.28);  // ProposedEnergy+.idd
  }

  IddObjectType DaylightingDeviceTubular::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DaylightingDevice_Tubular);
    return result;
  }

  SubSurface DaylightingDeviceTubular::subSurfaceDome() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->subSurfaceDome();
  }

  SubSurface DaylightingDeviceTubular::subSurfaceDiffuser() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->subSurfaceDiffuser();
  }

  ConstructionBase DaylightingDeviceTubular::construction() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->construction();
  }

  double DaylightingDeviceTubular::diameter() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->diameter();
  }

  double DaylightingDeviceTubular::totalLength() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->totalLength();
  }

  double DaylightingDeviceTubular::effectiveThermalResistance() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->effectiveThermalResistance();
  }

  std::vector<TransitionZone> DaylightingDeviceTubular::transitionZones() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->transitionZones();
  }

  unsigned int DaylightingDeviceTubular::numberofTransitionZones() const {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->numberofTransitionZones();
  }

  bool DaylightingDeviceTubular::setConstruction(const ConstructionBase& construction) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->setConstruction(construction);
  }

  bool DaylightingDeviceTubular::setDiameter(double diameter) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->setDiameter(diameter);
  }

  bool DaylightingDeviceTubular::setTotalLength(double totalLength) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->setTotalLength(totalLength);
  }

  bool DaylightingDeviceTubular::setEffectiveThermalResistance(double effectiveThermalResistance) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->setEffectiveThermalResistance(effectiveThermalResistance);
  }

  bool DaylightingDeviceTubular::addTransitionZone(const TransitionZone& transitionZone) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->addTransitionZone(transitionZone);
  }

  bool DaylightingDeviceTubular::addTransitionZone(const ThermalZone& zone, double length) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->addTransitionZone(zone, length);
  }

  void DaylightingDeviceTubular::removeTransitionZone(int groupIndex) {
    getImpl<detail::DaylightingDeviceTubular_Impl>()->removeTransitionZone(groupIndex);
  }

  void DaylightingDeviceTubular::removeAllTransitionZones() {
    getImpl<detail::DaylightingDeviceTubular_Impl>()->removeAllTransitionZones();
  }

  bool DaylightingDeviceTubular::addTransitionZones(const std::vector<TransitionZone>& transitionZones) {
    return getImpl<detail::DaylightingDeviceTubular_Impl>()->addTransitionZones(transitionZones);
  }

  /// @cond
  DaylightingDeviceTubular::DaylightingDeviceTubular(std::shared_ptr<detail::DaylightingDeviceTubular_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
