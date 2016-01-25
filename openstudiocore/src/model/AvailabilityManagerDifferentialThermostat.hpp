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

#ifndef MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_HPP
#define MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class AvailabilityManagerDifferentialThermostat_Impl;

} // detail

/** AvailabilityManagerDifferentialThermostat is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:DifferentialThermostat'. */
class MODEL_API AvailabilityManagerDifferentialThermostat : public AvailabilityManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AvailabilityManagerDifferentialThermostat(const Model& model);

  virtual ~AvailabilityManagerDifferentialThermostat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Node> hotNode() const;

  boost::optional<Node> coldNode() const;

  double temperatureDifferenceOnLimit() const;

  double temperatureDifferenceOffLimit() const;

  //@}
  /** @name Setters */
  //@{

  bool setHotNode(const Node& node);

  void resetHotNode();

  bool setColdNode(const Node& node);

  void resetColdNode();

  void setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit);

  void setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AvailabilityManagerDifferentialThermostat_Impl ImplType;

  explicit AvailabilityManagerDifferentialThermostat(std::shared_ptr<detail::AvailabilityManagerDifferentialThermostat_Impl> impl);

  friend class detail::AvailabilityManagerDifferentialThermostat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerDifferentialThermostat");
};

/** \relates AvailabilityManagerDifferentialThermostat*/
typedef boost::optional<AvailabilityManagerDifferentialThermostat> OptionalAvailabilityManagerDifferentialThermostat;

/** \relates AvailabilityManagerDifferentialThermostat*/
typedef std::vector<AvailabilityManagerDifferentialThermostat> AvailabilityManagerDifferentialThermostatVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_HPP

