/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLIGHTSDEFINITION_IMPL_HPP
#define MODEL_EXTERIORLIGHTSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExteriorLightsDefinition_Impl is a ExteriorLoadDefinition_Impl that is the implementation class for ExteriorLightsDefinition.*/
    class MODEL_API ExteriorLightsDefinition_Impl : public ExteriorLoadDefinition_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExteriorLightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExteriorLightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExteriorLightsDefinition_Impl(const ExteriorLightsDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorLightsDefinition_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // TODO: remove (unused)
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double designLevel() const;

      //@}
      /** @name Setters */
      //@{

      bool setDesignLevel(double designLevel);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExteriorLightsDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLIGHTSDEFINITION_IMPL_HPP
