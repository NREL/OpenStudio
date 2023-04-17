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

#ifndef MODEL_COILCOOLINGFOURPIPEBEAM_HPP
#define MODEL_COILCOOLINGFOURPIPEBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Curve;
  class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

  namespace detail {

    class CoilCoolingFourPipeBeam_Impl;

  }  // namespace detail

  /** CoilCoolingFourPipeBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:FourPipeBeam'. */
  class MODEL_API CoilCoolingFourPipeBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingFourPipeBeam(const Model& model);

    virtual ~CoilCoolingFourPipeBeam() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingFourPipeBeam(const CoilCoolingFourPipeBeam& other) = default;
    CoilCoolingFourPipeBeam(CoilCoolingFourPipeBeam&& other) = default;
    CoilCoolingFourPipeBeam& operator=(const CoilCoolingFourPipeBeam&) = default;
    CoilCoolingFourPipeBeam& operator=(CoilCoolingFourPipeBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> chilledWaterInletNode() const;

    boost::optional<Node> chilledWaterOutletNode() const;

    double beamRatedCoolingCapacityperBeamLength() const;
    bool isBeamRatedCoolingCapacityperBeamLengthDefaulted() const;

    double beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const;
    bool isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const;

    double beamRatedChilledWaterVolumeFlowRateperBeamLength() const;
    bool isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const;

    Curve beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const;

    Curve beamCoolingCapacityAirFlowModificationFactorCurve() const;

    Curve beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength);
    void resetBeamRatedCoolingCapacityperBeamLength();

    bool setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
    void resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();

    bool setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength);
    void resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();

    bool setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);

    bool setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve);

    bool setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve);

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
    using ImplType = detail::CoilCoolingFourPipeBeam_Impl;

    explicit CoilCoolingFourPipeBeam(std::shared_ptr<detail::CoilCoolingFourPipeBeam_Impl> impl);

    friend class detail::CoilCoolingFourPipeBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingFourPipeBeam");
  };

  /** \relates CoilCoolingFourPipeBeam*/
  using OptionalCoilCoolingFourPipeBeam = boost::optional<CoilCoolingFourPipeBeam>;

  /** \relates CoilCoolingFourPipeBeam*/
  using CoilCoolingFourPipeBeamVector = std::vector<CoilCoolingFourPipeBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGFOURPIPEBEAM_HPP
