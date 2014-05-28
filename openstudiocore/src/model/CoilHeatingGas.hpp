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

#ifndef MODEL_COILHEATINGGAS_HPP
#define MODEL_COILHEATINGGAS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class Curve;

class Schedule;

namespace detail {
  class CoilHeatingGas_Impl;
} // detail

/** CoilHeatingGas is an interface to the IDD object named "OS:Coil:Heating:Gas"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  OS:Coil:Heating:Gas objects in energyplus.
 */
class MODEL_API CoilHeatingGas : public StraightComponent {

 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new CoilHeatingGas object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   *
   * Sets GasBurnerEffeciency to .8
   * Sets GasFields to AutoSize
   * Sets ParasiticElectricLoad to 0
   * Sets ParasiticGasLoad to 0
   */
  explicit CoilHeatingGas(const Model& model,
                          Schedule& schedule);

  virtual ~CoilHeatingGas() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters and Setters */
  //@{

  /**  Returns the Schedule referred to by the Availability Schedule field. **/
  Schedule availabilitySchedule() const;

  /** \deprecated */
  Schedule availableSchedule() const;

  /** Sets the Schedule referred to by the Availability Schedule field. **/
  bool setAvailabilitySchedule(Schedule& schedule);

  /** \deprecated */
  bool setAvailableSchedule(Schedule& schedule);

  /** Returns the value of the GasBurnerEfficiency field. **/
  double gasBurnerEfficiency();

  /** Sets the value of the GasBurnerEfficiency field. **/
  void setGasBurnerEfficiency(double value);

  /** Returns the value of the ParasiticElectricLoad field. **/
  double parasiticElectricLoad();

  /** Sets the value of the ParasiticElectricLoad field. **/
  void setParasiticElectricLoad(double value);

  /** Returns the value of the ParasiticGasLoad field. **/
  double parasiticGasLoad();

  /** Sets the value of the ParasiticGasLoad field. **/
  void setParasiticGasLoad(double value);

  boost::optional<double> nominalCapacity() const;

  OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

  bool isNominalCapacityAutosized() const;

  void setNominalCapacity(double nominalCapacity);

  bool setNominalCapacity(const Quantity& nominalCapacity);

  void resetNominalCapacity();

  void autosizeNominalCapacity();

  boost::optional<Curve> partLoadFractionCorrelationCurve() const;

  bool setPartLoadFractionCorrelationCurve( const Curve& curve );

  void resetPartLoadFractionCorrelationCurve();

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond

  typedef detail::CoilHeatingGas_Impl ImplType;

  explicit CoilHeatingGas(std::shared_ptr<detail::CoilHeatingGas_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.CoilHeatingGas");

  /// @endcond
};

/** \relates CoilHeatingGas */
typedef boost::optional<CoilHeatingGas> OptionalCoilHeatingGas;

/** \relates CoilHeatingGas */
typedef std::vector<CoilHeatingGas> CoilHeatingGasVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGGAS_HPP

