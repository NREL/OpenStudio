/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef OPENSTUDIO_FILEOPERATIONS_HPP
#define OPENSTUDIO_FILEOPERATIONS_HPP

#include "OpenStudioAPI.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../utilities/core/Path.hpp"

#include <boost/optional.hpp>

#include <map>
#include <vector>

class QObject;
class QWidget;

namespace openstudio {
  class ProgressBar;
  class BCLMeasure;

  namespace model {
    class Model;
  }

  namespace osversion {
    class VersionTranslator;
  }

  namespace energyplus {
    class ReverseTranslator;
  }

  namespace runmanager {
    class RunManager;
    class WorkItem;
  }

  // Recursively remove dirName.
  // Returns true on success and false if there is an error.
  OPENSTUDIO_API bool removeDir(const QString & dirName);

  // Recursively copies srcPath to dstPath;
  // If dstPath already exists it will be removed.
  OPENSTUDIO_API bool copyDir(const QString &srcPath, const QString &dstPath);

  /// Create a temporary directory for an openstudio model
  /// This is where changes to the files, etc are staged
  OPENSTUDIO_API openstudio::path createModelTempDir();

  /// Copy existing model resources to temp dir
  OPENSTUDIO_API bool initializeModelTempDir(const openstudio::path& osmPath, const openstudio::path& modelTempDir);

  /// Update the model in the temp directory to include the correct
  /// directory structure, copy files to relative locations, etc
  /// Returns true if any changes were made
  OPENSTUDIO_API bool updateModelTempDir(openstudio::model::Model& model, const openstudio::path& modelTempDir);
  
  // Saves the model temporary directory to a new directory alongside the osm file.
  OPENSTUDIO_API void saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath);

  // Saves the model to osm and copies files from temp dir, returns the path to the osm which may have been corrected
  OPENSTUDIO_API openstudio::path saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir);


  // Removes the model temporary directory.
  OPENSTUDIO_API void removeModelTempDir(const openstudio::path& modelTempDir);

  // Creates and saves the run manager database, overwriting previous database.
  OPENSTUDIO_API bool saveRunManagerDatabase(const openstudio::path& osmPath, const openstudio::path& modelTempDir, 
                                             std::vector <double> useRadianceForDaylightingCalculations,
                                             QWidget* parent = nullptr);
  OPENSTUDIO_API bool saveRunManagerDatabase(const openstudio::path& osmPath, const openstudio::path& modelTempDir, 
                                             const std::map<openstudio::path,std::vector<ruleset::UserScriptInfo> >& userScriptsByFolder,
                                             std::vector <double> useRadianceForDaylightingCalculations,
                                             QWidget* parent = nullptr);

  // Pause the run manager
  OPENSTUDIO_API void pauseRunManager(openstudio::runmanager::RunManager& rm);

  // Start the run manager
  OPENSTUDIO_API void startRunManager(openstudio::runmanager::RunManager& rm, const openstudio::path& osmPath, const openstudio::path& modelTempDir,
                                      std::vector <double> useRadianceForDaylightingCalculations, bool requireCalibrationReports, QWidget* parent = nullptr);

  OPENSTUDIO_API bool findBCLMeasureWorkItem(const std::vector<runmanager::WorkItem>& workItems, const openstudio::UUID& uuid);
  
  OPENSTUDIO_API bool addReportRequestMeasureWorkItem(std::vector<runmanager::WorkItem>& workItems, const openstudio::BCLMeasure& bclMeasure);

  OPENSTUDIO_API bool addReportingMeasureWorkItem(std::vector<runmanager::WorkItem>& workItems, const openstudio::BCLMeasure& bclMeasure);

  OPENSTUDIO_API boost::optional<openstudio::model::Model> modelFromOSM(const openstudio::path& path, openstudio::osversion::VersionTranslator& versionTranslator, openstudio::ProgressBar* progressBar = nullptr);

  OPENSTUDIO_API boost::optional<openstudio::model::Model> modelFromIDF(const openstudio::path& path, openstudio::energyplus::ReverseTranslator& reverseTranslator, openstudio::ProgressBar* progressBar = nullptr);


} // openstudio

#endif // OPENSTUDIO_FILEOPERATIONS_HPP
