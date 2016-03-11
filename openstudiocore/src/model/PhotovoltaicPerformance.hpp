/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class PhotovoltaicPerformance_Impl;
}

/** PhotovoltaicPerformance is the base class for photovoltaic performance objects.
 */
class MODEL_API PhotovoltaicPerformance : public ModelObject {

  public:

  PhotovoltaicPerformance(IddObjectType type, const Model& model);

  virtual ~PhotovoltaicPerformance() {}

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::PhotovoltaicPerformance_Impl ImplType;

  explicit PhotovoltaicPerformance(std::shared_ptr<detail::PhotovoltaicPerformance_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformance");

  /// @endcond 

};

typedef boost::optional<PhotovoltaicPerformance> OptionalPhotovoltaicPerformance;

typedef std::vector<PhotovoltaicPerformance> PhotovoltaicPerformanceVector;

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICPERFORMANCE_HPP

