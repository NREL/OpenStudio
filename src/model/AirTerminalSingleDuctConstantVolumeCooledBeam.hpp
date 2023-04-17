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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
//#include "ThermalZone.hpp"
#include "ModelObject.hpp"
#include "Connection.hpp"
//#include "Model.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class AirTerminalSingleDuctConstantVolumeCooledBeam_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctConstantVolumeCooledBeam is a ZoneHAVC that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:CooledBeam'. */
  class MODEL_API AirTerminalSingleDuctConstantVolumeCooledBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctConstantVolumeCooledBeam(const Model& model, Schedule& availabilitySchedule, HVACComponent& coilCoolingCooledBeam);

    virtual ~AirTerminalSingleDuctConstantVolumeCooledBeam() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctConstantVolumeCooledBeam(const AirTerminalSingleDuctConstantVolumeCooledBeam& other) = default;
    AirTerminalSingleDuctConstantVolumeCooledBeam(AirTerminalSingleDuctConstantVolumeCooledBeam&& other) = default;
    AirTerminalSingleDuctConstantVolumeCooledBeam& operator=(const AirTerminalSingleDuctConstantVolumeCooledBeam&) = default;
    AirTerminalSingleDuctConstantVolumeCooledBeam& operator=(AirTerminalSingleDuctConstantVolumeCooledBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    HVACComponent coilCoolingCooledBeam() const;

    static std::vector<std::string> cooledBeamTypeValues();

    std::string cooledBeamType() const;

    boost::optional<double> supplyAirVolumetricFlowRate() const;

    bool isSupplyAirVolumetricFlowRateDefaulted() const;

    bool isSupplyAirVolumetricFlowRateAutosized() const;

    boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate() const;

    bool isMaximumTotalChilledWaterVolumetricFlowRateDefaulted() const;

    bool isMaximumTotalChilledWaterVolumetricFlowRateAutosized() const;

    boost::optional<int> numberofBeams() const;

    bool isNumberofBeamsDefaulted() const;

    bool isNumberofBeamsAutosized() const;

    boost::optional<double> beamLength() const;

    bool isBeamLengthDefaulted() const;

    bool isBeamLengthAutosized() const;

    double designInletWaterTemperature() const;

    bool isDesignInletWaterTemperatureDefaulted() const;

    double designOutletWaterTemperature() const;

    bool isDesignOutletWaterTemperatureDefaulted() const;

    boost::optional<double> coefficientofInductionKin() const;

    bool isCoefficientofInductionKinDefaulted() const;

    bool isCoefficientofInductionKinAutocalculated() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setCoolingCoil(HVACComponent& coilCoolingCooledBeam);

    bool setCooledBeamType(const std::string& cooledBeamType);

    bool setSupplyAirVolumetricFlowRate(double supplyAirVolumetricFlowRate);

    void resetSupplyAirVolumetricFlowRate();

    void autosizeSupplyAirVolumetricFlowRate();

    bool setMaximumTotalChilledWaterVolumetricFlowRate(double maximumTotalChilledWaterVolumetricFlowRate);

    void resetMaximumTotalChilledWaterVolumetricFlowRate();

    void autosizeMaximumTotalChilledWaterVolumetricFlowRate();

    bool setNumberofBeams(int numberofBeams);

    void resetNumberofBeams();

    void autosizeNumberofBeams();

    bool setBeamLength(double beamLength);

    void resetBeamLength();

    void autosizeBeamLength();

    bool setDesignInletWaterTemperature(double designInletWaterTemperature);

    void resetDesignInletWaterTemperature();

    bool setDesignOutletWaterTemperature(double designOutletWaterTemperature);

    void resetDesignOutletWaterTemperature();

    bool setCoefficientofInductionKin(double coefficientofInductionKin);

    void resetCoefficientofInductionKin();

    void autocalculateCoefficientofInductionKin();

    // boost::optional<ThermalZone> thermalZone();

    // bool addToThermalZone(ThermalZone & thermalZone);

    // void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirVolumetricFlowRate() const;

    boost::optional<double> autosizedMaximumTotalChilledWaterVolumetricFlowRate() const;

    boost::optional<double> autosizedNumberofBeams() const;

    boost::optional<double> autosizedBeamLength() const;

    boost::optional<double> autosizedCoefficientofInductionKin() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl;

    explicit AirTerminalSingleDuctConstantVolumeCooledBeam(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl> impl);

    friend class detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeCooledBeam");
  };

  /** \relates AirTerminalSingleDuctConstantVolumeCooledBeam*/
  using OptionalAirTerminalSingleDuctConstantVolumeCooledBeam = boost::optional<AirTerminalSingleDuctConstantVolumeCooledBeam>;

  /** \relates AirTerminalSingleDuctConstantVolumeCooledBeam*/
  using AirTerminalSingleDuctConstantVolumeCooledBeamVector = std::vector<AirTerminalSingleDuctConstantVolumeCooledBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_HPP
