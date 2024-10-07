/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** PlantEquipmentOperationOutdoorDewpointDifference_Impl is a PlantEquipmentOperationRangeBasedScheme_Impl that is the implementation class for PlantEquipmentOperationOutdoorDewpointDifference.*/
    class MODEL_API PlantEquipmentOperationOutdoorDewpointDifference_Impl : public PlantEquipmentOperationRangeBasedScheme_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantEquipmentOperationOutdoorDewpointDifference_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantEquipmentOperationOutdoorDewpointDifference_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                            bool keepHandle);

      PlantEquipmentOperationOutdoorDewpointDifference_Impl(const PlantEquipmentOperationOutdoorDewpointDifference_Impl& other, Model_Impl* model,
                                                            bool keepHandle);

      virtual ~PlantEquipmentOperationOutdoorDewpointDifference_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> referenceTemperatureNode() const;

      //@}
      /** @name Setters */
      //@{

      bool setReferenceTemperatureNode(const boost::optional<Node>& node);

      void resetReferenceTemperatureNode();

      //@}
      /** @name Other */
      //@{

      double maximumUpperLimit() const override;

      double minimumLowerLimit() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDewpointDifference");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDEWPOINTDIFFERENCE_IMPL_HPP
