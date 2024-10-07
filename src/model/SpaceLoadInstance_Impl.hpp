/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACELOADINSTANCE_IMPL_HPP
#define MODEL_SPACELOADINSTANCE_IMPL_HPP

#include "SpaceLoad_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class SpaceLoadDefinition;
  class SpaceLoadInstance;

  namespace detail {

    class MODEL_API SpaceLoadInstance_Impl : public SpaceLoad_Impl
    {

     public:
      // Construct completely new object.
      SpaceLoadInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      SpaceLoadInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      SpaceLoadInstance_Impl(const SpaceLoadInstance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceLoadInstance_Impl() override = default;

      /** Returns the definition of this instance. */
      SpaceLoadDefinition definition() const;

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) = 0;

      /** Ensures that no other instances share this instances definition. */
      void makeUnique();

      /** Gets the multiplier for this space instance, defaults to 1. **/
      virtual double multiplier() const = 0;

      /** Returns true if the multiplier is defaulted. **/
      virtual bool isMultiplierDefaulted() const = 0;

      /** Returns the floor area associated with this space load instance.
    If this space load is associated with a single space then that space's floor area is returned.
    If this space load is associated with a space type then the sum of all space floor area in that space type is returned.
    Space and SpaceLoadInstance multipliers are included in the result.**/
      double floorArea() const;

      /** Returns the number of instances this space load instance represents.
    Space and SpaceLoadInstance multipliers are included in the result **/
      int quantity() const;

     protected:
      friend class openstudio::model::SpaceLoadInstance;

      // index of the definition name
      virtual int definitionIndex() const = 0;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceLoadInstance");

      boost::optional<ModelObject> definitionAsModelObject() const;

      bool setDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOADINSTANCE_IMPL_HPP
