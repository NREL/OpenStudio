/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP

#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

  class ZoneHVACEquipmentConnections;

  namespace detail {

    class MODEL_API SetpointManagerSingleZoneReheat_Impl : public SetpointManager_Impl
    {

     public:
      SetpointManagerSingleZoneReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSingleZoneReheat_Impl(const SetpointManagerSingleZoneReheat_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~SetpointManagerSingleZoneReheat_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual bool addToNode(Node& node) override;

      double minimumSupplyAirTemperature();

      bool setMinimumSupplyAirTemperature(double value);

      double maximumSupplyAirTemperature();

      bool setMaximumSupplyAirTemperature(double value);

      virtual boost::optional<Node> setpointNode() const override;

      virtual std::string controlVariable() const override;

      virtual bool setControlVariable(const std::string& controlVariable) override;

      boost::optional<ThermalZone> controlZone();

      bool setControlZone(ThermalZone& thermalZone);

      void resetControlZone();

     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      boost::optional<ModelObject> controlZoneAsModelObject();

      bool setControlZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP
