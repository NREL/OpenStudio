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

#ifndef MODEL_LUMINAIREDEFINITION_IMPL_HPP
#define MODEL_LUMINAIREDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {

class LuminaireDefinition;

namespace detail {

  /** LuminaireDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for LuminaireDefinition.*/
  class MODEL_API LuminaireDefinition_Impl : public SpaceLoadDefinition_Impl {
    Q_OBJECT;

    Q_PROPERTY(double lightingPower READ lightingPower WRITE setLightingPower RESET resetLightingPower);
    Q_PROPERTY(bool isLightingPowerDefaulted READ isLightingPowerDefaulted);
    Q_PROPERTY(openstudio::Quantity lightingPower_SI 
               READ lightingPower_SI 
               WRITE setLightingPower);
    Q_PROPERTY(openstudio::Quantity lightingPower_IP 
               READ lightingPower_IP 
               WRITE setLightingPower);

    Q_PROPERTY(double fractionRadiant READ fractionRadiant WRITE setFractionRadiant RESET resetFractionRadiant);
    Q_PROPERTY(bool isFractionRadiantDefaulted READ isFractionRadiantDefaulted);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_SI 
               READ fractionRadiant_SI 
               WRITE setFractionRadiant);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_IP 
               READ fractionRadiant_IP 
               WRITE setFractionRadiant);

    Q_PROPERTY(double fractionVisible READ fractionVisible WRITE setFractionVisible RESET resetFractionVisible);
    Q_PROPERTY(bool isFractionVisibleDefaulted READ isFractionVisibleDefaulted);
    Q_PROPERTY(openstudio::Quantity fractionVisible_SI 
               READ fractionVisible_SI 
               WRITE setFractionVisible);
    Q_PROPERTY(openstudio::Quantity fractionVisible_IP 
               READ fractionVisible_IP 
               WRITE setFractionVisible);

    Q_PROPERTY(double returnAirFraction READ returnAirFraction WRITE setReturnAirFraction RESET resetReturnAirFraction);
    Q_PROPERTY(bool isReturnAirFractionDefaulted READ isReturnAirFractionDefaulted);
    Q_PROPERTY(openstudio::Quantity returnAirFraction_SI 
               READ returnAirFraction_SI 
               WRITE setReturnAirFraction);
    Q_PROPERTY(openstudio::Quantity returnAirFraction_IP 
               READ returnAirFraction_IP 
               WRITE setReturnAirFraction);

    Q_PROPERTY(bool returnAirFractionCalculatedfromPlenumTemperature READ returnAirFractionCalculatedfromPlenumTemperature WRITE setReturnAirFractionCalculatedfromPlenumTemperature RESET resetReturnAirFractionCalculatedfromPlenumTemperature);
    Q_PROPERTY(bool isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted READ isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted);
    Q_PROPERTY(double returnAirFractionFunctionofPlenumTemperatureCoefficient1 READ returnAirFractionFunctionofPlenumTemperatureCoefficient1 WRITE setReturnAirFractionFunctionofPlenumTemperatureCoefficient1 RESET resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1);
    Q_PROPERTY(bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted READ isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted);
    Q_PROPERTY(double returnAirFractionFunctionofPlenumTemperatureCoefficient2 READ returnAirFractionFunctionofPlenumTemperatureCoefficient2 WRITE setReturnAirFractionFunctionofPlenumTemperatureCoefficient2 RESET resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2);
    Q_PROPERTY(bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted READ isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted);
   public:
    /** @name Constructors and Destructors */
    //@{

    LuminaireDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    LuminaireDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    LuminaireDefinition_Impl(const LuminaireDefinition_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~LuminaireDefinition_Impl() {}

    //@}

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    // Attributes

    ATTRIBUTE_DEFINITION(0,1,0,lightingPower,LightingPower)
    ATTRIBUTE_DEFINITION(0,1,0,fractionRadiant,FractionRadiant)
    ATTRIBUTE_DEFINITION(0,1,0,fractionVisible,FractionVisible)
    ATTRIBUTE_DEFINITION(0,1,0,returnAirFraction,ReturnAirFraction)

    /** @name Getters */
    //@{

    // TODO: Handle Non-Extensible IddField IES File Path.

    //double lightingPower() const;

    //bool isLightingPowerDefaulted() const;

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

    // TODO: Handle Non-Extensible IddField IES File Path.

    //bool setLightingPower(double lightingPower);

    //void resetLightingPower();

    //bool setFractionRadiant(double fractionRadiant);

    //void resetFractionRadiant();

    //bool setFractionVisible(double fractionVisible);

    //void resetFractionVisible();

    //bool setReturnAirFraction(double returnAirFraction);

    //void resetReturnAirFraction();

    void setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature);

    void resetReturnAirFractionCalculatedfromPlenumTemperature();

    bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1);

    void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();

    bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2);

    void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.LuminaireDefinition");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LUMINAIREDEFINITION_IMPL_HPP

