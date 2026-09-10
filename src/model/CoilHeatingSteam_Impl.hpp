/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGSTEAM_IMPL_HPP
#define MODEL_COILHEATINGSTEAM_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;

  namespace detail {

    class MODEL_API CoilHeatingSteam_Impl : public WaterToAirComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingSteam_Impl(const CoilHeatingSteam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingSteam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool removeFromPlantLoop() override;

      virtual ModelObject clone(Model model) const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool addToNode(Node& node) override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> maximumSteamFlowRate();

      bool isMaximumSteamFlowRateAutosized();

      double degreeofSubCooling() const;

      double degreeofLoopSubCooling() const;

      std::string coilControlType() const;

      boost::optional<Node> temperatureSetpointNode() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumSteamFlowRate(double value);

      void autosizeMaximumSteamFlowRate();

      bool setDegreeofSubCooling(double degreeofSubCooling);

      bool setDegreeofLoopSubCooling(double degreeofLoopSubCooling);

      bool setCoilControlType(const std::string& coilControlType);

      bool setTemperatureSetpointNode(Node& temperatureSetpointNode);

      void resetTemperatureSetpointNode();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedMaximumSteamFlowRate() const;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingSteam");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGSTEAM_IMPL_HPP
