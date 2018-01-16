/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  static IddObjectType iddObjectType();

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

  bool setPositionXCoordinate(double positionXCoordinate);

  bool setPositionYCoordinate(double positionYCoordinate);

  bool setPositionZCoordinate(double positionZCoordinate);

  bool setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

  void resetPsiRotationAroundXAxis();

  bool setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

  void resetThetaRotationAroundYAxis();

  bool setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

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
