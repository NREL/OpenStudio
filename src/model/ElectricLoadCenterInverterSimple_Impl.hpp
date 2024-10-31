/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Inverter_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** ElectricLoadCenterInverterSimple_Impl is a Inverter_Impl that is the implementation class for ElectricLoadCenterInverterSimple.*/
    class MODEL_API ElectricLoadCenterInverterSimple_Impl : public Inverter_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricLoadCenterInverterSimple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterInverterSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterInverterSimple_Impl(const ElectricLoadCenterInverterSimple_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricLoadCenterInverterSimple_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      boost::optional<double> radiativeFraction() const;

      boost::optional<double> inverterEfficiency() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      virtual bool setThermalZone(const ThermalZone& thermalZone) override;

      virtual void resetThermalZone() override;

      bool setRadiativeFraction(boost::optional<double> radiativeFraction);

      void resetRadiativeFraction();

      bool setInverterEfficiency(boost::optional<double> inverterEfficiency);

      void resetInverterEfficiency();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterSimple");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_IMPL_HPP
