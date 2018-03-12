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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Schedule;
class Schedule;
class Connection;
class Connection;
class CoolingCoilFourPipeBeam;
class HeatingCoilFourPipeBeam;

namespace detail {

  /** AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeFourPipeBeam.*/
  class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle);

    AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle);

    AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle);

    virtual ~AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> primaryAirAvailabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> coolingAvailabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> heatingAvailabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection primaryAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection primaryAirOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: CoolingCoilFourPipeBeam.
    CoolingCoilFourPipeBeam coolingCoil() const;

    // TODO: Check return type. From object lists, some candidates are: HeatingCoilFourPipeBeam.
    HeatingCoilFourPipeBeam heatingCoil() const;

    boost::optional<double> designPrimaryAirVolumeFlowRate() const;

    bool isDesignPrimaryAirVolumeFlowRateDefaulted() const;

    bool isDesignPrimaryAirVolumeFlowRateAutosized() const;

    boost::optional <double> autosizedDesignPrimaryAirVolumeFlowRate();

    boost::optional<double> designChilledWaterVolumeFlowRate() const;

    bool isDesignChilledWaterVolumeFlowRateDefaulted() const;

    bool isDesignChilledWaterVolumeFlowRateAutosized() const;

    boost::optional <double> autosizedDesignChilledWaterVolumeFlowRate();

    boost::optional<double> designHotWaterVolumeFlowRate() const;

    bool isDesignHotWaterVolumeFlowRateDefaulted() const;

    bool isDesignHotWaterVolumeFlowRateAutosized() const;

    boost::optional <double> autosizedDesignHotWaterVolumeFlowRate();

    boost::optional<double> zoneTotalBeamLength() const;

    bool isZoneTotalBeamLengthDefaulted() const;

    bool isZoneTotalBeamLengthAutosized() const;

    boost::optional <double> autosizedZoneTotalBeamLength();

    double ratedPrimaryAirFlowRateperBeamLength() const;

    bool isRatedPrimaryAirFlowRateperBeamLengthDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setPrimaryAirAvailabilitySchedule(Schedule& schedule);

    void resetPrimaryAirAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setCoolingAvailabilitySchedule(Schedule& schedule);

    void resetCoolingAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setHeatingAvailabilitySchedule(Schedule& schedule);

    void resetHeatingAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setPrimaryAirInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setPrimaryAirOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: CoolingCoilFourPipeBeam.
    bool setCoolingCoil(const CoolingCoilFourPipeBeam& coolingCoilFourPipeBeam);

    // TODO: Check argument type. From object lists, some candidates are: HeatingCoilFourPipeBeam.
    bool setHeatingCoil(const HeatingCoilFourPipeBeam& heatingCoilFourPipeBeam);

    bool setDesignPrimaryAirVolumeFlowRate(double designPrimaryAirVolumeFlowRate);

    void resetDesignPrimaryAirVolumeFlowRate();

    void autosizeDesignPrimaryAirVolumeFlowRate();

    bool setDesignChilledWaterVolumeFlowRate(double designChilledWaterVolumeFlowRate);

    void resetDesignChilledWaterVolumeFlowRate();

    void autosizeDesignChilledWaterVolumeFlowRate();

    bool setDesignHotWaterVolumeFlowRate(double designHotWaterVolumeFlowRate);

    void resetDesignHotWaterVolumeFlowRate();

    void autosizeDesignHotWaterVolumeFlowRate();

    bool setZoneTotalBeamLength(double zoneTotalBeamLength);

    void resetZoneTotalBeamLength();

    void autosizeZoneTotalBeamLength();

    bool setRatedPrimaryAirFlowRateperBeamLength(double ratedPrimaryAirFlowRateperBeamLength);

    void resetRatedPrimaryAirFlowRateperBeamLength();

    virtual void autosize() override;

    virtual void applySizingValues() override;

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeBeam");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalPrimaryAirInletNode() const;
    boost::optional<Connection> optionalPrimaryAirOutletNode() const;
    boost::optional<CoolingCoilFourPipeBeam> optionalCoolingCoil() const;
    boost::optional<HeatingCoilFourPipeBeam> optionalHeatingCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP

