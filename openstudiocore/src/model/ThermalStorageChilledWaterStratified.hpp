/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_HPP
#define MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ThermalStorageChilledWaterStratified_Impl;

} // detail

/** ThermalStorageChilledWaterStratified is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:ThermalStorage:ChilledWater:Stratified'. */
class MODEL_API ThermalStorageChilledWaterStratified : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ThermalStorageChilledWaterStratified(const Model& model);

  virtual ~ThermalStorageChilledWaterStratified() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> tankShapeValues();

  static std::vector<std::string> ambientTemperatureIndicatorValues();

  static std::vector<std::string> inletModeValues();

  /** @name Getters */
  //@{

  double tankVolume() const;

  double tankHeight() const;

  std::string tankShape() const;

  boost::optional<double> tankPerimeter() const;

  boost::optional<Schedule> setpointTemperatureSchedule() const;

  double deadbandTemperatureDifference() const;

  boost::optional<double> temperatureSensorHeight() const;

  boost::optional<double> minimumTemperatureLimit() const;

  boost::optional<double> nominalCoolingCapacity() const;

  std::string ambientTemperatureIndicator() const;

  boost::optional<Schedule> ambientTemperatureSchedule() const;

  boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

  boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

  boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

  double useSideHeatTransferEffectiveness() const;

  boost::optional<Schedule> useSideAvailabilitySchedule() const;

  boost::optional<double> useSideInletHeight() const;

  bool isUseSideInletHeightAutocalculated() const;

  double useSideOutletHeight() const;

  boost::optional<double> useSideDesignFlowRate() const;

  bool isUseSideDesignFlowRateAutosized() const;

  double sourceSideHeatTransferEffectiveness() const;

  boost::optional<Schedule> sourceSideAvailabilitySchedule() const;

  double sourceSideInletHeight() const;

  boost::optional<double> sourceSideOutletHeight() const;

  bool isSourceSideOutletHeightAutocalculated() const;

  boost::optional<double> sourceSideDesignFlowRate() const;

  bool isSourceSideDesignFlowRateAutosized() const;

  double tankRecoveryTime() const;

  std::string inletMode() const;

  int numberofNodes() const;

  double additionalDestratificationConductivity() const;

  double node1AdditionalLossCoefficient() const;

  double node2AdditionalLossCoefficient() const;

  double node3AdditionalLossCoefficient() const;

  double node4AdditionalLossCoefficient() const;

  double node5AdditionalLossCoefficient() const;

  double node6AdditionalLossCoefficient() const;

  double node7AdditionalLossCoefficient() const;

  double node8AdditionalLossCoefficient() const;

  double node9AdditionalLossCoefficient() const;

  double node10AdditionalLossCoefficient() const;

  //@}
  /** @name Setters */
  //@{

  bool setTankVolume(double tankVolume);

  bool setTankHeight(double tankHeight);

  bool setTankShape(std::string tankShape);

  bool setTankPerimeter(double tankPerimeter);

  void resetTankPerimeter();

  bool setSetpointTemperatureSchedule(Schedule& schedule);

  void resetSetpointTemperatureSchedule();

  bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

  bool setTemperatureSensorHeight(double temperatureSensorHeight);

  void resetTemperatureSensorHeight();

  bool setMinimumTemperatureLimit(double minimumTemperatureLimit);

  void resetMinimumTemperatureLimit();

  bool setNominalCoolingCapacity(double nominalCoolingCapacity);

  void resetNominalCoolingCapacity();

  bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

  bool setAmbientTemperatureSchedule(Schedule& schedule);

  void resetAmbientTemperatureSchedule();

  bool setAmbientTemperatureThermalZone(const ThermalZone& thermalZone);

  void resetAmbientTemperatureThermalZone();

  bool setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName);

  void resetAmbientTemperatureOutdoorAirNodeName();

  bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(double uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

  void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

  bool setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness);

  bool setUseSideAvailabilitySchedule(Schedule& schedule);

  void resetUseSideAvailabilitySchedule();

  bool setUseSideInletHeight(double useSideInletHeight);

  void autocalculateUseSideInletHeight();

  bool setUseSideOutletHeight(double useSideOutletHeight);

  bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

  void autosizeUseSideDesignFlowRate();

  bool setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness);

  bool setSourceSideAvailabilitySchedule(Schedule& schedule);

  void resetSourceSideAvailabilitySchedule();

  bool setSourceSideInletHeight(double sourceSideInletHeight);

  bool setSourceSideOutletHeight(double sourceSideOutletHeight);

  void autocalculateSourceSideOutletHeight();

  bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

  void autosizeSourceSideDesignFlowRate();

  bool setTankRecoveryTime(double tankRecoveryTime);

  bool setInletMode(std::string inletMode);

  bool setNumberofNodes(int numberofNodes);

  bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

  bool setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

  bool setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

  bool setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

  bool setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

  bool setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

  bool setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

  bool setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

  bool setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

  bool setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

  bool setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedUseSideDesignFlowRate() const ;

  boost::optional<double> autosizedSourceSideDesignFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::ThermalStorageChilledWaterStratified_Impl ImplType;

  explicit ThermalStorageChilledWaterStratified(std::shared_ptr<detail::ThermalStorageChilledWaterStratified_Impl> impl);

  friend class detail::ThermalStorageChilledWaterStratified_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ThermalStorageChilledWaterStratified");
};

/** \relates ThermalStorageChilledWaterStratified*/
typedef boost::optional<ThermalStorageChilledWaterStratified> OptionalThermalStorageChilledWaterStratified;

/** \relates ThermalStorageChilledWaterStratified*/
typedef std::vector<ThermalStorageChilledWaterStratified> ThermalStorageChilledWaterStratifiedVector;

} // model
} // openstudio

#endif // MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_HPP
