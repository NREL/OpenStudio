/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINT_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINT_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** PlantEquipmentOperationOutdoorDewpoint_Impl is a PlantEquipmentOperationRangeBasedScheme_Impl that is the implementation class for PlantEquipmentOperationOutdoorDewpoint.*/
    class MODEL_API PlantEquipmentOperationOutdoorDewpoint_Impl : public PlantEquipmentOperationRangeBasedScheme_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantEquipmentOperationOutdoorDewpoint_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationOutdoorDewpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationOutdoorDewpoint_Impl(const PlantEquipmentOperationOutdoorDewpoint_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlantEquipmentOperationOutdoorDewpoint_Impl() override = default;

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
      REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDewpoint");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINT_IMPL_HPP
