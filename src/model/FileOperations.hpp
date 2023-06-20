/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FILEOPERATIONS_HPP
#define MODEL_FILEOPERATIONS_HPP

#include "ModelAPI.hpp"

#include "../utilities/core/Path.hpp"

#include <boost/optional.hpp>

#include <map>
#include <vector>

namespace openstudio {
namespace model {

  class Model;

  // Recursively remove directory at path.
  // Returns true on success and false if there is an error.
  //MODEL_API bool removeDir(const openstudio::path &path);

  // Recursively copies sourceDir to destinationDir;
  // If destinationDir - typically that's the Companion Folder - already exists it will be removed and recreated.
  //MODEL_API bool replaceDir(const openstudio::path &sourceDir, const openstudio::path &destinationDir)

  /// Create a temporary directory for an openstudio model
  /// This is where changes to the files, etc are staged
  //MODEL_API openstudio::path createModelTempDir();

  /// Copy existing model resources to temp dir. Returns true if the directory was successfully initialized.
  //MODEL_API bool initializeModelTempDir(const openstudio::path& osmPath, const openstudio::path& modelTempDir);

  /// Update the model in the temp directory to include the correct
  /// directory structure, copy files to relative locations, etc
  /// Returns true if any changes were made
  //MODEL_API bool updateModelTempDir(openstudio::model::Model& model, const openstudio::path& modelTempDir);

  /// Connect the model to WorkflowJSON found in the temp directory.
  /// Returns true if existing WorkflowJSON was found.  If no existing WorkflowJSON is found, a new one is created in the temp dir.
  //MODEL_API bool attachWorkflow(openstudio::model::Model& model, const openstudio::path& modelTempDir);

  /// Initialize a temporary directory for a given model, if savedPath is passed in then any existing companion directories are copied to
  /// the temporary directory.  Returns path to the temporary directory.
  /// This method combines calls to createModelTempDir, initializeModelTempDir, updateModelTempDir, and attachWorkflow.
  MODEL_API openstudio::path initializeModel(openstudio::model::Model model);
  MODEL_API openstudio::path initializeModel(openstudio::model::Model model, const openstudio::path& savedPath);

  /// Initialize model with objects that will be automatically created by the OpenStudio app
  /// Prevents models from appearing changed on open
  // DLM: Would call this in Model ctor but that messes up components
  MODEL_API void initializeModelObjects(openstudio::model::Model model);

  // Saves the model temporary directory to a new directory alongside the osm file.
  //MODEL_API bool saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath);

  // Saves the workflow and model to their temp locations and copies files from temp dir, returns true if successful.
  MODEL_API bool saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir);

  // Removes the model temporary directory.
  MODEL_API void removeModelTempDir(const openstudio::path& modelTempDir);

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FILEOPERATIONS_HPP
