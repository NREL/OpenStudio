/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINVARIABLE_IMPL_HPP
#define MODEL_PYTHONPLUGINVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PythonPluginTrendVariable;
  class PythonPluginOutputVariable;

  namespace detail {

    /** PythonPluginVariable_Impl is a ModelObject_Impl that is the implementation class for PythonPluginVariable.*/
    class MODEL_API PythonPluginVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PythonPluginVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PythonPluginVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PythonPluginVariable_Impl(const PythonPluginVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PythonPluginVariable_Impl() override = default;

      /** Removes all PythonPluginTrendVariable and PythonPluginOutputVariable attached to it */
      virtual std::vector<IdfObject> remove() override;

      /** Also clones all PythonPluginTrendVariable and PythonPluginOutputVariable attached to it */
      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{

      std::vector<PythonPluginTrendVariable> pythonPluginTrendVariables() const;
      std::vector<PythonPluginOutputVariable> pythonPluginOutputVariables() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PythonPluginVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINVARIABLE_IMPL_HPP
