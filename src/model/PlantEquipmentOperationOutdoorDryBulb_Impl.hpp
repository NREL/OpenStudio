/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** PlantEquipmentOperationOutdoorDryBulb_Impl is a PlantEquipmentOperationRangeBasedScheme_Impl that is the implementation class for PlantEquipmentOperationOutdoorDryBulb.*/
    class MODEL_API PlantEquipmentOperationOutdoorDryBulb_Impl : public PlantEquipmentOperationRangeBasedScheme_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantEquipmentOperationOutdoorDryBulb_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationOutdoorDryBulb_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationOutdoorDryBulb_Impl(const PlantEquipmentOperationOutdoorDryBulb_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlantEquipmentOperationOutdoorDryBulb_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{

      double maximumUpperLimit() const override;

      double minimumLowerLimit() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDryBulb");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULB_IMPL_HPP
