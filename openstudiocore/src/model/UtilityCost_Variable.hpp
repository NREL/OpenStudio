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

#ifndef MODEL_UTILITYCOST_VARIABLE_HPP
#define MODEL_UTILITYCOST_VARIABLE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Variable_Impl;
}

/** UtilityCost_Variable derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Variable".
 *
 *  UtilityCost_Variable allows for the direct entry of monthly values into a variable.
 */
class MODEL_API UtilityCost_Variable : public ParentObject{
 
public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Variable object in the model.
  explicit UtilityCost_Variable(const Model& model);

  virtual ~UtilityCost_Variable() {}


  //@}
  /** @name Getters */
  //@{

  boost::optional<double> januaryValue() const;

  boost::optional<double> februaryValue() const;

  boost::optional<double> marchValue() const;

  boost::optional<double> aprilValue() const;

  boost::optional<double> mayValue() const;

  boost::optional<double> juneValue() const;

  boost::optional<double> julyValue() const;

  boost::optional<double> augustValue() const;

  boost::optional<double> septemberValue() const;

  boost::optional<double> octoberValue() const;

  boost::optional<double> novemberValue() const;

  boost::optional<double> decemberValue() const;

  boost::optional<std::string> name() const;

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> variableType() const;

  //@}
  /** @name Setters */
  //@{
  
  void setJanuaryValue(double januaryValue);

  void setFebruaryValue(double februaryValue);
  
  void setMarchValue(double marchValue);
  
  void setAprilValue(double aprilValue);
  
  void setMayValue(double mayValue);

  void setJuneValue(double juneValue);

  void setJulyValue(double julyValue);
  
  void setAugustValue(double augustValue);

  void setSeptemberValue(double septemberValue);
  
  void setOctoberValue(double octoberValue);

  void setNovemberValue(double novemberValue);

  void setDecemberValue(double decemberValue);

  void setName(const std::string& name);

  void setTariffName(const std::string& tariffName);
 
  void setVariableType(const std::string& variableType);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond

  typedef detail::UtilityCost_Variable_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Variable(std::shared_ptr<detail::UtilityCost_Variable_Impl> impl);

private:

  REGISTER_LOGGER("openstudio.model.UtilityCost_Variable");

  /// @endcond

};

/** \relates UtilityCost_Variable */
typedef boost::optional<UtilityCost_Variable> OptionalUtilityCost_Variable;

/** \relates UtilityCost_Variable */
typedef std::vector<UtilityCost_Variable> UtilityCost_VariableVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_VARIABLE_HPP
