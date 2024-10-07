/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HVACCOMPONENT_IMPL_HPP
#define MODEL_HVACCOMPONENT_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "PlantLoop.hpp"

namespace openstudio {

class AppGFuelType;
class ComponentType;
class FuelType;

namespace model {

  class Node;

  class Splitter;

  class ZoneHVACComponent;

  namespace detail {

    class MODEL_API HVACComponent_Impl : public ParentObject_Impl
    {
     public:
      HVACComponent_Impl(IddObjectType type, Model_Impl* model);

      HVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HVACComponent_Impl(const HVACComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~HVACComponent_Impl() override = default;

      virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

      virtual boost::optional<PlantLoop> plantLoop() const;

      virtual boost::optional<Loop> loop() const;

      virtual boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

      virtual bool addToNode(Node& node);

      virtual bool addToSplitter(Splitter& splitter);

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual void disconnect();

      virtual std::vector<IdfObject> remove() override;

      virtual bool isRemovable() const;

      // children controls what we see in the Inspector in OS App, and it's not necessarilly what we want to clone
      // So the parent/children link is broken on purpose here as it will forward to ModelObject_Impl::clone and **not** ParentObject_Impl
      virtual ModelObject clone(Model model) const override;

      /** This function returns a vector of HVACComponent that are directly downstream
   *  from this object on an AirLoopHVAC or PlantLoop.
   *  @param[in] previous is used to indicate which path of edges to return
  **/
      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& previous);

      virtual boost::optional<HVACComponent> containingHVACComponent() const;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

      virtual boost::optional<StraightComponent> containingStraightComponent() const;

      bool addToNode(Node& node, const HVACComponent& systemStartComponent, const HVACComponent& systemEndComponent, unsigned componentInletPort,
                     unsigned componentOutletPort);

      bool removeFromLoop(const HVACComponent& systemStartComponent, const HVACComponent& systemEndComponent, unsigned componentInletPort,
                          unsigned componentOutletPort);

      virtual void autosize();

      virtual void applySizingValues();

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const = 0;
      virtual std::vector<FuelType> coolingFuelTypes() const = 0;
      virtual std::vector<FuelType> heatingFuelTypes() const = 0;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const = 0;

     protected:
      friend class Model_Impl;
      friend class AirLoopHVAC_Impl;

      mutable boost::optional<AirLoopHVAC> m_airLoopHVAC;
      mutable boost::optional<PlantLoop> m_plantLoop;
      mutable boost::optional<AirLoopHVACOutdoorAirSystem> m_airLoopHVACOutdoorAirSystem;

     private:
      REGISTER_LOGGER("openstudio.model.HVACComponent");

      boost::optional<ModelObject> airLoopHVACAsModelObject() const;
      boost::optional<ModelObject> plantLoopAsModelObject() const;
      boost::optional<ModelObject> airLoopHVACOutdoorAirSystemAsModelObject() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif
