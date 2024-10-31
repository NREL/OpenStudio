/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Surface;
  class ThermalZone;

  namespace detail {

    /** ZoneHVACBaseboardRadiantConvectiveElectric_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardRadiantConvectiveElectric.*/
    class MODEL_API ZoneHVACBaseboardRadiantConvectiveElectric_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const ZoneHVACBaseboardRadiantConvectiveElectric_Impl& other, Model_Impl* model,
                                                      bool keepHandle);

      virtual ~ZoneHVACBaseboardRadiantConvectiveElectric_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;

      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      double efficiency() const;

      double fractionRadiant() const;

      double fractionofRadiantEnergyIncidentonPeople() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      bool setEfficiency(double efficiency);

      bool setFractionRadiant(double fractionRadiant);

      bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

      //@}
      /** @name Other */
      //@{

      std::vector<Surface> surfaces() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveElectric");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_IMPL_HPP
