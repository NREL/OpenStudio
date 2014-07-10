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

#ifndef MODEL_GLARESENSOR_HPP
#define MODEL_GLARESENSOR_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

namespace detail {

  class GlareSensor_Impl;

} // detail

/** GlareSensor is a SpaceItem that wraps the OpenStudio IDD object 'OS:Glare:Sensor'. */
class MODEL_API GlareSensor : public SpaceItem {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GlareSensor(const Model& model);

  virtual ~GlareSensor() {}

  //@}
  /** @name Static Methods */
  //@{

  static const IddObjectType& iddObjectType();

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

  int numberofGlareViewVectors() const;

  boost::optional<double> maximumAllowableDaylightGlareProbability() const;

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

  bool setNumberofGlareViewVectors(int numberofGlareViewVectors);

  void resetNumberofGlareViewVectors();

  bool setMaximumAllowableDaylightGlareProbability(double maximumAllowableDaylightGlareProbability);

  void resetMaximumAllowableDaylightGlareProbability();

  //@}

  /// Returns a Point3d representing position only in meters relative to the Space.
  openstudio::Point3d position() const;

  /// Sets a Point3d representing position only position in meters relative to the Space.
  /// Does not change the orientation of the GlareSensor.
  bool setPosition(const openstudio::Point3d& position);

  /// Returns a Transformation representing position and orientation in meters relative to the Space.
  openstudio::Transformation transformation() const;
  
  /** Sets a Transformation representing position and orientation in meters relative to the Space. */
  bool setTransformation(const openstudio::Transformation& transformation);

  /// Keeps current position and orients the view towards the target, target is in space coordinates.
  bool aimAt(const Point3d& target);

 protected:
  /// @cond
  typedef detail::GlareSensor_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit GlareSensor(std::shared_ptr<detail::GlareSensor_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.GlareSensor");
};

/** \relates GlareSensor*/
typedef boost::optional<GlareSensor> OptionalGlareSensor;

/** \relates GlareSensor*/
typedef std::vector<GlareSensor> GlareSensorVector;

} // model
} // openstudio

#endif // MODEL_GLARESENSOR_HPP

