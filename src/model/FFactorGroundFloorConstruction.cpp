/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FFactorGroundFloorConstruction.hpp"
#include "FFactorGroundFloorConstruction_Impl.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_Construction_FfactorGroundFloor_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    FFactorGroundFloorConstruction_Impl::FFactorGroundFloorConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == FFactorGroundFloorConstruction::iddObjectType());
    }

    FFactorGroundFloorConstruction_Impl::FFactorGroundFloorConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == FFactorGroundFloorConstruction::iddObjectType());
    }

    FFactorGroundFloorConstruction_Impl::FFactorGroundFloorConstruction_Impl(const FFactorGroundFloorConstruction_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& FFactorGroundFloorConstruction_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType FFactorGroundFloorConstruction_Impl::iddObjectType() const {
      return FFactorGroundFloorConstruction::iddObjectType();
    }

    bool FFactorGroundFloorConstruction_Impl::isOpaque() const {
      return true;
    }

    bool FFactorGroundFloorConstruction_Impl::isFenestration() const {
      return false;
    }

    bool FFactorGroundFloorConstruction_Impl::isSolarDiffusing() const {
      return false;
    }

    bool FFactorGroundFloorConstruction_Impl::isModelPartition() const {
      return false;
    }

    int FFactorGroundFloorConstruction_Impl::renderingColorIndex() const {
      return OS_Construction_FfactorGroundFloorFields::SurfaceRenderingName;
    }

    double FFactorGroundFloorConstruction_Impl::fFactor() const {
      boost::optional<double> value = getDouble(OS_Construction_FfactorGroundFloorFields::FFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FFactorGroundFloorConstruction_Impl::setFFactor(double fFactor) {
      bool result = setDouble(OS_Construction_FfactorGroundFloorFields::FFactor, fFactor);
      return result;
    }

    double FFactorGroundFloorConstruction_Impl::area() const {
      boost::optional<double> value = getDouble(OS_Construction_FfactorGroundFloorFields::Area, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FFactorGroundFloorConstruction_Impl::setArea(double area) {
      bool result = setDouble(OS_Construction_FfactorGroundFloorFields::Area, area);
      return result;
    }

    double FFactorGroundFloorConstruction_Impl::perimeterExposed() const {
      boost::optional<double> value = getDouble(OS_Construction_FfactorGroundFloorFields::PerimeterExposed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FFactorGroundFloorConstruction_Impl::setPerimeterExposed(double perimeterExposed) {
      bool result = setDouble(OS_Construction_FfactorGroundFloorFields::PerimeterExposed, perimeterExposed);
      return result;
    }

  }  // namespace detail

  FFactorGroundFloorConstruction::FFactorGroundFloorConstruction(const Model& model, double fFactor, double area, double perimeterExposed)
    : ConstructionBase(FFactorGroundFloorConstruction::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FFactorGroundFloorConstruction_Impl>());

    setFFactor(fFactor);
    setArea(area);
    setPerimeterExposed(perimeterExposed);
  }

  IddObjectType FFactorGroundFloorConstruction::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Construction_FfactorGroundFloor);
    return result;
  }

  double FFactorGroundFloorConstruction::fFactor() const {
    return getImpl<detail::FFactorGroundFloorConstruction_Impl>()->fFactor();
  }

  bool FFactorGroundFloorConstruction::setFFactor(double fFactor) {
    return getImpl<detail::FFactorGroundFloorConstruction_Impl>()->setFFactor(fFactor);
  }

  double FFactorGroundFloorConstruction::area() const {
    return getImpl<detail::FFactorGroundFloorConstruction_Impl>()->area();
  }

  bool FFactorGroundFloorConstruction::setArea(double area) {
    return getImpl<detail::FFactorGroundFloorConstruction_Impl>()->setArea(area);
  }

  double FFactorGroundFloorConstruction::perimeterExposed() const {
    return getImpl<detail::FFactorGroundFloorConstruction_Impl>()->perimeterExposed();
  }

  bool FFactorGroundFloorConstruction::setPerimeterExposed(double perimeterExposed) {
    return getImpl<detail::FFactorGroundFloorConstruction_Impl>()->setPerimeterExposed(perimeterExposed);
  }

  /// @cond
  FFactorGroundFloorConstruction::FFactorGroundFloorConstruction(std::shared_ptr<detail::FFactorGroundFloorConstruction_Impl> impl)
    : ConstructionBase(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
