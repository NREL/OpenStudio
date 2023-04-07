/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_PEOPLEDEFINITION_IMPL_HPP
#define MODEL_PEOPLEDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class PeopleDefinition;

  namespace detail {

    /** PeopleDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for PeopleDefinition.*/
    class MODEL_API PeopleDefinition_Impl : public SpaceLoadDefinition_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PeopleDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PeopleDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PeopleDefinition_Impl(const PeopleDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PeopleDefinition_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // TODO: remove
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string numberofPeopleCalculationMethod() const;

      boost::optional<double> numberofPeople() const;

      boost::optional<double> peopleperSpaceFloorArea() const;

      boost::optional<double> spaceFloorAreaperPerson() const;

      double fractionRadiant() const;

      boost::optional<double> sensibleHeatFraction() const;

      bool isSensibleHeatFractionDefaulted() const;

      bool isSensibleHeatFractionAutocalculated() const;

      double carbonDioxideGenerationRate() const;

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

      bool setPeopleperSpaceFloorArea(boost::optional<double> peopleperSpaceFloorArea);

      bool setSpaceFloorAreaperPerson(boost::optional<double> spaceFloorAreaperPerson);

      bool setFractionRadiant(double fractionRadiant);

      bool setSensibleHeatFraction(double sensibleHeatFraction);

      void resetSensibleHeatFraction();

      void autocalculateSensibleHeatFraction();

      bool setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate);

      void resetCarbonDioxideGenerationRate();

      bool setEnableASHRAE55ComfortWarnings(bool enableASHRAE55ComfortWarnings);

      void resetEnableASHRAE55ComfortWarnings();

      bool setMeanRadiantTemperatureCalculationType(const std::string& meanRadiantTemperatureCalculationType);

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
      bool setNumberOfPeopleCalculationMethod(const std::string& method, double floorArea);

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.PeopleDefinition");

      std::vector<std::string> numberofPeopleCalculationMethodValues() const;
      std::vector<std::string> meanRadiantTemperatureCalculationTypeValues() const;
      std::vector<std::string> thermalComfortModelTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PEOPLEDEFINITION_IMPL_HPP
