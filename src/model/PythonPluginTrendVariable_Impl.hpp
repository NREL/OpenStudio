/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINTRENDVARIABLE_IMPL_HPP
#define MODEL_PYTHONPLUGINTRENDVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PythonPluginVariable;

  namespace detail {

    /** PythonPluginTrendVariable_Impl is a ModelObject_Impl that is the implementation class for PythonPluginTrendVariable.*/
    class MODEL_API PythonPluginTrendVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PythonPluginTrendVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PythonPluginTrendVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PythonPluginTrendVariable_Impl(const PythonPluginTrendVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PythonPluginTrendVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      PythonPluginVariable pythonPluginVariable() const;

      int numberofTimestepstobeLogged() const;

      //@}
      /** @name Setters */
      //@{

      bool setPythonPluginVariable(const PythonPluginVariable& pythonPluginVariable);

      bool setNumberofTimestepstobeLogged(int numberofTimestepstobeLogged);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PythonPluginTrendVariable");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<PythonPluginVariable> optionalPythonPluginVariable() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINTRENDVARIABLE_IMPL_HPP
