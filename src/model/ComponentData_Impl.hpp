/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMPONENTDATA_IMPL_HPP
#define MODEL_COMPONENTDATA_IMPL_HPP

#include "ComponentData.hpp"

#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API ComponentData_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ComponentData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ComponentData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      ComponentData_Impl(const ComponentData_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ComponentData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      UUID uuid() const;

      UUID versionUUID() const;

      boost::optional<int> creationTimestamp() const;

      boost::optional<int> versionTimestamp() const;

      /** Equivalent to getComponentObject(0). */
      ModelObject primaryComponentObject() const;

      std::vector<ModelObject> componentObjects() const;

      /** Throws if objectIndex >= numComponentObjects, or if ComponentData has been corrupted. */
      ModelObject getComponentObject(unsigned objectIndex) const;

      //@}
      /** @name Setters */
      //@{

      UUID createVersionUUID();

      /** Pushes a new extensible group containing object's data. Not exposed in the public
     *  interface. Use with care. */
      bool registerObject(const ModelObject& object);

      //@}
      /** @name Queries */
      //@{

      /** Returns the number of objects in the Component described by this object. Must be >= 1. */
      unsigned numComponentObjects() const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ComponentData");

      boost::optional<ModelObject> primaryComponentObjectAsModelObject() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENTDATA_IMPL_HPP
