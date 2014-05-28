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

#ifndef MODEL_DAYLIGHTINGCONTROL_HPP
#define MODEL_DAYLIGHTINGCONTROL_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

namespace detail {

  class DaylightingControl_Impl;

} // detail

/** DaylightingControl is a SpaceItem that wraps the OpenStudio IDD object 'OS:Daylighting:Control'. */
class MODEL_API DaylightingControl : public SpaceItem {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DaylightingControl(const Model& model);

  virtual ~DaylightingControl() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validLightingControlTypeValues();

  //@}
  /** @name Getters */
  //@{

  double positionXCoordinate() const;

  double positionYCoordinate() const;

  double positionZCoordinate() const;

  double psiRotationAroundXAxis() const;

  bool isPsiRotationAroundXAxisDefaulted() const;

  double thetaRotationAroundYAxis() const;

  bool isThetaRotationAroundYAxisDefaulted() const;

  double phiRotationAroundZAxis() const;

  bool isPhiRotationAroundZAxisDefaulted() const;

  double illuminanceSetpoint() const;

  bool isIlluminanceSetpointDefaulted() const;

  std::string lightingControlType() const;

  bool isLightingControlTypeDefaulted() const;

  double minimumInputPowerFractionforContinuousDimmingControl() const;

  bool isMinimumInputPowerFractionforContinuousDimmingControlDefaulted() const;

  double minimumLightOutputFractionforContinuousDimmingControl() const;

  bool isMinimumLightOutputFractionforContinuousDimmingControlDefaulted() const;

  int numberofSteppedControlSteps() const;

  bool isNumberofSteppedControlStepsDefaulted() const;

  double probabilityLightingwillbeResetWhenNeededinManualSteppedControl() const;

  bool isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted() const;

  int numberofDaylightingViews() const;

  bool isNumberofDaylightingViewsDefaulted() const;

  boost::optional<double> maximumAllowableDiscomfortGlareIndex() const;

  //@}
  /** @name Setters */
  //@{

  void setPositionXCoordinate(double positionXCoordinate);

  void setPositionYCoordinate(double positionYCoordinate);

  void setPositionZCoordinate(double positionZCoordinate);

  void setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

  void resetPsiRotationAroundXAxis();

  void setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

  void resetThetaRotationAroundYAxis();

  void setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

  void resetPhiRotationAroundZAxis();

  bool setIlluminanceSetpoint(double illuminanceSetpoint);

  void resetIlluminanceSetpoint();

  bool setLightingControlType(std::string lightingControlType);

  void resetLightingControlType();

  bool setMinimumInputPowerFractionforContinuousDimmingControl(double minimumInputPowerFractionforContinuousDimmingControl);

  void resetMinimumInputPowerFractionforContinuousDimmingControl();

  bool setMinimumLightOutputFractionforContinuousDimmingControl(double minimumLightOutputFractionforContinuousDimmingControl);

  void resetMinimumLightOutputFractionforContinuousDimmingControl();

  void setNumberofSteppedControlSteps(int numberofSteppedControlSteps);

  void resetNumberofSteppedControlSteps();

  bool setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(double probabilityLightingwillbeResetWhenNeededinManualSteppedControl);

  void resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl();

  bool setNumberofDaylightingViews(int numberofDaylightingViews);

  void resetNumberofDaylightingViews();

  bool setMaximumAllowableDiscomfortGlareIndex(double maximumAllowableDiscomfortGlareIndex);

  void resetMaximumAllowableDiscomfortGlareIndex();

  //@}

  /// Returns a Point3d representing position only in meters relative to the Space.
  openstudio::Point3d position() const;

  /// Sets a Point3d representing position only position in meters relative to the Space.
  /// Does not change the orientation of the DaylightingControl.
  bool setPosition(const openstudio::Point3d& position);

  /// Returns a Transformation representing position and orientation in meters relative to the Space.
  openstudio::Transformation transformation() const;
  
  /** Sets a Transformation representing position and orientation in meters relative to the Space. */
  bool setTransformation(const openstudio::Transformation& transformation);

  /// Is this the primary control point for the containing ThermalZone.
  bool isPrimaryDaylightingControl() const;

  /// Is this the secondary control point for the containing ThermalZone.
  bool isSecondaryDaylightingControl() const;

  /// Keeps current position and orients the view towards the target, target is in space coordinates.
  bool aimAt(const Point3d& target);

 protected:
  /// @cond
  typedef detail::DaylightingControl_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit DaylightingControl(std::shared_ptr<detail::DaylightingControl_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DaylightingControl");
};

/** \relates DaylightingControl*/
typedef boost::optional<DaylightingControl> OptionalDaylightingControl;

/** \relates DaylightingControl*/
typedef std::vector<DaylightingControl> DaylightingControlVector;

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGCONTROL_HPP

