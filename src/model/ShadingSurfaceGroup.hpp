/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~ShadingSurfaceGroup() override = default;
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
