/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP
#define MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;

  namespace detail {

    class DefaultSubSurfaceConstructions_Impl;

  }  // namespace detail

  /** DefaultSubSurfaceConstructions is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultSubSurfaceConstructions'. */
  class MODEL_API DefaultSubSurfaceConstructions : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DefaultSubSurfaceConstructions(const Model& model);

    virtual ~DefaultSubSurfaceConstructions() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DefaultSubSurfaceConstructions(const DefaultSubSurfaceConstructions& other) = default;
    DefaultSubSurfaceConstructions(DefaultSubSurfaceConstructions&& other) = default;
    DefaultSubSurfaceConstructions& operator=(const DefaultSubSurfaceConstructions&) = default;
    DefaultSubSurfaceConstructions& operator=(DefaultSubSurfaceConstructions&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<ConstructionBase> fixedWindowConstruction() const;

    boost::optional<ConstructionBase> operableWindowConstruction() const;

    boost::optional<ConstructionBase> doorConstruction() const;

    boost::optional<ConstructionBase> glassDoorConstruction() const;

    boost::optional<ConstructionBase> overheadDoorConstruction() const;

    boost::optional<ConstructionBase> skylightConstruction() const;

    boost::optional<ConstructionBase> tubularDaylightDomeConstruction() const;

    boost::optional<ConstructionBase> tubularDaylightDiffuserConstruction() const;

    //@}
    /** @name Setters */
    //@{

    bool setFixedWindowConstruction(const ConstructionBase& construction);

    void resetFixedWindowConstruction();

    bool setOperableWindowConstruction(const ConstructionBase& construction);

    void resetOperableWindowConstruction();

    bool setDoorConstruction(const ConstructionBase& construction);

    void resetDoorConstruction();

    bool setGlassDoorConstruction(const ConstructionBase& construction);

    void resetGlassDoorConstruction();

    bool setOverheadDoorConstruction(const ConstructionBase& construction);

    void resetOverheadDoorConstruction();

    bool setSkylightConstruction(const ConstructionBase& construction);

    void resetSkylightConstruction();

    bool setTubularDaylightDomeConstruction(const ConstructionBase& construction);

    void resetTubularDaylightDomeConstruction();

    bool setTubularDaylightDiffuserConstruction(const ConstructionBase& construction);

    void resetTubularDaylightDiffuserConstruction();

    //@}

    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultSubSurfaceConstructions& other);

   protected:
    /// @cond
    using ImplType = detail::DefaultSubSurfaceConstructions_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit DefaultSubSurfaceConstructions(std::shared_ptr<detail::DefaultSubSurfaceConstructions_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DefaultSubSurfaceConstructions");
  };

  /** \relates DefaultSubSurfaceConstructions*/
  using OptionalDefaultSubSurfaceConstructions = boost::optional<DefaultSubSurfaceConstructions>;

  /** \relates DefaultSubSurfaceConstructions*/
  using DefaultSubSurfaceConstructionsVector = std::vector<DefaultSubSurfaceConstructions>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP
