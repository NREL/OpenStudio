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

#ifndef MODEL_COILHEATINGWATERBASEBOARD_HPP
#define MODEL_COILHEATINGWATERBASEBOARD_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class CoilHeatingWaterBaseboard_Impl;

} // detail

/** CoilHeatingWaterBaseboard is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Water:Baseboard'. */
class MODEL_API CoilHeatingWaterBaseboard : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  CoilHeatingWaterBaseboard(const Model& model);

  virtual ~CoilHeatingWaterBaseboard() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{
 
  unsigned inletPort();

  unsigned outletPort();

  /** Returns the value of the UFactorTimesAreaValue field. **/
  boost::optional<double> uFactorTimesAreaValue() const;

  bool isUFactorTimesAreaValueDefaulted() const;

 /** Returns true if the UFactorTimesAreaValue field is set to autosize **/
  bool isUFactorTimesAreaValueAutosized() const;
 
  /** Returns the value of the MaximumWaterFlowRate field. **/
  boost::optional<double> maximumWaterFlowRate() const;

  //OSOptionalQuantity getMaximumWaterFlowRate(bool returnIP=false) const;

  bool isMaximumWaterFlowRateDefaulted() const;

  bool isMaximumWaterFlowRateAutosized() const;

  double convergenceTolerance() const;

  bool isConvergenceToleranceDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setUFactorTimesAreaValue(double uFactorTimesAreaValue);

  void resetUFactorTimesAreaValue();

  void autosizeUFactorTimesAreaValue();

  void setMaximumWaterFlowRate(double maximumWaterFlowRate);

  void resetMaximumWaterFlowRate();

  void autosizeMaximumWaterFlowRate();

  bool setConvergenceTolerance(double convergenceTolerance);

  void resetConvergenceTolerance();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingWaterBaseboard_Impl ImplType;

  explicit CoilHeatingWaterBaseboard(std::shared_ptr<detail::CoilHeatingWaterBaseboard_Impl> impl);

  friend class detail::CoilHeatingWaterBaseboard_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboard");
};

/** \relates CoilHeatingWaterBaseboard*/
typedef boost::optional<CoilHeatingWaterBaseboard> OptionalCoilHeatingWaterBaseboard;

/** \relates CoilHeatingWaterBaseboard*/
typedef std::vector<CoilHeatingWaterBaseboard> CoilHeatingWaterBaseboardVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERBASEBOARD_HPP

