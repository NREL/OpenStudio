/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMENOREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMENOREHEAT_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API AirTerminalSingleDuctConstantVolumeNoReheat_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      AirTerminalSingleDuctConstantVolumeNoReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      AirTerminalSingleDuctConstantVolumeNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      AirTerminalSingleDuctConstantVolumeNoReheat_Impl(const AirTerminalSingleDuctConstantVolumeNoReheat_Impl& other, Model_Impl* model,
                                                       bool keepHandle);

      // virtual destructor
      virtual ~AirTerminalSingleDuctConstantVolumeNoReheat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool isRemovable() const override;

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

      /** Returns the Schedule referred to by the AvailabilityScheduleName field. **/
      Schedule availabilitySchedule() const;

      /** Sets the Schedule referred to by the AvailabilityScheduleName field. **/
      bool setAvailabilitySchedule(Schedule& schedule);

      boost::optional<double> maximumAirFlowRate() const;

      bool isMaximumAirFlowRateAutosized() const;

      bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

      void autosizeMaximumAirFlowRate();

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeNoReheat");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMENOREHEAT_IMPL_HPP
