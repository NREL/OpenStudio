/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANCONSTANTVOLUME_IMPL_HPP
#define MODEL_FANCONSTANTVOLUME_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class AirflowNetworkFan;

  namespace detail {

    class MODEL_API FanConstantVolume_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      FanConstantVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      FanConstantVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      FanConstantVolume_Impl(const FanConstantVolume_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~FanConstantVolume_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      // Get all output variable names that could be associated with this object.
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
      /** @name Getters and Setters */
      //@{

      Schedule availabilitySchedule() const;
      bool setAvailabilitySchedule(Schedule& s);

      // Get FanTotalEfficiency
      double fanTotalEfficiency() const;

      // Set fanTotalEfficiency
      bool setFanTotalEfficiency(double val);

      // Get PressureRise
      double pressureRise() const;

      // Set PressureRise
      bool setPressureRise(double val);

      // Get MotorEfficiency
      double motorEfficiency() const;

      // Set MotorEfficiency
      bool setMotorEfficiency(double val);

      // Get MotorInAirstreamFraction
      double motorInAirstreamFraction() const;

      // Set MotorInAirstreamFraction
      bool setMotorInAirstreamFraction(double val);

      // Get EndUseSubcategory
      std::string endUseSubcategory() const;

      // Set EndUseSubcategory
      bool setEndUseSubcategory(const std::string& val);

      boost::optional<double> maximumFlowRate() const;

      bool isMaximumFlowRateAutosized() const;

      bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

      void resetMaximumFlowRate();

      void autosizeMaximumFlowRate();

      AirflowNetworkFan getAirflowNetworkFan();
      boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

      boost::optional<double> autosizedMaximumFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.FanConstantVolume");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailibiltyScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANCONSTANTVOLUME_IMPL_HPP
