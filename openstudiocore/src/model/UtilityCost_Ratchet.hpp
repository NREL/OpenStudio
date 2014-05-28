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

#ifndef MODEL_UTILITYCOST_RATCHET_HPP
#define MODEL_UTILITYCOST_RATCHET_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Ratchet_Impl;
}

/** UtilityCost_Ratchet derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Ratchet".
 *
 *  UtilityCost_Ratchet allows the modeling of tariffs that include some type of seasonal ratcheting. 
 *  Ratchets are most common when used with electric demand charges.
 */
class MODEL_API UtilityCost_Ratchet : public ParentObject{
 
public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Ratchet object in the model.
  explicit UtilityCost_Ratchet(const Model& model);

  virtual ~UtilityCost_Ratchet() {}


  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> baselineSourceVariable() const;

  boost::optional<std::string> adjustmentSourceVariable() const;

  boost::optional<std::string> seasonFrom() const;

  boost::optional<std::string> seasonTo() const;

  boost::optional<std::string> multiplierValueOrVariableName() const;

  boost::optional<std::string> offsetValueOrVariableName() const;

  //@}
  /** @name Setters */
  //@{
  
  void setTariffName(const std::string& tariffName);

  void setBaselineSourceVariable(const std::string& baselineSourceVariable);
  
  void setAdjustmentSourceVariable(const std::string& adjustmentSourceVariable);
  
  void setSeasonFrom(const std::string& seasonFrom);
  
  void setSeasonTo(const std::string& seasonTo);
  
  void setMultiplierValueOrVariableName(const std::string& multiplierValueOrVariableName);
  
  void setOffsetValueOrVariableName(const std::string& offsetValueOrVariableName);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:
  
  /// @cond

  typedef detail::UtilityCost_Ratchet_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Ratchet(std::shared_ptr<detail::UtilityCost_Ratchet_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Ratchet");

  /// @endcond

};

/** \relates UtilityCost_Ratchet */
typedef boost::optional<UtilityCost_Ratchet> OptionalUtilityCost_Ratchet;

/** \relates UtilityCost_Ratchet */
typedef std::vector<UtilityCost_Ratchet> UtilityCost_RatchetVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_RATCHET_HPP
