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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_IMPL_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_IMPL_HPP

#include "PlantEquipmentOperationScheme_Impl.hpp"

namespace openstudio {
namespace model {

class PlantLoop;

namespace detail {

  class MODEL_API PlantEquipmentOperationRangeBasedScheme_Impl : public PlantEquipmentOperationScheme_Impl {
   public:

    PlantEquipmentOperationRangeBasedScheme_Impl(IddObjectType type, Model_Impl* model);

    PlantEquipmentOperationRangeBasedScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    PlantEquipmentOperationRangeBasedScheme_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    PlantEquipmentOperationRangeBasedScheme_Impl(const PlantEquipmentOperationRangeBasedScheme_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~PlantEquipmentOperationRangeBasedScheme_Impl() {}

    bool addLoadRange(double upperLimit, const std::vector<HVACComponent> & equipment);

    std::vector<HVACComponent> removeLoadRange(double upperLimit);

    std::vector<double> loadRangeUpperLimits() const;

    std::vector<HVACComponent> equipment(double upperLimit) const;

    bool addEquipment(double upperLimit, const HVACComponent & equipment);

    bool addEquipment(const HVACComponent & equipment);

    bool removeEquipment(double upperLimit, const HVACComponent & equipment);

    void clearLoadRanges();

    virtual double maximumUpperLimit() const = 0;

    virtual double minimumLowerLimit() const = 0;

   private:

    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationRangeBasedScheme");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONRANGEBASEDSCHEME_IMPL_HPP
