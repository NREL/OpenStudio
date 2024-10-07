/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINOUTPUTVARIABLE_HPP
#define MODEL_PYTHONPLUGINOUTPUTVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class PythonPluginVariable;

  namespace detail {

    class PythonPluginOutputVariable_Impl;

  }  // namespace detail

  /** PythonPluginOutputVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:PythonPlugin:OutputVariable'. */
  class MODEL_API PythonPluginOutputVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PythonPluginOutputVariable(const PythonPluginVariable& pythonPluginVariable);

    virtual ~PythonPluginOutputVariable() override = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> typeofDatainVariableValues();

    static std::vector<std::string> updateFrequencyValues();

    static std::vector<std::string> resourceTypeValues();

    static std::vector<std::string> groupTypeValues();

    static std::vector<std::string> endUseCategoryValues();

    /** @name Getters */
    //@{

    PythonPluginVariable pythonPluginVariable() const;

    std::string typeofDatainVariable() const;

    std::string updateFrequency() const;

    boost::optional<std::string> units() const;

    boost::optional<std::string> resourceType() const;

    boost::optional<std::string> groupType() const;

    boost::optional<std::string> endUseCategory() const;

    boost::optional<std::string> endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setPythonPluginVariable(const PythonPluginVariable& pythonPluginVariable);

    bool setTypeofDatainVariable(const std::string& typeofDatainVariable);

    bool setUpdateFrequency(const std::string& updateFrequency);

    bool setUnits(const std::string& units);

    void resetUnits();

    bool setResourceType(const std::string& resourceType);

    void resetResourceType();

    bool setGroupType(const std::string& groupType);

    void resetGroupType();

    bool setEndUseCategory(const std::string& endUseCategory);

    void resetEndUseCategory();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PythonPluginOutputVariable_Impl;

    explicit PythonPluginOutputVariable(std::shared_ptr<detail::PythonPluginOutputVariable_Impl> impl);

    friend class detail::PythonPluginOutputVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PythonPluginOutputVariable");
  };

  /** \relates PythonPluginOutputVariable*/
  using OptionalPythonPluginOutputVariable = boost::optional<PythonPluginOutputVariable>;

  /** \relates PythonPluginOutputVariable*/
  using PythonPluginOutputVariableVector = std::vector<PythonPluginOutputVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINOUTPUTVARIABLE_HPP
