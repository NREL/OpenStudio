/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNON_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNON_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** AvailabilityManagerHighTemperatureTurnOn_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerHighTemperatureTurnOn.*/
    class MODEL_API AvailabilityManagerHighTemperatureTurnOn_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerHighTemperatureTurnOn_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHighTemperatureTurnOn_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHighTemperatureTurnOn_Impl(const AvailabilityManagerHighTemperatureTurnOn_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerHighTemperatureTurnOn_Impl() override = default;

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
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerHighTemperatureTurnOn");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNON_IMPL_HPP
