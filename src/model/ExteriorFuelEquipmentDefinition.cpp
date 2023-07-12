/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExteriorFuelEquipmentDefinition.hpp"
#include "ExteriorFuelEquipmentDefinition_Impl.hpp"

#include <utilities/idd/OS_Exterior_FuelEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorFuelEquipmentDefinition_Impl::ExteriorFuelEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExteriorFuelEquipmentDefinition::iddObjectType());
    }

    ExteriorFuelEquipmentDefinition_Impl::ExteriorFuelEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExteriorFuelEquipmentDefinition::iddObjectType());
    }

    ExteriorFuelEquipmentDefinition_Impl::ExteriorFuelEquipmentDefinition_Impl(const ExteriorFuelEquipmentDefinition_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : ExteriorLoadDefinition_Impl(other, model, keepHandle) {}

    // TODO: remove
    const std::vector<std::string>& ExteriorFuelEquipmentDefinition_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExteriorFuelEquipmentDefinition_Impl::iddObjectType() const {
      return ExteriorFuelEquipmentDefinition::iddObjectType();
    }

    double ExteriorFuelEquipmentDefinition_Impl::designLevel() const {
      boost::optional<double> value = getDouble(OS_Exterior_FuelEquipment_DefinitionFields::DesignLevel, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorFuelEquipmentDefinition_Impl::setDesignLevel(double designLevel) {
      bool result = setDouble(OS_Exterior_FuelEquipment_DefinitionFields::DesignLevel, designLevel);
      return result;
    }

  }  // namespace detail

  ExteriorFuelEquipmentDefinition::ExteriorFuelEquipmentDefinition(const Model& model)
    : ExteriorLoadDefinition(ExteriorFuelEquipmentDefinition::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExteriorFuelEquipmentDefinition_Impl>());

    bool ok = setDesignLevel(0.0);
    OS_ASSERT(ok);
  }

  IddObjectType ExteriorFuelEquipmentDefinition::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Exterior_FuelEquipment_Definition);
    return result;
  }

  double ExteriorFuelEquipmentDefinition::designLevel() const {
    return getImpl<detail::ExteriorFuelEquipmentDefinition_Impl>()->designLevel();
  }

  bool ExteriorFuelEquipmentDefinition::setDesignLevel(double designLevel) {
    return getImpl<detail::ExteriorFuelEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
  }

  /// @cond
  ExteriorFuelEquipmentDefinition::ExteriorFuelEquipmentDefinition(std::shared_ptr<detail::ExteriorFuelEquipmentDefinition_Impl> impl)
    : ExteriorLoadDefinition(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
