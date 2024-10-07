/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANVARIABLEVOLUME_IMPL_HPP
#define MODEL_FANVARIABLEVOLUME_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkFan;
  class FanSystemModel;
  class Schedule;

  namespace detail {

    /** FanVariableVolume_Impl is a StraightComponent_Impl that is the implementation
 *  class for FanVariableVolume.*/
    class MODEL_API FanVariableVolume_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanVariableVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanVariableVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanVariableVolume_Impl(const FanVariableVolume_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanVariableVolume_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      double fanTotalEfficiency() const;

      bool isFanTotalEfficiencyDefaulted() const;

      double pressureRise() const;

      boost::optional<double> maximumFlowRate() const;

      bool isMaximumFlowRateAutosized() const;

      std::string fanPowerMinimumFlowRateInputMethod() const;

      bool isFanPowerMinimumFlowRateInputMethodDefaulted() const;

      double fanPowerMinimumFlowFraction() const;

      bool isFanPowerMinimumFlowFractionDefaulted() const;

      boost::optional<double> fanPowerMinimumAirFlowRate() const;

      double motorEfficiency() const;

      bool isMotorEfficiencyDefaulted() const;

      double motorInAirstreamFraction() const;

      bool isMotorInAirstreamFractionDefaulted() const;

      boost::optional<double> fanPowerCoefficient1() const;

      boost::optional<double> fanPowerCoefficient2() const;

      boost::optional<double> fanPowerCoefficient3() const;

      boost::optional<double> fanPowerCoefficient4() const;

      boost::optional<double> fanPowerCoefficient5() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      boost::optional<double> autosizedMaximumFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      void resetFanTotalEfficiency();

      bool setPressureRise(double pressureRise);

      bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

      void resetMaximumFlowRate();

      void autosizeMaximumFlowRate();

      bool setFanPowerMinimumFlowRateInputMethod(const std::string& fanPowerMinimumFlowRateInputMethod);

      void resetFanPowerMinimumFlowRateInputMethod();

      bool setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction);

      void resetFanPowerMinimumFlowFraction();

      bool setFanPowerMinimumAirFlowRate(boost::optional<double> fanPowerMinimumAirFlowRate);

      void resetFanPowerMinimumAirFlowRate();

      bool setMotorEfficiency(double motorEfficiency);

      void resetMotorEfficiency();

      bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

      void resetMotorInAirstreamFraction();

      bool setFanPowerCoefficient1(boost::optional<double> fanPowerCoefficient1);

      void resetFanPowerCoefficient1();

      bool setFanPowerCoefficient2(boost::optional<double> fanPowerCoefficient2);

      void resetFanPowerCoefficient2();

      bool setFanPowerCoefficient3(boost::optional<double> fanPowerCoefficient3);

      void resetFanPowerCoefficient3();

      bool setFanPowerCoefficient4(boost::optional<double> fanPowerCoefficient4);

      void resetFanPowerCoefficient4();

      bool setFanPowerCoefficient5(boost::optional<double> fanPowerCoefficient5);

      void resetFanPowerCoefficient5();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkFan getAirflowNetworkFan();
      boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

      FanSystemModel convertToFanSystemModel() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanVariableVolume");

      std::vector<std::string> fanPowerMinimumFlowRateInputMethodValues() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANVARIABLEVOLUME_IMPL_HPP
