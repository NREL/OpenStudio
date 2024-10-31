/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKZONE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKZONE_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;
  class AirflowNetworkOccupantVentilationControl;

  namespace detail {

    /** AirflowNetworkZone_Impl is a AirflowNetworkNode_Impl that is the implementation class for AirflowNetworkZone.*/
    class MODEL_API AirflowNetworkZone_Impl : public AirflowNetworkNode_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkZone_Impl(const AirflowNetworkZone_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkZone_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      ThermalZone thermalZone() const;

      std::string ventilationControlMode() const;

      bool isVentilationControlModeDefaulted() const;

      boost::optional<Schedule> ventilationControlZoneTemperatureSetpointSchedule() const;

      double minimumVentingOpenFactor() const;

      bool isMinimumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const;

      bool isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const;

      bool isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const;

      bool isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const;

      bool isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

      boost::optional<Schedule> ventingAvailabilitySchedule() const;

      std::string singleSidedWindPressureCoefficientAlgorithm() const;

      bool isSingleSidedWindPressureCoefficientAlgorithmDefaulted() const;

      double facadeWidth() const;

      bool isFacadeWidthDefaulted() const;

      boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

      //@}
      /** @name Setters */
      //@{

      bool setThermalZone(const ThermalZone& thermalZone);

      void resetThermalZone();

      bool setVentilationControlMode(const std::string& ventilationControlMode);

      void resetVentilationControlMode();

      // Note Schedules are passed by reference, not const reference.
      bool setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule);

      void resetVentilationControlZoneTemperatureSetpointSchedule();

      bool setMinimumVentingOpenFactor(double minimumVentingOpenFactor);

      void resetMinimumVentingOpenFactor();

      bool setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
        double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);

      void resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();

      bool setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
        double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);

      void resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();

      bool setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
        double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);

      void resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();

      bool setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
        double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);

      void resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();

      // Note Schedules are passed by reference, not const reference.
      bool setVentingAvailabilitySchedule(Schedule& schedule);

      void resetVentingAvailabilitySchedule();

      bool setSingleSidedWindPressureCoefficientAlgorithm(const std::string& singleSidedWindPressureCoefficientAlgorithm);

      void resetSingleSidedWindPressureCoefficientAlgorithm();

      bool setFacadeWidth(double facadeWidth);

      void resetFacadeWidth();

      bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);

      void resetOccupantVentilationControl();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkZone");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<ThermalZone> optionalThermalZone() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKZONE_IMPL_HPP
