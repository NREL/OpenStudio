/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_MODELOBJECTLIST_IMPL_HPP
#define MODEL_MODELOBJECTLIST_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>
#include <utilities/idf/WorkspaceExtensibleGroup.hpp>

namespace openstudio {
namespace model {

namespace detail {

  /** ModelObjectList_Impl is a ModelObject_Impl that is the implementation class for ModelObjectList.*/
  class MODEL_API ModelObjectList_Impl : public ModelObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ModelObjectList_Impl(const IdfObject& idfObject,
                         Model_Impl* model,
                         bool keepHandle);

    ModelObjectList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    ModelObjectList_Impl(const ModelObjectList_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    virtual ~ModelObjectList_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    std::vector<IdfObject> remove();

    ModelObject clone(Model model) const;

    bool addModelObject(const ModelObject & modelObject );

    void removeModelObject(const ModelObject & modelObject );

    void removeAllModelObjects();

    std::vector< ModelObject > modelObjects() const;

    WorkspaceExtensibleGroup getGroupForModelObject(const ModelObject& modelObject);

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ModelObjectList");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_MODELOBJECTLIST_IMPL_HPP

