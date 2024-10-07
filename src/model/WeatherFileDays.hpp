/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WEATHERFILEDAYS_HPP
#define MODEL_WEATHERFILEDAYS_HPP

#include "ModelAPI.hpp"
#include "SizingPeriod.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class WeatherFileDays_Impl;
  }  // namespace detail

  class MODEL_API WeatherFileDays : public SizingPeriod
  {
   public:
    // constructor
    explicit WeatherFileDays(const Model& model);

    virtual ~WeatherFileDays() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WeatherFileDays(const WeatherFileDays& other) = default;
    WeatherFileDays(WeatherFileDays&& other) = default;
    WeatherFileDays& operator=(const WeatherFileDays&) = default;
    WeatherFileDays& operator=(WeatherFileDays&&) = default;

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::WeatherFileDays_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit WeatherFileDays(std::shared_ptr<detail::WeatherFileDays_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.WeatherFileDays");
  };

  /** \relates WeatherFileDays */
  using OptionalWeatherFileDays = boost::optional<WeatherFileDays>;

  /** \relates WeatherFileDays */
  using WeatherFileDaysVector = std::vector<WeatherFileDays>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WEATHERFILEDAYS_HPP
