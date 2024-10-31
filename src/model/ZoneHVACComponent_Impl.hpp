/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACCOMPONENT_IMPL_HPP
#define MODEL_ZONEHVACCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;
  class ThermalZone;
  class AirLoopHVAC;
  class HVACComponent;

  namespace detail {

    class MODEL_API ZoneHVACComponent_Impl : public HVACComponent_Impl
    {

     public:
      ZoneHVACComponent_Impl(IddObjectType type, Model_Impl* model);

      ZoneHVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACComponent_Impl(const ZoneHVACComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~ZoneHVACComponent_Impl() override = default;

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual ModelObject clone(Model model) const override;

      virtual unsigned inletPort() const = 0;

      virtual unsigned outletPort() const = 0;

      boost::optional<Node> inletNode() const;

      boost::optional<Node> outletNode() const;

      virtual boost::optional<ThermalZone> thermalZone() const;

      virtual bool addToThermalZone(ThermalZone& thermalZone);

      virtual void removeFromThermalZone();

      std::vector<openstudio::IdfObject> remove() override;

      bool addToNode(Node& node) override;

      boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      bool removeFromAirLoopHVAC();

      void removeFromInletSideMixer();

      std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      boost::optional<ModelObject> airInletModelObject() const;

      boost::optional<ModelObject> airOutletModelObject() const;

      bool setReturnPlenum(const ThermalZone& plenumZone);

      void removeReturnPlenum();

      boost::optional<AirLoopHVACReturnPlenum> returnPlenum() const;

     protected:
      friend class Model_Impl;

     private:
      boost::optional<Connection> returnPlenumOutletNodeConnection() const;

      REGISTER_LOGGER("openstudio.model.ZoneHVACComponent");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
