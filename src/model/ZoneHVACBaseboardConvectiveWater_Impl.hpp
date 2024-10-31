/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class StraightComponent;

  namespace detail {

    /** ZoneHVACBaseboardConvectiveWater_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardConvectiveWater.*/
    class MODEL_API ZoneHVACBaseboardConvectiveWater_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACBaseboardConvectiveWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACBaseboardConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACBaseboardConvectiveWater_Impl(const ZoneHVACBaseboardConvectiveWater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACBaseboardConvectiveWater_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      unsigned inletPort() const override;
      unsigned outletPort() const override;

      Schedule availabilitySchedule() const;

      StraightComponent heatingCoil() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatingCoil(const StraightComponent& baseboardHeatingCoil);

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      //@}
      /** @name Other */
      //@{

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveWater");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<StraightComponent> optionalHeatingCoil() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> heatingCoilAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_IMPL_HPP
