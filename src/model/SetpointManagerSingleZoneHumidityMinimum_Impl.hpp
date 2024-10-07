/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class ThermalZone;

  namespace detail {

    /** SetpointManagerSingleZoneHumidityMinimum_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSingleZoneHumidityMinimum.*/
    class MODEL_API SetpointManagerSingleZoneHumidityMinimum_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerSingleZoneHumidityMinimum_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneHumidityMinimum_Impl(const SetpointManagerSingleZoneHumidityMinimum_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerSingleZoneHumidityMinimum_Impl() override = default;

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

      virtual boost::optional<Node> setpointNode() const override;

      boost::optional<ThermalZone> controlZone() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

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

      REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHumidityMinimum");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_IMPL_HPP
