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

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class AllHeatTranSurf;
class Schedule;
class UserConvectionModels;
class Schedule;
class UserConvectionModels;

namespace detail {

  class SurfacePropertyConvectionCoefficients_Impl;

} // detail

/** SurfacePropertyConvectionCoefficients is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ConvectionCoefficients'. */
class MODEL_API SurfacePropertyConvectionCoefficients : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SurfacePropertyConvectionCoefficients(const Model& model);

  virtual ~SurfacePropertyConvectionCoefficients() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> convectionCoefficient1LocationValues();

  static std::vector<std::string> convectionCoefficient1TypeValues();

  static std::vector<std::string> convectionCoefficient2LocationValues();

  static std::vector<std::string> convectionCoefficient2TypeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: AllHeatTranSurf.
  AllHeatTranSurf surface() const;

  std::string convectionCoefficient1Location() const;

  std::string convectionCoefficient1Type() const;

  boost::optional<double> convectionCoefficient1() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> convectionCoefficient1Schedule() const;

  // TODO: Check return type. From object lists, some candidates are: UserConvectionModels.
  boost::optional<UserConvectionModels> convectionCoefficient1UserCurve() const;

  boost::optional<std::string> convectionCoefficient2Location() const;

  boost::optional<std::string> convectionCoefficient2Type() const;

  double convectionCoefficient2() const;

  bool isConvectionCoefficient2Defaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> convectionCoefficient2Schedule() const;

  // TODO: Check return type. From object lists, some candidates are: UserConvectionModels.
  boost::optional<UserConvectionModels> convectionCoefficient2UserCurve() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: AllHeatTranSurf.
  bool setSurface(const AllHeatTranSurf& allHeatTranSurf);

  bool setConvectionCoefficient1Location(std::string convectionCoefficient1Location);

  bool setConvectionCoefficient1Type(std::string convectionCoefficient1Type);

  void setConvectionCoefficient1(double convectionCoefficient1);

  void resetConvectionCoefficient1();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setConvectionCoefficient1Schedule(Schedule& schedule);

  void resetConvectionCoefficient1Schedule();

  // TODO: Check argument type. From object lists, some candidates are: UserConvectionModels.
  bool setConvectionCoefficient1UserCurve(const UserConvectionModels& userConvectionModels);

  void resetConvectionCoefficient1UserCurve();

  bool setConvectionCoefficient2Location(std::string convectionCoefficient2Location);

  void resetConvectionCoefficient2Location();

  bool setConvectionCoefficient2Type(std::string convectionCoefficient2Type);

  void resetConvectionCoefficient2Type();

  void setConvectionCoefficient2(double convectionCoefficient2);

  void resetConvectionCoefficient2();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setConvectionCoefficient2Schedule(Schedule& schedule);

  void resetConvectionCoefficient2Schedule();

  // TODO: Check argument type. From object lists, some candidates are: UserConvectionModels.
  bool setConvectionCoefficient2UserCurve(const UserConvectionModels& userConvectionModels);

  void resetConvectionCoefficient2UserCurve();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SurfacePropertyConvectionCoefficients_Impl ImplType;

  explicit SurfacePropertyConvectionCoefficients(std::shared_ptr<detail::SurfacePropertyConvectionCoefficients_Impl> impl);

  friend class detail::SurfacePropertyConvectionCoefficients_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficients");
};

/** \relates SurfacePropertyConvectionCoefficients*/
typedef boost::optional<SurfacePropertyConvectionCoefficients> OptionalSurfacePropertyConvectionCoefficients;

/** \relates SurfacePropertyConvectionCoefficients*/
typedef std::vector<SurfacePropertyConvectionCoefficients> SurfacePropertyConvectionCoefficientsVector;

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_HPP

