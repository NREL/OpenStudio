/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANCOMPONENTMODEL_IMPL_HPP
#define MODEL_FANCOMPONENTMODEL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;
  class AirflowNetworkFan;

  namespace detail {

    /** FanComponentModel_Impl is a StraightComponent_Impl that is the implementation class for FanComponentModel.*/
    class MODEL_API FanComponentModel_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanComponentModel_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanComponentModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanComponentModel_Impl(const FanComponentModel_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanComponentModel_Impl() override = default;

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

      boost::optional<double> maximumFlowRate() const;

      bool isMaximumFlowRateAutosized() const;

      boost::optional<double> minimumFlowRate() const;

      bool isMinimumFlowRateAutosized() const;

      double fanSizingFactor() const;

      double fanWheelDiameter() const;

      double fanOutletArea() const;

      double maximumFanStaticEfficiency() const;

      double eulerNumberatMaximumFanStaticEfficiency() const;

      double maximumDimensionlessFanAirflow() const;

      boost::optional<double> motorFanPulleyRatio() const;

      bool isMotorFanPulleyRatioAutosized() const;

      boost::optional<double> beltMaximumTorque() const;

      bool isBeltMaximumTorqueAutosized() const;

      double beltSizingFactor() const;

      double beltFractionalTorqueTransition() const;

      double motorMaximumSpeed() const;

      boost::optional<double> maximumMotorOutputPower() const;

      bool isMaximumMotorOutputPowerAutosized() const;

      double motorSizingFactor() const;

      double motorInAirstreamFraction() const;

      std::string vFDEfficiencyType() const;

      boost::optional<double> maximumVFDOutputPower() const;

      bool isMaximumVFDOutputPowerAutosized() const;

      double vFDSizingFactor() const;

      Curve fanPressureRiseCurve() const;

      Curve ductStaticPressureResetCurve() const;

      Curve normalizedFanStaticEfficiencyCurveNonStallRegion() const;

      Curve normalizedFanStaticEfficiencyCurveStallRegion() const;

      Curve normalizedDimensionlessAirflowCurveNonStallRegion() const;

      Curve normalizedDimensionlessAirflowCurveStallRegion() const;

      boost::optional<Curve> maximumBeltEfficiencyCurve() const;

      boost::optional<Curve> normalizedBeltEfficiencyCurveRegion1() const;

      boost::optional<Curve> normalizedBeltEfficiencyCurveRegion2() const;

      boost::optional<Curve> normalizedBeltEfficiencyCurveRegion3() const;

      boost::optional<Curve> maximumMotorEfficiencyCurve() const;

      boost::optional<Curve> normalizedMotorEfficiencyCurve() const;

      boost::optional<Curve> vFDEfficiencyCurve() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumFlowRate(double maximumFlowRate);

      void autosizeMaximumFlowRate();

      bool setMinimumFlowRate(double minimumFlowRate);

      void autosizeMinimumFlowRate();

      bool setFanSizingFactor(double fanSizingFactor);

      bool setFanWheelDiameter(double fanWheelDiameter);

      bool setFanOutletArea(double fanOutletArea);

      bool setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency);

      bool setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency);

      bool setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow);

      bool setMotorFanPulleyRatio(double motorFanPulleyRatio);

      void autosizeMotorFanPulleyRatio();

      bool setBeltMaximumTorque(double beltMaximumTorque);

      void autosizeBeltMaximumTorque();

      bool setBeltSizingFactor(double beltSizingFactor);

      bool setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition);

      bool setMotorMaximumSpeed(double motorMaximumSpeed);

      bool setMaximumMotorOutputPower(double maximumMotorOutputPower);

      void autosizeMaximumMotorOutputPower();

      bool setMotorSizingFactor(double motorSizingFactor);

      bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

      bool setVFDEfficiencyType(const std::string& vFDEfficiencyType);

      bool setMaximumVFDOutputPower(double maximumVFDOutputPower);

      void autosizeMaximumVFDOutputPower();

      bool setVFDSizingFactor(double vFDSizingFactor);

      bool setFanPressureRiseCurve(const Curve& bivariateFunctions);

      bool setDuctStaticPressureResetCurve(const Curve& univariateFunctions);

      bool setNormalizedFanStaticEfficiencyCurveNonStallRegion(const Curve& univariateFunctions);

      bool setNormalizedFanStaticEfficiencyCurveStallRegion(const Curve& univariateFunctions);

      bool setNormalizedDimensionlessAirflowCurveNonStallRegion(const Curve& univariateFunctions);

      bool setNormalizedDimensionlessAirflowCurveStallRegion(const Curve& univariateFunctions);

      bool setMaximumBeltEfficiencyCurve(const Curve& univariateFunctions);

      void resetMaximumBeltEfficiencyCurve();

      bool setNormalizedBeltEfficiencyCurveRegion1(const Curve& univariateFunctions);

      void resetNormalizedBeltEfficiencyCurveRegion1();

      bool setNormalizedBeltEfficiencyCurveRegion2(const Curve& univariateFunctions);

      void resetNormalizedBeltEfficiencyCurveRegion2();

      bool setNormalizedBeltEfficiencyCurveRegion3(const Curve& univariateFunctions);

      void resetNormalizedBeltEfficiencyCurveRegion3();

      bool setMaximumMotorEfficiencyCurve(const Curve& univariateFunctions);

      void resetMaximumMotorEfficiencyCurve();

      bool setNormalizedMotorEfficiencyCurve(const Curve& univariateFunctions);

      void resetNormalizedMotorEfficiencyCurve();

      bool setVFDEfficiencyCurve(const Curve& univariateFunctions);

      void resetVFDEfficiencyCurve();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedMaximumFlowRate();
      boost::optional<double> autosizedMinimumFlowRate();
      boost::optional<double> autosizedMotorFanPulleyRatio();
      boost::optional<double> autosizedBeltMaximumTorque();
      boost::optional<double> autosizedMaximumMotorOutputPower();
      boost::optional<double> autosizedMaximumVFDOutputPower();

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanComponentModel");

      boost::optional<Curve> optionalFanPressureRiseCurve() const;
      boost::optional<Curve> optionalDuctStaticPressureResetCurve() const;
      boost::optional<Curve> optionalNormalizedFanStaticEfficiencyCurveNonStallRegion() const;
      boost::optional<Curve> optionalNormalizedFanStaticEfficiencyCurveStallRegion() const;
      boost::optional<Curve> optionalNormalizedDimensionlessAirflowCurveNonStallRegion() const;
      boost::optional<Curve> optionalNormalizedDimensionlessAirflowCurveStallRegion() const;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANCOMPONENTMODEL_IMPL_HPP
