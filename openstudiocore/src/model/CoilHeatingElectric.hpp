/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGELECTRIC_HPP
#define MODEL_COILHEATINGELECTRIC_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Node;

namespace detail {

class CoilHeatingElectric_Impl;

} // detail

/** CoilHeatingElectric is a StraightComponent that wraps the OpenStudio IDD
 *  object 'OS_Coil_Heating_Electric'. */
class MODEL_API CoilHeatingElectric : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  CoilHeatingElectric(const Model& model, Schedule & schedule);

  virtual ~CoilHeatingElectric() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  double efficiency() const;

  bool isEfficiencyDefaulted() const;

  boost::optional<double> nominalCapacity() const;

  bool isNominalCapacityDefaulted() const;

  bool isNominalCapacityAutosized() const;

  boost::optional<Node> temperatureSetpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule & schedule );

  bool setEfficiency(double efficiency);

  void resetEfficiency();

  void setNominalCapacity(double nominalCapacity);

  void resetNominalCapacity();

  void autosizeNominalCapacity();

  void setTemperatureSetpointNode(Node & temperatureSetpointNode);

  void resetTemperatureSetpointNode();

  //@}

 protected:
  /// @cond

  typedef detail::CoilHeatingElectric_Impl ImplType;

  friend class detail::CoilHeatingElectric_Impl;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit CoilHeatingElectric(std::shared_ptr<detail::CoilHeatingElectric_Impl> impl);

  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.CoilHeatingElectric");
};

/** \relates CoilHeatingElectric*/
typedef boost::optional<CoilHeatingElectric> OptionalCoilHeatingElectric;

/** \relates CoilHeatingElectric*/
typedef std::vector<CoilHeatingElectric> CoilHeatingElectricVector;

} // model

} // openstudio

#endif // MODEL_COILHEATINGELECTRIC_HPP

