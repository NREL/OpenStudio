/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACUNITHEATER_IMPL_HPP
#define MODEL_ZONEHVACUNITHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACUnitHeater_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACUnitHeater.*/

    class MODEL_API ZoneHVACUnitHeater_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */

      ZoneHVACUnitHeater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACUnitHeater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACUnitHeater_Impl(const ZoneHVACUnitHeater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACUnitHeater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      HVACComponent supplyAirFan() const;

      boost::optional<double> maximumSupplyAirFlowRate() const;

      bool isMaximumSupplyAirFlowRateAutosized() const;

      std::string fanControlType() const;

      HVACComponent heatingCoil() const;

      boost::optional<double> maximumHotWaterFlowRate() const;

      bool isMaximumHotWaterFlowRateAutosized() const;

      double minimumHotWaterFlowRate() const;

      bool isMinimumHotWaterFlowRateDefaulted() const;

      double heatingConvergenceTolerance() const;

      bool isHeatingConvergenceToleranceDefaulted() const;

      boost::optional<double> autosizedMaximumSupplyAirFlowRate() const;

      boost::optional<double> autosizedMaximumHotWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setSupplyAirFan(const HVACComponent& hvacComponent);

      bool setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate);

      void autosizeMaximumSupplyAirFlowRate();

      bool setFanControlType(const std::string& fanControlType);

      bool setHeatingCoil(const HVACComponent& hvacComponent);

      bool setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate);

      void resetMaximumHotWaterFlowRate();

      void autosizeMaximumHotWaterFlowRate();

      bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

      void resetMinimumHotWaterFlowRate();

      bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

      void resetHeatingConvergenceTolerance();

      //@}
      /** @name Other */
      //@{

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACUnitHeater");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;

      std::vector<std::string> fanControlTypeValues() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> supplyAirFanAsModelObject() const;
      boost::optional<ModelObject> heatingCoilAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACUNITHEATER_IMPL_HPP
