/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_COILCOOLINGWATER_HPP
#define MODEL_COILCOOLINGWATER_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

class ControllerWaterCoil;

namespace detail {
  class CoilCoolingWater_Impl;
} // detail

/** CoilCoolingWater is a WaterToAirComponent that wraps the IDD object named
 *  "OS:Coil:Cooling:Water".
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
class MODEL_API CoilCoolingWater : public WaterToAirComponent {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new CoilCoolingWater object and places it inside the
   *  model.  The coil is fully initialized with all companion objects. */
  CoilCoolingWater(const Model& model, Schedule & availableSchedule);

  virtual ~CoilCoolingWater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters and Setters */
  //@{

  /** Returns the schedule referred to by the Availability Schedule Name field. **/
  Schedule availabilitySchedule() const;

  /** \deprecated */
  Schedule availableSchedule() const;

  /** Sets the schedule referred to by the Availability Schedule Name field. **/
  bool setAvailabilitySchedule(Schedule& schedule);

  /** \deprecated */
  bool setAvailableSchedule(Schedule& schedule);

  /** Returns the value of the DesignWaterFlowRate field. **/
  boost::optional<double> designWaterFlowRate();

  /** Sets the value of the DesignWaterFlowRate field. **/
  void setDesignWaterFlowRate( double value );

  /** Returns true if the DesignWaterFlowRate field is set to autosize **/
  bool isDesignWaterFlowRateAutosized();

  /** Set the DesignWaterFlowRate field to autosize **/
  void autosizeDesignWaterFlowRate();

  /** Returns the value of the DesignAirFlowRate field. **/
  boost::optional<double> designAirFlowRate();

  /** Sets the value of the DesignAirFlowRate field. **/
  void setDesignAirFlowRate( double value );

  /** Returns true if the DesignAirFlowRate field is set to autosize **/
  bool isDesignAirFlowRateAutosized();

  /** Set the DesignAirFlowRate field to autosize **/
  void autosizeDesignAirFlowRate();

  /** Returns the value of the DesignInletWaterTemperature field. **/
  boost::optional<double> designInletWaterTemperature();

  /** Sets the value of the DesignInletWaterTemperature field. **/
  void setDesignInletWaterTemperature( double value );

  /** Returns true if the DesignInletWaterTemperature field is set to autosize **/
  bool isDesignInletWaterTemperatureAutosized();

  /** Set the DesignInletWaterTemperature field to autosize **/
  void autosizeDesignInletWaterTemperature();

  /** Returns the value of the DesignInletAirTemperature field. **/
  boost::optional<double> designInletAirTemperature();

  /** Sets the value of the DesignInletAirTemperature field. **/
  void setDesignInletAirTemperature( double value );

  /** Returns true if the DesignInletAirTemperature field is set to autosize **/
  bool isDesignInletAirTemperatureAutosized();

  /** Set the DesignInletAirTemperature field to autosize **/
  void autosizeDesignInletAirTemperature();

  /** Returns the value of the DesignOutletAirTemperature field. **/
  boost::optional<double> designOutletAirTemperature();

  /** Sets the value of the DesignOutletAirTemperature field. **/
  void setDesignOutletAirTemperature( double value );

  /** Returns true if the DesignOutletAirTemperature field is set to autosize **/
  bool isDesignOutletAirTemperatureAutosized();

  /** Set the DesignOutletAirTemperature field to autosize **/
  void autosizeDesignOutletAirTemperature();

  /** Returns the value of the DesignInletAirHumidityRatio field. **/
  boost::optional<double> designInletAirHumidityRatio();

  /** Sets the value of the DesignInletAirHumidityRatio field. **/
  void setDesignInletAirHumidityRatio( double value );

  /** Returns true if the DesignInletAirHumidityRatio field is set to autosize **/
  bool isDesignInletAirHumidityRatioAutosized();

  /** Set the DesignInletAirHumidityRatio field to autosize **/
  void autosizeDesignInletAirHumidityRatio();

  /** Returns the value of the DesignOutletAirHumidityRatio field. **/
  boost::optional<double> designOutletAirHumidityRatio();

  /** Sets the value of the DesignOutletAirHumidityRatio field. **/
  void setDesignOutletAirHumidityRatio( double value );

  /** Returns true if the DesignOutletAirHumidityRatio field is set to autosize **/
  bool isDesignOutletAirHumidityRatioAutosized();

  /** Set the DesignOutletAirHumidityRatio field to autosize **/
  void autosizeDesignOutletAirHumidityRatio();

  /** Returns the value of the TypeOfAnalysis field. **/
  std::string typeOfAnalysis();

  /** Sets the value of the TypeOfAnalysis field.
   *  Options are SimpleAnalysis and DetailedAnalysis
   */
  void setTypeOfAnalysis( std::string value );

  /** Returns the value of the HeatExchangerConfiguration field. **/
  std::string heatExchangerConfiguration();

  /** Sets the value of the HeatExchangerConfiguration field.
   *  Options are CrossFlow and CounterFlow.
   */
  void setHeatExchangerConfiguration( std::string value );

  /** Returns the optional ControllerWaterCoil associated with this coil.
   */
  boost::optional<ControllerWaterCoil> controllerWaterCoil();

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  /// @cond
  typedef detail::CoilCoolingWater_Impl ImplType;

  explicit CoilCoolingWater(std::shared_ptr<detail::CoilCoolingWater_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.CoilCoolingWater");

  CoilCoolingWater(const Handle& handle, const Model& model);

  /// @endcond

}; // detail

typedef boost::optional<CoilCoolingWater> OptionalCoilCoolingWater;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATER_HPP
