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

#ifndef MODEL_MODELOBJECTLIST_HPP
#define MODEL_MODELOBJECTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ModelObjectList_Impl;

} // detail

/** ModelObjectList is a ModelObject that wraps the OpenStudio IDD object 'OS:ModelObjectList'. */
class MODEL_API ModelObjectList : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ModelObjectList(const Model& model);

  virtual ~ModelObjectList() {}

  //@}

  static IddObjectType iddObjectType();

  std::vector<IdfObject> remove();

  ModelObject clone(Model model) const;

  bool addModelObject(const ModelObject & modelObject );

  void removeModelObject(const ModelObject & modelObject );

  void removeAllModelObjects();

  std::vector< ModelObject > modelObjects() const;

 protected:
  /// @cond
  typedef detail::ModelObjectList_Impl ImplType;

  explicit ModelObjectList(std::shared_ptr<detail::ModelObjectList_Impl> impl);

  friend class detail::ModelObjectList_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ModelObjectList");
};

/** \relates ModelObjectList*/
typedef boost::optional<ModelObjectList> OptionalModelObjectList;

/** \relates ModelObjectList*/
typedef std::vector<ModelObjectList> ModelObjectListVector;

} // model
} // openstudio

#endif // MODEL_MODELOBJECTLIST_HPP

