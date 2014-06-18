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

#ifndef MODEL_CURRENCYTYPE_HPP
#define MODEL_CURRENCYTYPE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class CurrencyType_Impl;
}

/** CurrencyType derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:CurrencyType".
 *
 *  CurrencyType is a unique object used to specify the currency that cost data are entered and presented in for this Model.
 */
class MODEL_API CurrencyType : public ParentObject{
 
public:
  virtual ~CurrencyType() {}

  /** @name Getters */
  //@{

  boost::optional<std::string>  monetaryUnit() const;

  //@}
  /** @name Setters */
  //@{

  void setMonetaryUnit(const std::string& monetaryUnit);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new CurrencyType object in the model.
  explicit CurrencyType(const Model& model);
  
  //@}

  /// @cond

  typedef detail::CurrencyType_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit CurrencyType(std::shared_ptr<detail::CurrencyType_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.CurrencyType");

  /// @endcond

};

/** \relates CurrencyType */
typedef boost::optional<CurrencyType> OptionalCurrencyType;

/** \relates CurrencyType */
typedef std::vector<CurrencyType> CurrencyTypeVector;

} // model
} // openstudio

#endif // MODEL_CURRENCYTYPE_HPP
