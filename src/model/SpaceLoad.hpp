/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACELOAD_HPP
#define MODEL_SPACELOAD_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {
namespace model {

  class SpaceType;

  namespace detail {
    class SpaceLoad_Impl;
  }

  /** SpaceLoad is an abstract class derived from SpaceItem.
 */
  class MODEL_API SpaceLoad : public SpaceItem
  {
   public:
    virtual ~SpaceLoad() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceLoad(const SpaceLoad& other) = default;
    SpaceLoad(SpaceLoad&& other) = default;
    SpaceLoad& operator=(const SpaceLoad&) = default;
    SpaceLoad& operator=(SpaceLoad&&) = default;

    /// Returns the parent SpaceType.
    boost::optional<SpaceType> spaceType() const;

    /// Sets the parent SpaceType.
    bool setSpaceType(const SpaceType& spaceType);

    void resetSpaceType();

    /// Converts space load levels to absolute values rather than by per area, etc.
    /// Also hard sizes any costs associated with this load.
    /// Only possible if this space load is parented by a space, returns false if
    /// parented by a space type.
    bool hardSize();

    /// Directly applies default schedules to this space load if found.
    bool hardApplySchedules();

    /// Returns true if the load represents an absolute quantity that must be added when combining space types
    bool isAbsolute() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new SpaceLoad object in the model.
    SpaceLoad(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::SpaceLoad_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SpaceLoad_Impl;

    explicit SpaceLoad(std::shared_ptr<detail::SpaceLoad_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SpaceLoad");

    /// @endcond
  };

  /** \relates SpaceLoad */
  using OptionalSpaceLoad = boost::optional<SpaceLoad>;

  /** \relates SpaceLoad */
  using SpaceLoadVector = std::vector<SpaceLoad>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOAD_HPP
