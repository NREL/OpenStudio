/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERTOAIRCOMPONENT_IMPL_HPP
#define MODEL_WATERTOAIRCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class ControllerWaterCoil;

  namespace detail {

    class MODEL_API WaterToAirComponent_Impl : public HVACComponent_Impl
    {
     public:
      WaterToAirComponent_Impl(IddObjectType type, Model_Impl* model);

      WaterToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterToAirComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterToAirComponent_Impl(const WaterToAirComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~WaterToAirComponent_Impl() override = default;

      virtual boost::optional<ModelObject> airInletModelObject() const;

      virtual boost::optional<ModelObject> airOutletModelObject() const;

      virtual boost::optional<ModelObject> waterInletModelObject() const;

      virtual boost::optional<ModelObject> waterOutletModelObject() const;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& previous) override;

      virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      virtual boost::optional<PlantLoop> plantLoop() const override;

      virtual boost::optional<Loop> loop() const override;

      virtual unsigned airInletPort() const = 0;

      virtual unsigned airOutletPort() const = 0;

      virtual unsigned waterInletPort() const = 0;

      virtual unsigned waterOutletPort() const = 0;

      virtual bool addToNode(Node& node) override;

      virtual bool addToSplitter(Splitter& splitter) override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual void disconnectWaterSide();

      virtual void disconnectAirSide();

      virtual void disconnect() override;

      virtual bool removeFromAirLoopHVAC();

      virtual bool removeFromPlantLoop();

      boost::optional<ControllerWaterCoil> controllerWaterCoil();

     private:
      REGISTER_LOGGER("openstudio.model.WaterToAirComponent");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
