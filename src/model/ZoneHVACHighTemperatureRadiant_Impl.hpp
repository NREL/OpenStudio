/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP
#define MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** ZoneHVACHighTemperatureRadiant_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACHighTemperatureRadiant.*/
    class MODEL_API ZoneHVACHighTemperatureRadiant_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACHighTemperatureRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACHighTemperatureRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACHighTemperatureRadiant_Impl(const ZoneHVACHighTemperatureRadiant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACHighTemperatureRadiant_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::vector<Surface> surfaces() const;

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> maximumPowerInput() const;

      bool isMaximumPowerInputAutosized() const;

      std::string fuelType() const;

      double combustionEfficiency() const;

      double fractionofInputConvertedtoRadiantEnergy() const;

      double fractionofInputConvertedtoLatentEnergy() const;

      double fractionofInputthatIsLost() const;

      std::string temperatureControlType() const;

      double heatingThrottlingRange() const;

      boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

      double fractionofRadiantEnergyIncidentonPeople() const;

      boost::optional<double> autosizedMaximumPowerInput() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setMaximumPowerInput(boost::optional<double> maximumPowerInput);

      void autosizeMaximumPowerInput();

      bool setFuelType(const std::string& fuelType);

      bool setCombustionEfficiency(double combustionEfficiency);

      bool setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy);

      bool setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy);

      bool setFractionofInputthatIsLost(double fractionofInputthatIsLost);

      bool setTemperatureControlType(const std::string& temperatureControlType);

      bool setHeatingThrottlingRange(double heatingThrottlingRange);

      bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

      void resetHeatingSetpointTemperatureSchedule();

      bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

      //@}
      /** @name Other */
      //@{

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;
      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACHighTemperatureRadiant");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP
