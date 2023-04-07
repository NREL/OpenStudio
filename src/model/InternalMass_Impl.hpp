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

#ifndef MODEL_INTERNALMASS_IMPL_HPP
#define MODEL_INTERNALMASS_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class InternalMass;
  class InternalMassDefinition;
  class SurfacePropertyConvectionCoefficients;

  namespace detail {

    /** InternalMass_Impl is a SpaceLoadInstance_Impl that is the implementation class for InternalMass.*/
    class MODEL_API InternalMass_Impl : public SpaceLoadInstance_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      InternalMass_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InternalMass_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InternalMass_Impl(const InternalMass_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InternalMass_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;

      //@}
      /** @name Getters */
      //@{

      /** Gets the InternalMassDefinition object. */
      InternalMassDefinition internalMassDefinition() const;

      /** Returns the surface property convection coefficients */
      boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

      //@}
      /** @name Setters */
      //@{

      /** Sets the InternalMassDefinition object. */
      bool setInternalMassDefinition(const InternalMassDefinition& definition);

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> surfaceArea() const;

      boost::optional<double> surfaceAreaPerFloorArea() const;

      boost::optional<double> surfaceAreaPerPerson() const;

      /** Returns the construction surface area represented by this instance, assuming floorArea (m^2)
     *  and numPeople. */
      double getSurfaceArea(double floorArea, double numPeople) const;

      /** Returns the m^2/m^2 of construction surface area per floor area represented by this
     *  instance, assuming floorArea (m^2) and numPeople. */
      double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

      /** Returns the m^2/person of construction surface area represented by this instance, assuming
     *  floorArea (m^2) and numPeople. */
      double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

      virtual ModelObject clone(Model model) const override;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.InternalMass");

      boost::optional<ModelObject> internalMassDefinitionAsModelObject() const;

      bool setInternalMassDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERNALMASS_IMPL_HPP
