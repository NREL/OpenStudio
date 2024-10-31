/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERUSECONNECTIONS_IMPL_HPP
#define MODEL_WATERUSECONNECTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class WaterUseEquipment;

  namespace detail {

    /** WaterUseConnections_Impl is a StraightComponent_Impl that is the implementation class for WaterUseConnections.*/
    class MODEL_API WaterUseConnections_Impl : public StraightComponent_Impl
    {

     public:
      WaterUseConnections_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterUseConnections_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterUseConnections_Impl(const WaterUseConnections_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterUseConnections_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

      boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

      bool setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule);

      void resetHotWaterSupplyTemperatureSchedule();

      bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

      void resetColdWaterSupplyTemperatureSchedule();

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      std::vector<WaterUseEquipment> waterUseEquipment() const;

      bool addWaterUseEquipment(const WaterUseEquipment& waterUseEquipment);

      bool removeWaterUseEquipment(WaterUseEquipment& waterUseEquipment);

      bool addToNode(Node& node) override;

      std::string drainWaterHeatExchangerType() const;

      bool setDrainWaterHeatExchangerType(const std::string& drainWaterHeatExchangerType);

      std::string drainWaterHeatExchangerDestination() const;

      bool setDrainWaterHeatExchangerDestination(const std::string& drainWaterHeatExchangerDestination);

      boost::optional<double> drainWaterHeatExchangerUFactorTimesArea() const;

      bool setDrainWaterHeatExchangerUFactorTimesArea(double drainWaterHeatExchangerUFactorTimesArea);

      void resetDrainWaterHeatExchangerUFactorTimesArea();

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.WaterUseConnections");

      boost::optional<ModelObject> hotWaterSupplyTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coldWaterSupplyTemperatureScheduleAsModelObject() const;

      bool setHotWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSECONNECTIONS_IMPL_HPP
