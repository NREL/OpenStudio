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

class Schedule;
class PlanarSurface;
class Surface;
class SubSurface;

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

  boost::optional<PlanarSurface> surface() const;

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

  bool setSurface(const PlanarSurface& surface);

  void resetSurface();

  bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

  void resetConvectionCoefficient1Location();

  bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

  void resetConvectionCoefficient1Type();

  bool setConvectionCoefficient1(double convectionCoefficient1);

  void resetConvectionCoefficient1();

  bool setConvectionCoefficient1Schedule(Schedule& schedule);

  void resetConvectionCoefficient1Schedule();

  // DLM: not supporting user curves yet
  //bool setConvectionCoefficient1UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

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
  //bool setConvectionCoefficient2UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

  //void resetConvectionCoefficient2UserCurve();

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

