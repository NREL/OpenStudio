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

#ifndef MODEL_DISTRICTHEATING_HPP
#define MODEL_DISTRICTHEATING_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class DistrictHeating_Impl;

} // detail

/** DistrictHeating is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictHeating'. */

class MODEL_API DistrictHeating : public StraightComponent {

 public:

  /** @name Constructors and Destructors */
  //@{

  explicit DistrictHeating(const Model& model);

  virtual ~DistrictHeating() {}

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
  typedef detail::DistrictHeating_Impl ImplType;

  explicit DistrictHeating(std::shared_ptr<detail::DistrictHeating_Impl> impl);

  friend class detail::DistrictHeating_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.DistrictHeating");

};


/** \relates DistrictHeating*/
typedef boost::optional<DistrictHeating> OptionalDistrictHeating;

/** \relates DistrictHeating*/
typedef std::vector<DistrictHeating> DistrictHeatingVector;

} // model
} // openstudio

#endif // MODEL_DISTRICTHEATING_HPP



