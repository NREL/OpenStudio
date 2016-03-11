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

#ifndef MODEL_INVERTER_IMPL_HPP
#define MODEL_INVERTER_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API Inverter_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Inverter_Impl(IddObjectType type, Model_Impl* model);

    Inverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Inverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    Inverter_Impl(const Inverter_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~Inverter_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{


    //@}
    /** @name Getters */
    //@{

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    virtual boost::optional<ThermalZone> thermalZone() const = 0;

    //@}
    /** @name Setters */
    //@{

    virtual bool setThermalZone(const ThermalZone& thermalZone) = 0;

    virtual void resetThermalZone() = 0;

    //@}
    /** @name Other */
    //@{
 
    //@}
   private:

    REGISTER_LOGGER("openstudio.model.Inverter");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INVERTER_IMPL_HPP
