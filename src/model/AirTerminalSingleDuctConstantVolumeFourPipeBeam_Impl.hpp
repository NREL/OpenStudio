/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class HVACComponent;
  class Schedule;
  class Node;
  class HVACComponent;
  class PlantLoop;

  namespace detail {

    /** AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeFourPipeBeam.*/
    class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual bool addToNode(Node& node) override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual bool isRemovable() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      /** Availability Schedules */

      Schedule primaryAirAvailabilitySchedule() const;

      Schedule coolingAvailabilitySchedule() const;

      Schedule heatingAvailabilitySchedule() const;

      boost::optional<Node> primaryAirInletNode() const;
      boost::optional<Node> primaryAirOutletNode() const;

      boost::optional<HVACComponent> coolingCoil() const;

      boost::optional<HVACComponent> heatingCoil() const;

      /** Autosizable fields */

      boost::optional<double> designPrimaryAirVolumeFlowRate() const;
      bool isDesignPrimaryAirVolumeFlowRateAutosized() const;

      boost::optional<double> designChilledWaterVolumeFlowRate() const;
      bool isDesignChilledWaterVolumeFlowRateAutosized() const;

      boost::optional<double> designHotWaterVolumeFlowRate() const;
      bool isDesignHotWaterVolumeFlowRateAutosized() const;

      boost::optional<double> zoneTotalBeamLength() const;
      bool isZoneTotalBeamLengthAutosized() const;

      /* Double fields with defaults */
      double ratedPrimaryAirFlowRateperBeamLength() const;
      bool isRatedPrimaryAirFlowRateperBeamLengthDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setPrimaryAirAvailabilitySchedule(Schedule& schedule);

      bool setCoolingAvailabilitySchedule(Schedule& schedule);

      bool setHeatingAvailabilitySchedule(Schedule& schedule);

      /* Set Cooling and Heating Coil. */
      bool setCoolingCoil(const HVACComponent& coilCoolingFourPipeBeam);
      bool setHeatingCoil(const HVACComponent& coilHeatingFourPipeBeam);

      /* Autosizable fields */
      bool setDesignPrimaryAirVolumeFlowRate(double designPrimaryAirVolumeFlowRate);
      void autosizeDesignPrimaryAirVolumeFlowRate();

      bool setDesignChilledWaterVolumeFlowRate(double designChilledWaterVolumeFlowRate);
      void autosizeDesignChilledWaterVolumeFlowRate();

      bool setDesignHotWaterVolumeFlowRate(double designHotWaterVolumeFlowRate);
      void autosizeDesignHotWaterVolumeFlowRate();

      bool setZoneTotalBeamLength(double zoneTotalBeamLength);
      void autosizeZoneTotalBeamLength();

      bool setRatedPrimaryAirFlowRateperBeamLength(double ratedPrimaryAirFlowRateperBeamLength);
      void resetRatedPrimaryAirFlowRateperBeamLength();

      //@}
      /** @name Other */
      //@{

      /* Convenience method to return the chilled water PlantLoop */
      boost::optional<PlantLoop> chilledWaterPlantLoop() const;
      boost::optional<Node> chilledWaterInletNode() const;
      boost::optional<Node> chilledWaterOutletNode() const;

      /* Convenience method to return the hot water PlantLoop */
      boost::optional<PlantLoop> hotWaterPlantLoop() const;
      boost::optional<Node> hotWaterInletNode() const;
      boost::optional<Node> hotWaterOutletNode() const;

      /* Queries the SQL file after a successful sizing run */
      boost::optional<double> autosizedDesignPrimaryAirVolumeFlowRate();
      boost::optional<double> autosizedDesignChilledWaterVolumeFlowRate();
      boost::optional<double> autosizedDesignHotWaterVolumeFlowRate();
      boost::optional<double> autosizedZoneTotalBeamLength();

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeBeam");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Node> optionalPrimaryAirInletNode() const;
      boost::optional<Node> optionalPrimaryAirOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP
