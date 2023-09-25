/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_LOCATION_HPP
#define ISOMODEL_LOCATION_HPP

#include "WeatherData.hpp"

namespace openstudio {
namespace isomodel {

  class Location
  {
   public:
    double terrain() const {
      return _terrain;
    }
    void setTerrain(double value) {
      _terrain = value;
    }
    std::shared_ptr<WeatherData> weather() const {
      return _weather;
    }
    void setWeatherData(std::shared_ptr<WeatherData> value) {
      _weather = value;
    }

   private:
    double _terrain = 0;
    std::shared_ptr<WeatherData> _weather;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_LOCATION_HPP
