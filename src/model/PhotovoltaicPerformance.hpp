/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class PhotovoltaicPerformance_Impl;
  }

  /** PhotovoltaicPerformance is the base class for photovoltaic performance objects.
 */
  class MODEL_API PhotovoltaicPerformance : public ModelObject
  {

   public:
    PhotovoltaicPerformance(IddObjectType type, const Model& model);

    virtual ~PhotovoltaicPerformance() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PhotovoltaicPerformance(const PhotovoltaicPerformance& other) = default;
    PhotovoltaicPerformance(PhotovoltaicPerformance&& other) = default;
    PhotovoltaicPerformance& operator=(const PhotovoltaicPerformance&) = default;
    PhotovoltaicPerformance& operator=(PhotovoltaicPerformance&&) = default;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::PhotovoltaicPerformance_Impl;

    explicit PhotovoltaicPerformance(std::shared_ptr<detail::PhotovoltaicPerformance_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformance");

    /// @endcond
  };

  using OptionalPhotovoltaicPerformance = boost::optional<PhotovoltaicPerformance>;

  using PhotovoltaicPerformanceVector = std::vector<PhotovoltaicPerformance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PHOTOVOLTAICPERFORMANCE_HPP
