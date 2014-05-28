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

#ifndef MODEL_FANCONSTANTVOLUME_HPP
#define MODEL_FANCONSTANTVOLUME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {
  class FanConstantVolume_Impl;
} // detail

/** FanConstantVolume is a StraightComponent that wraps the IDD object named
 *  "OS:Fan:ConstantVolume". */
class MODEL_API FanConstantVolume : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new FanConstantVolume object and places it inside the
   *  model.  The object is fully initialized with all companion objects. */
  FanConstantVolume(const Model& model,Schedule& s);

  virtual ~FanConstantVolume() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  /** Returns the value of the FanEfficiency field. **/
  double fanEfficiency();

  /** Returns the value of the PressureRise field. **/
  double pressureRise();

  /** Returns the value of the MotorEfficiency field. **/
  double motorEfficiency();

  /** Returns the value of the MotorInAirStreamFraction field. **/
  double motorInAirstreamFraction();

  /** Returns the value of the EndUseSubcategory field. **/
  std::string endUseSubcategory();

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& s);

  /** Sets the value of the FanEfficiency field. **/
  void setFanEfficiency(double value);

  /** Sets the value of the PressureRise field. **/
  void setPressureRise(double value);

  /** Sets the value of the MotorEfficiency field. **/
  void setMotorEfficiency(double value);

  /** Sets the value of the MotorInAirStreamFraction field. **/
  void setMotorInAirstreamFraction(double value);

  /** Sets the value of the EndUseSubcategory field. **/
  void setEndUseSubcategory(std::string value);

  //@}

  boost::optional<double> maximumFlowRate() const;

  OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

  bool isMaximumFlowRateAutosized() const;

  bool setMaximumFlowRate(double maximumFlowRate);

  bool setMaximumFlowRate(const Quantity& maximumFlowRate);

  void resetMaximumFlowRate();

  void autosizeMaximumFlowRate();

 protected:
  friend class Model;
  friend class openstudio::IdfObject;

  /// @cond
  typedef detail::FanConstantVolume_Impl ImplType;
  explicit FanConstantVolume(std::shared_ptr<detail::FanConstantVolume_Impl> impl);
 private:
  REGISTER_LOGGER("openstudio.model.FanConstantVolume");
  /// @endcond
};

/** \relates FanConstantVolume */
typedef boost::optional<FanConstantVolume> OptionalFanConstantVolume;

/** \relates FanConstantVolume */
typedef std::vector<FanConstantVolume> FanConstantVolumeVector;

} // model
} // openstudio

#endif // MODEL_FANCONSTANTVOLUME_HPP

