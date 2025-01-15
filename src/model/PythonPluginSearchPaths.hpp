/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINSEARCHPATHS_HPP
#define MODEL_PYTHONPLUGINSEARCHPATHS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

#include "../utilities/core/Filesystem.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PythonPluginSearchPaths_Impl;

  }  // namespace detail

  /** PythonPluginSearchPaths is a ModelObject that wraps the OpenStudio IDD object 'OS:PythonPlugin:SearchPaths'. */
  class MODEL_API PythonPluginSearchPaths : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~PythonPluginSearchPaths() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PythonPluginSearchPaths(const PythonPluginSearchPaths& other) = default;
    PythonPluginSearchPaths(PythonPluginSearchPaths&& other) = default;
    PythonPluginSearchPaths& operator=(const PythonPluginSearchPaths&) = default;
    PythonPluginSearchPaths& operator=(PythonPluginSearchPaths&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool addCurrentWorkingDirectorytoSearchPath() const;

    bool addInputFileDirectorytoSearchPath() const;

    bool addepinEnvironmentVariabletoSearchPath() const;

    std::vector<openstudio::path> searchPaths() const;

    //@}
    /** @name Setters */
    //@{

    bool setAddCurrentWorkingDirectorytoSearchPath(bool addCurrentWorkingDirectorytoSearchPath);

    bool setAddInputFileDirectorytoSearchPath(bool addInputFileDirectorytoSearchPath);

    bool setAddepinEnvironmentVariabletoSearchPath(bool addepinEnvironmentVariabletoSearchPath);

    bool addSearchPath(const openstudio::path& searchPath);
    bool setSearchPaths(const std::vector<openstudio::path>& searchPaths);

    // Convenience, forwards to the openstudio::path equivalent
    bool setSearchPaths(const std::vector<std::string>& searchPaths);

    void clearSearchPaths();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PythonPluginSearchPaths_Impl;

    explicit PythonPluginSearchPaths(std::shared_ptr<detail::PythonPluginSearchPaths_Impl> impl);

    friend class detail::PythonPluginSearchPaths_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit PythonPluginSearchPaths(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PythonPluginSearchPaths");
  };

  /** \relates PythonPluginSearchPaths*/
  using OptionalPythonPluginSearchPaths = boost::optional<PythonPluginSearchPaths>;

  /** \relates PythonPluginSearchPaths*/
  using PythonPluginSearchPathsVector = std::vector<PythonPluginSearchPaths>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINSEARCHPATHS_HPP
