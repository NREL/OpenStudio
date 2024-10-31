/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGININSTANCE_IMPL_HPP
#define MODEL_PYTHONPLUGININSTANCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ExternalFile;

  namespace detail {

    /** PythonPluginInstance_Impl is a ResourceObject_Impl that is the implementation class for PythonPluginInstance.*/
    class MODEL_API PythonPluginInstance_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PythonPluginInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PythonPluginInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PythonPluginInstance_Impl(const PythonPluginInstance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PythonPluginInstance_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ResourceObject> resources() const override;

      //@}
      /** @name Getters */
      //@{

      ExternalFile externalFile() const;

      bool runDuringWarmupDays() const;
      bool isRunDuringWarmupDaysDefaulted() const;

      std::string pluginClassName() const;

      //@}
      /** @name Setters */
      //@{

      bool setRunDuringWarmupDays(bool runDuringWarmupDays);
      void resetRunDuringWarmupDays();

      bool setPluginClassName(const std::string& pluginClassName);

      //@}
      /** @name Other */
      //@{

      bool findPluginClassNameInFile(const std::string& pluginClassName) const;

      std::vector<std::string> validPluginClassNamesInFile() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PythonPluginInstance");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGININSTANCE_IMPL_HPP
