/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WEATHERFILECONDITIONTYPE_HPP
#define MODEL_WEATHERFILECONDITIONTYPE_HPP

#include "ModelAPI.hpp"
#include "SizingPeriod.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class WeatherFileConditionType_Impl;
  }  // namespace detail

  class MODEL_API WeatherFileConditionType : public SizingPeriod
  {
   public:
    // constructor
    explicit WeatherFileConditionType(const Model& model);

    virtual ~WeatherFileConditionType() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WeatherFileConditionType(const WeatherFileConditionType& other) = default;
    WeatherFileConditionType(WeatherFileConditionType&& other) = default;
    WeatherFileConditionType& operator=(const WeatherFileConditionType&) = default;
    WeatherFileConditionType& operator=(WeatherFileConditionType&&) = default;

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::WeatherFileConditionType_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit WeatherFileConditionType(std::shared_ptr<detail::WeatherFileConditionType_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.WeatherFileConditionType");
  };

  /** \relates WeatherFileConditionType */
  using OptionalWeatherFileConditionType = boost::optional<WeatherFileConditionType>;

  /** \relates WeatherFileConditionType */
  using WeatherFileConditionTypeVector = std::vector<WeatherFileConditionType>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WEATHERFILECONDITIONTYPE_HPP
