/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include "../utilities/core/DeprecatedHelpers.hpp"

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

    Surface SurfacePropertyExposedFoundationPerimeter_Impl::surface() const {
      boost::optional<Surface> value = optionalSurface();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Surface attached.");
      }
      return value.get();
    }

    boost::optional<Surface> SurfacePropertyExposedFoundationPerimeter_Impl::optionalSurface() const {
      return getObject<ModelObject>().getModelObjectTarget<Surface>(OS_SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName);
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

    bool SurfacePropertyExposedFoundationPerimeter_Impl::setExposedPerimeterCalculationMethod(const std::string& exposedPerimeterCalculationMethod) {
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
    return {IddObjectType::OS_SurfaceProperty_ExposedFoundationPerimeter};
  }

  std::string SurfacePropertyExposedFoundationPerimeter::surfaceName() const {
    DEPRECATED_AT_MSG(3, 7, 0, "Use surface.nameString instead.");
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->surface().nameString();
  }

  Surface SurfacePropertyExposedFoundationPerimeter::surface() const {
    return getImpl<detail::SurfacePropertyExposedFoundationPerimeter_Impl>()->surface();
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

  bool SurfacePropertyExposedFoundationPerimeter::setExposedPerimeterCalculationMethod(const std::string& exposedPerimeterCalculationMethod) {
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
