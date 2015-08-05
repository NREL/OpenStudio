/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONLOADSCHEME_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONLOADSCHEME_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationScheme.hpp"

namespace openstudio {
namespace model {

class HVACComponent;

namespace detail{
  class PlantEquipmentOperationLoadScheme_Impl;
}

/** PlantEquipmentOperationLoadScheme instances 
 * will be created with a single continuous load range between zero and a very large upper bound. 
 * This continuum can be subdivided to create multiple discrete load ranges with different equipment assignments. 
 * Load ranges are identified by the value of their upper limit. A load range is allowed to be empty.
 */
class MODEL_API PlantEquipmentOperationLoadScheme : public PlantEquipmentOperationScheme {

  public:

  PlantEquipmentOperationLoadScheme(IddObjectType type,const Model& model);

  virtual ~PlantEquipmentOperationLoadScheme() {}

  friend class openstudio::IdfObject;

  /** Add a new load range, using the adjacent range as the lower limit, and "upperLimit" as the upper limit of the new range. 
    * Returns true if the load range was succesfully added. Duplicates wil be removed from the equipment vector **/
  bool addLoadRange(double upperLimit, const std::vector<HVACComponent> & equipment);

  /** Remove an existing load range identified by its upper limit. 
    * Every operation scheme in OpenStudio must span the entire continuum between 0 and a predefined upper limit,
    * therefore it is not possible to remove the largest upper limit. Return the hvac equipment that was on the range. 
    * The next "higher" load range fills in the place in the load continum that the removed range occupied. */
  std::vector<HVACComponent> removeLoadRange(double upperLimit);

  /** Return a vector of all of the load range upper limits. 
    * A default constructed load range scheme, will return a vector with one element. */
  std::vector<double> loadRangeUpperLimits() const;

  /** Return a vector of the equipment at a given load range, where the load range is identified by the upper limit. */
  std::vector<HVACComponent> equipment(double upperLimit) const;

  /** Add an HVACComponent instance to an existing load range. */
  bool addEquipment(double upperLimit, const HVACComponent & equipment);

  /** Add an HVACComponent instance to the highest load range.
    * This method is useful to add equipment to the single default load range
    * after a load operation scheme is default constructed. */
  bool addEquipment(const HVACComponent & equipment);

  /** Remove an HVACComponent instance from an existing load range. */
  bool removeEquipment(double upperLimit, const HVACComponent & equipment);

  /** Remove all load ranges except the default range. All equipment will be removed. */
  void clearLoadRanges();

  /// @cond 

  typedef detail::PlantEquipmentOperationLoadScheme_Impl ImplType;

  explicit PlantEquipmentOperationLoadScheme(std::shared_ptr<detail::PlantEquipmentOperationLoadScheme_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationLoadScheme");

  /// @endcond 

};

typedef boost::optional<PlantEquipmentOperationLoadScheme> OptionalPlantEquipmentOperationLoadScheme;

typedef std::vector<PlantEquipmentOperationLoadScheme> PlantEquipmentOperationLoadSchemeVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONLOADSCHEME_HPP

