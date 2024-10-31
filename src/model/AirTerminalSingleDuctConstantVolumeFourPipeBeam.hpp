/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class HVACComponent;
  class Schedule;
  class Node;
  class HVACComponent;
  class PlantLoop;

  namespace detail {

    class AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctConstantVolumeFourPipeBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam'. */
  class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctConstantVolumeFourPipeBeam(const Model& model, HVACComponent& coilCoolingFourPipeBeam,
                                                    HVACComponent& coilHeatingFourPipeBeam);

    /* This constructor does not instantiate any CoilCoolingFourPipeBeam nor CoilHeatingFourPipeBeam. At least one of these two is required
   * for this object to be ForwardTranslated, so you will have to do it manually and use setCoolingCoil and/or setHeatingCoil */
    AirTerminalSingleDuctConstantVolumeFourPipeBeam(const Model& model);

    virtual ~AirTerminalSingleDuctConstantVolumeFourPipeBeam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctConstantVolumeFourPipeBeam(const AirTerminalSingleDuctConstantVolumeFourPipeBeam& other) = default;
    AirTerminalSingleDuctConstantVolumeFourPipeBeam(AirTerminalSingleDuctConstantVolumeFourPipeBeam&& other) = default;
    AirTerminalSingleDuctConstantVolumeFourPipeBeam& operator=(const AirTerminalSingleDuctConstantVolumeFourPipeBeam&) = default;
    AirTerminalSingleDuctConstantVolumeFourPipeBeam& operator=(AirTerminalSingleDuctConstantVolumeFourPipeBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Availability Schedules */

    Schedule primaryAirAvailabilitySchedule() const;

    Schedule coolingAvailabilitySchedule() const;

    Schedule heatingAvailabilitySchedule() const;

    boost::optional<Node> primaryAirInletNode() const;
    boost::optional<Node> primaryAirOutletNode() const;

    /* This returns an optional because you can omit the cooling coil if you are modeling a heating-only beam
   * The only child class it will return is a CoilCoolingFourPipeBeam */
    boost::optional<HVACComponent> coolingCoil() const;

    /* This returns an optional because you can omit the heating coil if you are modeling a cooling-only beam
   * The only child class it will return is a CoilHeatingFourPipeBeam */
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

    /* The only object this method will accept is the companion class CoilCoolingFourPipeBeam */
    bool setCoolingCoil(const HVACComponent& coilCoolingFourPipeBeam);

    /* The only object this method will accept is the companion class CoilHeatingFourPipeBeam */
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

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl;

    explicit AirTerminalSingleDuctConstantVolumeFourPipeBeam(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl> impl);

    friend class detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeBeam");
  };

  /** \relates AirTerminalSingleDuctConstantVolumeFourPipeBeam*/
  using OptionalAirTerminalSingleDuctConstantVolumeFourPipeBeam = boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam>;

  /** \relates AirTerminalSingleDuctConstantVolumeFourPipeBeam*/
  using AirTerminalSingleDuctConstantVolumeFourPipeBeamVector = std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_HPP
