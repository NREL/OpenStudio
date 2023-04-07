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

#ifndef MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_HPP
#define MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Curve;
  class Schedule;

  namespace detail {

    class AirflowNetworkOccupantVentilationControl_Impl;

  }  // namespace detail

  /** AirflowNetworkOccupantVentilationControl is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkOccupantVentilationControl'. */
  class MODEL_API AirflowNetworkOccupantVentilationControl : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    // Remove this ctor unless a suitable default curve can be found
    // explicit AirflowNetworkOccupantVentilationControl(const Model& model);
    /** Construct an occupant ventilation control object with a low temperature curve. */
    AirflowNetworkOccupantVentilationControl(const Model& model, const Curve& curve);

    virtual ~AirflowNetworkOccupantVentilationControl() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& other) = default;
    AirflowNetworkOccupantVentilationControl(AirflowNetworkOccupantVentilationControl&& other) = default;
    AirflowNetworkOccupantVentilationControl& operator=(const AirflowNetworkOccupantVentilationControl&) = default;
    AirflowNetworkOccupantVentilationControl& operator=(AirflowNetworkOccupantVentilationControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the minimum opening time. */
    double minimumOpeningTime() const;
    /** Returns true if the minimum opening time is defaulted. */
    bool isMinimumOpeningTimeDefaulted() const;
    /** Returns the minimum closing time. */
    double minimumClosingTime() const;
    /** Returns true if the minimum closing time is defaulted. */
    bool isMinimumClosingTimeDefaulted() const;

    /** Returns the low temperature thermal comfort curve. */
    Curve thermalComfortLowTemperatureCurve() const;
    /** Returns the boundary temperature between the low and high temperature thermal comfort curves. */
    double thermalComfortTemperatureBoundaryPoint() const;
    /** Returns true if the boundary temperature is defaulted. */
    bool isThermalComfortTemperatureBoundaryPointDefaulted() const;

    /** Returns the high temperature thermal comfort curve. */
    boost::optional<Curve> thermalComfortHighTemperatureCurve() const;
    /** Returns the maximum PPD threshold. */
    double maximumPredictedPercentageofDissatisfiedThreshold() const;
    /** Returns true if the maximum PPD threshold is defaulted. */
    bool isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted() const;
    /** Returns true if an occupancy check is to be performed as part of the opening probability check. */
    bool occupancyCheck() const;
    /** Returns true if the occupancy check is defaulted. */
    bool isOccupancyCheckDefaulted() const;

    /** Returns the opening probability schedule, if any. */
    boost::optional<Schedule> openingProbabilitySchedule() const;

    /** Returns the closing probability schedule, if any. */
    boost::optional<Schedule> closingProbabilitySchedule() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the minimum opening time. */
    bool setMinimumOpeningTime(double minimumOpeningTime);
    /** Resets the minimum opening time. */
    void resetMinimumOpeningTime();
    /** Sets the minimum closing time. */
    bool setMinimumClosingTime(double minimumClosingTime);
    /** Resets the minimum closing time. */
    void resetMinimumClosingTime();

    /** Sets the low temperature thermal comfort curve. */
    bool setThermalComfortLowTemperatureCurve(const Curve& curve);

    //void resetThermalComfortLowTemperatureCurve();
    /** Sets the temperature boundary point. */
    bool setThermalComfortTemperatureBoundaryPoint(double thermalComfortTemperatureBoundaryPoint);
    /** Resets the temperature boundary point. */
    void resetThermalComfortTemperatureBoundaryPoint();

    /** Sets the high temperature thermal comfort curve. */
    bool setThermalComfortHighTemperatureCurve(const Curve& curve);
    /** Resets the high temperature thermal comfort curve. */
    void resetThermalComfortHighTemperatureCurve();
    /** Sets the maximum PPD threshold. */
    bool setMaximumPredictedPercentageofDissatisfiedThreshold(double maximumPredictedPercentageofDissatisfiedThreshold);
    /** Resets the maximum PPD threshold. */
    void resetMaximumPredictedPercentageofDissatisfiedThreshold();
    /** Sets the occupancy check. */
    void setOccupancyCheck(bool occupancyCheck);
    /** Resets the occupancy check. */
    void resetOccupancyCheck();

    /** Sets the opening probability schedule. */
    bool setOpeningProbabilitySchedule(Schedule& schedule);
    /** Resets the opening probability schedule. */
    void resetOpeningProbabilitySchedule();

    /** Sets the closing probability schedule. */
    bool setClosingProbabilitySchedule(Schedule& schedule);
    /** Resets the closing probability schedule. */
    void resetClosingProbabilitySchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkOccupantVentilationControl_Impl;

    explicit AirflowNetworkOccupantVentilationControl(std::shared_ptr<detail::AirflowNetworkOccupantVentilationControl_Impl> impl);

    friend class detail::AirflowNetworkOccupantVentilationControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkOccupantVentilationControl");
  };

  /** \relates AirflowNetworkOccupantVentilationControl*/
  using OptionalAirflowNetworkOccupantVentilationControl = boost::optional<AirflowNetworkOccupantVentilationControl>;

  /** \relates AirflowNetworkOccupantVentilationControl*/
  using AirflowNetworkOccupantVentilationControlVector = std::vector<AirflowNetworkOccupantVentilationControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_HPP
