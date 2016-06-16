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

#ifndef MODEL_ELECTRICALSTORAGE_IMPL_HPP
#define MODEL_ELECTRICALSTORAGE_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** ElectricStorage_Impl is a ParentObject_Impl that is the implementation class for ElectricStorage_Impl.
  It is the Base Class of both ElectricLoadCenterStorageSimple and ElectricLoadCenterStorageBattery*/
  class MODEL_API ElectricalStorage_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricalStorage_Impl(IddObjectType type, Model_Impl* model);

    ElectricalStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ElectricalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    ElectricalStorage_Impl(const ElectricalStorage_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~ElectricalStorage_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{


    //@}
    /** @name Getters */
    //@{

    // Convenience method to return the (optional) electricalLoadCenter on which it's assigned
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

    REGISTER_LOGGER("openstudio.model.ElectricalStorage");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICALSTORAGE_IMPL_HPP
