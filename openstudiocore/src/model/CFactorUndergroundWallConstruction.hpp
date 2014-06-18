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

#ifndef MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_HPP
#define MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class CFactorUndergroundWallConstruction_Impl;
}

/** CFactorUndergroundWallConstruction is a ConstructionBase object that wraps EnergyPlus IDD 
 *  object 'Construction:CfactorUndergroundWall'. */
class MODEL_API CFactorUndergroundWallConstruction : public ConstructionBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** TODO default values should be reviewed */
  explicit CFactorUndergroundWallConstruction(const Model& model,
    double cFactor = 0.1,
    double height = 0.1);

  virtual ~CFactorUndergroundWallConstruction() {}

  /** Attribute Name: 'cFactor' */
  virtual double cFactor() const;
  virtual Quantity getCFactor(bool returnIP = false) const;
  virtual bool setCFactor(double cFactor);
  virtual bool setCFactor(const Quantity& cFactor);

  /** Attribute Name: 'height' */
  virtual double height() const;
  virtual Quantity getHeight(bool returnIP = false) const;
  virtual bool setHeight(double height);
  virtual bool setHeight(const Quantity& height);

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
 protected:
  /// @cond
  typedef detail::CFactorUndergroundWallConstruction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::CFactorUndergroundWallConstruction_Impl;

  explicit CFactorUndergroundWallConstruction(
      std::shared_ptr<detail::CFactorUndergroundWallConstruction_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CFactorUndergroundWallConstruction");
};

/** \relates CFactorUndergroundWallConstruction */
typedef boost::optional<CFactorUndergroundWallConstruction> OptionalCFactorUndergroundWallConstruction;

/** \relates CFactorUndergroundWallConstruction */
typedef std::vector<CFactorUndergroundWallConstruction> CFactorUndergroundWallConstructionVector;

} // model
} // openstudio

#endif // MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_HPP
