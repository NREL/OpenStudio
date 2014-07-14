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

  void setEndUseSubcategory(std::string endUseSubcategory);

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

