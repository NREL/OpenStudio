/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INTERIORPARTITIONSURFACEGROUP_HPP
#define MODEL_INTERIORPARTITIONSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup.hpp"

namespace openstudio {

class Transformation;

namespace model {

  class Space;
  class InteriorPartitionSurface;

  namespace detail {

    class InteriorPartitionSurfaceGroup_Impl;

  }  // namespace detail

  /** InteriorPartitionSurfaceGroup is a PlanarSurfaceGroup that wraps the OpenStudio IDD object 'OS_InteriorPartitionSurfaceGroup'. */
  class MODEL_API InteriorPartitionSurfaceGroup : public PlanarSurfaceGroup
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit InteriorPartitionSurfaceGroup(const Model& model);

    virtual ~InteriorPartitionSurfaceGroup() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& other) = default;
    InteriorPartitionSurfaceGroup(InteriorPartitionSurfaceGroup&& other) = default;
    InteriorPartitionSurfaceGroup& operator=(const InteriorPartitionSurfaceGroup&) = default;
    InteriorPartitionSurfaceGroup& operator=(InteriorPartitionSurfaceGroup&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    int multiplier() const;

    bool isMultiplierDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMultiplier(int multiplier);

    void resetMultiplier();

    //@}

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    void resetSpace();

    /// Returns all \link InteriorPartitionSurface InteriorPartitionSurfaces \endlink in this group.
    std::vector<InteriorPartitionSurface> interiorPartitionSurfaces() const;

   protected:
    /// @cond
    using ImplType = detail::InteriorPartitionSurfaceGroup_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit InteriorPartitionSurfaceGroup(std::shared_ptr<detail::InteriorPartitionSurfaceGroup_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.InteriorPartitionSurfaceGroup");
  };

  /** \relates InteriorPartitionSurfaceGroup*/
  using OptionalInteriorPartitionSurfaceGroup = boost::optional<InteriorPartitionSurfaceGroup>;

  /** \relates InteriorPartitionSurfaceGroup*/
  using InteriorPartitionSurfaceGroupVector = std::vector<InteriorPartitionSurfaceGroup>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACEGROUP_HPP
