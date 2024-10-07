/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DEFAULTCONSTRUCTIONSET_HPP
#define MODEL_DEFAULTCONSTRUCTIONSET_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;
  class ConstructionBase;
  class DefaultSubSurfaceConstructions;
  class DefaultSurfaceConstructions;

  namespace detail {

    class DefaultConstructionSet_Impl;

  }  // namespace detail

  /** DefaultConstructionSet is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultConstructionSet'. */
  class MODEL_API DefaultConstructionSet : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DefaultConstructionSet(const Model& model);

    virtual ~DefaultConstructionSet() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DefaultConstructionSet(const DefaultConstructionSet& other) = default;
    DefaultConstructionSet(DefaultConstructionSet&& other) = default;
    DefaultConstructionSet& operator=(const DefaultConstructionSet&) = default;
    DefaultConstructionSet& operator=(DefaultConstructionSet&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<DefaultSurfaceConstructions> defaultExteriorSurfaceConstructions() const;

    boost::optional<DefaultSurfaceConstructions> defaultInteriorSurfaceConstructions() const;

    boost::optional<DefaultSurfaceConstructions> defaultGroundContactSurfaceConstructions() const;

    boost::optional<DefaultSubSurfaceConstructions> defaultExteriorSubSurfaceConstructions() const;

    boost::optional<DefaultSubSurfaceConstructions> defaultInteriorSubSurfaceConstructions() const;

    boost::optional<ConstructionBase> interiorPartitionConstruction() const;

    boost::optional<ConstructionBase> spaceShadingConstruction() const;

    boost::optional<ConstructionBase> buildingShadingConstruction() const;

    boost::optional<ConstructionBase> siteShadingConstruction() const;

    boost::optional<ConstructionBase> adiabaticSurfaceConstruction() const;

    //@}
    /** @name Setters */
    //@{

    bool setDefaultExteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

    void resetDefaultExteriorSurfaceConstructions();

    bool setDefaultInteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

    void resetDefaultInteriorSurfaceConstructions();

    bool setDefaultGroundContactSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

    void resetDefaultGroundContactSurfaceConstructions();

    bool setDefaultExteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

    void resetDefaultExteriorSubSurfaceConstructions();

    bool setDefaultInteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

    void resetDefaultInteriorSubSurfaceConstructions();

    bool setInteriorPartitionConstruction(const ConstructionBase& construction);

    void resetInteriorPartitionConstruction();

    bool setSpaceShadingConstruction(const ConstructionBase& construction);

    void resetSpaceShadingConstruction();

    bool setBuildingShadingConstruction(const ConstructionBase& construction);

    void resetBuildingShadingConstruction();

    bool setSiteShadingConstruction(const ConstructionBase& construction);

    void resetSiteShadingConstruction();

    bool setAdiabaticSurfaceConstruction(const ConstructionBase& construction);

    void resetAdiabaticSurfaceConstruction();

    //@}

    /// Returns the default construction for this planar surface if available.
    boost::optional<ConstructionBase> getDefaultConstruction(const PlanarSurface& planarSurface) const;

    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultConstructionSet& other);

   protected:
    /// @cond
    using ImplType = detail::DefaultConstructionSet_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit DefaultConstructionSet(std::shared_ptr<detail::DefaultConstructionSet_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DefaultConstructionSet");
  };

  /** \relates DefaultConstructionSet*/
  using OptionalDefaultConstructionSet = boost::optional<DefaultConstructionSet>;

  /** \relates DefaultConstructionSet*/
  using DefaultConstructionSetVector = std::vector<DefaultConstructionSet>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTCONSTRUCTIONSET_HPP
