/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINOUTPUTVARIABLE_IMPL_HPP
#define MODEL_PYTHONPLUGINOUTPUTVARIABLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PythonPluginVariable;

  namespace detail {

    /** PythonPluginOutputVariable_Impl is a ModelObject_Impl that is the implementation class for PythonPluginOutputVariable.*/
    class MODEL_API PythonPluginOutputVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PythonPluginOutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PythonPluginOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PythonPluginOutputVariable_Impl(const PythonPluginOutputVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PythonPluginOutputVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.PythonPluginOutputVariable");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<PythonPluginVariable> optionalPythonPluginVariable() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINOUTPUTVARIABLE_IMPL_HPP
