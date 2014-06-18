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

#ifndef MODEL_WEATHERFILEDAYS_IMPL_HPP
#define MODEL_WEATHERFILEDAYS_IMPL_HPP

#include "SizingPeriod_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API WeatherFileDays_Impl : public SizingPeriod_Impl {
   public:

    // constructor
    WeatherFileDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    WeatherFileDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                   Model_Impl* model, 
                   bool keepHandle);

    // clone copy constructor
    WeatherFileDays_Impl(const WeatherFileDays_Impl& other,Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~WeatherFileDays_Impl(){}

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

   private:

    REGISTER_LOGGER("openstudio.model.WeatherFileDays");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_WEATHERFILEDAYS_IMPL_HPP
