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

#ifndef MODEL_UTILITYCOST_QUALIFY_HPP
#define MODEL_UTILITYCOST_QUALIFY_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio{
namespace model{

namespace detail{
  class UtilityCost_Qualify_Impl;
}

/** UtilityCost_Qualify derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:UtilityCost:Qualify".
 *
 *  UtilityCost_Qualify specifies demand and energy use requirements for buildings to qualify for individual UtilityCost_Tariff.
 *  This is commonly used for utilities which separate customers into groups such as "low demand", "medium demand", and "high demand".
 */
class MODEL_API UtilityCost_Qualify : public ParentObject{
 
public:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new UtilityCost_Qualify object in the model.
  explicit UtilityCost_Qualify(const Model& model);

  virtual ~UtilityCost_Qualify() {}

  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> name() const;

  boost::optional<std::string> tariffName() const;

  boost::optional<std::string> variableName() const;

  boost::optional<std::string> qualifyType() const;

  boost::optional<std::string> thresholdValueOrVariableName() const;

  boost::optional<std::string> season() const;

  boost::optional<std::string> thresholdTest() const;

  boost::optional<int> numberOfMonths() const;

  //@}
  /** @name Setters */
  //@{

  void setName(const std::string& name);

  void setTariffName(const std::string& tariffName);
  
  void setVariableName(const std::string& variableName);

  void setQualifyType(const std::string& qualifyType);

  void setThresholdValueOrVariableName(const std::string& thresholdValueOrVariableName);
  
  void setSeason(const std::string& season);

  void setThresholdTest(const std::string& thresholdTest);
  
  void setNumberOfMonths(int numberOfMonths);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

protected:

  /// @cond

  typedef detail::UtilityCost_Qualify_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit UtilityCost_Qualify(std::shared_ptr<detail::UtilityCost_Qualify_Impl> impl);

private:
  REGISTER_LOGGER("openstudio.model.UtilityCost_Qualify");

  /// @endcond

};

/** \relates UtilityCost_Qualify */
typedef boost::optional<UtilityCost_Qualify> OptionalUtilityCost_Qualify;

/** \relates UtilityCost_Qualify */
typedef std::vector<UtilityCost_Qualify> UtilityCost_QualifyVector;

} // model
} // openstudio

#endif // MODEL_UTILITYCOST_QUALIFY_HPP
