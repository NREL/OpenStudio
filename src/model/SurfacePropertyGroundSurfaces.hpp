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

#ifndef MODEL_SURFACEPROPERTYGROUNDSURFACES_HPP
#define MODEL_SURFACEPROPERTYGROUNDSURFACES_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  class SurfacePropertyLocalEnvironment;

  namespace detail {

    class SurfacePropertyGroundSurfaces_Impl;

  }  // namespace detail

  /** This class implements an extensible group */
  class MODEL_API GroundSurfaceGroup
  {
   public:
    GroundSurfaceGroup(std::string groundSurfaceName, double viewFactor, boost::optional<Schedule> temperatureSchedule = boost::none,
                       boost::optional<Schedule> reflectanceSchedule = boost::none);

    std::string groundSurfaceName() const;
    double viewFactor() const;
    boost::optional<Schedule> temperatureSchedule() const;

    boost::optional<Schedule> reflectanceSchedule() const;

   private:
    // From
    std::string m_groundSurfaceName;
    double m_viewFactor;
    boost::optional<Schedule> m_temperatureSch;
    boost::optional<Schedule> m_reflectanceSch;
    REGISTER_LOGGER("openstudio.model.GroundSurfaceGroup");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::GroundSurfaceGroup& groundSurfaceGroup);

  /** SurfacePropertyGroundSurfaces is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:GroundSurfaces'. */
  class MODEL_API SurfacePropertyGroundSurfaces : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyGroundSurfaces(const Model& model);

    virtual ~SurfacePropertyGroundSurfaces() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyGroundSurfaces(const SurfacePropertyGroundSurfaces& other) = default;
    SurfacePropertyGroundSurfaces(SurfacePropertyGroundSurfaces&& other) = default;
    SurfacePropertyGroundSurfaces& operator=(const SurfacePropertyGroundSurfaces&) = default;
    SurfacePropertyGroundSurfaces& operator=(SurfacePropertyGroundSurfaces&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    // Helper to get the parent SurfaceProperty:LocalEnvironment object
    boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

    // Handle this object's extensible fields.
    std::vector<GroundSurfaceGroup> groundSurfaceGroups() const;

    unsigned int numberofGroundSurfaceGroups() const;

    /** Looks up by Surface Name (case-insensitive)  */
    boost::optional<unsigned> groundSurfaceGroupIndex(const GroundSurfaceGroup& groundSurfaceGroup) const;
    boost::optional<unsigned> groundSurfaceGroupIndex(const std::string& groundSurfaceName) const;

    boost::optional<GroundSurfaceGroup> getGroundSurfaceGroup(unsigned groupIndex) const;

    /** If a groundSurfaceGroup group is already present (cf `groundSurfaceGroupIndex()`), it will Warn and override the groundSurfaceGroup value */
    bool addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup);

    // Overloads, it creates a GroundSurfaceGroup wrapper, then call `addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup)`
    bool addGroundSurfaceGroup(const std::string& groundSurfaceName, double viewFactor, boost::optional<Schedule> temperatureSchedule = boost::none,
                               boost::optional<Schedule> reflectanceSchedule = boost::none);

    bool addGroundSurfaceGroups(const std::vector<GroundSurfaceGroup>& groundSurfaceGroups);

    bool removeGroundSurfaceGroup(unsigned groupIndex);

    void removeAllGroundSurfaceGroups();

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyGroundSurfaces_Impl;

    explicit SurfacePropertyGroundSurfaces(std::shared_ptr<detail::SurfacePropertyGroundSurfaces_Impl> impl);

    friend class detail::SurfacePropertyGroundSurfaces_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyGroundSurfaces");
  };

  /** \relates SurfacePropertyGroundSurfaces*/
  using OptionalSurfacePropertyGroundSurfaces = boost::optional<SurfacePropertyGroundSurfaces>;

  /** \relates SurfacePropertyGroundSurfaces*/
  using SurfacePropertyGroundSurfacesVector = std::vector<SurfacePropertyGroundSurfaces>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYGROUNDSURFACES_HPP
