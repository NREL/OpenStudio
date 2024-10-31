/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONSCHEME_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONSCHEME_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PlantLoop;

  namespace detail {

    class MODEL_API PlantEquipmentOperationScheme_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantEquipmentOperationScheme_Impl(IddObjectType type, Model_Impl* model);

      PlantEquipmentOperationScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationScheme_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationScheme_Impl(const PlantEquipmentOperationScheme_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~PlantEquipmentOperationScheme_Impl() override = default;

      boost::optional<PlantLoop> plantLoop() const;

     private:
      REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationScheme");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONSCHEME_IMPL_HPP
