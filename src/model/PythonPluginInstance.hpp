/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGININSTANCE_HPP
#define MODEL_PYTHONPLUGININSTANCE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class ExternalFile;

  namespace detail {

    class PythonPluginInstance_Impl;

  }  // namespace detail

  /** PythonPluginInstance is a ResourceObject that wraps the OpenStudio IDD object 'OS:PythonPlugin:Instance'. */
  class MODEL_API PythonPluginInstance : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    PythonPluginInstance(const ExternalFile& externalfile, const std::string& pluginClassName);

    virtual ~PythonPluginInstance() override = default;

    //@}

    static IddObjectType iddObjectType();

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

    /** Helper that will return whether the pluginClassName can be found in the linked file */
    bool findPluginClassNameInFile(const std::string& pluginClassName) const;

    std::vector<std::string> validPluginClassNamesInFile() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::PythonPluginInstance_Impl;

    explicit PythonPluginInstance(std::shared_ptr<detail::PythonPluginInstance_Impl> impl);

    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PythonPluginInstance");
  };

  /** \relates PythonPluginInstance*/
  using OptionalPythonPluginInstance = boost::optional<PythonPluginInstance>;

  /** \relates PythonPluginInstance*/
  using PythonPluginInstanceVector = std::vector<PythonPluginInstance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGININSTANCE_HPP
