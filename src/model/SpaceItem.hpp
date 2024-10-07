/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEITEM_HPP
#define MODEL_SPACEITEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Space;

  namespace detail {
    class SpaceItem_Impl;
  }

  /** SpaceItem is an abstract class derived from ModelObject.
 */
  class MODEL_API SpaceItem : public ModelObject
  {
   public:
    virtual ~SpaceItem() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceItem(const SpaceItem& other) = default;
    SpaceItem(SpaceItem&& other) = default;
    SpaceItem& operator=(const SpaceItem&) = default;
    SpaceItem& operator=(SpaceItem&&) = default;

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    void resetSpace();

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new SpaceItem object in the model.
    SpaceItem(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::SpaceItem_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SpaceItem_Impl;

    explicit SpaceItem(std::shared_ptr<detail::SpaceItem_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SpaceItem");

    /// @endcond
  };

  /** \relates SpaceItem */
  using OptionalSpaceItem = boost::optional<SpaceItem>;

  /** \relates SpaceItem */
  using SpaceItemVector = std::vector<SpaceItem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEITEM_HPP
