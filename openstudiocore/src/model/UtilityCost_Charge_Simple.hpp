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

#ifndef MODEL_UTILITYCOST_CHARGE_SIMPLE_HPP
#define MODEL_UTILITYCOST_CHARGE_SIMPLE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Charge_Simple_Impl;
}

/** UtilityCost_Charge_Simple derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Charge:Simple".
 *
 *  UtilityCost_Charge_Simple is used to compute flat charges such as taxes and fees related to a utility bill.
 */
class MODEL_API UtilityCost_Charge_Simple : public ParentObject{
 
public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Charge_Simple object in the model.
  explicit UtilityCost_Charge_Simple(const Model& model);

  virtual ~UtilityCost_Charge_Simple() {}

  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> sourceVariable() const;

  boost::optional<std::string> season() const;

  boost::optional<std::string> categoryVariableName() const;

  boost::optional<std::string> costPerUnitValueOrVariableName() const;

  //@}
  /** @name Setters */
  //@{

  void setTariffName(const std::string& tariffName);

  void setSourceVariable(const std::string& sourceVariable);

  void setSeason(const std::string& season);

  void setCategoryVariableName(const std::string& categoryVariableName);

  void setCostPerUnitValueOrVariableName(const std::string& costPerUnitValueOrVariableName);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond

  typedef detail::UtilityCost_Charge_Simple_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Charge_Simple(std::shared_ptr<detail::UtilityCost_Charge_Simple_Impl> impl);

private:
  REGISTER_LOGGER("openstudio.model.UtilityCost_Charge_Simple");

  /// @endcond

};

/** \relates UtilityCost_Charge_Simple */
typedef boost::optional<UtilityCost_Charge_Simple> OptionalUtilityCost_Charge_Simple;

/** \relates UtilityCost_Charge_Simple */
typedef std::vector<UtilityCost_Charge_Simple> UtilityCost_Charge_SimpleVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_CHARGE_SIMPLE_HPP
