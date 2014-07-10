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

#ifndef MODEL_AIRWALLMATERIAL_HPP
#define MODEL_AIRWALLMATERIAL_HPP

#include "ModelAPI.hpp"
#include "ModelPartitionMaterial.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class AirWallMaterial_Impl;

} // detail

/** AirWallMaterial is a ModelPartitionMaterial that wraps the OpenStudio IDD object 'OS:Material:AirWall'. */
class MODEL_API AirWallMaterial : public ModelPartitionMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirWallMaterial(const Model& model);

  virtual ~AirWallMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  //@}
  /** @name Setters */
  //@{

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirWallMaterial_Impl ImplType;

  explicit AirWallMaterial(std::shared_ptr<detail::AirWallMaterial_Impl> impl);

  friend class detail::AirWallMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirWallMaterial");
};

/** \relates AirWallMaterial*/
typedef boost::optional<AirWallMaterial> OptionalAirWallMaterial;

/** \relates AirWallMaterial*/
typedef std::vector<AirWallMaterial> AirWallMaterialVector;

} // model
} // openstudio

#endif // MODEL_AIRWALLMATERIAL_HPP

