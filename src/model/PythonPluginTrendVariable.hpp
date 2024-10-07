/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINTRENDVARIABLE_HPP
#define MODEL_PYTHONPLUGINTRENDVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class PythonPluginVariable;

  namespace detail {

    class PythonPluginTrendVariable_Impl;

  }  // namespace detail

  /** PythonPluginTrendVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:PythonPlugin:TrendVariable'.
    * While this object is fully functional, it is not the recommended way to do trending:
    * just use a storage variable (eg: a list) in the python script */
  class MODEL_API PythonPluginTrendVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PythonPluginTrendVariable(const PythonPluginVariable& pythonPluginVariable);

    virtual ~PythonPluginTrendVariable() override = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::PythonPluginTrendVariable_Impl;

    explicit PythonPluginTrendVariable(std::shared_ptr<detail::PythonPluginTrendVariable_Impl> impl);

    friend class detail::PythonPluginTrendVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PythonPluginTrendVariable");
  };

  /** \relates PythonPluginTrendVariable*/
  using OptionalPythonPluginTrendVariable = boost::optional<PythonPluginTrendVariable>;

  /** \relates PythonPluginTrendVariable*/
  using PythonPluginTrendVariableVector = std::vector<PythonPluginTrendVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINTRENDVARIABLE_HPP
