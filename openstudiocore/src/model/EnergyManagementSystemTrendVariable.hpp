/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class EnergyManagementSystemTrendVariable_Impl;

} // detail

/** EnergyManagementSystemTrendVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:TrendVariable'. */
class MODEL_API EnergyManagementSystemTrendVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemTrendVariable(const Model& model);

  virtual ~EnergyManagementSystemTrendVariable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string eMSVariableName() const;

  int numberofTimestepstobeLogged() const;

  //@}
  /** @name Setters */
  //@{

  bool setEMSVariableName(const std::string& eMSVariableName);

  bool setNumberofTimestepstobeLogged(int numberofTimestepstobeLogged);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemTrendVariable_Impl ImplType;

  explicit EnergyManagementSystemTrendVariable(std::shared_ptr<detail::EnergyManagementSystemTrendVariable_Impl> impl);

  friend class detail::EnergyManagementSystemTrendVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemTrendVariable");
};

/** \relates EnergyManagementSystemTrendVariable*/
typedef boost::optional<EnergyManagementSystemTrendVariable> OptionalEnergyManagementSystemTrendVariable;

/** \relates EnergyManagementSystemTrendVariable*/
typedef std::vector<EnergyManagementSystemTrendVariable> EnergyManagementSystemTrendVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_HPP

