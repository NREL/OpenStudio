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

#ifndef MODEL_CURVEFANPRESSURERISE_HPP
#define MODEL_CURVEFANPRESSURERISE_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveFanPressureRise_Impl;

} // detail

/** CurveFanPressureRise is a Curve that wraps the OpenStudio IDD object 
 *  'OS:Curve:FanPressureRise'. The functional form is deltaP_fan,tot = 
 *  c1 * Q_fan^2 + c2 * Q_fan + c3 * Q_fan * sqrt(P_sm - P_o) + c4 * (P_sm - P_o),
 *  where
 *  P_o = 0.0. */
class MODEL_API CurveFanPressureRise : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets c1 = c2 = c3 = c4 = 1.0, Q_fan within [0.0,10.0], P_sm within [0.0,500.0]. */
  explicit CurveFanPressureRise(const Model& model);

  virtual ~CurveFanPressureRise() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double coefficient1C1() const;

  double coefficient2C2() const;

  double coefficient3C3() const;

  double coefficient4C4() const;

  double minimumValueofQfan() const;

  double maximumValueofQfan() const;

  double minimumValueofPsm() const;

  double maximumValueofPsm() const;

  boost::optional<double> minimumCurveOutput() const;

  boost::optional<double> maximumCurveOutput() const;

  //@}
  /** @name Setters */
  //@{

  void setCoefficient1C1(double coefficient1C1);

  void setCoefficient2C2(double coefficient2C2);

  void setCoefficient3C3(double coefficient3C3);

  void setCoefficient4C4(double coefficient4C4);

  void setMinimumValueofQfan(double minimumValueofQfan);

  void setMaximumValueofQfan(double maximumValueofQfan);

  void setMinimumValueofPsm(double minimumValueofPsm);

  void setMaximumValueofPsm(double maximumValueofPsm);

  void setMinimumCurveOutput(double minimumCurveOutput);

  void resetMinimumCurveOutput();

  void setMaximumCurveOutput(double maximumCurveOutput);

  void resetMaximumCurveOutput();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CurveFanPressureRise_Impl ImplType;

  explicit CurveFanPressureRise(std::shared_ptr<detail::CurveFanPressureRise_Impl> impl);

  friend class detail::CurveFanPressureRise_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveFanPressureRise");
};

/** \relates CurveFanPressureRise*/
typedef boost::optional<CurveFanPressureRise> OptionalCurveFanPressureRise;

/** \relates CurveFanPressureRise*/
typedef std::vector<CurveFanPressureRise> CurveFanPressureRiseVector;

} // model
} // openstudio

#endif // MODEL_CURVEFANPRESSURERISE_HPP

