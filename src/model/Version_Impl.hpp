/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_VERSION_IMPL_HPP
#define MODEL_VERSION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ParentObject;

  namespace detail {

    class MODEL_API Version_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      Version_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      Version_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      Version_Impl(const Version_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~Version_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string versionIdentifier() const;

      boost::optional<std::string> prereleaseIdentifier() const;

      //@}
      /** @name Setters */
      //@{

      bool setVersionIdentifier(const std::string& s);

      bool setPrereleaseIdentifier(const std::string& s);
      //@}
     private:
      REGISTER_LOGGER("openstudio.model.Version");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_VERSION_IMPL_HPP
