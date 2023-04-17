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

#ifndef MODEL_COILHEATINGFOURPIPEBEAM_HPP
#define MODEL_COILHEATINGFOURPIPEBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Curve;
  class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

  namespace detail {

    class CoilHeatingFourPipeBeam_Impl;

  }  // namespace detail

  /** CoilHeatingFourPipeBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:FourPipeBeam'. */
  class MODEL_API CoilHeatingFourPipeBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingFourPipeBeam(const Model& model);

    virtual ~CoilHeatingFourPipeBeam() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingFourPipeBeam(const CoilHeatingFourPipeBeam& other) = default;
    CoilHeatingFourPipeBeam(CoilHeatingFourPipeBeam&& other) = default;
    CoilHeatingFourPipeBeam& operator=(const CoilHeatingFourPipeBeam&) = default;
    CoilHeatingFourPipeBeam& operator=(CoilHeatingFourPipeBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> hotWaterInletNode() const;

    boost::optional<Node> hotWaterOutletNode() const;

    double beamRatedHeatingCapacityperBeamLength() const;
    bool isBeamRatedHeatingCapacityperBeamLengthDefaulted() const;

    double beamRatedHeatingRoomAirHotWaterTemperatureDifference() const;
    bool isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const;

    double beamRatedHotWaterVolumeFlowRateperBeamLength() const;
    bool isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const;

    Curve beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const;

    Curve beamHeatingCapacityAirFlowModificationFactorCurve() const;

    Curve beamHeatingCapacityHotWaterFlowModificationFactorCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength);
    void resetBeamRatedHeatingCapacityperBeamLength();

    bool setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference);
    void resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference();

    bool setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength);
    void resetBeamRatedHotWaterVolumeFlowRateperBeamLength();

    bool setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);

    bool setBeamHeatingCapacityAirFlowModificationFactorCurve(const Curve& curve);

    bool setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const Curve& curve);

    // No reset functions provided for the three curves, because all curves are required if you connect the ATU FourPipeBeam to a hot water plant loop
    // And given that we added a subclass specifically for the heating portion of the ATU FourPipeBeam, they become required at all times

    //@}
    /** @name Other */
    //@{

    /* Convenience function to return the parent AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam */
    boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> airTerminalSingleDuctConstantVolumeFourPipeBeam() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingFourPipeBeam_Impl;

    explicit CoilHeatingFourPipeBeam(std::shared_ptr<detail::CoilHeatingFourPipeBeam_Impl> impl);

    friend class detail::CoilHeatingFourPipeBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingFourPipeBeam");
  };

  /** \relates CoilHeatingFourPipeBeam*/
  using OptionalCoilHeatingFourPipeBeam = boost::optional<CoilHeatingFourPipeBeam>;

  /** \relates CoilHeatingFourPipeBeam*/
  using CoilHeatingFourPipeBeamVector = std::vector<CoilHeatingFourPipeBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGFOURPIPEBEAM_HPP
