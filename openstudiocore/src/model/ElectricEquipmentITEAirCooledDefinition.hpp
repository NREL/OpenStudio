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

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Curve;
class Connection;
class Schedule;

namespace detail {

class ElectricEquipmentITEAirCooledDefinition_Impl;

} // detail

/** ElectricEquipmentITEAirCooledDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 'OS:ElectricEquipment:ITE:AirCooled:Definition'. */
class MODEL_API ElectricEquipmentITEAirCooledDefinition : public SpaceLoadDefinition {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricEquipmentITEAirCooledDefinition(const Model& model,
    Curve& cPUPowerInputFunctionofLoadingandAirTemperatureCurve,
    Curve& airFlowFunctionofLoadingandAirTemperatureCurve,
    Curve& fanPowerInputFunctionofFlowCurve);

  /** Create ElectricEquipmentITEAirCooledDefinition with default curves **/
  explicit ElectricEquipmentITEAirCooledDefinition(const Model& model);

  virtual ~ElectricEquipmentITEAirCooledDefinition() {}

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

  // TODO: Check return type. From object lists, some candidates are: BiVariateCurves, BiVariateTables.
  Curve cPUPowerInputFunctionofLoadingandAirTemperatureCurve() const;

  double designFanPowerInputFraction() const;

  bool isDesignFanPowerInputFractionDefaulted() const;

  boost::optional<double> designFanAirFlowRateperPowerInput() const;

  // TODO: Check return type. From object lists, some candidates are: BiVariateCurves, BiVariateTables.
  Curve airFlowFunctionofLoadingandAirTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: UniVariateCurves, UniVariateTables.
  Curve fanPowerInputFunctionofFlowCurve() const;

  double designEnteringAirTemperature() const;

  bool isDesignEnteringAirTemperatureDefaulted() const;

  std::string environmentalClass() const;

  std::string airInletConnectionType() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> supplyAirNode() const;

  double designRecirculationFraction() const;

  bool isDesignRecirculationFractionDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: BiVariateCurves, BiVariateTables.
  boost::optional<Curve> recirculationFunctionofLoadingandSupplyTemperatureCurve() const;

  double designElectricPowerSupplyEfficiency() const;

  bool isDesignElectricPowerSupplyEfficiencyDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: UniVariateCurves, UniVariateTables.
  boost::optional<Curve> electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() const;

  double fractionofElectricPowerSupplyLossestoZone() const;

  bool isFractionofElectricPowerSupplyLossestoZoneDefaulted() const;

  double supplyTemperatureDifference() const;

  bool isSupplyTemperatureDifferenceDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> supplyTemperatureDifferenceSchedule() const;

  double returnTemperatureDifference() const;

  bool isReturnTemperatureDifferenceDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> returnTemperatureDifferenceSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setAirFlowCalculationMethod(const std::string& airFlowCalculationMethod);

  bool setDesignPowerInputCalculationMethod(const std::string& designPowerInputCalculationMethod, double floorArea);

  bool setWattsperUnit(double wattsperUnit);

  bool setWattsperZoneFloorArea(double wattsperZoneFloorArea);

  // TODO: Check argument type. From object lists, some candidates are: BiVariateCurves, BiVariateTables.
  bool setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(const Curve& curve);

  bool setDesignFanPowerInputFraction(double designFanPowerInputFraction);

  void resetDesignFanPowerInputFraction();

  bool setDesignFanAirFlowRateperPowerInput(double designFanAirFlowRateperPowerInput);

  // TODO: Check argument type. From object lists, some candidates are: BiVariateCurves, BiVariateTables.
  bool setAirFlowFunctionofLoadingandAirTemperatureCurve(const Curve& curve);

  // TODO: Check argument type. From object lists, some candidates are: UniVariateCurves, UniVariateTables.
  bool setFanPowerInputFunctionofFlowCurve(const Curve& curve);

  void setDesignEnteringAirTemperature(double designEnteringAirTemperature);

  void resetDesignEnteringAirTemperature();

  bool setEnvironmentalClass(const std::string& environmentalClass);

  bool setAirInletConnectionType(const std::string& airInletConnectionType);

  //// TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setSupplyAirNode(const Connection& connection);

  //void resetSupplyAirNode();

  bool setDesignRecirculationFraction(double designRecirculationFraction);

  void resetDesignRecirculationFraction();

  // TODO: Check argument type. From object lists, some candidates are: BiVariateCurves, BiVariateTables.
  bool setRecirculationFunctionofLoadingandSupplyTemperatureCurve(const Curve& curve);

  void resetRecirculationFunctionofLoadingandSupplyTemperatureCurve();

  bool setDesignElectricPowerSupplyEfficiency(double designElectricPowerSupplyEfficiency);

  void resetDesignElectricPowerSupplyEfficiency();

  // TODO: Check argument type. From object lists, some candidates are: UniVariateCurves, UniVariateTables.
  bool setElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve);

  void resetElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();

  bool setFractionofElectricPowerSupplyLossestoZone(double fractionofElectricPowerSupplyLossestoZone);

  void resetFractionofElectricPowerSupplyLossestoZone();

  bool setSupplyTemperatureDifference(double supplyTemperatureDifference);

  void resetSupplyTemperatureDifference();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
  bool setSupplyTemperatureDifferenceSchedule(Schedule& schedule);

  void resetSupplyTemperatureDifferenceSchedule();

  bool setReturnTemperatureDifference(double returnTemperatureDifference);

  void resetReturnTemperatureDifference();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
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
  typedef detail::ElectricEquipmentITEAirCooledDefinition_Impl ImplType;

  explicit ElectricEquipmentITEAirCooledDefinition(std::shared_ptr<detail::ElectricEquipmentITEAirCooledDefinition_Impl> impl);

  friend class Model;
  friend class openstudio::IdfObject;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooledDefinition");
};

/** \relates ElectricEquipmentITEAirCooledDefinition*/
typedef boost::optional<ElectricEquipmentITEAirCooledDefinition> OptionalElectricEquipmentITEAirCooledDefinition;

/** \relates ElectricEquipmentITEAirCooledDefinition*/
typedef std::vector<ElectricEquipmentITEAirCooledDefinition> ElectricEquipmentITEAirCooledDefinitionVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_HPP

