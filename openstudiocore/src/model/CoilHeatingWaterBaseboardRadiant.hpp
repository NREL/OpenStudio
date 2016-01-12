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

#ifndef MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP
#define MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class CoilHeatingWaterBaseboardRadiant_Impl;

} // detail

/** CoilHeatingWaterBaseboardRadiant is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Water:Baseboard:Radiant'. */
class MODEL_API CoilHeatingWaterBaseboardRadiant : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingWaterBaseboardRadiant(const Model& model);

  virtual ~CoilHeatingWaterBaseboardRadiant() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatingDesignCapacityMethodValues();

  /** @name Getters */
  //@{

  double ratedAverageWaterTemperature() const;

  double ratedWaterMassFlowRate() const;

  std::string heatingDesignCapacityMethod() const;

  boost::optional<double> heatingDesignCapacity() const;

  bool isHeatingDesignCapacityAutosized() const;

  double heatingDesignCapacityPerFloorArea() const;

  double fractionofAutosizedHeatingDesignCapacity() const;

  boost::optional<double> maximumWaterFlowRate() const;

  bool isMaximumWaterFlowRateAutosized() const;

  double convergenceTolerance() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedAverageWaterTemperature(double ratedAverageWaterTemperature);

  bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

  bool setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod);

  bool setHeatingDesignCapacity(double heatingDesignCapacity);

  void autosizeHeatingDesignCapacity();

  bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

  bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

  void setMaximumWaterFlowRate(double maximumWaterFlowRate);

  void autosizeMaximumWaterFlowRate();

  bool setConvergenceTolerance(double convergenceTolerance);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingWaterBaseboardRadiant_Impl ImplType;

  explicit CoilHeatingWaterBaseboardRadiant(std::shared_ptr<detail::CoilHeatingWaterBaseboardRadiant_Impl> impl);

  friend class detail::CoilHeatingWaterBaseboardRadiant_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboardRadiant");
};

/** \relates CoilHeatingWaterBaseboardRadiant*/
typedef boost::optional<CoilHeatingWaterBaseboardRadiant> OptionalCoilHeatingWaterBaseboardRadiant;

/** \relates CoilHeatingWaterBaseboardRadiant*/
typedef std::vector<CoilHeatingWaterBaseboardRadiant> CoilHeatingWaterBaseboardRadiantVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP

