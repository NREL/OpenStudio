/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERTOWATERCOMPONENT_IMPL_HPP
#define MODEL_WATERTOWATERCOMPONENT_IMPL_HPP

#include "WaterToWaterComponent.hpp"

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVAC;

  namespace detail {

    class MODEL_API WaterToWaterComponent_Impl : public HVACComponent_Impl
    {
     public:
      WaterToWaterComponent_Impl(IddObjectType type, Model_Impl* model);

      WaterToWaterComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterToWaterComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterToWaterComponent_Impl(const WaterToWaterComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~WaterToWaterComponent_Impl() override = default;

      virtual unsigned supplyInletPort() const = 0;

      virtual unsigned supplyOutletPort() const = 0;

      virtual unsigned demandInletPort() const = 0;

      virtual unsigned demandOutletPort() const = 0;

      virtual boost::optional<ModelObject> supplyInletModelObject() const;

      virtual boost::optional<ModelObject> supplyOutletModelObject() const;

      virtual boost::optional<ModelObject> demandInletModelObject() const;

      virtual boost::optional<ModelObject> demandOutletModelObject() const;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      void disconnect() override;

      /*
   * This method checks for presence of the WaterToWaterComponent on the supply side of plantLoops
   * and checks that it isn't the tertiary plant loop
   */
      virtual boost::optional<PlantLoop> plantLoop() const override;

      /*
  * This method checks for presence of the WaterToWaterComponent on the demand side of plantLoops
  * and checks that it isn't the tertiary plant loop
  */
      virtual boost::optional<PlantLoop> secondaryPlantLoop() const;

      bool removeFromPlantLoop();

      virtual bool removeFromSecondaryPlantLoop();

      virtual unsigned tertiaryInletPort() const;

      virtual unsigned tertiaryOutletPort() const;

      /*
  * This method checks for presence of the WaterToWaterComponent on either the supply or demand side of plantLoops
  * and does an extra check for actual node: the tertiaryOutletModelObject has to be on the plant loop too
  */
      virtual boost::optional<PlantLoop> tertiaryPlantLoop() const;

      virtual bool removeFromTertiaryPlantLoop();

      virtual bool addToTertiaryNode(Node& node);

      boost::optional<ModelObject> tertiaryInletModelObject() const;

      boost::optional<ModelObject> tertiaryOutletModelObject() const;

     protected:
      friend class Model_Impl;

      mutable boost::optional<PlantLoop> m_secondaryPlantLoop;
      mutable boost::optional<PlantLoop> m_tertiaryPlantLoop;

     private:
      REGISTER_LOGGER("openstudio.model.WaterToWaterComponent");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
