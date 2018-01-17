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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  class EnergyManagementSystemCurveOrTableIndexVariable_Impl;

} // detail

/** EnergyManagementSystemCurveOrTableIndexVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:CurveOrTableIndexVariable'. */
class MODEL_API EnergyManagementSystemCurveOrTableIndexVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemCurveOrTableIndexVariable(const Model& model, const Curve& curve);

  explicit EnergyManagementSystemCurveOrTableIndexVariable(const Model& model);

  virtual ~EnergyManagementSystemCurveOrTableIndexVariable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  ModelObject curveOrTableObject() const;

  //@}
  /** @name Setters */
  //@{

  bool setCurveOrTableObject(const Curve& allCurves);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl ImplType;

  explicit EnergyManagementSystemCurveOrTableIndexVariable(std::shared_ptr<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl> impl);

  friend class detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemCurveOrTableIndexVariable");
};

/** \relates EnergyManagementSystemCurveOrTableIndexVariable*/
typedef boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> OptionalEnergyManagementSystemCurveOrTableIndexVariable;

/** \relates EnergyManagementSystemCurveOrTableIndexVariable*/
typedef std::vector<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemCurveOrTableIndexVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_HPP

