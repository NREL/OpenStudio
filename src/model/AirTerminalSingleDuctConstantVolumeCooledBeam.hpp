/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~AirTerminalSingleDuctConstantVolumeCooledBeam() override = default;
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
