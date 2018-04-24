/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGFOURPIPEBEAM_IMPL_HPP
#define MODEL_COILCOOLINGFOURPIPEBEAM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Node;
class Curve;
class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

namespace detail {

  /** CoilCoolingFourPipeBeam_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingFourPipeBeam.*/
  class MODEL_API CoilCoolingFourPipeBeam_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingFourPipeBeam_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoilCoolingFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoilCoolingFourPipeBeam_Impl(const CoilCoolingFourPipeBeam_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~CoilCoolingFourPipeBeam_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;

    virtual boost::optional<StraightComponent> containingStraightComponent() const override;

    virtual bool addToNode(Node & node) override;

    //@}
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

    bool setChilledWaterInletNode(const Node& connection);
    void resetChilledWaterInletNode();

    bool setChilledWaterOutletNode(const Node& connection);
    void resetChilledWaterOutletNode();

    bool setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength);
    void resetBeamRatedCoolingCapacityperBeamLength();

    bool setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
    void resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();

    bool setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength);
    void resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();

    bool setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);

    bool setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve);

    bool setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    /* Convenience function to return the parent AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam */
    boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> airTerminalSingleDuctConstantVolumeFourPipeBeam() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingFourPipeBeam");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGFOURPIPEBEAM_IMPL_HPP

