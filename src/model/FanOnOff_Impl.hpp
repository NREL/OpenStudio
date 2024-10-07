/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANONOFF_IMPL_HPP
#define MODEL_FANONOFF_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class AirflowNetworkFan;

  namespace detail {

    /** FanOnOff_Impl is a StraightComponent_Impl that is the implementation class for FanOnOff.*/
    class MODEL_API FanOnOff_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanOnOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanOnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanOnOff_Impl(const FanOnOff_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanOnOff_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      // Field Availability Schedule Name

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      // Field Fan Efficiency

      double fanTotalEfficiency() const;

      bool isFanTotalEfficiencyDefaulted() const;

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      void resetFanTotalEfficiency();

      // Field Pressure Rise

      double pressureRise() const;

      bool setPressureRise(double pressureRise);

      // Field Maximum Flow Rate

      boost::optional<double> maximumFlowRate() const;

      bool isMaximumFlowRateAutosized() const;

      bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

      void resetMaximumFlowRate();

      void autosizeMaximumFlowRate();

      // Field Motor Efficiency

      double motorEfficiency() const;

      bool isMotorEfficiencyDefaulted() const;

      bool setMotorEfficiency(double motorEfficiency);

      void resetMotorEfficiency();

      // Field Motor In Airstream Fraction

      boost::optional<double> motorInAirstreamFraction() const;

      bool setMotorInAirstreamFraction(boost::optional<double> motorInAirstreamFraction);

      void resetMotorInAirstreamFraction();

      // Field End-Use Subcategory

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      //@}
      /** @name Curves */
      //@{

      Curve fanPowerRatioFunctionofSpeedRatioCurve() const;
      bool setFanPowerRatioFunctionofSpeedRatioCurve(const Curve& curve);

      Curve fanEfficiencyRatioFunctionofSpeedRatioCurve() const;
      bool setFanEfficiencyRatioFunctionofSpeedRatioCurve(const Curve& curve);

      boost::optional<double> autosizedMaximumFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}

      AirflowNetworkFan getAirflowNetworkFan();
      boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

      //protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanOnOff");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANONOFF_IMPL_HPP
