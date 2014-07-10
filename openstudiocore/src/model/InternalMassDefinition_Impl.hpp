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

#ifndef MODEL_INTERNALMASSDEFINITION_IMPL_HPP
#define MODEL_INTERNALMASSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;
class InternalMassDefinition;

namespace detail {

  /** InternalMassDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for InternalMassDefinition.*/
  class MODEL_API InternalMassDefinition_Impl : public SpaceLoadDefinition_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string designLevelCalculationMethod READ designLevelCalculationMethod);

    Q_PROPERTY(boost::optional<double> surfaceArea READ surfaceArea WRITE setSurfaceArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceArea_SI 
               READ surfaceArea_SI 
               WRITE setSurfaceArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceArea_IP 
               READ surfaceArea_IP 
               WRITE setSurfaceArea);

    Q_PROPERTY(boost::optional<double> surfaceAreaperSpaceFloorArea 
               READ surfaceAreaperSpaceFloorArea 
               WRITE setSurfaceAreaperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperSpaceFloorArea_SI 
               READ surfaceAreaperSpaceFloorArea_SI 
               WRITE setSurfaceAreaperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperSpaceFloorArea_IP 
               READ surfaceAreaperSpaceFloorArea_IP 
               WRITE setSurfaceAreaperSpaceFloorArea);

    Q_PROPERTY(boost::optional<double> surfaceAreaperPerson 
               READ surfaceAreaperPerson 
               WRITE setSurfaceAreaperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperPerson_SI 
               READ surfaceAreaperPerson_SI 
               WRITE setSurfaceAreaperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperPerson_IP 
               READ surfaceAreaperPerson_IP 
               WRITE setSurfaceAreaperPerson);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> construction 
               READ constructionAsModelObject 
               WRITE setConstructionAsModelObject 
               RESET resetConstruction);

 
//    Q_PROPERTY(boost::optional<double> supplyAirFlowRateDuringCoolingOperation READ supplyAirFlowRateDuringCoolingOperation WRITE setSupplyAirFlowRateDuringCoolingOperation);
//    Q_PROPERTY(openstudio::OSOptionalQuantity supplyAirFlowRateDuringCoolingOperation_SI READ supplyAirFlowRateDuringCoolingOperation_SI WRITE setSupplyAirFlowRateDuringCoolingOperation);
//    Q_PROPERTY(openstudio::OSOptionalQuantity supplyAirFlowRateDuringCoolingOperation_IP READ supplyAirFlowRateDuringCoolingOperation_IP WRITE setSupplyAirFlowRateDuringCoolingOperation);
//    Q_PROPERTY(bool isSupplyAirFlowRateDuringCoolingOperationAutosized READ isSupplyAirFlowRateDuringCoolingOperationAutosized);


   public:
    /** @name Constructors and Destructors */
    //@{

    InternalMassDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    InternalMassDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    InternalMassDefinition_Impl(const InternalMassDefinition_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~InternalMassDefinition_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    // Attributes

    ATTRIBUTE_DEFINITION(1,0,0,surfaceArea,SurfaceArea)
    ATTRIBUTE_DEFINITION(1,0,0,surfaceAreaperSpaceFloorArea,SurfaceAreaperSpaceFloorArea)
    ATTRIBUTE_DEFINITION(1,0,0,surfaceAreaperPerson,SurfaceAreaperPerson)

    //@}
    /** @name Getters */
    //@{

    /// Returns the construction object, if this object does not directly specify construction the hierarchy is searched.
    boost::optional<ConstructionBase> construction() const;

    /// Returns true if the construction is not directly referenced by this surface .
    bool isConstructionDefaulted() const;

    std::string designLevelCalculationMethod() const;

    //boost::optional<double> surfaceArea() const;

    //boost::optional<double> surfaceAreaperSpaceFloorArea() const;

    //boost::optional<double> surfaceAreaperPerson() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the construction object.
    bool setConstruction(const ConstructionBase& construction);

    /// Resets the construction object.
    void resetConstruction();

    /// Sets designLevelCalculationMethod to 'SurfaceArea', clears surfaceAreaperSpaceFloorArea
    /// and surfaceAreaperPerson, and sets surfaceArea.
    //bool setSurfaceArea(boost::optional<double> surfaceArea);

    /// Sets designLevelCalculationMethod to 'SurfaceArea/Area', clears surfaceArea
    /// and surfaceAreaperPerson, and sets surfaceAreaperSpaceFloorArea.
    //bool setSurfaceAreaperSpaceFloorArea(boost::optional<double> surfaceAreaperSpaceFloorArea);

    /// Sets designLevelCalculationMethod to 'SurfaceArea/Person', clears surfaceArea
    /// and surfaceAreaperSpaceFloorArea, and sets surfaceAreaperPerson.
    //bool setSurfaceAreaperPerson(boost::optional<double> surfaceAreaperPerson);

    //@}
    /** @name Other */
    //@{

    /** Returns the construction surface area represented by this definition, assuming floorArea 
     *  (m^2) and numPeople. */
    double getSurfaceArea(double floorArea, double numPeople) const;

    /** Returns the m^2/m^2 of construction surface area per floor area represented by this 
     *  definition, assuming floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the m^2/person of construction surface area represented by this definition, assuming 
     *  floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

    /** If method is a \link validDesignLevelCalculationMethodValues valid design level calculation 
     *  method \endlink, changes this definition to an equivalent construction surface area, under 
     *  the assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method,
                                         double floorArea,
                                         double numPeople);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.InternalMassDefinition");

    boost::optional<ModelObject> constructionAsModelObject() const;

    bool setConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INTERNALMASSDEFINITION_IMPL_HPP

