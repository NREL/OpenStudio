/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_IMPL_HPP
#define MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Curve;
  class Schedule;

  namespace detail {

    /** AirflowNetworkOccupantVentilationControl_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkOccupantVentilationControl.*/
    class MODEL_API AirflowNetworkOccupantVentilationControl_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkOccupantVentilationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkOccupantVentilationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkOccupantVentilationControl_Impl(const AirflowNetworkOccupantVentilationControl_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkOccupantVentilationControl_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      double minimumOpeningTime() const;

      bool isMinimumOpeningTimeDefaulted() const;

      double minimumClosingTime() const;

      bool isMinimumClosingTimeDefaulted() const;

      Curve thermalComfortLowTemperatureCurve() const;

      double thermalComfortTemperatureBoundaryPoint() const;

      bool isThermalComfortTemperatureBoundaryPointDefaulted() const;

      boost::optional<Curve> thermalComfortHighTemperatureCurve() const;

      double maximumPredictedPercentageofDissatisfiedThreshold() const;

      bool isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted() const;

      bool occupancyCheck() const;

      bool isOccupancyCheckDefaulted() const;

      boost::optional<Schedule> openingProbabilitySchedule() const;

      boost::optional<Schedule> closingProbabilitySchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setMinimumOpeningTime(double minimumOpeningTime);

      void resetMinimumOpeningTime();

      bool setMinimumClosingTime(double minimumClosingTime);

      void resetMinimumClosingTime();

      bool setThermalComfortLowTemperatureCurve(const Curve& curve);

      //void resetThermalComfortLowTemperatureCurve();

      bool setThermalComfortTemperatureBoundaryPoint(double thermalComfortTemperatureBoundaryPoint);

      void resetThermalComfortTemperatureBoundaryPoint();

      bool setThermalComfortHighTemperatureCurve(const Curve& curve);

      void resetThermalComfortHighTemperatureCurve();

      bool setMaximumPredictedPercentageofDissatisfiedThreshold(double maximumPredictedPercentageofDissatisfiedThreshold);

      void resetMaximumPredictedPercentageofDissatisfiedThreshold();

      void setOccupancyCheck(bool occupancyCheck);

      void resetOccupancyCheck();

      bool setOpeningProbabilitySchedule(Schedule& schedule);

      void resetOpeningProbabilitySchedule();

      bool setClosingProbabilitySchedule(Schedule& schedule);

      void resetClosingProbabilitySchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkOccupantVentilationControl");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_IMPL_HPP
