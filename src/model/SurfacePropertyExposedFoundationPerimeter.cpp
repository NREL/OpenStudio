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

#include <vector>
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SurfacePropertyExposedFoundationPerimeter.hpp"
#include "SurfacePropertyExposedFoundationPerimeter_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_ExposedFoundationPerimeter_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SurfacePropertyExposedFoundationPerimeter_Impl::SurfacePropertyExposedFoundationPerimeter_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                   bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyExposedFoundationPerimeter::iddObjectType());
    }

    SurfacePropertyExposedFoundationPerimeter_Impl::SurfacePropertyExposedFoundationPerimeter_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfacePropertyExposedFoundationPerimeter::iddObjectType());
    }

    SurfacePropertyExposedFoundationPerimeter_Impl::SurfacePropertyExposedFoundationPerimeter_Impl(
      const SurfacePropertyExposedFoundationPerimeter_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SurfacePropertyExposedFoundationPerimeter_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SurfacePropertyExposedFoundationPerimeter_Impl::iddObjectType() const {
      return SurfacePropertyExposedFoundationPerimeter::iddObjectType();
    }

    std::string SurfacePropertyExposedFoundationPerimeter_Impl::surfaceName() const {
      boost::optional<Surface> surface =
        getObject<ModelObject>().getModelObjectTarget<Surface>(OS_SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName);
      OS_ASSERT(surface);
      return surface.get().name().get();
    }

    std::string SurfacePropertyExposedFoundationPerimeter_Impl::exposedPerimeterCalculationMethod() const {
      boost::optional<std::string> value = getString(OS_SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterCalculationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SurfacePropertyExposedFoundationPerimeter_Impl::totalExposedPerimeter() {
      return getDouble(OS_SurfaceProperty_ExposedFoundationPerimeterFields::TotalExposedPerimeter, true);
    }

    double SurfacePropertyExposedFoundationPerimeter_Impl::exposedPerimeterFraction() const {
      boost::optional<double> value = getDouble(OS_SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SurfacePropertyExposedFoundationPerimeter_Impl::isExposedPerimeterFractionDefaulted() const {
      return isEmpty(OS_SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction);
    }

    bool SurfacePropertyExposedFoundationPerimeter_Impl::setExposedPerimeterCalculationMethod(std::string exposedPerimeterCalculationMethod) {
      bool result =
        setString(OS_SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterCalculationMethod, exposedPerimeterCalculationMethod);
      OS_ASSERT(result);
      return result;
    }

    bool SurfacePropertyExposedFoundationPerimeter_Impl::setTotalExposedPerimeter(double totalExposedPerimeter) {
      bool result = setDouble(OS_SurfaceProperty_ExposedFoundationPerimeterFields::TotalExposedPerimeter, totalExposedPerimeter);
      OS_ASSERT(result);
      return result;
    }

    bool SurfacePropertyExposedFoundationPerimeter_Impl::setExposedPerimeterFraction(double exposedPerimeterFraction) {
      bool result = setDouble(OS_SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction, exposedPerimeterFraction);
      OS_ASSERT(result);
      return result;
    }

    void SurfacePropertyExposedFoundationPerimeter_Impl::resetExposedPerimeterFraction() {
      bool result = setString(OS_SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction, "");
      OS_ASSERT(result);
    }

    bool SurfacePropertyExposedFoundationPerimeter_Impl::setParent(ParentObject& newParent) {
      bool result = false;

      if ((model() == newParent.model()) && newParent.optionalCast<Surface>()) {
        result = setPointer(OS_SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName, newParent.handle());
      }
      return result;
    }

    boost::optional<ParentObject> SurfacePropertyExposedFoundationPerimeter_Impl::parent() const {
      return getObject<ModelObject>().getModelObjectTarget<ParentObject>(OS_SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName);
    }

  }  // namespace detail

  SurfacePropertyExposedFoundationPerimeter::SurfacePropertyExposedFoundationPerimeter(Surface& surface,
                                                                                       std::string exposedPerimeterCalculationMethod,
                                                                                       double exposedPerimeter)
    : ModelObject(SurfacePropertyExposedFoundationPerimeter::iddObjectType(), surface.model()) {
    OS_ASSERT(getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>());

    if (surface.surfacePropertyExposedFoundationPerimeter()) {
      LOG_AND_THROW("Surface '" << surface.nameString() << "' already has an associated SurfacePropertyExposedFoundationPerimeter object");
    }

    bool ok = true;
    OS_ASSERT(ok);

    ok = setPointer(OS_SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName, surface.handle());
    OS_ASSERT(ok);

    ok = setExposedPerimeterCalculationMethod(exposedPerimeterCalculationMethod);
    OS_ASSERT(ok);

    if (exposedPerimeterCalculationMethod == "TotalExposedPerimeter") {
      ok = setTotalExposedPerimeter(exposedPerimeter);
      OS_ASSERT(ok);
    } else if (exposedPerimeterCalculationMethod == "ExposedPerimeterFraction") {
      ok = setExposedPerimeterFraction(exposedPerimeter);
      OS_ASSERT(ok);
    }
  }

  IddObjectType SurfacePropertyExposedFoundationPerimeter::iddObjectType() {
    return IddObjectType(IddObjectType::OS_SurfaceProperty_ExposedFoundationPerimeter);
  }

  std::string SurfacePropertyExposedFoundationPerimeter::surfaceName() const {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->surfaceName();
  }

  std::string SurfacePropertyExposedFoundationPerimeter::exposedPerimeterCalculationMethod() const {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->exposedPerimeterCalculationMethod();
  }

  boost::optional<double> SurfacePropertyExposedFoundationPerimeter::totalExposedPerimeter() {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->totalExposedPerimeter();
  }

  double SurfacePropertyExposedFoundationPerimeter::exposedPerimeterFraction() const {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->exposedPerimeterFraction();
  }

  bool SurfacePropertyExposedFoundationPerimeter::isExposedPerimeterFractionDefaulted() const {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->isExposedPerimeterFractionDefaulted();
  }

  bool SurfacePropertyExposedFoundationPerimeter::setExposedPerimeterCalculationMethod(std::string exposedPerimeterCalculationMethod) {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->setExposedPerimeterCalculationMethod(exposedPerimeterCalculationMethod);
  }

  bool SurfacePropertyExposedFoundationPerimeter::setTotalExposedPerimeter(double totalExposedPerimeter) {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->setTotalExposedPerimeter(totalExposedPerimeter);
  }

  bool SurfacePropertyExposedFoundationPerimeter::setExposedPerimeterFraction(double exposedPerimeterFraction) {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->setExposedPerimeterFraction(exposedPerimeterFraction);
  }

  void SurfacePropertyExposedFoundationPerimeter::resetExposedPerimeterFraction() {
    getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->resetExposedPerimeterFraction();
  }

  /// @cond
  SurfacePropertyExposedFoundationPerimeter::SurfacePropertyExposedFoundationPerimeter(
    std::shared_ptr<detail::SurfacePropertyExposedFoundationPerimeter_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
