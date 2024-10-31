/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** PlantEquipmentOperationHeatingLoad_Impl is a PlantEquipmentOperationRangeBasedScheme_Impl that is the implementation class for PlantEquipmentOperationHeatingLoad.*/
    class MODEL_API PlantEquipmentOperationHeatingLoad_Impl : public PlantEquipmentOperationRangeBasedScheme_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantEquipmentOperationHeatingLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationHeatingLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationHeatingLoad_Impl(const PlantEquipmentOperationHeatingLoad_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlantEquipmentOperationHeatingLoad_Impl() override = default;

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
      REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationHeatingLoad");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONHEATINGLOAD_IMPL_HPP
