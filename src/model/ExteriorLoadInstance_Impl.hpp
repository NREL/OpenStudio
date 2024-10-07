/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLOADINSTANCE_IMPL_HPP
#define MODEL_EXTERIORLOADINSTANCE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class ExteriorLoadDefinition;
  class ExteriorLoadInstance;

  namespace detail {

    class MODEL_API ExteriorLoadInstance_Impl : public ModelObject_Impl
    {

     public:
      // Construct completely new object.
      ExteriorLoadInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      ExteriorLoadInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      ExteriorLoadInstance_Impl(const ExteriorLoadInstance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorLoadInstance_Impl() override = default;

      /** Returns the definition of this instance. */
      ExteriorLoadDefinition definition() const;

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const ExteriorLoadDefinition& definition) = 0;

      /** Ensures that no other instances share this instances definition. */
      void makeUnique();

      virtual double multiplier() const = 0;

      virtual bool isMultiplierDefaulted() const = 0;

      int quantity() const;

     protected:
      friend class openstudio::model::ExteriorLoadInstance;

      // index of the definition name
      virtual int definitionIndex() const = 0;

     private:
      REGISTER_LOGGER("openstudio.model.ExteriorLoadInstance");

      boost::optional<ModelObject> definitionAsModelObject() const;

      bool setDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLOADINSTANCE_IMPL_HPP
