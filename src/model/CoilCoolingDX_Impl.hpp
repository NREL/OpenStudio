/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDX_IMPL_HPP
#define MODEL_COILCOOLINGDX_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class CoilCoolingDXCurveFitPerformance;

  namespace detail {

    /** CoilCoolingDX_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDX.*/
    class MODEL_API CoilCoolingDX_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDX_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDX_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDX_Impl(const CoilCoolingDX_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDX_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<ThermalZone> condenserZone() const;

      CoilCoolingDXCurveFitPerformance performanceObject() const;

      // boost::optional<HVACComponent> condensateCollectionWaterStorageTank() const;

      // boost::optional<HVACComponent> evaporativeCondenserSupplyWaterStorageTank() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setCondenserZone(const ThermalZone& thermalZone);
      void resetCondenserZone();

      // TODO: Eventually provide an API to set these Nodes once E+ is done implementing support for it (as of 9.3.0 it's not the case)
      // boost::optional<std::string> condenserInletNodeName() const;
      // bool setCondenserInletNodeName(const std::string& condenserInletNodeName);
      // void resetCondenserInletNodeName();

      // boost::optional<std::string> condenserOutletNodeName() const;
      // bool setCondenserOutletNodeName(const std::string& condenserOutletNodeName);
      // void resetCondenserOutletNodeName();

      bool setPerformanceObject(const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance);

      // bool setCondensateCollectionWaterStorageTank(const HVACComponent& condensateCollectionWaterStorageTank);
      // void resetCondensateCollectionWaterStorageTank();

      // bool setEvaporativeCondenserSupplyWaterStorageTank(const HVACComponent& evaporativeCondenserSupplyWaterStorageTank);
      // void resetEvaporativeCondenserSupplyWaterStorageTank();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDX");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<CoilCoolingDXCurveFitPerformance> optionalPerformanceObject() const;

      boost::optional<Connection> optionalAirInletNode() const;
      boost::optional<Connection> optionalAirOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDX_IMPL_HPP
