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

#ifndef MODEL_COILHEATINGWATER_HPP
#define MODEL_COILHEATINGWATER_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ControllerWaterCoil;

namespace detail {
  class CoilHeatingWater_Impl;
} // detail

/** CoilHeatingWater is a WaterToAirComponent that wraps the IDD object named
 *  "OS:Coil:Heating:Water".
 *
 *  Polymorphic behavior:
 *  <ol>
 *  <li> bool addToNode(Node& node);</li>
 *    <ul>
 *      <li>inherited from HVACComponent</li>
 *      <li>Adds this coil to node.  If the coil is not allowed at the given node
 *          returns false.  If controllerWaterCoil is associated with this coil, then
 *          the controller's sensor node will be set to the air outlet node.</li>
 *    </ul>
 *  </ol>
 */
class MODEL_API CoilHeatingWater : public WaterToAirComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  CoilHeatingWater(const Model& model, Schedule & availableSchedule);

  virtual ~CoilHeatingWater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters and Setters */
  //@{

  /** Returns the schedule referred to by the Availability Schedule Name field. **/
  Schedule availabilitySchedule() const;

  /** \deprecated */
  Schedule availableSchedule() const;

  /** Sets the schedule referred to by the Availability Schedule Name field. **/
  bool setAvailabilitySchedule(Schedule & schedule);

  /** \deprecated */
  bool setAvailableSchedule(Schedule & schedule);

  /** Returns the value of the UFactorTimesAreaValue field. **/
  boost::optional<double> uFactorTimesAreaValue();

  /** Sets the value of the UFactorTimesAreaValue field. **/
  void setUFactorTimesAreaValue( double value );

  /** Returns true if the UFactorTimesAreaValue field is set to autosize **/
  bool isUFactorTimesAreaValueAutosized();

  /** Sets the UFactorTimesAreaValue field to autosize **/
  void autosizeUFactorTimesAreaValue();

  /** Returns the value of the MaximumWaterFlowRate field. **/
  boost::optional<double> maximumWaterFlowRate();

  /** Sets the value of the MaximumWaterFlowRate field. **/
  void setMaximumWaterFlowRate( double value );

  /** Returns true if the MaximumWaterFlowRate field is set to autosize **/
  bool isMaximumWaterFlowRateAutosized();

  /** Sets the MaximumWaterFlowRate field to autosize **/
  void autosizeMaximumWaterFlowRate();

  /** Returns the value of the PerformanceInputMethod field. **/
  std::string performanceInputMethod();

  /** Sets the value of the PerformanceInputMethod field.
   *  Options are UFactorTimesAreaAndDesignWaterFlowRate and NominalCapacity.
   */
  void setPerformanceInputMethod( std::string value );

  /** Returns the value of the RatedCapacity field. **/
  boost::optional<double> ratedCapacity();

  /** Sets the value of the RatedCapacity field. **/
  void setRatedCapacity( double value );

  /** Returns true if the RatedCapacity field is set to autosize **/
  bool isRatedCapacityAutosized();

  /** Sets the RatedCapacity field to autosize **/
  void autosizeRatedCapacity();

  /** Returns the value of RatedInletWaterTemperature field. **/
  double ratedInletWaterTemperature();

  /** Sets the value of RatedInletWaterTemperature field. **/
  void setRatedInletWaterTemperature( double value );

  /** Returns the value of RatedInletWaterTemperature field. **/
  double ratedInletAirTemperature();

  /** Sets the value of RatedInletWaterTemperature field. **/
  void setRatedInletAirTemperature( double value );

  /** Returns the value of RatedOutletWaterTemperature field. **/
  double ratedOutletWaterTemperature();

  /** Sets the value of RatedOutletWaterTemperature field. **/
  void setRatedOutletWaterTemperature( double value );

  /** Returns the value of RatedOutletAirTemperature field. **/
  double ratedOutletAirTemperature();

  /** Sets the value of RatedOutletAirTemperature field. **/
  void setRatedOutletAirTemperature( double value );

  /** Returns the value of RatedRatioForAirAndWaterConvection field. **/
  double ratedRatioForAirAndWaterConvection();

  /** Sets the value of RatedRatioForAirAndWaterConvection field. **/
  void setRatedRatioForAirAndWaterConvection( double value );

  /** Returns the optional ControllerWaterCoil associated with this coil.
   */
  boost::optional<ControllerWaterCoil> controllerWaterCoil();

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @cond
  typedef detail::CoilHeatingWater_Impl ImplType;

  explicit CoilHeatingWater(std::shared_ptr<detail::CoilHeatingWater_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingWater");

  CoilHeatingWater(const Handle& handle, const Model& model);

  /// @endcond
}; // detail

typedef boost::optional<CoilHeatingWater> OptionalCoilHeatingWater;

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATER_HPP
