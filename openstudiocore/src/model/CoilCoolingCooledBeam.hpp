/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILCOOLINGCOOLEDBEAM_HPP
#define MODEL_COILCOOLINGCOOLEDBEAM_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {

namespace model {

namespace detail {

  class CoilCoolingCooledBeam_Impl;

} // detail

/** CoilCoolingCooledBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:CooledBeam'. */
class MODEL_API CoilCoolingCooledBeam : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingCooledBeam(const Model& model);

  virtual ~CoilCoolingCooledBeam() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  //unsigned inletPort();

  //unsigned outletPort();
  
  double coilSurfaceAreaperCoilLength() const;

  bool isCoilSurfaceAreaperCoilLengthDefaulted() const;

  double modelParametera() const;

  bool isModelParameteraDefaulted() const;

  double modelParametern1() const;

  bool isModelParametern1Defaulted() const;

  double modelParametern2() const;

  bool isModelParametern2Defaulted() const;

  double modelParametern3() const;

  bool isModelParametern3Defaulted() const;

  double modelParametera0() const;

  bool isModelParametera0Defaulted() const;

  double modelParameterK1() const;

  bool isModelParameterK1Defaulted() const;

  double modelParametern() const;

  bool isModelParameternDefaulted() const;

  double leavingPipeInsideDiameter() const;

  bool isLeavingPipeInsideDiameterDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setCoilSurfaceAreaperCoilLength(double coilSurfaceAreaperCoilLength);

  void resetCoilSurfaceAreaperCoilLength();

  bool setModelParametera(double modelParametera);

  void resetModelParametera();

  bool setModelParametern1(double modelParametern1);

  void resetModelParametern1();

  bool setModelParametern2(double modelParametern2);

  void resetModelParametern2();

  bool setModelParametern3(double modelParametern3);

  void resetModelParametern3();

  bool setModelParametera0(double modelParametera0);

  void resetModelParametera0();

  bool setModelParameterK1(double modelParameterK1);

  void resetModelParameterK1();

  bool setModelParametern(double modelParametern);

  void resetModelParametern();

  bool setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter);

  void resetLeavingPipeInsideDiameter();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingCooledBeam_Impl ImplType;

  explicit CoilCoolingCooledBeam(boost::shared_ptr<detail::CoilCoolingCooledBeam_Impl> impl);

  friend class detail::CoilCoolingCooledBeam_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingCooledBeam");
};

/** \relates CoilCoolingCooledBeam*/
typedef boost::optional<CoilCoolingCooledBeam> OptionalCoilCoolingCooledBeam;

/** \relates CoilCoolingCooledBeam*/
typedef std::vector<CoilCoolingCooledBeam> CoilCoolingCooledBeamVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGCOOLEDBEAM_HPP

