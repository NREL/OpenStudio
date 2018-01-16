/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_ELECTRICEQUIPMENTDEFINITION_IMPL_HPP
#define MODEL_ELECTRICEQUIPMENTDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {

class ElectricEquipmentDefinition;

namespace detail {

  /** ElectricEquipmentDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for ElectricEquipmentDefinition.*/
  class MODEL_API ElectricEquipmentDefinition_Impl : public SpaceLoadDefinition_Impl {


    Q_PROPERTY(openstudio::OSOptionalQuantity designLevel_SI
               READ designLevel_SI
               WRITE setDesignLevel);
    Q_PROPERTY(openstudio::OSOptionalQuantity designLevel_IP
               READ designLevel_IP
               WRITE setDesignLevel);


    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperSpaceFloorArea_SI
               READ wattsperSpaceFloorArea_SI
               WRITE setWattsperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperSpaceFloorArea_IP
               READ wattsperSpaceFloorArea_IP
               WRITE setWattsperSpaceFloorArea);


    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperPerson_SI
               READ wattsperPerson_SI
               WRITE setWattsperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity wattsperPerson_IP
               READ wattsperPerson_IP
               WRITE setWattsperPerson);


    Q_PROPERTY(openstudio::Quantity fractionRadiant_SI
               READ fractionRadiant_SI
               WRITE setFractionRadiant);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_IP
               READ fractionRadiant_IP
               WRITE setFractionRadiant);


    Q_PROPERTY(openstudio::Quantity fractionLatent_SI
               READ fractionLatent_SI
               WRITE setFractionLatent);
    Q_PROPERTY(openstudio::Quantity fractionLatent_IP
               READ fractionLatent_IP
               WRITE setFractionLatent);


    Q_PROPERTY(openstudio::Quantity fractionLost_SI
               READ fractionLost_SI
               WRITE setFractionLost);
    Q_PROPERTY(openstudio::Quantity fractionLost_IP
               READ fractionLost_IP
               WRITE setFractionLost);
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ElectricEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    ElectricEquipmentDefinition_Impl(const ElectricEquipmentDefinition_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~ElectricEquipmentDefinition_Impl() {}

    //@}

    // TODO: Remove
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    // Attributes

    ATTRIBUTE_DEFINITION(1,0,0,designLevel,DesignLevel)
    ATTRIBUTE_DEFINITION(1,0,0,wattsperPerson,WattsperPerson)
    ATTRIBUTE_DEFINITION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea)
    ATTRIBUTE_DEFINITION(0,1,0,fractionRadiant,FractionRadiant)
    ATTRIBUTE_DEFINITION(0,1,0,fractionLatent,FractionLatent)
    ATTRIBUTE_DEFINITION(0,1,0,fractionLost,FractionLost)

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
    REGISTER_LOGGER("openstudio.model.ElectricEquipmentDefinition");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICEQUIPMENTDEFINITION_IMPL_HPP

