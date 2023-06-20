/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationScheme.hpp"
#include "PlantEquipmentOperationScheme_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    PlantEquipmentOperationScheme_Impl::PlantEquipmentOperationScheme_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    PlantEquipmentOperationScheme_Impl::PlantEquipmentOperationScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    PlantEquipmentOperationScheme_Impl::PlantEquipmentOperationScheme_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    PlantEquipmentOperationScheme_Impl::PlantEquipmentOperationScheme_Impl(const PlantEquipmentOperationScheme_Impl& other, Model_Impl* model,
                                                                           bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

    boost::optional<PlantLoop> PlantEquipmentOperationScheme_Impl::plantLoop() const {
      return boost::none;
    }

  }  // namespace detail

  PlantEquipmentOperationScheme::PlantEquipmentOperationScheme(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationScheme_Impl>());
  }

  PlantEquipmentOperationScheme::PlantEquipmentOperationScheme(std::shared_ptr<detail::PlantEquipmentOperationScheme_Impl> p)
    : ModelObject(std::move(p)) {}

  boost::optional<PlantLoop> PlantEquipmentOperationScheme::plantLoop() const {
    return getImpl<detail::PlantEquipmentOperationScheme_Impl>()->plantLoop();
  }

}  // namespace model

}  // namespace openstudio
