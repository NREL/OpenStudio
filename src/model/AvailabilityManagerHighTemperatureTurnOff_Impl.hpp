/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNOFF_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNOFF_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** AvailabilityManagerHighTemperatureTurnOff_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerHighTemperatureTurnOff.*/
    class MODEL_API AvailabilityManagerHighTemperatureTurnOff_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerHighTemperatureTurnOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHighTemperatureTurnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHighTemperatureTurnOff_Impl(const AvailabilityManagerHighTemperatureTurnOff_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerHighTemperatureTurnOff_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> sensorNode() const;

      double temperature() const;

      //@}
      /** @name Setters */
      //@{

      bool setSensorNode(const Node& node);

      void resetSensorNode();

      bool setTemperature(double temperature);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerHighTemperatureTurnOff");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNOFF_IMPL_HPP
