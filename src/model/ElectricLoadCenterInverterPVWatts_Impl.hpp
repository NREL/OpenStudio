/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERINVERTERPVWATTS_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERPVWATTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "Inverter_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** ElectricLoadCenterInverterPVWatts_Impl is a Inverter_Impl that is the implementation class for ElectricLoadCenterInverterPVWatts.*/
    class MODEL_API ElectricLoadCenterInverterPVWatts_Impl : public Inverter_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricLoadCenterInverterPVWatts_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterInverterPVWatts_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterInverterPVWatts_Impl(const ElectricLoadCenterInverterPVWatts_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricLoadCenterInverterPVWatts_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      virtual bool setThermalZone(const ThermalZone& thermalZone) override;

      virtual void resetThermalZone() override;

      //@}
      /** @name Getters */
      //@{

      double dcToACSizeRatio() const;

      bool isDCToACSizeRatioDefaulted() const;

      double inverterEfficiency() const;

      bool isInverterEfficiencyDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setDCToACSizeRatio(double dcToACSizeRatio);

      void resetDCToACSizeRatio();

      bool setInverterEfficiency(double inverterEfficiency);

      void resetInverterEfficiency();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterPVWatts");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERINVERTERPVWATTS_IMPL_HPP
