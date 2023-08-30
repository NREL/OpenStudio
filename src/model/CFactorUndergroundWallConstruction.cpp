/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CFactorUndergroundWallConstruction.hpp"
#include "CFactorUndergroundWallConstruction_Impl.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_Construction_CfactorUndergroundWall_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CFactorUndergroundWallConstruction_Impl::CFactorUndergroundWallConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CFactorUndergroundWallConstruction::iddObjectType());
    }

    CFactorUndergroundWallConstruction_Impl::CFactorUndergroundWallConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CFactorUndergroundWallConstruction::iddObjectType());
    }

    CFactorUndergroundWallConstruction_Impl::CFactorUndergroundWallConstruction_Impl(const CFactorUndergroundWallConstruction_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CFactorUndergroundWallConstruction_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CFactorUndergroundWallConstruction_Impl::iddObjectType() const {
      return CFactorUndergroundWallConstruction::iddObjectType();
    }

    bool CFactorUndergroundWallConstruction_Impl::isOpaque() const {
      return true;
    }

    bool CFactorUndergroundWallConstruction_Impl::isFenestration() const {
      return false;
    }

    bool CFactorUndergroundWallConstruction_Impl::isSolarDiffusing() const {
      return false;
    }

    bool CFactorUndergroundWallConstruction_Impl::isModelPartition() const {
      return false;
    }

    int CFactorUndergroundWallConstruction_Impl::renderingColorIndex() const {
      return OS_Construction_CfactorUndergroundWallFields::SurfaceRenderingName;
    }

    double CFactorUndergroundWallConstruction_Impl::cFactor() const {
      boost::optional<double> value = getDouble(OS_Construction_CfactorUndergroundWallFields::CFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CFactorUndergroundWallConstruction_Impl::setCFactor(double cFactor) {
      bool result = setDouble(OS_Construction_CfactorUndergroundWallFields::CFactor, cFactor);
      return result;
    }

    double CFactorUndergroundWallConstruction_Impl::height() const {
      boost::optional<double> value = getDouble(OS_Construction_CfactorUndergroundWallFields::Height, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CFactorUndergroundWallConstruction_Impl::setHeight(double height) {
      bool result = setDouble(OS_Construction_CfactorUndergroundWallFields::Height, height);
      return result;
    }

  }  // namespace detail

  CFactorUndergroundWallConstruction::CFactorUndergroundWallConstruction(const Model& model, double cFactor, double height)
    : ConstructionBase(CFactorUndergroundWallConstruction::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CFactorUndergroundWallConstruction_Impl>());

    this->setCFactor(cFactor);
    this->setHeight(height);
  }

  IddObjectType CFactorUndergroundWallConstruction::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Construction_CfactorUndergroundWall);
    return result;
  }

  double CFactorUndergroundWallConstruction::cFactor() const {
    return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->cFactor();
  }

  bool CFactorUndergroundWallConstruction::setCFactor(double cFactor) {
    return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->setCFactor(cFactor);
  }

  double CFactorUndergroundWallConstruction::height() const {
    return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->height();
  }

  bool CFactorUndergroundWallConstruction::setHeight(double height) {
    return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->setHeight(height);
  }

  /// @cond
  CFactorUndergroundWallConstruction::CFactorUndergroundWallConstruction(std::shared_ptr<detail::CFactorUndergroundWallConstruction_Impl> impl)
    : ConstructionBase(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
