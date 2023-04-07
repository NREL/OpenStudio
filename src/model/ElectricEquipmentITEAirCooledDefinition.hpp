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

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Connection;
  class Schedule;

  namespace detail {

    class ElectricEquipmentITEAirCooledDefinition_Impl;

  }  // namespace detail

  /** ElectricEquipmentITEAirCooledDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 'OS:ElectricEquipment:ITE:AirCooled:Definition'. */
  class MODEL_API ElectricEquipmentITEAirCooledDefinition : public SpaceLoadDefinition
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricEquipmentITEAirCooledDefinition(const Model& model, const Curve& cPUPowerInputFunctionofLoadingandAirTemperatureCurve,
                                                     const Curve& airFlowFunctionofLoadingandAirTemperatureCurve,
                                                     const Curve& fanPowerInputFunctionofFlowCurve);

    /** Create ElectricEquipmentITEAirCooledDefinition with default curves **/
    explicit ElectricEquipmentITEAirCooledDefinition(const Model& model);

    virtual ~ElectricEquipmentITEAirCooledDefinition() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricEquipmentITEAirCooledDefinition(const ElectricEquipmentITEAirCooledDefinition& other) = default;
    ElectricEquipmentITEAirCooledDefinition(ElectricEquipmentITEAirCooledDefinition&& other) = default;
    ElectricEquipmentITEAirCooledDefinition& operator=(const ElectricEquipmentITEAirCooledDefinition&) = default;
    ElectricEquipmentITEAirCooledDefinition& operator=(ElectricEquipmentITEAirCooledDefinition&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validAirFlowCalculationMethodValues();

    static std::vector<std::string> validDesignPowerInputCalculationMethodValues();

    static std::vector<std::string> validEnvironmentalClassValues();

    static std::vector<std::string> validAirInletConnectionTypeValues();

    /** @name Getters */
    //@{

    std::string airFlowCalculationMethod() const;

    std::string designPowerInputCalculationMethod() const;

    boost::optional<double> wattsperUnit() const;

    boost::optional<double> wattsperZoneFloorArea() const;

    Curve cPUPowerInputFunctionofLoadingandAirTemperatureCurve() const;

    double designFanPowerInputFraction() const;

    bool isDesignFanPowerInputFractionDefaulted() const;

    boost::optional<double> designFanAirFlowRateperPowerInput() const;

    Curve airFlowFunctionofLoadingandAirTemperatureCurve() const;

    Curve fanPowerInputFunctionofFlowCurve() const;

    double designEnteringAirTemperature() const;

    bool isDesignEnteringAirTemperatureDefaulted() const;

    std::string environmentalClass() const;

    std::string airInletConnectionType() const;

    double designRecirculationFraction() const;

    bool isDesignRecirculationFractionDefaulted() const;

    boost::optional<Curve> recirculationFunctionofLoadingandSupplyTemperatureCurve() const;

    double designElectricPowerSupplyEfficiency() const;

    bool isDesignElectricPowerSupplyEfficiencyDefaulted() const;

    boost::optional<Curve> electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() const;

    double fractionofElectricPowerSupplyLossestoZone() const;

    bool isFractionofElectricPowerSupplyLossestoZoneDefaulted() const;

    double supplyTemperatureDifference() const;

    bool isSupplyTemperatureDifferenceDefaulted() const;

    boost::optional<Schedule> supplyTemperatureDifferenceSchedule() const;

    double returnTemperatureDifference() const;

    bool isReturnTemperatureDifferenceDefaulted() const;

    boost::optional<Schedule> returnTemperatureDifferenceSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAirFlowCalculationMethod(const std::string& airFlowCalculationMethod);

    bool setDesignPowerInputCalculationMethod(const std::string& designPowerInputCalculationMethod, double floorArea);

    bool setWattsperUnit(double wattsperUnit);

    bool setWattsperZoneFloorArea(double wattsperZoneFloorArea);

    bool setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(const Curve& curve);

    bool setDesignFanPowerInputFraction(double designFanPowerInputFraction);

    void resetDesignFanPowerInputFraction();

    bool setDesignFanAirFlowRateperPowerInput(double designFanAirFlowRateperPowerInput);

    bool setAirFlowFunctionofLoadingandAirTemperatureCurve(const Curve& curve);

    bool setFanPowerInputFunctionofFlowCurve(const Curve& curve);

    void setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    void resetDesignEnteringAirTemperature();

    bool setEnvironmentalClass(const std::string& environmentalClass);

    bool setAirInletConnectionType(const std::string& airInletConnectionType);

    bool setDesignRecirculationFraction(double designRecirculationFraction);

    void resetDesignRecirculationFraction();

    bool setRecirculationFunctionofLoadingandSupplyTemperatureCurve(const Curve& curve);

    void resetRecirculationFunctionofLoadingandSupplyTemperatureCurve();

    bool setDesignElectricPowerSupplyEfficiency(double designElectricPowerSupplyEfficiency);

    void resetDesignElectricPowerSupplyEfficiency();

    bool setElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve);

    void resetElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();

    bool setFractionofElectricPowerSupplyLossestoZone(double fractionofElectricPowerSupplyLossestoZone);

    void resetFractionofElectricPowerSupplyLossestoZone();

    bool setSupplyTemperatureDifference(double supplyTemperatureDifference);

    void resetSupplyTemperatureDifference();

    // Note Schedules are passed by reference, not const reference.
    bool setSupplyTemperatureDifferenceSchedule(Schedule& schedule);

    void resetSupplyTemperatureDifferenceSchedule();

    bool setReturnTemperatureDifference(double returnTemperatureDifference);

    void resetReturnTemperatureDifference();

    // Note Schedules are passed by reference, not const reference.
    bool setReturnTemperatureDifferenceSchedule(Schedule& schedule);

    void resetReturnTemperatureDifferenceSchedule();

    //@}
    /** @name Other */
    //@{

    double getWattsperUnit(double floorArea) const;

    double getWattsperZoneFloorArea(double floorArea) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricEquipmentITEAirCooledDefinition_Impl;

    explicit ElectricEquipmentITEAirCooledDefinition(std::shared_ptr<detail::ElectricEquipmentITEAirCooledDefinition_Impl> impl);

    friend class Model;
    friend class openstudio::IdfObject;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooledDefinition");
  };

  /** \relates ElectricEquipmentITEAirCooledDefinition*/
  using OptionalElectricEquipmentITEAirCooledDefinition = boost::optional<ElectricEquipmentITEAirCooledDefinition>;

  /** \relates ElectricEquipmentITEAirCooledDefinition*/
  using ElectricEquipmentITEAirCooledDefinitionVector = std::vector<ElectricEquipmentITEAirCooledDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP
