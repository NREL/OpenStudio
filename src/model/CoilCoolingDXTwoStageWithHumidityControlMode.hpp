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

#ifndef MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP
#define MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class CoilPerformanceDXCooling;
  class AirflowNetworkEquivalentDuct;

  namespace detail {

    class CoilCoolingDXTwoStageWithHumidityControlMode_Impl;

  }  // namespace detail

  /** CoilCoolingDXTwoStageWithHumidityControlMode is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode'. */
  class MODEL_API CoilCoolingDXTwoStageWithHumidityControlMode : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXTwoStageWithHumidityControlMode(const Model& model);

    virtual ~CoilCoolingDXTwoStageWithHumidityControlMode() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXTwoStageWithHumidityControlMode(const CoilCoolingDXTwoStageWithHumidityControlMode& other) = default;
    CoilCoolingDXTwoStageWithHumidityControlMode(CoilCoolingDXTwoStageWithHumidityControlMode&& other) = default;
    CoilCoolingDXTwoStageWithHumidityControlMode& operator=(const CoilCoolingDXTwoStageWithHumidityControlMode&) = default;
    CoilCoolingDXTwoStageWithHumidityControlMode& operator=(CoilCoolingDXTwoStageWithHumidityControlMode&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    int numberofCapacityStages() const;

    int numberofEnhancedDehumidificationModes() const;

    boost::optional<CoilPerformanceDXCooling> normalModeStage1CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> normalModeStage1Plus2CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1Plus2CoilPerformance() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setNumberofCapacityStages(int numberofCapacityStages);

    bool setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes);

    bool setNormalModeStage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetNormalModeStage1CoilPerformance();

    bool setNormalModeStage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetNormalModeStage1Plus2CoilPerformance();

    bool setDehumidificationMode1Stage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetDehumidificationMode1Stage1CoilPerformance();

    bool setDehumidificationMode1Stage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetDehumidificationMode1Stage1Plus2CoilPerformance();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    //@}
    /** @name Other */
    //@{

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;

    explicit CoilCoolingDXTwoStageWithHumidityControlMode(std::shared_ptr<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl> impl);

    friend class detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoStageWithHumidityControlMode");
  };

  /** \relates CoilCoolingDXTwoStageWithHumidityControlMode*/
  using OptionalCoilCoolingDXTwoStageWithHumidityControlMode = boost::optional<CoilCoolingDXTwoStageWithHumidityControlMode>;

  /** \relates CoilCoolingDXTwoStageWithHumidityControlMode*/
  using CoilCoolingDXTwoStageWithHumidityControlModeVector = std::vector<CoilCoolingDXTwoStageWithHumidityControlMode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP
