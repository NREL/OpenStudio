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
