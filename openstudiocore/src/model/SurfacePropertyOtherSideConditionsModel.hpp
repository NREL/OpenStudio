/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class SurfacePropertyOtherSideConditionsModel_Impl;

} // detail

/** SurfacePropertyOtherSideConditionsModel is a ResourceObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:OtherSideConditionsModel'. */
class MODEL_API SurfacePropertyOtherSideConditionsModel : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SurfacePropertyOtherSideConditionsModel(const Model& model);

  virtual ~SurfacePropertyOtherSideConditionsModel() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> typeOfModelingValues();

  /** @name Getters */
  //@{

  std::string typeOfModeling() const;

  bool isTypeOfModelingDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setTypeOfModeling(const std::string& typeOfModeling);

  void resetTypeOfModeling();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SurfacePropertyOtherSideConditionsModel_Impl ImplType;

  explicit SurfacePropertyOtherSideConditionsModel(std::shared_ptr<detail::SurfacePropertyOtherSideConditionsModel_Impl> impl);

  friend class detail::SurfacePropertyOtherSideConditionsModel_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SurfacePropertyOtherSideConditionsModel");
};

/** \relates SurfacePropertyOtherSideConditionsModel*/
typedef boost::optional<SurfacePropertyOtherSideConditionsModel> OptionalSurfacePropertyOtherSideConditionsModel;

/** \relates SurfacePropertyOtherSideConditionsModel*/
typedef std::vector<SurfacePropertyOtherSideConditionsModel> SurfacePropertyOtherSideConditionsModelVector;

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_HPP

