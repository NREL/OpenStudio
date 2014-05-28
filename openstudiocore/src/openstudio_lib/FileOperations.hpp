/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
 *  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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


  // Returns true if the given runmanager database uses radiance for its daylight calculations
  OPENSTUDIO_API bool usesRadianceForDaylightCalculations(openstudio::runmanager::RunManager rm);


  // Removes the model temporary directory.
  OPENSTUDIO_API void removeModelTempDir(const openstudio::path& modelTempDir);

  // Creates and saves the run manager database, overwriting previous database.
  OPENSTUDIO_API bool saveRunManagerDatabase(const openstudio::path& osmPath, const openstudio::path& modelTempDir, 
                                             bool useRadianceForDaylightingCalculations,
                                             QWidget* parent = NULL);
  OPENSTUDIO_API bool saveRunManagerDatabase(const openstudio::path& osmPath, const openstudio::path& modelTempDir, 
                                             const std::map<openstudio::path,std::vector<ruleset::UserScriptInfo> >& userScriptsByFolder,
                                             bool useRadianceForDaylightingCalculations,
                                             QWidget* parent = NULL);

  // Pause the run manager
  OPENSTUDIO_API void pauseRunManager(openstudio::runmanager::RunManager& rm);

  // Start the run manager
  OPENSTUDIO_API void startRunManager(openstudio::runmanager::RunManager& rm, const openstudio::path& osmPath, const openstudio::path& modelTempDir,
      bool useRadianceForDaylightingCalculations, bool requireCalibrationReports, QWidget* parent = NULL);

  OPENSTUDIO_API bool findBCLMeasureWorkItem(const std::vector<runmanager::WorkItem>& workItems, const openstudio::UUID& uuid);

  OPENSTUDIO_API bool addReportingMeasureWorkItem(std::vector<runmanager::WorkItem>& workItems, const openstudio::BCLMeasure& bclMeasure);

  OPENSTUDIO_API boost::optional<openstudio::model::Model> modelFromOSM(const openstudio::path& path, openstudio::osversion::VersionTranslator& versionTranslator, openstudio::ProgressBar* progressBar = NULL);

  OPENSTUDIO_API boost::optional<openstudio::model::Model> modelFromIDF(const openstudio::path& path, openstudio::energyplus::ReverseTranslator& reverseTranslator, openstudio::ProgressBar* progressBar = NULL);


} // openstudio

#endif // OPENSTUDIO_FILEOPERATIONS_HPP
