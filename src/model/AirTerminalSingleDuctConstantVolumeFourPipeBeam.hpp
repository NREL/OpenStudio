/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~AirTerminalSingleDuctConstantVolumeFourPipeBeam() = default;
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
