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

class HVACComponent;
class Schedule;
class Node;
class HVACComponent;
class PlantLoop;

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

    virtual std::vector<ModelObject> children() const override;

    virtual bool addToNode(Node & node) override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual ModelObject clone(Model model) const override;

    // TODO: virtual std::vector<IdfObject> remove() override;

    virtual bool isRemovable() const override;

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
    boost::optional <double> autosizedDesignPrimaryAirVolumeFlowRate();
    boost::optional <double> autosizedDesignChilledWaterVolumeFlowRate();
    boost::optional <double> autosizedDesignHotWaterVolumeFlowRate();
    boost::optional <double> autosizedZoneTotalBeamLength();

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

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEBEAM_IMPL_HPP

