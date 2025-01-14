/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PYTHONPLUGINSEARCHPATHS_IMPL_HPP
#define MODEL_PYTHONPLUGINSEARCHPATHS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"
#include "../utilities/core/Filesystem.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** PythonPluginSearchPaths_Impl is a ModelObject_Impl that is the implementation class for PythonPluginSearchPaths.*/
    class MODEL_API PythonPluginSearchPaths_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PythonPluginSearchPaths_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PythonPluginSearchPaths_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PythonPluginSearchPaths_Impl(const PythonPluginSearchPaths_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PythonPluginSearchPaths_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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

      void clearSearchPaths();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PythonPluginSearchPaths");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PYTHONPLUGINSEARCHPATHS_IMPL_HPP
