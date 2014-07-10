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

#ifndef MODEL_GASEQUIPMENTDEFINITION_IMPL_HPP
#define MODEL_GASEQUIPMENTDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {

class GasEquipmentDefinition;

namespace detail {

  /** GasEquipmentDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for GasEquipmentDefinition.*/
  class MODEL_API GasEquipmentDefinition_Impl : public SpaceLoadDefinition_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string designLevelCalculationMethod READ designLevelCalculationMethod);

    Q_PROPERTY(boost::optional<double> designLevel READ designLevel WRITE setDesignLevel);
    Q_PROPERTY(openstudio::OSOptionalQuantity designLevel_SI 
               READ designLevel_SI 
               WRITE setDesignLevel);
    Q_PROPERTY(openstudio::OSOptionalQuantity designLevel_IP 
               READ designLevel_IP 
               WRITE setDesignLevel);

    Q_PROPERTY(boost::optional<double> wattsperSpaceFloorArea READ wattsperSpaceFloorArea WRITE setWattsperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperSpaceFloorArea_SI 
               READ wattsperSpaceFloorArea_SI 
               WRITE setWattsperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperSpaceFloorArea_IP 
               READ wattsperSpaceFloorArea_IP 
               WRITE setWattsperSpaceFloorArea);

    Q_PROPERTY(boost::optional<double> wattsperPerson READ wattsperPerson WRITE setWattsperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperPerson_SI 
               READ wattsperPerson_SI 
               WRITE setWattsperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperPerson_IP 
               READ wattsperPerson_IP 
               WRITE setWattsperPerson);

    Q_PROPERTY(double fractionRadiant READ fractionRadiant WRITE setFractionRadiant RESET resetFractionRadiant);
    Q_PROPERTY(bool isFractionRadiantDefaulted READ isFractionRadiantDefaulted);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_SI 
               READ fractionRadiant_SI 
               WRITE setFractionRadiant);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_IP 
               READ fractionRadiant_IP 
               WRITE setFractionRadiant);

    Q_PROPERTY(double fractionLatent READ fractionLatent WRITE setFractionLatent RESET resetFractionLatent);
    Q_PROPERTY(bool isFractionLatentDefaulted READ isFractionLatentDefaulted);
    Q_PROPERTY(openstudio::Quantity fractionLatent_SI 
               READ fractionLatent_SI 
               WRITE setFractionLatent);
    Q_PROPERTY(openstudio::Quantity fractionLatent_IP 
               READ fractionLatent_IP 
               WRITE setFractionLatent);

    Q_PROPERTY(double fractionLost READ fractionLost WRITE setFractionLost RESET resetFractionLost);
    Q_PROPERTY(bool isFractionLostDefaulted READ isFractionLostDefaulted);
    Q_PROPERTY(openstudio::Quantity fractionLost_SI 
               READ fractionLost_SI 
               WRITE setFractionLost);
    Q_PROPERTY(openstudio::Quantity fractionLost_IP 
               READ fractionLost_IP 
               WRITE setFractionLost);

    Q_PROPERTY(double carbonDioxideGenerationRate 
               READ carbonDioxideGenerationRate 
               WRITE setCarbonDioxideGenerationRate 
               RESET resetCarbonDioxideGenerationRate);
    Q_PROPERTY(bool isCarbonDioxideGenerationRateDefaulted READ isCarbonDioxideGenerationRateDefaulted);
    Q_PROPERTY(openstudio::Quantity carbonDioxideGenerationRate_SI 
               READ carbonDioxideGenerationRate_SI 
               WRITE setFractionLost);
    Q_PROPERTY(openstudio::Quantity carbonDioxideGenerationRate_IP 
               READ carbonDioxideGenerationRate_IP 
               WRITE setCarbonDioxideGenerationRate);

   public:
    /** @name Constructors and Destructors */
    //@{

    GasEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    GasEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    GasEquipmentDefinition_Impl(const GasEquipmentDefinition_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~GasEquipmentDefinition_Impl() {}

    //@}
    
    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    // Attributes

    ATTRIBUTE_DEFINITION(1,0,0,designLevel,DesignLevel)
    ATTRIBUTE_DEFINITION(1,0,0,wattsperPerson,WattsperPerson)
    ATTRIBUTE_DEFINITION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea)
    ATTRIBUTE_DEFINITION(0,1,0,fractionRadiant,FractionRadiant)
    ATTRIBUTE_DEFINITION(0,1,0,fractionLatent,FractionLatent)
    ATTRIBUTE_DEFINITION(0,1,0,fractionLost,FractionLost)
    ATTRIBUTE_DEFINITION(0,1,0,carbonDioxideGenerationRate,CarbonDioxideGenerationRate)

    /** @name Getters */
    //@{

    std::string designLevelCalculationMethod() const;

    //boost::optional<double> designLevel() const;

    //boost::optional<double> wattsperSpaceFloorArea() const;

    //boost::optional<double> wattsperPerson() const;

    //double fractionLatent() const;

    //bool isFractionLatentDefaulted() const;

    //double fractionRadiant() const;

    //bool isFractionRadiantDefaulted() const;

    //double fractionLost() const;

    //bool isFractionLostDefaulted() const;

    //double carbonDioxideGenerationRate() const;

    //bool isCarbonDioxideGenerationRateDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    //bool setDesignLevel(boost::optional<double> designLevel);

    //bool setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea);

    //bool setWattsperPerson(boost::optional<double> wattsperPerson);

    //bool setFractionLatent(double fractionLatent);

    //void resetFractionLatent();

    //bool setFractionRadiant(double fractionRadiant);

    //void resetFractionRadiant();

    //bool setFractionLost(double fractionLost);

    //void resetFractionLost();

    //bool setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate);

    //void resetCarbonDioxideGenerationRate();

    //@}
    /** @name Other */
    //@{

    /** Returns the design level represented by this definition, assuming floorArea (m^2) and 
     *  numPeople. */
    double getDesignLevel(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and 
     *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and 
     *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;
   
    /** If method is a \link validDesignLevelCalculationMethodValues valid design level 
     *  calculation method \endlink, changes this definition to an equivalent power level, under 
     *  the assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method, 
                                         double floorArea, 
                                         double numPeople);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GasEquipmentDefinition");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GASEQUIPMENTDEFINITION_IMPL_HPP

