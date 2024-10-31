/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP
#define MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class PortList;
  class ZoneHVACIdealLoadsAirSystem;

  namespace detail {

    /** AirLoopHVACReturnPlenum_Impl is a Mixer_Impl that is the implementation class for AirLoopHVACReturnPlenum.*/
    class MODEL_API AirLoopHVACReturnPlenum_Impl : public Mixer_Impl
    {

     public:
      AirLoopHVACReturnPlenum_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACReturnPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACReturnPlenum_Impl(const AirLoopHVACReturnPlenum_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACReturnPlenum_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      boost::optional<ThermalZone> thermalZone() const;

      bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

      void resetThermalZone();

      unsigned outletPort() const override;

      PortList inducedAirOutletPortList();

      unsigned inletPort(unsigned branchIndex) const override;

      unsigned nextInletPort() const override;

      bool addToNode(Node& node) override;

      bool addBranchForZone(openstudio::model::ThermalZone& thermalZone);

      bool addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal);

      bool addBranchForZoneImpl(openstudio::model::ThermalZone& thermalZone, boost::optional<HVACComponent>& terminal);

      std::vector<IdfObject> remove() override;

      std::vector<model::ZoneHVACIdealLoadsAirSystem> zoneHVACIdealLoadsAirSystems() const;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACReturnPlenum");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP
