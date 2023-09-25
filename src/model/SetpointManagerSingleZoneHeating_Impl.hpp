/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHEATING_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHEATING_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class ThermalZone;
  class Node;

  namespace detail {

    /** SetpointManagerSingleZoneHeating_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSingleZoneHeating.*/
    class MODEL_API SetpointManagerSingleZoneHeating_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerSingleZoneHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneHeating_Impl(const SetpointManagerSingleZoneHeating_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerSingleZoneHeating_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double minimumSupplyAirTemperature() const;

      double maximumSupplyAirTemperature() const;

      boost::optional<ThermalZone> controlZone() const;

      //@}
      /** @name Setters */
      //@{

      bool setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature);

      bool setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature);

      bool setControlZone(const ThermalZone& thermalZone);

      void resetControlZone();

      //@}
      /** @name Other */
      //@{

      bool addToNode(Node& node) override;

      virtual std::string controlVariable() const override;

      virtual bool setControlVariable(const std::string& value) override;

      virtual boost::optional<Node> setpointNode() const override;

      /** This is used to set the field SetpointNodeName.
      * Use addToNode to add a setpoint manager to a node. */
      virtual bool setSetpointNode(const Node& node) override;

      /** This is used to reset the field SetpointNodeName. */
      virtual void resetSetpointNode() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHeating");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEHEATING_IMPL_HPP
