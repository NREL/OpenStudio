/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSURFACE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkLinkage_Impl.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;
  class AirflowNetworkComponent;
  class AirflowNetworkExternalNode;
  class Schedule;
  class AirflowNetworkOccupantVentilationControl;

  namespace detail {

    /** AirflowNetworkSurface_Impl is a AirflowNetworkLinkage_Impl that is the implementation class for AirflowNetworkSurface.*/
    class MODEL_API AirflowNetworkSurface_Impl : public AirflowNetworkLinkage_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkSurface_Impl(const AirflowNetworkSurface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkSurface_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool setParent(ParentObject& surfAndSubSurf) override;

      //@}
      /** @name Getters */
      //@{

      PlanarSurface surface() const;

      boost::optional<AirflowNetworkComponent> leakageComponent() const;

      boost::optional<AirflowNetworkExternalNode> externalNode() const;

      double windowDoorOpeningFactorOrCrackFactor() const;

      bool isWindowDoorOpeningFactorOrCrackFactorDefaulted() const;

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

      boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

      //@}
      /** @name Setters */
      //@{

      bool setSurface(const PlanarSurface& surfAndSubSurf);

      void resetSurface();

      bool setLeakageComponent(const AirflowNetworkComponent& surfaceAirflowLeakage);

      bool setExternalNode(const AirflowNetworkExternalNode& externalNode);

      void resetExternalNode();

      bool setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor);

      void resetWindowDoorOpeningFactorOrCrackFactor();

      bool setVentilationControlMode(const std::string& ventilationControlMode);

      void resetVentilationControlMode();

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

      bool setVentingAvailabilitySchedule(Schedule& schedule);

      void resetVentingAvailabilitySchedule();

      bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);

      void resetOccupantVentilationControl();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkSurface");

      boost::optional<PlanarSurface> optionalSurface() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSURFACE_IMPL_HPP
