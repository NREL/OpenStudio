/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "FoundationKiva.hpp"
#include "FoundationKiva_Impl.hpp"
#include "Material.hpp"
#include "Material_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Foundation_Kiva_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FoundationKiva_Impl::FoundationKiva_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FoundationKiva::iddObjectType());
  }

  FoundationKiva_Impl::FoundationKiva_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FoundationKiva::iddObjectType());
  }

  FoundationKiva_Impl::FoundationKiva_Impl(const FoundationKiva_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FoundationKiva_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType FoundationKiva_Impl::iddObjectType() const {
    return FoundationKiva::iddObjectType();
  }

  boost::optional<Material> FoundationKiva_Impl::interiorHorizontalInsulationMaterial() const {
    return getObject<ModelObject>().getModelObjectTarget<Material>(OS_Foundation_KivaFields::InteriorHorizontalInsulationMaterialName);
  }

  double FoundationKiva_Impl::interiorHorizontalInsulationDepth() const {
    boost::optional<double> value = getDouble(OS_Foundation_KivaFields::InteriorHorizontalInsulationDepth, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKiva_Impl::isInteriorHorizontalInsulationDepthDefaulted() const {
    return isEmpty(OS_Foundation_KivaFields::InteriorHorizontalInsulationDepth);
  }

  boost::optional<double> FoundationKiva_Impl::interiorHorizontalInsulationWidth() {
    return getDouble(OS_Foundation_KivaFields::InteriorHorizontalInsulationWidth);
  }
  
  boost::optional<Material> FoundationKiva_Impl::interiorVerticalInsulationMaterial() const {
    return getObject<ModelObject>().getModelObjectTarget<Material>(OS_Foundation_KivaFields::InteriorVerticalInsulationMaterialName);
  }

  boost::optional<double> FoundationKiva_Impl::interiorVerticalInsulationDepth() {
    return getDouble(OS_Foundation_KivaFields::InteriorVerticalInsulationDepth);
  }

  boost::optional<Material> FoundationKiva_Impl::exteriorHorizontalInsulationMaterial() const {
    return getObject<ModelObject>().getModelObjectTarget<Material>(OS_Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName);
  }

  boost::optional<double> FoundationKiva_Impl::exteriorHorizontalInsulationDepth() {
    return getDouble(OS_Foundation_KivaFields::ExteriorHorizontalInsulationDepth);
  }

  double FoundationKiva_Impl::exteriorHorizontalInsulationWidth() const {
    boost::optional<double> value = getDouble(OS_Foundation_KivaFields::ExteriorHorizontalInsulationWidth, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKiva_Impl::isExteriorHorizontalInsulationWidthDefaulted() const {
    return isEmpty(OS_Foundation_KivaFields::ExteriorHorizontalInsulationWidth);
  }

  boost::optional<Material> FoundationKiva_Impl::exteriorVerticalInsulationMaterial() const {
    return getObject<ModelObject>().getModelObjectTarget<Material>(OS_Foundation_KivaFields::ExteriorVerticalInsulationMaterialName);
  }

  boost::optional<double> FoundationKiva_Impl::exteriorVerticalInsulationDepth() {
    return getDouble(OS_Foundation_KivaFields::ExteriorVerticalInsulationDepth);
  }

  double FoundationKiva_Impl::wallHeightAboveGrade() const {
    boost::optional<double> value = getDouble(OS_Foundation_KivaFields::WallHeightAboveGrade, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKiva_Impl::isWallHeightAboveGradeDefaulted() const {
    return isEmpty(OS_Foundation_KivaFields::WallHeightAboveGrade);
  }

  double FoundationKiva_Impl::wallDepthBelowSlab() const {
    boost::optional<double> value = getDouble(OS_Foundation_KivaFields::WallDepthBelowSlab, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKiva_Impl::isWallDepthBelowSlabDefaulted() const {
    return isEmpty(OS_Foundation_KivaFields::WallDepthBelowSlab);
  }

  boost::optional<ConstructionBase> FoundationKiva_Impl::footingWallConstruction() const {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_Foundation_KivaFields::FootingWallConstructionName);
  }

  boost::optional<Material> FoundationKiva_Impl::footingMaterial() const {
    return getObject<ModelObject>().getModelObjectTarget<Material>(OS_Foundation_KivaFields::FootingMaterialName);
  }

  double FoundationKiva_Impl::footingDepth() const {
    boost::optional<double> value = getDouble(OS_Foundation_KivaFields::FootingDepth, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKiva_Impl::isFootingDepthDefaulted() const {
    return isEmpty(OS_Foundation_KivaFields::FootingDepth);
  }

  bool FoundationKiva_Impl::setInteriorHorizontalInsulationMaterial(const Material& material) {
    return setPointer(OS_Foundation_KivaFields::InteriorHorizontalInsulationMaterialName, material.handle());
  }

  void FoundationKiva_Impl::resetInteriorHorizontalInsulationMaterial() {
    bool result = setString(OS_Foundation_KivaFields::InteriorHorizontalInsulationMaterialName, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setInteriorHorizontalInsulationDepth(double interiorHorizontalInsulationDepth) {
    bool result = setDouble(OS_Foundation_KivaFields::InteriorHorizontalInsulationDepth, interiorHorizontalInsulationDepth);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKiva_Impl::resetInteriorHorizontalInsulationDepth() {
    bool result = setString(OS_Foundation_KivaFields::InteriorHorizontalInsulationDepth, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setInteriorHorizontalInsulationWidth(double interiorHorizontalInsulationWidth) {
    bool result = setDouble(OS_Foundation_KivaFields::InteriorHorizontalInsulationWidth, interiorHorizontalInsulationWidth);
    OS_ASSERT(result);
    return result;
  }

  bool FoundationKiva_Impl::setInteriorVerticalInsulationMaterial(const Material& material) {
    return setPointer(OS_Foundation_KivaFields::InteriorVerticalInsulationMaterialName, material.handle());
  }

  void FoundationKiva_Impl::resetInteriorVerticalInsulationMaterial() {
    bool result = setString(OS_Foundation_KivaFields::InteriorVerticalInsulationMaterialName, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setInteriorVerticalInsulationDepth(double interiorVerticalInsulationDepth) {
    bool result = setDouble(OS_Foundation_KivaFields::InteriorVerticalInsulationDepth, interiorVerticalInsulationDepth);
    OS_ASSERT(result);
    return result;
  }

  bool FoundationKiva_Impl::setExteriorHorizontalInsulationMaterial(const Material& material) {
    return setPointer(OS_Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName, material.handle());
  }

  void FoundationKiva_Impl::resetExteriorHorizontalInsulationMaterial() {
    bool result = setString(OS_Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setExteriorHorizontalInsulationDepth(double exteriorHorizontalInsulationDepth) {
    bool result = setDouble(OS_Foundation_KivaFields::ExteriorHorizontalInsulationDepth, exteriorHorizontalInsulationDepth);
    OS_ASSERT(result);
    return result;
  }

  bool FoundationKiva_Impl::setExteriorHorizontalInsulationWidth(double exteriorHorizontalInsulationWidth) {
    bool result = setDouble(OS_Foundation_KivaFields::ExteriorHorizontalInsulationWidth, exteriorHorizontalInsulationWidth);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKiva_Impl::resetExteriorHorizontalInsulationWidth() {
    bool result = setString(OS_Foundation_KivaFields::ExteriorHorizontalInsulationWidth, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setExteriorVerticalInsulationMaterial(const Material& material) {
    return setPointer(OS_Foundation_KivaFields::ExteriorVerticalInsulationMaterialName, material.handle());
  }

  void FoundationKiva_Impl::resetExteriorVerticalInsulationMaterial() {
    bool result = setString(OS_Foundation_KivaFields::ExteriorVerticalInsulationMaterialName, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setExteriorVerticalInsulationDepth(double exteriorVerticalInsulationDepth) {
    bool result = setDouble(OS_Foundation_KivaFields::ExteriorVerticalInsulationDepth, exteriorVerticalInsulationDepth);
    OS_ASSERT(result);
    return result;
  }

  bool FoundationKiva_Impl::setWallHeightAboveGrade(double wallHeightAboveGrade) {
    bool result = setDouble(OS_Foundation_KivaFields::WallHeightAboveGrade, wallHeightAboveGrade);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKiva_Impl::resetWallHeightAboveGrade() {
    bool result = setString(OS_Foundation_KivaFields::WallHeightAboveGrade, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setWallDepthBelowSlab(double wallDepthBelowSlab) {
    bool result = setDouble(OS_Foundation_KivaFields::WallDepthBelowSlab, wallDepthBelowSlab);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKiva_Impl::resetWallDepthBelowSlab() {
    bool result = setString(OS_Foundation_KivaFields::WallDepthBelowSlab, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setFootingWallConstruction(const ConstructionBase& construction) {
    return setPointer(OS_Foundation_KivaFields::FootingWallConstructionName, construction.handle());
  }

  void FoundationKiva_Impl::resetFootingWallConstruction() {
    bool result = setString(OS_Foundation_KivaFields::FootingWallConstructionName, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setFootingMaterial(const Material& material) {
    return setPointer(OS_Foundation_KivaFields::FootingMaterialName, material.handle());
  }

  void FoundationKiva_Impl::resetFootingMaterial() {
    bool result = setString(OS_Foundation_KivaFields::FootingMaterialName, "");
    OS_ASSERT(result);
  }

  bool FoundationKiva_Impl::setFootingDepth(double footingDepth) {
    bool result = setDouble(OS_Foundation_KivaFields::FootingDepth, footingDepth);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKiva_Impl::resetFootingDepth() {
    bool result = setString(OS_Foundation_KivaFields::FootingDepth, "");
    OS_ASSERT(result);
  }

  SurfaceVector FoundationKiva_Impl::surfaces() const {
    return getObject<ModelObject>().getModelObjectSources<Surface>(Surface::iddObjectType());
  }

} // detail

FoundationKiva::FoundationKiva(Model& model)
  : ModelObject(FoundationKiva::iddObjectType(), model)
{}

IddObjectType FoundationKiva::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Foundation_Kiva);
}

boost::optional<Material> FoundationKiva::interiorHorizontalInsulationMaterial() const {
  return getImpl<detail::FoundationKiva_Impl>()->interiorHorizontalInsulationMaterial();
}

double FoundationKiva::interiorHorizontalInsulationDepth() const {
  return getImpl<detail::FoundationKiva_Impl>()->interiorHorizontalInsulationDepth();
}

bool FoundationKiva::isInteriorHorizontalInsulationDepthDefaulted() const {
  return getImpl<detail::FoundationKiva_Impl>()->isInteriorHorizontalInsulationDepthDefaulted();
}

boost::optional<double> FoundationKiva::interiorHorizontalInsulationWidth() {
  return getImpl<detail::FoundationKiva_Impl>()->interiorHorizontalInsulationWidth();
}

boost::optional<Material> FoundationKiva::interiorVerticalInsulationMaterial() const {
  return getImpl<detail::FoundationKiva_Impl>()->interiorVerticalInsulationMaterial();
}

boost::optional<double> FoundationKiva::interiorVerticalInsulationDepth() {
  return getImpl<detail::FoundationKiva_Impl>()->interiorVerticalInsulationDepth();
}

boost::optional<Material> FoundationKiva::exteriorHorizontalInsulationMaterial() const {
  return getImpl<detail::FoundationKiva_Impl>()->exteriorHorizontalInsulationMaterial();
}

boost::optional<double> FoundationKiva::exteriorHorizontalInsulationDepth() {
  return getImpl<detail::FoundationKiva_Impl>()->exteriorHorizontalInsulationDepth();
}

double FoundationKiva::exteriorHorizontalInsulationWidth() const {
  return getImpl<detail::FoundationKiva_Impl>()->exteriorHorizontalInsulationWidth();
}

bool FoundationKiva::isExteriorHorizontalInsulationWidthDefaulted() const {
  return getImpl<detail::FoundationKiva_Impl>()->isExteriorHorizontalInsulationWidthDefaulted();
}

boost::optional<Material> FoundationKiva::exteriorVerticalInsulationMaterial() const {
  return getImpl<detail::FoundationKiva_Impl>()->exteriorVerticalInsulationMaterial();
}

boost::optional<double> FoundationKiva::exteriorVerticalInsulationDepth() {
  return getImpl<detail::FoundationKiva_Impl>()->exteriorVerticalInsulationDepth();
}

double FoundationKiva::wallHeightAboveGrade() const {
  return getImpl<detail::FoundationKiva_Impl>()->wallHeightAboveGrade();
}

bool FoundationKiva::isWallHeightAboveGradeDefaulted() const {
  return getImpl<detail::FoundationKiva_Impl>()->isWallHeightAboveGradeDefaulted();
}

double FoundationKiva::wallDepthBelowSlab() const {
  return getImpl<detail::FoundationKiva_Impl>()->wallDepthBelowSlab();
}

bool FoundationKiva::isWallDepthBelowSlabDefaulted() const {
  return getImpl<detail::FoundationKiva_Impl>()->isWallDepthBelowSlabDefaulted();
}

boost::optional<ConstructionBase> FoundationKiva::footingWallConstruction() const {
  return getImpl<detail::FoundationKiva_Impl>()->footingWallConstruction();
}

boost::optional<Material> FoundationKiva::footingMaterial() const {
  return getImpl<detail::FoundationKiva_Impl>()->footingMaterial();
}

double FoundationKiva::footingDepth() const {
  return getImpl<detail::FoundationKiva_Impl>()->footingDepth();
}

bool FoundationKiva::isFootingDepthDefaulted() const {
  return getImpl<detail::FoundationKiva_Impl>()->isFootingDepthDefaulted();
}

bool FoundationKiva::setInteriorHorizontalInsulationMaterial(const Material& material) {
  return getImpl<detail::FoundationKiva_Impl>()->setInteriorHorizontalInsulationMaterial(material);
}

void FoundationKiva::resetInteriorHorizontalInsulationMaterial() {
  getImpl<detail::FoundationKiva_Impl>()->resetInteriorHorizontalInsulationMaterial();
}

bool FoundationKiva::setInteriorHorizontalInsulationDepth(double interiorHorizontalInsulationDepth) {
  return getImpl<detail::FoundationKiva_Impl>()->setInteriorHorizontalInsulationDepth(interiorHorizontalInsulationDepth);
}

void FoundationKiva::resetInteriorHorizontalInsulationDepth() {
  getImpl<detail::FoundationKiva_Impl>()->resetInteriorHorizontalInsulationDepth();
}

bool FoundationKiva::setInteriorHorizontalInsulationWidth(double interiorHorizontalInsulationWidth) {
  return getImpl<detail::FoundationKiva_Impl>()->setInteriorHorizontalInsulationWidth(interiorHorizontalInsulationWidth);
}

bool FoundationKiva::setInteriorVerticalInsulationMaterial(const Material& material) {
  return getImpl<detail::FoundationKiva_Impl>()->setInteriorVerticalInsulationMaterial(material);
}

void FoundationKiva::resetInteriorVerticalInsulationMaterial() {
  getImpl<detail::FoundationKiva_Impl>()->resetInteriorVerticalInsulationMaterial();
}

bool FoundationKiva::setInteriorVerticalInsulationDepth(double interiorVerticalInsulationDepth) {
  return getImpl<detail::FoundationKiva_Impl>()->setInteriorVerticalInsulationDepth(interiorVerticalInsulationDepth);
}

bool FoundationKiva::setExteriorHorizontalInsulationMaterial(const Material& material) {
  return getImpl<detail::FoundationKiva_Impl>()->setExteriorHorizontalInsulationMaterial(material);
}

void FoundationKiva::resetExteriorHorizontalInsulationMaterial() {
  getImpl<detail::FoundationKiva_Impl>()->resetExteriorHorizontalInsulationMaterial();
}

bool FoundationKiva::setExteriorHorizontalInsulationDepth(double exteriorHorizontalInsulationDepth) {
  return getImpl<detail::FoundationKiva_Impl>()->setExteriorHorizontalInsulationDepth(exteriorHorizontalInsulationDepth);
}

bool FoundationKiva::setExteriorHorizontalInsulationWidth(double exteriorHorizontalInsulationWidth) {
  return getImpl<detail::FoundationKiva_Impl>()->setExteriorHorizontalInsulationWidth(exteriorHorizontalInsulationWidth);
}

void FoundationKiva::resetExteriorHorizontalInsulationWidth() {
  getImpl<detail::FoundationKiva_Impl>()->resetExteriorHorizontalInsulationWidth();
}

bool FoundationKiva::setExteriorVerticalInsulationMaterial(const Material& material) {
  return getImpl<detail::FoundationKiva_Impl>()->setExteriorVerticalInsulationMaterial(material);
}

void FoundationKiva::resetExteriorVerticalInsulationMaterial() {
  getImpl<detail::FoundationKiva_Impl>()->resetExteriorVerticalInsulationMaterial();
}

bool FoundationKiva::setExteriorVerticalInsulationDepth(double exteriorVerticalInsulationDepth) {
  return getImpl<detail::FoundationKiva_Impl>()->setExteriorVerticalInsulationDepth(exteriorVerticalInsulationDepth);
}

bool FoundationKiva::setWallHeightAboveGrade(double wallHeightAboveGrade) {
  return getImpl<detail::FoundationKiva_Impl>()->setWallHeightAboveGrade(wallHeightAboveGrade);
}

void FoundationKiva::resetWallHeightAboveGrade() {
  getImpl<detail::FoundationKiva_Impl>()->resetWallHeightAboveGrade();
}

bool FoundationKiva::setWallDepthBelowSlab(double wallDepthBelowSlab) {
  return getImpl<detail::FoundationKiva_Impl>()->setWallDepthBelowSlab(wallDepthBelowSlab);
}

void FoundationKiva::resetWallDepthBelowSlab() {
  getImpl<detail::FoundationKiva_Impl>()->resetWallDepthBelowSlab();
}

bool FoundationKiva::setFootingWallConstruction(const ConstructionBase& construction) {
  return getImpl<detail::FoundationKiva_Impl>()->setFootingWallConstruction(construction);
}

void FoundationKiva::resetFootingWallConstruction() {
  getImpl<detail::FoundationKiva_Impl>()->resetFootingWallConstruction();
}

bool FoundationKiva::setFootingMaterial(const Material& material) {
  return getImpl<detail::FoundationKiva_Impl>()->setFootingMaterial(material);
}

void FoundationKiva::resetFootingMaterial() {
  getImpl<detail::FoundationKiva_Impl>()->resetFootingMaterial();
}

bool FoundationKiva::setFootingDepth(double footingDepth) {
  return getImpl<detail::FoundationKiva_Impl>()->setFootingDepth(footingDepth);
}

void FoundationKiva::resetFootingDepth() {
  getImpl<detail::FoundationKiva_Impl>()->resetFootingDepth();
}

std::vector<Surface> FoundationKiva::surfaces() const {
  return getImpl<detail::FoundationKiva_Impl>()->surfaces();
}

/// @cond
FoundationKiva::FoundationKiva(std::shared_ptr<detail::FoundationKiva_Impl> impl)
  : ModelObject(std::move(impl))
{}

/// @endcond

} // model
} // openstudio