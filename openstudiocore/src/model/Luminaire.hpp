/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_LUMINAIRE_HPP
#define MODEL_LUMINAIRE_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

class Schedule;
class LuminaireDefinition;

namespace detail {

  class Luminaire_Impl;

} // detail

/** Luminaire is a SpaceLoadInstance that wraps the OpenStudio IDD object
 * 'OS:Luminaire'. \sa LuminaireDefinition */
class MODEL_API Luminaire : public SpaceLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Luminaire(const LuminaireDefinition& luminaireDefinition);

  virtual ~Luminaire() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Gets the LuminaireDefinition object. */
  LuminaireDefinition luminaireDefinition() const;

  /** Returns the (fractional) lighting schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  double positionXcoordinate() const;

  Quantity getPositionXcoordinate(bool returnIP=false) const;

  double positionYcoordinate() const;

  Quantity getPositionYcoordinate(bool returnIP=false) const;

  double positionZcoordinate() const;

  Quantity getPositionZcoordinate(bool returnIP=false) const;

  double psiRotationAroundXaxis() const;

  Quantity getPsiRotationAroundXaxis(bool returnIP=false) const;

  bool isPsiRotationAroundXaxisDefaulted() const;

  double thetaRotationAroundYaxis() const;

  Quantity getThetaRotationAroundYaxis(bool returnIP=false) const;

  bool isThetaRotationAroundYaxisDefaulted() const;

  double phiRotationAroundZaxis() const;

  Quantity getPhiRotationAroundZaxis(bool returnIP=false) const;

  bool isPhiRotationAroundZaxisDefaulted() const;

  double fractionReplaceable() const;

  Quantity getFractionReplaceable(bool returnIP=false) const;

  bool isFractionReplaceableDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the LuminaireDefinition object. */
  bool setLuminaireDefinition(const LuminaireDefinition& definition);

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  void setPositionXcoordinate(double positionXcoordinate);

  bool setPositionXcoordinate(const Quantity& positionXcoordinate);

  void setPositionYcoordinate(double positionYcoordinate);

  bool setPositionYcoordinate(const Quantity& positionYcoordinate);

  void setPositionZcoordinate(double positionZcoordinate);

  bool setPositionZcoordinate(const Quantity& positionZcoordinate);

  void setPsiRotationAroundXaxis(double psiRotationAroundXaxis);

  bool setPsiRotationAroundXaxis(const Quantity& psiRotationAroundXaxis);

  void resetPsiRotationAroundXaxis();

  void setThetaRotationAroundYaxis(double thetaRotationAroundYaxis);

  bool setThetaRotationAroundYaxis(const Quantity& thetaRotationAroundYaxis);

  void resetThetaRotationAroundYaxis();

  void setPhiRotationAroundZaxis(double phiRotationAroundZaxis);

  bool setPhiRotationAroundZaxis(const Quantity& phiRotationAroundZaxis);

  void resetPhiRotationAroundZaxis();

  bool setFractionReplaceable(double fractionReplaceable);

  bool setFractionReplaceable(const Quantity& fractionReplaceable);

  void resetFractionReplaceable();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  bool setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  /** @name Other */
  //@{

  /// Returns a Point3d representing position only in meters relative to the Space.
  openstudio::Point3d position() const;

  /// Sets a Point3d representing position only position in meters relative to the Space.
  /// Does not change the orientation of the Luminaire.
  bool setPosition(const openstudio::Point3d& position);

  /// Returns a Transformation representing position and orientation in meters relative to the Space.
  openstudio::Transformation transformation() const;

  /// Sets a Transformation representing position and orientation in meters relative to the Space.
  bool setTransformation(const openstudio::Transformation& transformation);

  /** Returns the design level represented by this instance. */
  double lightingPower() const;

  /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2). */
  double getPowerPerFloorArea(double floorArea) const;

  /** Returns the watts/person represented by this instance, assuming numPeople. */
  double getPowerPerPerson(double numPeople) const;

  //@}
 protected:
  /// @cond
  typedef detail::Luminaire_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit Luminaire(std::shared_ptr<detail::Luminaire_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Luminaire");
};

/** \relates Luminaire*/
typedef boost::optional<Luminaire> OptionalLuminaire;

/** \relates Luminaire*/
typedef std::vector<Luminaire> LuminaireVector;

} // model
} // openstudio

#endif // MODEL_LUMINAIRE_HPP

