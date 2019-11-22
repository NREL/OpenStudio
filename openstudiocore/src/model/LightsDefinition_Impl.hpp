/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_LIGHTSDEFINITION_IMPL_HPP
#define MODEL_LIGHTSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {

class LightsDefinition;

namespace detail {

  /** LightsDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for LightsDefinition.*/
  class MODEL_API LightsDefinition_Impl : public SpaceLoadDefinition_Impl {


    Q_PROPERTY(openstudio::OSOptionalQuantity lightingLevel_SI
               READ lightingLevel_SI
               WRITE setLightingLevel);
    Q_PROPERTY(openstudio::OSOptionalQuantity lightingLevel_IP
               READ lightingLevel_IP
               WRITE setLightingLevel);


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


    Q_PROPERTY(openstudio::Quantity fractionVisible_SI
               READ fractionVisible_SI
               WRITE setFractionVisible);
    Q_PROPERTY(openstudio::Quantity fractionVisible_IP
               READ fractionVisible_IP
               WRITE setFractionVisible);


    Q_PROPERTY(openstudio::Quantity returnAirFraction_SI
               READ returnAirFraction_SI
               WRITE setReturnAirFraction);
    Q_PROPERTY(openstudio::Quantity returnAirFraction_IP
               READ returnAirFraction_IP
               WRITE setReturnAirFraction);


   public:
    /** @name Constructors and Destructors */
    //@{

    LightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    LightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    LightsDefinition_Impl(const LightsDefinition_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~LightsDefinition_Impl() {}

    // Attributes

    ATTRIBUTE_DEFINITION(1,0,0,lightingLevel,LightingLevel)
    ATTRIBUTE_DEFINITION(1,0,0,wattsperPerson,WattsperPerson)
    ATTRIBUTE_DEFINITION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea)
    ATTRIBUTE_DEFINITION(0,1,0,fractionRadiant,FractionRadiant)
    ATTRIBUTE_DEFINITION(0,1,0,fractionVisible,FractionVisible)
    ATTRIBUTE_DEFINITION(0,1,0,returnAirFraction,ReturnAirFraction)

    //@}
    /** @name Virtual Methods */
    //@{

    // TODO: remove (unused)
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    std::string designLevelCalculationMethod() const;

    //boost::optional<double> lightingLevel() const;

    //boost::optional<double> wattsperSpaceFloorArea() const;

    //boost::optional<double> wattsperPerson() const;

    //double fractionRadiant() const;

    //bool isFractionRadiantDefaulted() const;

    //double fractionVisible() const;

    //bool isFractionVisibleDefaulted() const;

    //double returnAirFraction() const;

    //bool isReturnAirFractionDefaulted() const;

    bool returnAirFractionCalculatedfromPlenumTemperature() const;

    bool isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const;

    double returnAirFractionFunctionofPlenumTemperatureCoefficient1() const;

    bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const;

    double returnAirFractionFunctionofPlenumTemperatureCoefficient2() const;

    bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const;

    //@}
    /** @name Setters */
    //@{

    //bool setLightingLevel(boost::optional<double> lightingLevel);

    //bool setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea);

    //bool setWattsperPerson(boost::optional<double> wattsperPerson);

    //bool setFractionRadiant(double fractionRadiant);

    //void resetFractionRadiant();

    //bool setFractionVisible(double fractionVisible);

    //void resetFractionVisible();

    //bool setReturnAirFraction(double returnAirFraction);

    //void resetReturnAirFraction();

    bool setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature);

    void resetReturnAirFractionCalculatedfromPlenumTemperature();

    bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1);

    void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();

    bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2);

    void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();

    //@}
    /** @name Other */
    //@{

    /** Returns the lighting level represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
    double getLightingPower(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    /** If method is a \link validDesignLevelCalculationMethodValues valid design level
     *  calculation method \endlink, changes this definition to an equivalent lighting level, under
     *  the assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method,
                                         double floorArea,
                                         double numPeople);

    virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

    virtual std::vector<std::string> emsInternalVariableNames() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.LightsDefinition");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LIGHTSDEFINITION_IMPL_HPP
