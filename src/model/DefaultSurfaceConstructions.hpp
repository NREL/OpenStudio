/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP
#define MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;

  namespace detail {

    class DefaultSurfaceConstructions_Impl;

  }  // namespace detail

  /** DefaultSurfaceConstructions is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultSurfaceConstructions'. */
  class MODEL_API DefaultSurfaceConstructions : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DefaultSurfaceConstructions(const Model& model);

    virtual ~DefaultSurfaceConstructions() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DefaultSurfaceConstructions(const DefaultSurfaceConstructions& other) = default;
    DefaultSurfaceConstructions(DefaultSurfaceConstructions&& other) = default;
    DefaultSurfaceConstructions& operator=(const DefaultSurfaceConstructions&) = default;
    DefaultSurfaceConstructions& operator=(DefaultSurfaceConstructions&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<ConstructionBase> floorConstruction() const;

    boost::optional<ConstructionBase> wallConstruction() const;

    boost::optional<ConstructionBase> roofCeilingConstruction() const;

    //@}
    /** @name Setters */
    //@{

    bool setFloorConstruction(const ConstructionBase& construction);

    void resetFloorConstruction();

    bool setWallConstruction(const ConstructionBase& construction);

    void resetWallConstruction();

    bool setRoofCeilingConstruction(const ConstructionBase& construction);

    void resetRoofCeilingConstruction();

    //@}

    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultSurfaceConstructions& other);

   protected:
    /// @cond
    using ImplType = detail::DefaultSurfaceConstructions_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit DefaultSurfaceConstructions(std::shared_ptr<detail::DefaultSurfaceConstructions_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DefaultSurfaceConstructions");
  };

  /** \relates DefaultSurfaceConstructions*/
  using OptionalDefaultSurfaceConstructions = boost::optional<DefaultSurfaceConstructions>;

  /** \relates DefaultSurfaceConstructions*/
  using DefaultSurfaceConstructionsVector = std::vector<DefaultSurfaceConstructions>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP
