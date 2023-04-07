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

#ifndef MODEL_SHADINGSURFACEGROUP_HPP
#define MODEL_SHADINGSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup.hpp"

namespace openstudio {

class Transformation;

namespace model {

  class Space;
  class ShadingSurface;
  class Surface;
  class SubSurface;

  namespace detail {

    class ShadingSurfaceGroup_Impl;

  }  // namespace detail

  /** ShadingSurfaceGroup is a PlanarSurfaceGroup that wraps the OpenStudio IDD object 'OS_ShadingSurfaceGroup'. */
  class MODEL_API ShadingSurfaceGroup : public PlanarSurfaceGroup
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ShadingSurfaceGroup(const Model& model);

    virtual ~ShadingSurfaceGroup() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ShadingSurfaceGroup(const ShadingSurfaceGroup& other) = default;
    ShadingSurfaceGroup(ShadingSurfaceGroup&& other) = default;
    ShadingSurfaceGroup& operator=(const ShadingSurfaceGroup&) = default;
    ShadingSurfaceGroup& operator=(ShadingSurfaceGroup&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validShadingSurfaceTypeValues();

    //@}
    /** @name Getters */
    //@{

    std::string shadingSurfaceType() const;

    //@}
    /** @name Setters */
    //@{

    /// this will fail if "Space" and space is not set, use setSpace instead
    bool setShadingSurfaceType(const std::string& shadingSurfaceType);

    //@}

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    /** If (space()), calls setShadingSurfaceType("Building") to remove the relationship. */
    void resetSpace();

    /// Returns child shading surfaces.
    std::vector<ShadingSurface> shadingSurfaces() const;

    /** Returns the specific sub surface shaded by this group if specified. */
    boost::optional<SubSurface> shadedSubSurface() const;

    /** Returns the surface shaded by this group if specified. */
    boost::optional<Surface> shadedSurface() const;

    /** Sets the specifc sub surface shaded by this group, sub surface must be in the same parent space. */
    bool setShadedSubSurface(const SubSurface& subSurface);

    /** Sets the surface shaded by this group, surface must be in the same parent space. */
    bool setShadedSurface(const Surface& surface);

    /** Resets the sub surface shaded by this group. */
    void resetShadedObject();

   protected:
    /// @cond
    using ImplType = detail::ShadingSurfaceGroup_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ShadingSurfaceGroup(std::shared_ptr<detail::ShadingSurfaceGroup_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ShadingSurfaceGroup");
  };

  /** \relates ShadingSurfaceGroup*/
  using OptionalShadingSurfaceGroup = boost::optional<ShadingSurfaceGroup>;

  /** \relates ShadingSurfaceGroup*/
  using ShadingSurfaceGroupVector = std::vector<ShadingSurfaceGroup>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGSURFACEGROUP_HPP
