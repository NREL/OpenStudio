/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACSUPPLYPLENUM_IMPL_HPP
#define MODEL_AIRLOOPHVACSUPPLYPLENUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "Splitter_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;

  namespace detail {

    /** AirLoopHVACSupplyPlenum_Impl is a Splitter_Impl that is the implementation class for AirLoopHVACSupplyPlenum.*/
    class MODEL_API AirLoopHVACSupplyPlenum_Impl : public Splitter_Impl
    {

     public:
      AirLoopHVACSupplyPlenum_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACSupplyPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACSupplyPlenum_Impl(const AirLoopHVACSupplyPlenum_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACSupplyPlenum_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      boost::optional<ThermalZone> thermalZone() const;

      bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

      void resetThermalZone();

      virtual unsigned inletPort() const override;

      unsigned outletPort(unsigned branchIndex) const override;

      unsigned nextOutletPort() const override;

      bool addToNode(Node& node) override;

      bool addBranchForZone(openstudio::model::ThermalZone& thermalZone);

      bool addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal);

      bool addBranchForZoneImpl(openstudio::model::ThermalZone& thermalZone, boost::optional<HVACComponent>& terminal);

      std::vector<IdfObject> remove() override;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACSupplyPlenum");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACSUPPLYPLENUM_IMPL_HPP
