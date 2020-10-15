/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILLIQUIDDESICCANTSIMPLE_HPP
#define MODEL_COILLIQUIDDESICCANTSIMPLE_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class AirflowNetworkEquivalentDuct;
class ControllerWaterCoil;

namespace detail {
  class CoilLiquidDesiccantSimple_Impl;
} // detail

/** CoilLiquidDesiccantSimple is a WaterToAirComponent that wraps the IDD object named
 *  "OS:Coil:LiquidDesiccant:Simple".
 *
 *  Polymorphic behavior:
 *  <ol>
 *  <li> bool addToNode(Node & node);</li>
 *    <ul>
 *      <li>inherited from HVACComponent</li>
 *      <li>Adds this coil to node. If the coil is not allowed at the given node
 *      returns false.  If controllerWaterCoil is associated with this coil, then
 *      the controller's sensor node will be set to the air outlet node.</li>
 *    </ul>
 *  </ol>
 */
class MODEL_API CoilLiquidDesiccantSimple : public WaterToAirComponent {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new CoilLiquidDesiccantSimple object and places it inside the
   *  model.  The coil is fully initialized with all companion objects. */
  CoilLiquidDesiccantSimple(const Model& model, Schedule& availabilitySchedule);

  CoilLiquidDesiccantSimple(const Model& model);

  virtual ~CoilLiquidDesiccantSimple() {}

  //@}

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> designWaterFlowRate();

  bool isDesignWaterFlowRateAutosized();

  boost::optional<double> designAirFlowRate();

  bool isDesignAirFlowRateAutosized();

  boost::optional<double> designInletWaterTemperature();

  bool isDesignInletWaterTemperatureAutosized();

  boost::optional<double> designInletAirTemperature();

  bool isDesignInletAirTemperatureAutosized();

  boost::optional<double> designOutletAirTemperature();

  bool isDesignOutletAirTemperatureAutosized();

  boost::optional<double> designInletAirHumidityRatio();

  bool isDesignInletAirHumidityRatioAutosized();

  boost::optional<double> designOutletAirHumidityRatio();

  bool isDesignOutletAirHumidityRatioAutosized();

  boost::optional<double> designInletSolutionConcentration();

  bool isDesignInletSolutionConcentrationAutosized();

  boost::optional<double> designFanPowerPerUnitMassFlowRate();

  bool isDesignFanPowerPerUnitMassFlowRateAutosized();

  boost::optional<double> outdoorAirFlowRates();

  bool isOutdoorAirFlowRatesAutosized();

  boost::optional<double> designPumpPower();

  bool isDesignPumpPowerAutosized();

  boost::optional<double> designEffectivenessAtNormalCondition();

  bool isDesignEffectivenessAtNormalConditionAutosized();

  std::string typeOfOperationMode();

  bool isTypeOfOperationModeDefaulted();

  std::string airSource();

  bool isAirSourceDefaulted();

  std::string material();

  bool isMaterialDefaulted();

  // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

  boost::optional<double> designLiquidDesiccantConcentrationDifference();

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void autosizeDesignWaterFlowRate();

  bool setDesignAirFlowRate(double designAirFlowRate);

  void autosizeDesignAirFlowRate();

  bool setDesignInletWaterTemperature(double designInletWaterTemperature);

  void autosizeDesignInletWaterTemperature();

  bool setDesignInletAirTemperature(double designInletAirTemperature);

  void autosizeDesignInletAirTemperature();

  bool setDesignOutletAirTemperature(double designOutletAirTemperature);

  void autosizeDesignOutletAirTemperature();

  bool setDesignInletAirHumidityRatio(double designInletAirHumidityRatio);

  void autosizeDesignInletAirHumidityRatio();

  bool setDesignOutletAirHumidityRatio(double designOutletAirHumidityRatio);

  void autosizeDesignOutletAirHumidityRatio();

  bool setDesignInletSolutionConcentration(double designInletSolutionConcentration);

  void autosizeDesignInletSolutionConcentration();

  bool setDesignFanPowerPerUnitMassFlowRate(double designFanPowerPerUnitMassFlowRate);

  void autosizeDesignFanPowerPerUnitMassFlowRate();

  bool setOutdoorAirFlowRates(double outdoorAirFlowRates);

  void autosizeOutdoorAirFlowRates();

  bool setDesignPumpPower(double designPumpPower);

  void autosizeDesignPumpPower();

  bool setDesignEffectivenessAtNormalCondition(double designEffectivenessAtNormalCondition);

  void autosizeDesignEffectivenessAtNormalCondition();

  bool setTypeOfOperationMode(std::string typeOfOperationMode);

  void resetTypeOfOperationMode();

  bool setAirSource(std::string airSource);

  void resetAirSource();

  bool setMaterial(std::string material);

  void resetMaterial();

  // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

  // void resetCondensateCollectionWaterStorageTank();

  bool setDesignLiquidDesiccantConcentrationDifference(double designLiquidDesiccantConcentrationDifference);

  void resetDesignLiquidDesiccantConcentrationDifference();

  //@}
  /** @name Other */
  //@{

  boost::optional<ControllerWaterCoil> controllerWaterCoil();

  AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

  boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

  boost::optional<double> autosizedDesignWaterFlowRate() const;

  boost::optional<double> autosizedDesignAirFlowRate() const;

  boost::optional<double> autosizedDesignInletWaterTemperature() const;

  boost::optional<double> autosizedDesignInletAirTemperature() const;

  boost::optional<double> autosizedDesignOutletAirTemperature() const;

  boost::optional<double> autosizedDesignInletAirHumidityRatio() const;

  boost::optional<double> autosizedDesignOutletAirHumidityRatio() const;

  boost::optional<double> autosizedDesignInletSolutionConcentration() const;

  boost::optional<double> autosizedDesignFanPowerPerUnitMassFlowRate() const;

  boost::optional<double> autosizedOutdoorAirFlowRates() const;

  boost::optional<double> autosizedDesignPumpPower() const;

  boost::optional<double> autosizedDesignEffectivenessAtNormalCondition() const;

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  /// @cond
  typedef detail::CoilLiquidDesiccantSimple_Impl ImplType;

  explicit CoilLiquidDesiccantSimple(std::shared_ptr<detail::CoilLiquidDesiccantSimple_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.CoilLiquidDesiccantSimple");

  CoilLiquidDesiccantSimple(const Handle& handle, const Model& model);

  /// @endcond

}; // detail

typedef boost::optional<CoilLiquidDesiccantSimple> OptionalCoilLiquidDesiccantSimple;

} // model
} // openstudio

#endif // MODEL_COILLIQUIDDESICCANTSIMPLE_HPP
