/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGER_HPP
#define MODEL_AVAILABILITYMANAGER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Loop;

  namespace detail {
    class AvailabilityManager_Impl;
  }

  class MODEL_API AvailabilityManager : public ModelObject
  {

   public:
    AvailabilityManager(IddObjectType type, const Model& model);

    virtual ~AvailabilityManager() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManager(const AvailabilityManager& other) = default;
    AvailabilityManager(AvailabilityManager&& other) = default;
    AvailabilityManager& operator=(const AvailabilityManager&) = default;
    AvailabilityManager& operator=(AvailabilityManager&&) = default;

    boost::optional<Loop> loop() const;

   protected:
    /// @cond
    using ImplType = detail::AvailabilityManager_Impl;

    explicit AvailabilityManager(std::shared_ptr<detail::AvailabilityManager_Impl> impl);

    friend class detail::AvailabilityManager_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManager");
  };

  using OptionalAvailabilityManager = boost::optional<AvailabilityManager>;

  using AvailabilityManagerVector = std::vector<AvailabilityManager>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGER_HPP
