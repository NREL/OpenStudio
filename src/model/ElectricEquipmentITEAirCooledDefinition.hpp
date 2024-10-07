/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

#include "../utilities/core/Deprecated.hpp"

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

    virtual ~ElectricEquipmentITEAirCooledDefinition() override = default;
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

    boost::optional<double> designLevel() const;
    OS_DEPRECATED(3, 8, 0) boost::optional<double> wattsperUnit() const;

    boost::optional<double> wattsperSpaceFloorArea() const;
    OS_DEPRECATED(3, 8, 0) boost::optional<double> wattsperZoneFloorArea() const;

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

    /** Also sets setDesignPowerInputCalculationMethod accordingly. */
    bool setDesignLevel(double designLevel);
    OS_DEPRECATED(3, 8, 0) bool setWattsperUnit(double designLevel);

    /** Also sets setDesignPowerInputCalculationMethod accordingly. */
    bool setWattsperSpaceFloorArea(double wattsperSpaceFloorArea);
    OS_DEPRECATED(3, 8, 0) bool setWattsperZoneFloorArea(double wattsperSpaceFloorArea);

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

    /** Returns the design level represented by this definition, assuming floorArea (m^2). */
    double getDesignLevel(double floorArea) const;
    OS_DEPRECATED(3, 8, 0) double getWattsperUnit(double floorArea) const;

    /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) */
    double getPowerPerFloorArea(double floorArea) const;
    OS_DEPRECATED(3, 8, 0) double getWattsperZoneFloorArea(double floorArea) const;

    /** If method is a \link validDesignPowerInputCalculationMethodValues valid design power input calculation method \endlink,
      * changes this definition to an equivalent power level, under the assumptions of floorArea (m^2). */
    bool setDesignPowerInputCalculationMethod(const std::string& designPowerInputCalculationMethod, double floorArea);

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
