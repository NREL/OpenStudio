/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINVARIABLE_HPP
#define MODEL_PYTHONPLUGINVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class PythonPluginTrendVariable;
  class PythonPluginOutputVariable;

  namespace detail {

    class PythonPluginVariable_Impl;

  }  // namespace detail

  /** PythonPluginVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:PythonPlugin:Variable'. */
  class MODEL_API PythonPluginVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PythonPluginVariable(const Model& model);

    virtual ~PythonPluginVariable() override = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::PythonPluginVariable_Impl;

    explicit PythonPluginVariable(std::shared_ptr<detail::PythonPluginVariable_Impl> impl);

    friend class detail::PythonPluginVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PythonPluginVariable");
  };

  /** \relates PythonPluginVariable*/
  using OptionalPythonPluginVariable = boost::optional<PythonPluginVariable>;

  /** \relates PythonPluginVariable*/
  using PythonPluginVariableVector = std::vector<PythonPluginVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINVARIABLE_HPP
