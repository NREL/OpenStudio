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

#ifndef MODEL_CONNECTION_IMPL_HPP
#define MODEL_CONNECTION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API Connection_Impl : public ModelObject_Impl {
   public:

    Connection_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Connection_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                    Model_Impl* model, 
                    bool keepHandle);

    Connection_Impl(const Connection_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~Connection_Impl();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    boost::optional<ModelObject> sourceObject();

    boost::optional<unsigned> sourceObjectPort();

    boost::optional<ModelObject> targetObject();

    boost::optional<unsigned> targetObjectPort();

    void setSourceObject(ModelObject object);

    void setSourceObjectPort(unsigned port);

    void setTargetObject(ModelObject object);

    void setTargetObjectPort(unsigned port);

   private:

    REGISTER_LOGGER("openstudio.model.Connection");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CONNECTION_IMPL_HPP

