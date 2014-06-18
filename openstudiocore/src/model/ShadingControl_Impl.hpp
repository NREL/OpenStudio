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

#ifndef MODEL_SHADINGCONTROL_IMPL_HPP
#define MODEL_SHADINGCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

class Construction;
class ShadingMaterial;
class Schedule;

namespace detail {

  /** ShadingControl_Impl is a ResourceObject_Impl that is the implementation class for ShadingControl.*/
  class MODEL_API ShadingControl_Impl : public ResourceObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ShadingControl_Impl(const IdfObject& idfObject,
                        Model_Impl* model,
                        bool keepHandle);

    ShadingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    ShadingControl_Impl(const ShadingControl_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~ShadingControl_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;
    
    //@}
    /** @name Getters */
    //@{

    boost::optional<Construction> construction() const;

    boost::optional<ShadingMaterial> shadingMaterial() const;

    std::string shadingType() const;
    
    std::string shadingControlType() const;

    boost::optional<Schedule> schedule() const;

    //@}
    /** @name Setters */
    //@{
    
    bool setShadingType(const std::string& shadingType);
    
    bool setShadingControlType(const std::string& shadingControlType);
    
    bool setSchedule(const Schedule& schedule);
    
    void resetSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   
   private:
   
    REGISTER_LOGGER("openstudio.model.ShadingControl");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_SHADINGCONTROL_IMPL_HPP

