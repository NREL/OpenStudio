/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACELOADINSTANCE_HPP
#define MODEL_SPACELOADINSTANCE_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

  class SpaceLoadDefinition;

  namespace detail {
    class SpaceLoadInstance_Impl;
  }

  /** SpaceLoadInstance is an abstract class derived from SpaceLoad. \link SpaceLoadInstance
 *  SpaceLoadInstances \endlink instantiate a given SpaceLoadDefinition in a Space or SpaceType
 *  by defining a multiplier and a schedule.
 */
  class MODEL_API SpaceLoadInstance : public SpaceLoad
  {
   public:
    virtual ~SpaceLoadInstance() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceLoadInstance(const SpaceLoadInstance& other) = default;
    SpaceLoadInstance(SpaceLoadInstance&& other) = default;
    SpaceLoadInstance& operator=(const SpaceLoadInstance&) = default;
    SpaceLoadInstance& operator=(SpaceLoadInstance&&) = default;

    /** Returns the definition of this instance. **/
    SpaceLoadDefinition definition() const;

    /** Sets the definition of this instance. **/
    bool setDefinition(const SpaceLoadDefinition& definition);

    /** Ensures that no other instances share this instances definition. **/
    void makeUnique();

    /** Gets the multiplier for this space instance, defaults to 1. **/
    double multiplier() const;

    /** Returns true if the multiplier is defaulted. **/
    bool isMultiplierDefaulted() const;

    /** Returns the floor area associated with this space load instance.
  If this space load is associated with a single space then that space's floor area is returned.
  If this space load is associated with a space type then the sum of all space floor area in that space type is returned.
  Space and SpaceLoadInstance multipliers are included in the result **/
    double floorArea() const;

    /** Returns the number of instances this space load instance represents.
  Space and SpaceLoadInstance multipliers are included in the result **/
    int quantity() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new SpaceLoadInstance object in the model.
    SpaceLoadInstance(IddObjectType type, const SpaceLoadDefinition& definition);

    //@}

    /// @cond

    using ImplType = detail::SpaceLoadInstance_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SpaceLoadInstance_Impl;

    explicit SpaceLoadInstance(std::shared_ptr<detail::SpaceLoadInstance_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SpaceLoadInstance");

    /// @endcond
  };

  /** \relates SpaceLoadInstance */
  using OptionalSpaceLoadInstance = boost::optional<SpaceLoadInstance>;

  /** \relates SpaceLoadInstance */
  using SpaceLoadInstanceVector = std::vector<SpaceLoadInstance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOADINSTANCE_HPP
