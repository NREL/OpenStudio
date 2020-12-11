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

#include "ShadowCalculation.hpp"
#include "ShadowCalculation_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ModelObject.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ShadowCalculation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    ShadowCalculation_Impl::ShadowCalculation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ShadowCalculation::iddObjectType());
    }

    ShadowCalculation_Impl::ShadowCalculation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ShadowCalculation::iddObjectType());
    }

    ShadowCalculation_Impl::ShadowCalculation_Impl(const ShadowCalculation_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ShadowCalculation_Impl::parent() const {
      boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
      return result;
    }

    // set the parent, child may have to call methods on the parent
    bool ShadowCalculation_Impl::setParent(ParentObject& newParent) {
      if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
        return true;
      }
      return false;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ShadowCalculation_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ShadowCalculation_Impl::iddObjectType() const {
      return ShadowCalculation::iddObjectType();
    }

    std::string ShadowCalculation_Impl::shadingCalculationMethod() const {
      boost::optional<std::string> value = getString(OS_ShadowCalculationFields::ShadingCalculationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string ShadowCalculation_Impl::shadingCalculationUpdateFrequencyMethod() const {
      boost::optional<std::string> value = getString(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ShadowCalculation_Impl::isShadingCalculationUpdateFrequencyMethodDefaulted() const {
      return isEmpty(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod);
    }

    int ShadowCalculation_Impl::shadingCalculationUpdateFrequency() const {
      boost::optional<int> value = getInt(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ShadowCalculation_Impl::isShadingCalculationUpdateFrequencyDefaulted() const {
      return isEmpty(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequency);
    }

    int ShadowCalculation_Impl::maximumFiguresInShadowOverlapCalculations() const {
      boost::optional<int> value = getInt(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ShadowCalculation_Impl::isMaximumFiguresInShadowOverlapCalculationsDefaulted() const {
      return isEmpty(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations);
    }

    std::string ShadowCalculation_Impl::polygonClippingAlgorithm() const {
      boost::optional<std::string> value = getString(OS_ShadowCalculationFields::PolygonClippingAlgorithm, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ShadowCalculation_Impl::pixelCountingResolution() const {
      boost::optional<int> value = getInt(OS_ShadowCalculationFields::PixelCountingResolution, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string ShadowCalculation_Impl::skyDiffuseModelingAlgorithm() const {
      boost::optional<std::string> value = getString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ShadowCalculation_Impl::outputExternalShadingCalculationResults() const {
      return getBooleanFieldValue(OS_ShadowCalculationFields::OutputExternalShadingCalculationResults);
    }

    bool ShadowCalculation_Impl::disableSelfShadingWithinShadingZoneGroups() const {
      return getBooleanFieldValue(OS_ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups);
    }

    bool ShadowCalculation_Impl::disableSelfShadingFromShadingZoneGroupstoOtherZones() const {
      return getBooleanFieldValue(OS_ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones);
    }

    bool ShadowCalculation_Impl::setShadingCalculationMethod(const std::string& shadingCalculationMethod) {
      bool result = setString(OS_ShadowCalculationFields::ShadingCalculationMethod, shadingCalculationMethod);
      return result;
    }

    bool ShadowCalculation_Impl::setShadingCalculationUpdateFrequencyMethod(const std::string& shadingCalculationUpdateFrequencyMethod) {
      bool result = setString(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod, shadingCalculationUpdateFrequencyMethod);
      return result;
    }

    void ShadowCalculation_Impl::resetShadingCalculationUpdateFrequencyMethod() {
      bool result = setString(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod, "");
      OS_ASSERT(result);
    }

    bool ShadowCalculation_Impl::setShadingCalculationUpdateFrequency(int shadingCalculationUpdateFrequency) {
      bool result = setInt(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequency, shadingCalculationUpdateFrequency);
      return result;
    }

    void ShadowCalculation_Impl::resetShadingCalculationUpdateFrequency() {
      bool result = setString(OS_ShadowCalculationFields::ShadingCalculationUpdateFrequency, "");
      OS_ASSERT(result);
    }

    bool ShadowCalculation_Impl::setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations) {
      bool result = setInt(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, maximumFiguresInShadowOverlapCalculations);
      return result;
    }

    void ShadowCalculation_Impl::resetMaximumFiguresInShadowOverlapCalculations() {
      bool result = setString(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, "");
      OS_ASSERT(result);
    }

    bool ShadowCalculation_Impl::setPolygonClippingAlgorithm(const std::string& polygonClippingAlgorithm) {
      bool result = setString(OS_ShadowCalculationFields::PolygonClippingAlgorithm, polygonClippingAlgorithm);
      return result;
    }

    void ShadowCalculation_Impl::resetPolygonClippingAlgorithm() {
      bool result = setString(OS_ShadowCalculationFields::PolygonClippingAlgorithm, "");
      OS_ASSERT(result);
    }

    bool ShadowCalculation_Impl::setPixelCountingResolution(int pixelCountingResolution) {
      bool result = setInt(OS_ShadowCalculationFields::PixelCountingResolution, pixelCountingResolution);
      OS_ASSERT(result);
      return result;
    }

    bool ShadowCalculation_Impl::setSkyDiffuseModelingAlgorithm(const std::string& skyDiffuseModelingAlgorithm) {
      bool result = setString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm, skyDiffuseModelingAlgorithm);
      return result;
    }

    void ShadowCalculation_Impl::resetSkyDiffuseModelingAlgorithm() {
      bool result = setString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm, "");
      OS_ASSERT(result);
    }

    bool ShadowCalculation_Impl::setOutputExternalShadingCalculationResults(bool outputExternalShadingCalculationResults) {
      return setBooleanFieldValue(OS_ShadowCalculationFields::OutputExternalShadingCalculationResults, outputExternalShadingCalculationResults);
    }

    bool ShadowCalculation_Impl::setDisableSelfShadingWithinShadingZoneGroups(bool disableSelfShadingWithinShadingZoneGroups) {
      return setBooleanFieldValue(OS_ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups, disableSelfShadingWithinShadingZoneGroups);
    }

    bool ShadowCalculation_Impl::setDisableSelfShadingFromShadingZoneGroupstoOtherZones(bool disableSelfShadingFromShadingZoneGroupstoOtherZones) {
      return setBooleanFieldValue(OS_ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones,
                                  disableSelfShadingFromShadingZoneGroupstoOtherZones);
    }

    unsigned int ShadowCalculation_Impl::numberofShadingZoneGroups() const {
      return numExtensibleGroups();
    }

    boost::optional<ModelObjectList> ShadowCalculation_Impl::getShadingZoneGroupModelObjectList(unsigned groupIndex) const {

      boost::optional<ModelObjectList> result;

      if (groupIndex >= numberofShadingZoneGroups()) {
        LOG(Error, "Asked to get Shading Zone Group with index " << groupIndex << ", but ShadowCalculation has just " << numberofShadingZoneGroups()
                                                                 << " Shading Zone Groups.");
        return result;
      }

      ModelExtensibleGroup group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();

      boost::optional<ModelObjectList> mo_list = group.getModelObjectTarget<ModelObjectList>(OS_ShadowCalculationExtensibleFields::ShadingZoneGroup);

      if (mo_list) {
        result = mo_list.get();
      } else {
        LOG_AND_THROW("ShadowCalculation does not appear to have a ModelObjectList for Shading Zone Group " << groupIndex);
      }

      return result;
    }

    std::vector<ThermalZone> ShadowCalculation_Impl::getShadingZoneGroup(unsigned groupIndex) const {

      std::vector<ThermalZone> result;

      if (auto _mo_list = getShadingZoneGroupModelObjectList(groupIndex)) {
        for (const ModelObject& mo : _mo_list->modelObjects()) {
          boost::optional<ThermalZone> thermalZone = mo.optionalCast<ThermalZone>();
          if (thermalZone) {
            result.push_back(thermalZone.get());
          } else {
            LOG_AND_THROW(briefDescription() << " appears to have non ThermalZone objects in the Shading Zone Group " << groupIndex);
          }
        }
      }

      return result;
    }

    bool ShadowCalculation_Impl::addShadingZoneGroup(const std::vector<ThermalZone>& thermalZones) {

      bool ok = false;

      ModelObjectList shadingZoneGroupList = ModelObjectList(this->model());
      shadingZoneGroupList.setName("ShadowCalculation Shading Zone Group");

      for (const auto& tz : thermalZones) {
        bool ok = shadingZoneGroupList.addModelObject(tz);
        if (!ok) {
          LOG(Warn, "Failed to add " << tz.briefDescription() << " to " << shadingZoneGroupList.name() << ". Continuing");
        }
      }

      WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      ok = eg.setPointer(OS_ShadowCalculationExtensibleFields::ShadingZoneGroup, shadingZoneGroupList.handle());
      OS_ASSERT(ok);

      return true;
    }

    bool ShadowCalculation_Impl::removeShadingZoneGroup(unsigned groupIndex) {
      if (auto _mo_list = getShadingZoneGroupModelObjectList(groupIndex)) {
        _mo_list->removeAllModelObjects();
        _mo_list->remove();
        // ModelObjectList will remove any object it has, so have to clear it first
        eraseExtensibleGroup(groupIndex);
        return true;
      }

      return false;
    }

    void ShadowCalculation_Impl::removeAllShadingZoneGroups() {

      for (auto eg : extensibleGroups()) {
        ModelExtensibleGroup group = eg.cast<ModelExtensibleGroup>();
        if (boost::optional<ModelObjectList> _mo_list =
              group.getModelObjectTarget<ModelObjectList>(OS_ShadowCalculationExtensibleFields::ShadingZoneGroup)) {
          _mo_list->removeAllModelObjects();
          _mo_list->remove();
        }
      }

      clearExtensibleGroups();

      OS_ASSERT(numberofShadingZoneGroups() == 0u);
    }

  }  // namespace detail

  /// constructor
  ShadowCalculation::ShadowCalculation(const Model& model) : ModelObject(ShadowCalculation::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ShadowCalculation_Impl>());

    bool ok = setShadingCalculationMethod("PolygonClipping");
    OS_ASSERT(ok);

    // ok = setShadingCalculationUpdateFrequencyMethod("Periodic");
    // OS_ASSERT(ok);

    // These two are prexisting, and with default, but they were already hardcoded before
    ok = setShadingCalculationUpdateFrequency(20);
    OS_ASSERT(ok);
    ok = setMaximumFiguresInShadowOverlapCalculations(15000);
    OS_ASSERT(ok);

    // ok = setPolygonClippingAlgorithm("SutherlandHodgman");
    // OS_ASSERT(ok);
    ok = setPixelCountingResolution(512);
    OS_ASSERT(ok);
    // ok = setSkyDiffuseModelingAlgorithm("SimpleSkyDiffuseModeling");
    // OS_ASSERT(ok);
    ok = setOutputExternalShadingCalculationResults(false);
    OS_ASSERT(ok);
    ok = setDisableSelfShadingWithinShadingZoneGroups(false);
    OS_ASSERT(ok);
    ok = setDisableSelfShadingFromShadingZoneGroupstoOtherZones(false);
    OS_ASSERT(ok);
  }

  // constructor
  ShadowCalculation::ShadowCalculation(std::shared_ptr<detail::ShadowCalculation_Impl> impl) : ModelObject(std::move(impl)) {}

  IddObjectType ShadowCalculation::iddObjectType() {
    IddObjectType result(IddObjectType::OS_ShadowCalculation);
    return result;
  }

  std::vector<std::string> ShadowCalculation::shadingCalculationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadowCalculationFields::ShadingCalculationMethod);
  }
  std::vector<std::string> ShadowCalculation::validShadingCalculationMethodValues() {
    return shadingCalculationMethodValues();
  }

  std::vector<std::string> ShadowCalculation::shadingCalculationUpdateFrequencyMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod);
  }
  std::vector<std::string> ShadowCalculation::validShadingCalculationUpdateFrequencyMethodValues() {
    return shadingCalculationUpdateFrequencyMethodValues();
  }

  std::vector<std::string> ShadowCalculation::polygonClippingAlgorithmValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadowCalculationFields::PolygonClippingAlgorithm);
  }
  std::vector<std::string> ShadowCalculation::validPolygonClippingAlgorithmValues() {
    return polygonClippingAlgorithmValues();
  }

  std::vector<std::string> ShadowCalculation::skyDiffuseModelingAlgorithmValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm);
  }
  std::vector<std::string> ShadowCalculation::validSkyDiffuseModelingAlgorithmValues() {
    return skyDiffuseModelingAlgorithmValues();
  }

  std::string ShadowCalculation::shadingCalculationMethod() const {
    return getImpl<detail::ShadowCalculation_Impl>()->shadingCalculationMethod();
  }

  std::string ShadowCalculation::shadingCalculationUpdateFrequencyMethod() const {
    return getImpl<detail::ShadowCalculation_Impl>()->shadingCalculationUpdateFrequencyMethod();
  }

  bool ShadowCalculation::isShadingCalculationUpdateFrequencyMethodDefaulted() const {
    return getImpl<detail::ShadowCalculation_Impl>()->isShadingCalculationUpdateFrequencyMethodDefaulted();
  }

  int ShadowCalculation::shadingCalculationUpdateFrequency() const {
    return getImpl<detail::ShadowCalculation_Impl>()->shadingCalculationUpdateFrequency();
  }

  bool ShadowCalculation::isShadingCalculationUpdateFrequencyDefaulted() const {
    return getImpl<detail::ShadowCalculation_Impl>()->isShadingCalculationUpdateFrequencyDefaulted();
  }

  int ShadowCalculation::maximumFiguresInShadowOverlapCalculations() const {
    return getImpl<detail::ShadowCalculation_Impl>()->maximumFiguresInShadowOverlapCalculations();
  }

  bool ShadowCalculation::isMaximumFiguresInShadowOverlapCalculationsDefaulted() const {
    return getImpl<detail::ShadowCalculation_Impl>()->isMaximumFiguresInShadowOverlapCalculationsDefaulted();
  }

  std::string ShadowCalculation::polygonClippingAlgorithm() const {
    return getImpl<detail::ShadowCalculation_Impl>()->polygonClippingAlgorithm();
  }

  int ShadowCalculation::pixelCountingResolution() const {
    return getImpl<detail::ShadowCalculation_Impl>()->pixelCountingResolution();
  }

  std::string ShadowCalculation::skyDiffuseModelingAlgorithm() const {
    return getImpl<detail::ShadowCalculation_Impl>()->skyDiffuseModelingAlgorithm();
  }

  bool ShadowCalculation::outputExternalShadingCalculationResults() const {
    return getImpl<detail::ShadowCalculation_Impl>()->outputExternalShadingCalculationResults();
  }

  bool ShadowCalculation::disableSelfShadingWithinShadingZoneGroups() const {
    return getImpl<detail::ShadowCalculation_Impl>()->disableSelfShadingWithinShadingZoneGroups();
  }
  bool ShadowCalculation::disableSelfShadingFromShadingZoneGroupstoOtherZones() const {
    return getImpl<detail::ShadowCalculation_Impl>()->disableSelfShadingFromShadingZoneGroupstoOtherZones();
  }

  bool ShadowCalculation::setShadingCalculationMethod(const std::string& shadingCalculationMethod) {
    return getImpl<detail::ShadowCalculation_Impl>()->setShadingCalculationMethod(shadingCalculationMethod);
  }

  bool ShadowCalculation::setShadingCalculationUpdateFrequencyMethod(const std::string& shadingCalculationUpdateFrequencyMethod) {
    return getImpl<detail::ShadowCalculation_Impl>()->setShadingCalculationUpdateFrequencyMethod(shadingCalculationUpdateFrequencyMethod);
  }

  void ShadowCalculation::resetShadingCalculationUpdateFrequencyMethod() {
    getImpl<detail::ShadowCalculation_Impl>()->resetShadingCalculationUpdateFrequencyMethod();
  }

  bool ShadowCalculation::setShadingCalculationUpdateFrequency(int shadingCalculationUpdateFrequency) {
    return getImpl<detail::ShadowCalculation_Impl>()->setShadingCalculationUpdateFrequency(shadingCalculationUpdateFrequency);
  }

  void ShadowCalculation::resetShadingCalculationUpdateFrequency() {
    getImpl<detail::ShadowCalculation_Impl>()->resetShadingCalculationUpdateFrequency();
  }

  bool ShadowCalculation::setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations) {
    return getImpl<detail::ShadowCalculation_Impl>()->setMaximumFiguresInShadowOverlapCalculations(maximumFiguresInShadowOverlapCalculations);
  }

  void ShadowCalculation::resetMaximumFiguresInShadowOverlapCalculations() {
    getImpl<detail::ShadowCalculation_Impl>()->resetMaximumFiguresInShadowOverlapCalculations();
  }

  bool ShadowCalculation::setPolygonClippingAlgorithm(const std::string& polygonClippingAlgorithm) {
    return getImpl<detail::ShadowCalculation_Impl>()->setPolygonClippingAlgorithm(polygonClippingAlgorithm);
  }

  void ShadowCalculation::resetPolygonClippingAlgorithm() {
    getImpl<detail::ShadowCalculation_Impl>()->resetPolygonClippingAlgorithm();
  }

  bool ShadowCalculation::setPixelCountingResolution(int pixelCountingResolution) {
    return getImpl<detail::ShadowCalculation_Impl>()->setPixelCountingResolution(pixelCountingResolution);
  }

  bool ShadowCalculation::setSkyDiffuseModelingAlgorithm(const std::string& skyDiffuseModelingAlgorithm) {
    return getImpl<detail::ShadowCalculation_Impl>()->setSkyDiffuseModelingAlgorithm(skyDiffuseModelingAlgorithm);
  }

  void ShadowCalculation::resetSkyDiffuseModelingAlgorithm() {
    getImpl<detail::ShadowCalculation_Impl>()->resetSkyDiffuseModelingAlgorithm();
  }

  bool ShadowCalculation::setOutputExternalShadingCalculationResults(bool outputExternalShadingCalculationResults) {
    return getImpl<detail::ShadowCalculation_Impl>()->setOutputExternalShadingCalculationResults(outputExternalShadingCalculationResults);
  }

  bool ShadowCalculation::setDisableSelfShadingWithinShadingZoneGroups(bool disableSelfShadingWithinShadingZoneGroups) {
    return getImpl<detail::ShadowCalculation_Impl>()->setDisableSelfShadingWithinShadingZoneGroups(disableSelfShadingWithinShadingZoneGroups);
  }

  bool ShadowCalculation::setDisableSelfShadingFromShadingZoneGroupstoOtherZones(bool disableSelfShadingFromShadingZoneGroupstoOtherZones) {
    return getImpl<detail::ShadowCalculation_Impl>()->setDisableSelfShadingFromShadingZoneGroupstoOtherZones(
      disableSelfShadingFromShadingZoneGroupstoOtherZones);
  }

  unsigned int ShadowCalculation::numberofShadingZoneGroups() const {
    return getImpl<detail::ShadowCalculation_Impl>()->numberofShadingZoneGroups();
  }

  std::vector<ThermalZone> ShadowCalculation::getShadingZoneGroup(unsigned groupIndex) const {
    return getImpl<detail::ShadowCalculation_Impl>()->getShadingZoneGroup(groupIndex);
  }

  bool ShadowCalculation::addShadingZoneGroup(const std::vector<ThermalZone>& thermalZones) {
    return getImpl<detail::ShadowCalculation_Impl>()->addShadingZoneGroup(thermalZones);
  }
  bool ShadowCalculation::removeShadingZoneGroup(unsigned groupIndex) {
    return getImpl<detail::ShadowCalculation_Impl>()->removeShadingZoneGroup(groupIndex);
  }
  void ShadowCalculation::removeAllShadingZoneGroups() {
    getImpl<detail::ShadowCalculation_Impl>()->removeAllShadingZoneGroups();
  }

}  // namespace model
}  // namespace openstudio
