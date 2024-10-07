/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    /** SetpointManagerSingleZoneOneStageCooling_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSingleZoneOneStageCooling.*/
    class MODEL_API SetpointManagerSingleZoneOneStageCooling_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerSingleZoneOneStageCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneOneStageCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneOneStageCooling_Impl(const SetpointManagerSingleZoneOneStageCooling_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerSingleZoneOneStageCooling_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual bool addToNode(Node& node) override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      double coolingStageOnSupplyAirSetpointTemperature() const;

      double coolingStageOffSupplyAirSetpointTemperature() const;

      boost::optional<ThermalZone> controlZone() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature);

      bool setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature);

      bool setControlZone(const ThermalZone& thermalZone);

      void resetControlZone();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneOneStageCooling");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_IMPL_HPP
