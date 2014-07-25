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

#ifndef MODEL_PIPEADIABATIC_IMPL_HPP
#define MODEL_PIPEADIABATIC_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class MODEL_API PipeAdiabatic_Impl : public StraightComponent_Impl {
  public:

    // constructor
    PipeAdiabatic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    PipeAdiabatic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    // copy constructor
    PipeAdiabatic_Impl(const PipeAdiabatic_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    // virtual destructor
    virtual ~PipeAdiabatic_Impl();

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    unsigned inletPort();

    unsigned outletPort();

    bool addToNode(Node & node);

  private:

    REGISTER_LOGGER("openstudio.model.PipeAdiabatic");
  };

} // detail

} // model

} // openstudio

#endif // MODEL_PIPEADIABATIC_IMPL_HPP

