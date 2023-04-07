/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~PythonPluginTrendVariable() = default;

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
