/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMALSTORAGEICEDETAILED_IMPL_HPP
#define MODEL_THERMALSTORAGEICEDETAILED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** ThermalStorageIceDetailed_Impl is a StraightComponent_Impl that is the implementation class for ThermalStorageIceDetailed.*/
    class MODEL_API ThermalStorageIceDetailed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ThermalStorageIceDetailed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ThermalStorageIceDetailed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ThermalStorageIceDetailed_Impl(const ThermalStorageIceDetailed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ThermalStorageIceDetailed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;
      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;
      virtual std::vector<ModelObject> children() const override;
      virtual ModelObject clone(Model model) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double capacity() const;

      std::string dischargingCurveVariableSpecifications() const;

      Curve dischargingCurve() const;

      std::string chargingCurveVariableSpecifications() const;

      Curve chargingCurve() const;

      double timestepoftheCurveData() const;

      double parasiticElectricLoadDuringDischarging() const;

      double parasiticElectricLoadDuringCharging() const;

      double tankLossCoefficient() const;

      double freezingTemperatureofStorageMedium() const;

      std::string thawProcessIndicator() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setCapacity(double capacity);

      bool setDischargingCurveVariableSpecifications(const std::string& dischargingCurveVariableSpecifications);

      bool setDischargingCurve(const Curve& dischargingCurve);

      bool setChargingCurveVariableSpecifications(const std::string& chargingCurveVariableSpecifications);

      bool setChargingCurve(const Curve& chargingCurve);

      bool setTimestepoftheCurveData(double timestepoftheCurveData);

      bool setParasiticElectricLoadDuringDischarging(double parasiticElectricLoadDuringDischarging);

      bool setParasiticElectricLoadDuringCharging(double parasiticElectricLoadDuringCharging);

      bool setTankLossCoefficient(double tankLossCoefficient);

      bool setFreezingTemperatureofStorageMedium(double freezingTemperatureofStorageMedium);

      bool setThawProcessIndicator(const std::string& thawProcessIndicator);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ThermalStorageIceDetailed");

      boost::optional<Curve> optionalDischargingCurve() const;
      boost::optional<Curve> optionalChargingCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMALSTORAGEICEDETAILED_IMPL_HPP
