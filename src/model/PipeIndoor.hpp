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

#ifndef MODEL_PIPEINDOOR_HPP
#define MODEL_PIPEINDOOR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Construction;
  class Schedule;
  class ThermalZone;

  namespace detail {

    class PipeIndoor_Impl;

  }  // namespace detail

  /** PipeIndoor is a  StraightComponent that wraps the OpenStudio IDD object 'OS:Pipe:Indoor'. */
  class MODEL_API PipeIndoor : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PipeIndoor(const Model& model);

    virtual ~PipeIndoor() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PipeIndoor(const PipeIndoor& other) = default;
    PipeIndoor(PipeIndoor&& other) = default;
    PipeIndoor& operator=(const PipeIndoor&) = default;
    PipeIndoor& operator=(PipeIndoor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> environmentTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Construction> construction() const;

    std::string environmentType() const;

    boost::optional<ThermalZone> ambientTemperatureZone() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<Schedule> ambientAirVelocitySchedule() const;

    double pipeInsideDiameter() const;

    double pipeLength() const;

    //@}
    /** @name Setters */
    //@{

    bool setConstruction(const Construction& construction);

    void resetConstruction();

    bool setEnvironmentType(const std::string& environmentType);

    bool setAmbientTemperatureZone(const ThermalZone& thermalZone);

    void resetAmbientTemperatureZone();

    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientAirVelocitySchedule(Schedule& schedule);

    void resetAmbientAirVelocitySchedule();

    bool setPipeInsideDiameter(double pipeInsideDiameter);

    bool setPipeLength(double pipeLength);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PipeIndoor_Impl;

    explicit PipeIndoor(std::shared_ptr<detail::PipeIndoor_Impl> impl);

    friend class detail::PipeIndoor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PipeIndoor");
  };

  /** \relates PipeIndoor*/
  using OptionalPipeIndoor = boost::optional<PipeIndoor>;

  /** \relates PipeIndoor*/
  using PipeIndoorVector = std::vector<PipeIndoor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PIPEINDOOR_HPP
