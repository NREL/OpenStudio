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

#ifndef MODEL_FANCONSTANTVOLUME_HPP
#define MODEL_FANCONSTANTVOLUME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class AirflowNetworkFan;

  namespace detail {
    class FanConstantVolume_Impl;
  }  // namespace detail

  /** FanConstantVolume is a StraightComponent that wraps the IDD object named
 *  "OS:Fan:ConstantVolume". */
  class MODEL_API FanConstantVolume : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new FanConstantVolume object and places it inside the
   *  model.  The object is fully initialized with all companion objects. */
    FanConstantVolume(const Model& model, Schedule& s);

    FanConstantVolume(const Model& model);

    virtual ~FanConstantVolume() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanConstantVolume(const FanConstantVolume& other) = default;
    FanConstantVolume(FanConstantVolume&& other) = default;
    FanConstantVolume& operator=(const FanConstantVolume&) = default;
    FanConstantVolume& operator=(FanConstantVolume&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    /** Returns the value of the Fan Total Efficiency field **/
    double fanTotalEfficiency() const;

    /** Returns the value of the Fan Total Efficiency field. Deprecated, forwards to fanTotalEfficiency **/
    double fanEfficiency() const;

    /** Returns the value of the PressureRise field. **/
    double pressureRise() const;

    /** Returns the value of the MotorEfficiency field. **/
    double motorEfficiency() const;

    /** Returns the value of the MotorInAirStreamFraction field. **/
    double motorInAirstreamFraction() const;

    /** Returns the value of the EndUseSubcategory field. **/
    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& s);

    /** Sets the value of the Fan Total Efficiency field. **/
    bool setFanTotalEfficiency(double value);

    /** Sets the value of the Fan Total Efficiency field. Deprecated, forwards to setFanTotalEfficiency. **/
    bool setFanEfficiency(double value);

    /** Sets the value of the PressureRise field. **/
    bool setPressureRise(double value);

    /** Sets the value of the MotorEfficiency field. **/
    bool setMotorEfficiency(double value);

    /** Sets the value of the MotorInAirStreamFraction field. **/
    bool setMotorInAirstreamFraction(double value);

    /** Sets the value of the EndUseSubcategory field. **/
    bool setEndUseSubcategory(const std::string& value);

    //@}

    boost::optional<double> maximumFlowRate() const;

    bool isMaximumFlowRateAutosized() const;

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    boost::optional<double> autosizedMaximumFlowRate() const;

   protected:
    friend class Model;
    friend class openstudio::IdfObject;

    /// @cond
    using ImplType = detail::FanConstantVolume_Impl;
    explicit FanConstantVolume(std::shared_ptr<detail::FanConstantVolume_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.FanConstantVolume");
    /// @endcond
  };

  /** \relates FanConstantVolume */
  using OptionalFanConstantVolume = boost::optional<FanConstantVolume>;

  /** \relates FanConstantVolume */
  using FanConstantVolumeVector = std::vector<FanConstantVolume>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANCONSTANTVOLUME_HPP
