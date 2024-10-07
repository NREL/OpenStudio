/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~InternalMass_Impl() override = default;

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
