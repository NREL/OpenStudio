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

#ifndef MODEL_COILHEATINGWATER_HPP
#define MODEL_COILHEATINGWATER_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ControllerWaterCoil;
  class AirflowNetworkEquivalentDuct;

  namespace detail {
    class CoilHeatingWater_Impl;
  }  // namespace detail

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
  class MODEL_API CoilHeatingWater : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWater(const Model& model, Schedule& availableSchedule);

    CoilHeatingWater(const Model& model);

    virtual ~CoilHeatingWater() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWater(const CoilHeatingWater& other) = default;
    CoilHeatingWater(CoilHeatingWater&& other) = default;
    CoilHeatingWater& operator=(const CoilHeatingWater&) = default;
    CoilHeatingWater& operator=(CoilHeatingWater&&) = default;

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

    /** Returns the value of the UFactorTimesAreaValue field. **/
    boost::optional<double> uFactorTimesAreaValue();

    /** Sets the value of the UFactorTimesAreaValue field. **/
    bool setUFactorTimesAreaValue(double value);

    /** Returns true if the UFactorTimesAreaValue field is set to autosize **/
    bool isUFactorTimesAreaValueAutosized();

    /** Sets the UFactorTimesAreaValue field to autosize **/
    void autosizeUFactorTimesAreaValue();

    /** Returns the value of the MaximumWaterFlowRate field. **/
    boost::optional<double> maximumWaterFlowRate();

    /** Sets the value of the MaximumWaterFlowRate field. **/
    bool setMaximumWaterFlowRate(double value);

    /** Returns true if the MaximumWaterFlowRate field is set to autosize **/
    bool isMaximumWaterFlowRateAutosized();

    /** Sets the MaximumWaterFlowRate field to autosize **/
    void autosizeMaximumWaterFlowRate();

    /** Returns the value of the PerformanceInputMethod field. **/
    std::string performanceInputMethod();

    /** Sets the value of the PerformanceInputMethod field.
   *  Options are UFactorTimesAreaAndDesignWaterFlowRate and NominalCapacity.
   */
    bool setPerformanceInputMethod(const std::string& value);

    /** Returns the value of the RatedCapacity field. **/
    boost::optional<double> ratedCapacity();

    /** Sets the value of the RatedCapacity field. **/
    bool setRatedCapacity(double value);

    /** Returns true if the RatedCapacity field is set to autosize **/
    bool isRatedCapacityAutosized();

    /** Sets the RatedCapacity field to autosize **/
    void autosizeRatedCapacity();

    /** Returns the value of RatedInletWaterTemperature field. **/
    double ratedInletWaterTemperature();

    /** Sets the value of RatedInletWaterTemperature field. **/
    bool setRatedInletWaterTemperature(double value);

    /** Returns the value of RatedInletWaterTemperature field. **/
    double ratedInletAirTemperature();

    /** Sets the value of RatedInletWaterTemperature field. **/
    bool setRatedInletAirTemperature(double value);

    /** Returns the value of RatedOutletWaterTemperature field. **/
    double ratedOutletWaterTemperature();

    /** Sets the value of RatedOutletWaterTemperature field. **/
    bool setRatedOutletWaterTemperature(double value);

    /** Returns the value of RatedOutletAirTemperature field. **/
    double ratedOutletAirTemperature();

    /** Sets the value of RatedOutletAirTemperature field. **/
    bool setRatedOutletAirTemperature(double value);

    /** Returns the value of RatedRatioForAirAndWaterConvection field. **/
    double ratedRatioForAirAndWaterConvection();

    /** Sets the value of RatedRatioForAirAndWaterConvection field. **/
    bool setRatedRatioForAirAndWaterConvection(double value);

    /** Returns the optional ControllerWaterCoil associated with this coil.
   */
    boost::optional<ControllerWaterCoil> controllerWaterCoil();

    boost::optional<double> autosizedUFactorTimesAreaValue() const;

    boost::optional<double> autosizedMaximumWaterFlowRate() const;

    boost::optional<double> autosizedRatedCapacity() const;

    //@}
    /** @name Other */
    //@{

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @cond
    using ImplType = detail::CoilHeatingWater_Impl;

    explicit CoilHeatingWater(std::shared_ptr<detail::CoilHeatingWater_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWater");

    CoilHeatingWater(const Handle& handle, const Model& model);

    /// @endcond
  };  // detail

  using OptionalCoilHeatingWater = boost::optional<CoilHeatingWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATER_HPP
