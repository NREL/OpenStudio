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

#ifndef MODEL_DISTRICTCOOLING_HPP
#define MODEL_DISTRICTCOOLING_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class DistrictCooling_Impl;

} // detail

/** DistrictCooling is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictCooling'. */

class MODEL_API DistrictCooling : public StraightComponent {

 public:

  /** @name Constructors and Destructors */
  //@{

  explicit DistrictCooling(const Model& model);

  virtual ~DistrictCooling() {}

  //@}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{


  double nominalCapacity() const;

  Quantity getNominalCapacity(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setNominalCapacity(double nominalCapacity);

  bool setNominalCapacity(const Quantity& nominalCapacity);

  //@}
  /** @name Other */
  //@{

  //@}

 protected:
  /// @cond
  typedef detail::DistrictCooling_Impl ImplType;

  explicit DistrictCooling(std::shared_ptr<detail::DistrictCooling_Impl> impl);

  friend class detail::DistrictCooling_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.DistrictCooling");

};


/** \relates DistrictCooling*/
typedef boost::optional<DistrictCooling> OptionalDistrictCooling;

/** \relates DistrictCooling*/
typedef std::vector<DistrictCooling> DistrictCoolingVector;

} // model
} // openstudio

#endif // MODEL_DISTRICTCOOLING_HPP



