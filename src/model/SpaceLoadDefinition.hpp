/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACELOADDEFINITION_HPP
#define MODEL_SPACELOADDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class SpaceLoadInstance;

  namespace detail {
    class SpaceLoadDefinition_Impl;
  }

  /** SpaceLoadDefinition is an abstract class derived from ResourceObject. \link SpaceLoadDefinition
 *  SpaceLoadDefinitions \endlink describe internal loads in a generic way that can be used by
 *  multiple \link Space Spaces \endlink or \link SpaceType SpaceTypes \endlink. For instance, a
 *  LightsDefinition might define the appropriate amount of task lighting for a workstation in
 *  W/person, or an ElectricEquipmentDefinition might define a coffee maker in terms of its maximum
 *  power draw (W). Multipliers and schedules are defined by the corresponding SpaceLoadInstance
 *  objects. (Different usage patterns of the same amount of task lighting, or different numbers of
 *  coffee makers, can be defined on a space-by-space basis, for instance.)
 */
  class MODEL_API SpaceLoadDefinition : public ResourceObject
  {
   public:
    virtual ~SpaceLoadDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceLoadDefinition(const SpaceLoadDefinition& other) = default;
    SpaceLoadDefinition(SpaceLoadDefinition&& other) = default;
    SpaceLoadDefinition& operator=(const SpaceLoadDefinition&) = default;
    SpaceLoadDefinition& operator=(SpaceLoadDefinition&&) = default;

    /** Returns all instances of this definition. */
    std::vector<SpaceLoadInstance> instances() const;

    // ETH@20140113 - No units mark-up here. What is the pattern we are trying to move toward?
    // (Programmatically available, but string-based, right?)

    /** Returns the floor area associated with instances of this space load definition.
  If two instances are in the same space then that floor area will be double counted.
  If two instances used in the same space type then that floor area will be double counted.
  Space and SpaceLoadInstance multipliers are included in the result. **/
    double floorArea() const;

    /** Returns the number of instances of this space load.
  Space and SpaceLoadInstance multipliers are included in the result **/
    int quantity() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new SpaceLoadDefinition object in the model.
    SpaceLoadDefinition(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::SpaceLoadDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SpaceLoadDefinition_Impl;

    explicit SpaceLoadDefinition(std::shared_ptr<detail::SpaceLoadDefinition_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SpaceLoadDefinition");

    /// @endcond
  };

  /** \relates SpaceLoadDefinition */
  using OptionalSpaceLoadDefinition = boost::optional<SpaceLoadDefinition>;

  /** \relates SpaceLoadDefinition */
  using SpaceLoadDefinitionVector = std::vector<SpaceLoadDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOADDEFINITION_HPP
