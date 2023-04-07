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

#ifndef MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_HPP
#define MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SubSurface;
  class Schedule;

  namespace detail {

    class SurfacePropertyIncidentSolarMultiplier_Impl;

  }  // namespace detail

  /** SurfacePropertyIncidentSolarMultiplier is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:IncidentSolarMultiplier'. */
  class MODEL_API SurfacePropertyIncidentSolarMultiplier : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyIncidentSolarMultiplier(const SubSurface& subSurface);

    virtual ~SurfacePropertyIncidentSolarMultiplier() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyIncidentSolarMultiplier(const SurfacePropertyIncidentSolarMultiplier& other) = default;
    SurfacePropertyIncidentSolarMultiplier(SurfacePropertyIncidentSolarMultiplier&& other) = default;
    SurfacePropertyIncidentSolarMultiplier& operator=(const SurfacePropertyIncidentSolarMultiplier&) = default;
    SurfacePropertyIncidentSolarMultiplier& operator=(SurfacePropertyIncidentSolarMultiplier&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    SubSurface subSurface() const;

    double incidentSolarMultiplier() const;

    boost::optional<Schedule> incidentSolarMultiplierSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSubSurface(const SubSurface& subSurface);

    bool setIncidentSolarMultiplier(double incidentSolarMultiplier);

    bool setIncidentSolarMultiplierSchedule(Schedule& schedule);

    void resetIncidentSolarMultiplierSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyIncidentSolarMultiplier_Impl;

    explicit SurfacePropertyIncidentSolarMultiplier(std::shared_ptr<detail::SurfacePropertyIncidentSolarMultiplier_Impl> impl);

    friend class detail::SurfacePropertyIncidentSolarMultiplier_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyIncidentSolarMultiplier");
  };

  /** \relates SurfacePropertyIncidentSolarMultiplier*/
  using OptionalSurfacePropertyIncidentSolarMultiplier = boost::optional<SurfacePropertyIncidentSolarMultiplier>;

  /** \relates SurfacePropertyIncidentSolarMultiplier*/
  using SurfacePropertyIncidentSolarMultiplierVector = std::vector<SurfacePropertyIncidentSolarMultiplier>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_HPP
