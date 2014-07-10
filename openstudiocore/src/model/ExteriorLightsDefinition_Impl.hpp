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

#ifndef MODEL_EXTERIORLIGHTSDEFINITION_IMPL_HPP
#define MODEL_EXTERIORLIGHTSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** ExteriorLightsDefinition_Impl is a ResourceObject_Impl that is the implementation class for ExteriorLightsDefinition.*/
  class MODEL_API ExteriorLightsDefinition_Impl : public ResourceObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(double designLevel READ designLevel WRITE setDesignLevel);
   public:
    /** @name Constructors and Destructors */
    //@{

    ExteriorLightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ExteriorLightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    ExteriorLightsDefinition_Impl(const ExteriorLightsDefinition_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    virtual ~ExteriorLightsDefinition_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double designLevel() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignLevel(double designLevel);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLightsDefinition");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EXTERIORLIGHTSDEFINITION_IMPL_HPP

