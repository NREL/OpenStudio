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

#ifndef MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP
#define MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class FFactorGroundFloorConstruction_Impl;
}

/** FFactorGroundFloorConstruction is a ConstructionBase that wraps EnergyPlus IDD object 
 *  'Construction:FfactorGroundFloor'. This is a simple, unlayered construction object that may
 *  be applied to a Surface (typically a ground floor). */
class MODEL_API FFactorGroundFloorConstruction : public ConstructionBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FFactorGroundFloorConstruction(const Model& model,
    double fFactor = 0.1,
    double area = 0.1,
    double perimeterExposed = 0.1);

  virtual ~FFactorGroundFloorConstruction() {}

  /** Attribute Name: 'fFactor' */
  virtual double fFactor() const;
  virtual Quantity getFFactor(bool returnIP = false) const;
  virtual bool setFFactor(double fFactor);
  virtual bool setFFactor(const Quantity& fFactor);

  /** Attribute Name: 'area' */
  virtual double area() const;
  virtual Quantity getArea(bool returnIP = false) const;
  virtual bool setArea(double area);
  virtual bool setArea(const Quantity& area);

  /** Attribute Name: 'perimeterExposed' */
  virtual double perimeterExposed() const;
  virtual Quantity getPerimeterExposed(bool returnIP = false) const;
  virtual bool setPerimeterExposed(double perimeterExposed);
  virtual bool setPerimeterExposed(const Quantity& perimeterExposed);

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
 protected:
  /// @cond
  typedef detail::FFactorGroundFloorConstruction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::FFactorGroundFloorConstruction_Impl;

  explicit FFactorGroundFloorConstruction(
      std::shared_ptr<detail::FFactorGroundFloorConstruction_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FFactorGroundFloorConstruction");
};

/** \relates FFactorGroundFloorConstruction */
typedef boost::optional<FFactorGroundFloorConstruction> OptionalFFactorGroundFloorConstruction;

/** \relates FFactorGroundFloorConstruction */
typedef std::vector<FFactorGroundFloorConstruction> FFactorGroundFloorConstructionVector;

} // model
} // openstudio

#endif // MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP
