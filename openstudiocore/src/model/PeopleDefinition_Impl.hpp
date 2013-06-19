/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_PEOPLEDEFINITION_IMPL_HPP
#define MODEL_PEOPLEDEFINITION_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/SpaceLoadDefinition_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {
namespace model {

class PeopleDefinition;

namespace detail {

  /** PeopleDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for PeopleDefinition.*/
  class MODEL_API PeopleDefinition_Impl : public SpaceLoadDefinition_Impl {
    Q_OBJECT;
    
    Q_PROPERTY(std::string numberofPeopleCalculationMethod READ numberofPeopleCalculationMethod);
    Q_PROPERTY(std::vector<std::string> numberofPeopleCalculationMethodValues READ numberofPeopleCalculationMethodValues);
    
    Q_PROPERTY(boost::optional<double> numberofPeople READ numberofPeople WRITE setNumberofPeople);
    Q_PROPERTY(openstudio::OSOptionalQuantity numberofPeople_SI READ numberofPeople_SI WRITE setNumberofPeople);
    Q_PROPERTY(openstudio::OSOptionalQuantity numberofPeople_IP READ numberofPeople_IP WRITE setNumberofPeople);
    
    Q_PROPERTY(boost::optional<double> peopleperSpaceFloorArea READ peopleperSpaceFloorArea WRITE setPeopleperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity peopleperSpaceFloorArea_SI READ peopleperSpaceFloorArea_SI WRITE setPeopleperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity peopleperSpaceFloorArea_IP READ peopleperSpaceFloorArea_IP WRITE setPeopleperSpaceFloorArea);    
    
    Q_PROPERTY(boost::optional<double> spaceFloorAreaperPerson READ spaceFloorAreaperPerson WRITE setSpaceFloorAreaperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity spaceFloorAreaperPerson_SI READ spaceFloorAreaperPerson_SI WRITE setSpaceFloorAreaperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity spaceFloorAreaperPerson_IP READ spaceFloorAreaperPerson_IP WRITE setSpaceFloorAreaperPerson);    
    
    Q_PROPERTY(double fractionRadiant READ fractionRadiant WRITE setFractionRadiant);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_SI READ fractionRadiant_SI WRITE setFractionRadiant);
    Q_PROPERTY(openstudio::Quantity fractionRadiant_IP READ fractionRadiant_IP WRITE setFractionRadiant);    
    
    Q_PROPERTY(boost::optional<double> sensibleHeatFraction READ sensibleHeatFraction WRITE setSensibleHeatFraction RESET resetSensibleHeatFraction);
    Q_PROPERTY(openstudio::OSOptionalQuantity sensibleHeatFraction_SI READ sensibleHeatFraction_SI WRITE setSensibleHeatFraction RESET resetSensibleHeatFraction);
    Q_PROPERTY(openstudio::OSOptionalQuantity sensibleHeatFraction_IP READ sensibleHeatFraction_IP WRITE setSensibleHeatFraction RESET resetSensibleHeatFraction);    
    Q_PROPERTY(bool isSensibleHeatFractionDefaulted READ isSensibleHeatFractionDefaulted);
    Q_PROPERTY(bool isSensibleHeatFractionAutocalculated READ isSensibleHeatFractionAutocalculated);
    
    Q_PROPERTY(double carbonDioxideGenerationRate READ carbonDioxideGenerationRate WRITE setCarbonDioxideGenerationRate RESET resetCarbonDioxideGenerationRate);
    Q_PROPERTY(openstudio::Quantity carbonDioxideGenerationRate_SI READ carbonDioxideGenerationRate_SI WRITE setCarbonDioxideGenerationRate RESET resetCarbonDioxideGenerationRate);
    Q_PROPERTY(openstudio::Quantity carbonDioxideGenerationRate_IP READ carbonDioxideGenerationRate_IP WRITE setCarbonDioxideGenerationRate RESET resetCarbonDioxideGenerationRate);    
    Q_PROPERTY(bool isCarbonDioxideGenerationRateDefaulted READ isCarbonDioxideGenerationRateDefaulted);
    
    Q_PROPERTY(bool enableASHRAE55ComfortWarnings READ enableASHRAE55ComfortWarnings WRITE setEnableASHRAE55ComfortWarnings RESET resetEnableASHRAE55ComfortWarnings);
    Q_PROPERTY(bool isEnableASHRAE55ComfortWarningsDefaulted READ isEnableASHRAE55ComfortWarningsDefaulted);
    
    Q_PROPERTY(std::string meanRadiantTemperatureCalculationType READ meanRadiantTemperatureCalculationType WRITE setMeanRadiantTemperatureCalculationType RESET resetMeanRadiantTemperatureCalculationType);
    Q_PROPERTY(bool isMeanRadiantTemperatureCalculationTypeDefaulted READ isMeanRadiantTemperatureCalculationTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> meanRadiantTemperatureCalculationTypeValues READ meanRadiantTemperatureCalculationTypeValues);
    
    Q_PROPERTY(std::vector<std::string> thermalComfortModelTypeValues READ thermalComfortModelTypeValues);
   public:
    /** @name Constructors and Destructors */
    //@{

    PeopleDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    PeopleDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    PeopleDefinition_Impl(const PeopleDefinition_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~PeopleDefinition_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string numberofPeopleCalculationMethod() const;

    boost::optional<double> numberofPeople() const;
    
    OSOptionalQuantity getNumberofPeople(bool returnIP=false) const;

    boost::optional<double> peopleperSpaceFloorArea() const;
    
    OSOptionalQuantity getPeopleperSpaceFloorArea(bool returnIP=false) const;    

    boost::optional<double> spaceFloorAreaperPerson() const;
    
    OSOptionalQuantity getSpaceFloorAreaperPerson(bool returnIP=false) const;    

    double fractionRadiant() const;
    
    Quantity getFractionRadiant(bool returnIP=false) const;    

    boost::optional<double> sensibleHeatFraction() const;
    
    OSOptionalQuantity getSensibleHeatFraction(bool returnIP=false) const;    

    bool isSensibleHeatFractionDefaulted() const;

    bool isSensibleHeatFractionAutocalculated() const;

    double carbonDioxideGenerationRate() const;
    
    Quantity getCarbonDioxideGenerationRate(bool returnIP=false) const;    

    bool isCarbonDioxideGenerationRateDefaulted() const;

    bool enableASHRAE55ComfortWarnings() const;

    bool isEnableASHRAE55ComfortWarningsDefaulted() const;

    std::string meanRadiantTemperatureCalculationType() const;

    bool isMeanRadiantTemperatureCalculationTypeDefaulted() const;

    /** Return the thermal comfort model type in extensible group i. Indexing starts at 0, and
     *  i should be less than numThermalComfortModelTypes. */
    boost::optional<std::string> getThermalComfortModelType(int i) const;

    /** Returns the number of thermal comfort model types registered in this object. */
    int numThermalComfortModelTypes() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberofPeople(boost::optional<double> numberofPeople);
    
    bool setNumberofPeople(const OSOptionalQuantity& numberofPeople);    

    bool setPeopleperSpaceFloorArea(boost::optional<double> peopleperSpaceFloorArea);
    
    bool setPeopleperSpaceFloorArea(const OSOptionalQuantity& peopleperSpaceFloorArea);    

    bool setSpaceFloorAreaperPerson(boost::optional<double> spaceFloorAreaperPerson);

    bool setSpaceFloorAreaperPerson(const OSOptionalQuantity& spaceFloorAreaperPerson);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionRadiant(const Quantity& fractionRadiant);    

    bool setSensibleHeatFraction(boost::optional<double> sensibleHeatFraction);

    bool setSensibleHeatFraction(double sensibleHeatFraction);
    
    bool setSensibleHeatFraction(const OSOptionalQuantity& sensibleHeatFraction);    

    void resetSensibleHeatFraction();

    void autocalculateSensibleHeatFraction();

    bool setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate);
    
    bool setCarbonDioxideGenerationRate(const Quantity& carbonDioxideGenerationRate);    

    void resetCarbonDioxideGenerationRate();

    void setEnableASHRAE55ComfortWarnings(bool enableASHRAE55ComfortWarnings);

    void resetEnableASHRAE55ComfortWarnings();

    bool setMeanRadiantTemperatureCalculationType(std::string meanRadiantTemperatureCalculationType);

    void resetMeanRadiantTemperatureCalculationType();

    bool pushThermalComfortModelType(const std::string& thermalComfortModelType);

    bool setThermalComfortModelType(int i, const std::string& thermalComfortModelType);

    bool eraseThermalComfortModelType(int i);

    //@}
    /** @name Other */
    //@{

    /** Returns the number of people represented by this definition, assuming floorArea (m^2). */
    double getNumberOfPeople(double floorArea) const;

    /** Returns the people/m^2 represented by this definition, assuming floorArea (m^2). */
    double getPeoplePerFloorArea(double floorArea) const;

    /** Returns the m^2/person represented by this definition, assuming floorArea (m^2). */
    double getFloorAreaPerPerson(double floorArea) const;

    /** If method is a \link validNumberOfPeopleCalculationMethodValues valid number of people 
     *  calculation method \endlnk, changes this definition to an equivalent number of people, 
     *  under the assumption of floorArea (m^2). */
    bool setNumberOfPeopleCalculationMethod(const std::string& method,double floorArea);

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.PeopleDefinition");
    
    std::vector<std::string> numberofPeopleCalculationMethodValues() const;
    openstudio::OSOptionalQuantity numberofPeople_SI() const;
    openstudio::OSOptionalQuantity numberofPeople_IP() const;
    openstudio::OSOptionalQuantity peopleperSpaceFloorArea_SI() const;
    openstudio::OSOptionalQuantity peopleperSpaceFloorArea_IP() const;
    openstudio::OSOptionalQuantity spaceFloorAreaperPerson_SI() const;
    openstudio::OSOptionalQuantity spaceFloorAreaperPerson_IP() const;
    openstudio::Quantity fractionRadiant_SI() const;
    openstudio::Quantity fractionRadiant_IP() const;
    openstudio::OSOptionalQuantity sensibleHeatFraction_SI() const;
    openstudio::OSOptionalQuantity sensibleHeatFraction_IP() const;
    openstudio::Quantity carbonDioxideGenerationRate_SI() const;
    openstudio::Quantity carbonDioxideGenerationRate_IP() const;
    std::vector<std::string> meanRadiantTemperatureCalculationTypeValues() const;
    std::vector<std::string> thermalComfortModelTypeValues() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PEOPLEDEFINITION_IMPL_HPP

