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

#ifndef MODEL_SCHEDULE_IMPL_HPP
#define MODEL_SCHEDULE_IMPL_HPP 

#include "ScheduleBase_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

class ScheduleTypeLimits;

namespace detail {
    
  // derive Schedule_Impl from ResourceObject_Impl to override virtual methods
  class MODEL_API Schedule_Impl : public ScheduleBase_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    Schedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    Schedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                  Model_Impl* model, 
                  bool keepHandle);

    // clone copy constructor
    Schedule_Impl(const Schedule_Impl& other, Model_Impl* model,bool keepHandles);

    // virtual destructor
    virtual ~Schedule_Impl(){}

    //@}
   protected:
    virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const;

    virtual bool okToResetScheduleTypeLimits() const;
   private:
    REGISTER_LOGGER("openstudio.model.Schedule");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULE_IMPL_HPP
