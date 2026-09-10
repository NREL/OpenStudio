/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVESTEAM_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVESTEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Surface;
  class ThermalZone;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACBaseboardRadiantConvectiveSteam_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardRadiantConvectiveSteam.*/
    class MODEL_API ZoneHVACBaseboardRadiantConvectiveSteam_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACBaseboardRadiantConvectiveSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACBaseboardRadiantConvectiveSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACBaseboardRadiantConvectiveSteam_Impl(const ZoneHVACBaseboardRadiantConvectiveSteam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACBaseboardRadiantConvectiveSteam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      virtual ModelObject clone(Model model) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IdfObject> remove() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      double fractionRadiant() const;

      double fractionofRadiantEnergyIncidentonPeople() const;

      HVACComponent heatingCoil() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setFractionRadiant(double fractionRadiant);

      bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

      bool setHeatingCoil(const HVACComponent& heatingCoil);

      //@}
      /** @name Other */
      //@{

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      std::vector<Surface> surfaces() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveSteam");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVESTEAM_IMPL_HPP
