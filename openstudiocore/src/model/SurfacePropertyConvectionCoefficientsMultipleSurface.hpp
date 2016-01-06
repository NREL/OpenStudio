/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;

} // detail

/** SurfacePropertyConvectionCoefficientsMultipleSurface is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ConvectionCoefficients:MultipleSurface'. */
class MODEL_API SurfacePropertyConvectionCoefficientsMultipleSurface : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SurfacePropertyConvectionCoefficientsMultipleSurface(const Model& model);

  virtual ~SurfacePropertyConvectionCoefficientsMultipleSurface() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> surfaceTypeValues();

  static std::vector<std::string> convectionCoefficient1LocationValues();

  static std::vector<std::string> convectionCoefficient1TypeValues();

  static std::vector<std::string> convectionCoefficient2LocationValues();

  static std::vector<std::string> convectionCoefficient2TypeValues();

  /** @name Getters */
  //@{

  boost::optional<std::string> surfaceType() const;

  boost::optional<std::string> convectionCoefficient1Location() const;

  boost::optional<std::string> convectionCoefficient1Type() const;

  boost::optional<double> convectionCoefficient1() const;

  boost::optional<Schedule> convectionCoefficient1Schedule() const;

  // DLM: not supporting user curves yet
  //boost::optional<UserConvectionModels> convectionCoefficient1UserCurve() const;

  boost::optional<std::string> convectionCoefficient2Location() const;

  boost::optional<std::string> convectionCoefficient2Type() const;

  boost::optional<double> convectionCoefficient2() const;

  boost::optional<Schedule> convectionCoefficient2Schedule() const;

  // DLM: not supporting user curves yet
  //boost::optional<UserConvectionModels> convectionCoefficient2UserCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setSurfaceType(const std::string& surfaceType);

  void resetSurfaceType();

  bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

  void resetConvectionCoefficient1Location();

  bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

  void resetConvectionCoefficient1Type();

  bool setConvectionCoefficient1(double convectionCoefficient1);

  void resetConvectionCoefficient1();

  bool setConvectionCoefficient1Schedule(Schedule& schedule);

  void resetConvectionCoefficient1Schedule();

  // DLM: not supporting user curves yet
  //bool setConvectionCoefficient1UserCurve(const UserConvectionModels& userConvectionModels);

  //void resetConvectionCoefficient1UserCurve();

  bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

  void resetConvectionCoefficient2Location();

  bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

  void resetConvectionCoefficient2Type();

  bool setConvectionCoefficient2(double convectionCoefficient2);

  void resetConvectionCoefficient2();

  bool setConvectionCoefficient2Schedule(Schedule& schedule);

  void resetConvectionCoefficient2Schedule();

  // DLM: not supporting user curves yet
  //bool setConvectionCoefficient2UserCurve(const UserConvectionModels& userConvectionModels);

  //void resetConvectionCoefficient2UserCurve();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl ImplType;

  explicit SurfacePropertyConvectionCoefficientsMultipleSurface(std::shared_ptr<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl> impl);

  friend class detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficientsMultipleSurface");
};

/** \relates SurfacePropertyConvectionCoefficientsMultipleSurface*/
typedef boost::optional<SurfacePropertyConvectionCoefficientsMultipleSurface> OptionalSurfacePropertyConvectionCoefficientsMultipleSurface;

/** \relates SurfacePropertyConvectionCoefficientsMultipleSurface*/
typedef std::vector<SurfacePropertyConvectionCoefficientsMultipleSurface> SurfacePropertyConvectionCoefficientsMultipleSurfaceVector;

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP

