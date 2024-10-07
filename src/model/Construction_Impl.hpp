/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONSTRUCTION_IMPL_HPP
#define MODEL_CONSTRUCTION_IMPL_HPP

#include "LayeredConstruction_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API Construction_Impl : public LayeredConstruction_Impl
    {
     public:
      // constructor
      Construction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      Construction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Construction_Impl(const Construction_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~Construction_Impl() override = default;

      /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual int renderingColorIndex() const override;

      /** Searches the model for a layered construction with layers which are the reverse of this one.  Returns that construction
     *  if found, creates one if not. */
      Construction reverseConstruction() const;

     private:
      REGISTER_LOGGER("openstudio.model.Construction");
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTION_IMPL_HPP
