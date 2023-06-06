/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExteriorWaterEquipmentDefinition.hpp"
#include "ExteriorWaterEquipmentDefinition_Impl.hpp"

#include <utilities/idd/OS_Exterior_WaterEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorWaterEquipmentDefinition_Impl::ExteriorWaterEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExteriorWaterEquipmentDefinition::iddObjectType());
    }

    ExteriorWaterEquipmentDefinition_Impl::ExteriorWaterEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExteriorWaterEquipmentDefinition::iddObjectType());
    }

    ExteriorWaterEquipmentDefinition_Impl::ExteriorWaterEquipmentDefinition_Impl(const ExteriorWaterEquipmentDefinition_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(other, model, keepHandle) {}

    // TODO: remove
    const std::vector<std::string>& ExteriorWaterEquipmentDefinition_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExteriorWaterEquipmentDefinition_Impl::iddObjectType() const {
      return ExteriorWaterEquipmentDefinition::iddObjectType();
    }

    double ExteriorWaterEquipmentDefinition_Impl::designLevel() const {
      boost::optional<double> value = getDouble(OS_Exterior_WaterEquipment_DefinitionFields::DesignLevel, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorWaterEquipmentDefinition_Impl::setDesignLevel(double designLevel) {
      bool result = setDouble(OS_Exterior_WaterEquipment_DefinitionFields::DesignLevel, designLevel);
      return result;
    }

  }  // namespace detail

  ExteriorWaterEquipmentDefinition::ExteriorWaterEquipmentDefinition(const Model& model)
    : ExteriorLoadDefinition(ExteriorWaterEquipmentDefinition::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExteriorWaterEquipmentDefinition_Impl>());

    bool ok = setDesignLevel(0.0);
    OS_ASSERT(ok);
  }

  IddObjectType ExteriorWaterEquipmentDefinition::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Exterior_WaterEquipment_Definition);
    return result;
  }

  double ExteriorWaterEquipmentDefinition::designLevel() const {
    return getImpl<detail::ExteriorWaterEquipmentDefinition_Impl>()->designLevel();
  }

  bool ExteriorWaterEquipmentDefinition::setDesignLevel(double designLevel) {
    return getImpl<detail::ExteriorWaterEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
  }

  /// @cond
  ExteriorWaterEquipmentDefinition::ExteriorWaterEquipmentDefinition(std::shared_ptr<detail::ExteriorWaterEquipmentDefinition_Impl> impl)
    : ExteriorLoadDefinition(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
