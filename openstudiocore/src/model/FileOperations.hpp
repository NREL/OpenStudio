/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef MODEL_FILEOPERATIONS_HPP
#define MODEL_FILEOPERATIONS_HPP

#include "ModelAPI.hpp"

#include "../utilities/core/Path.hpp"

#include <boost/optional.hpp>

#include <QString>

#include <map>
#include <vector>


namespace openstudio {
namespace model {

  class Model;

  // Recursively remove dirName.
  // Returns true on success and false if there is an error.
  MODEL_API bool removeDir(const QString & dirName);

  // Recursively copies srcPath to dstPath;
  // If dstPath already exists it will be removed.
  MODEL_API bool copyDir(const QString &srcPath, const QString &dstPath);

  /// Create a temporary directory for an openstudio model
  /// This is where changes to the files, etc are staged
  MODEL_API openstudio::path createModelTempDir();

  /// Copy existing model resources to temp dir
  MODEL_API bool initializeModelTempDir(const openstudio::path& osmPath, const openstudio::path& modelTempDir);

  /// Update the model in the temp directory to include the correct
  /// directory structure, copy files to relative locations, etc
  /// Returns true if any changes were made
  MODEL_API bool updateModelTempDir(openstudio::model::Model& model, const openstudio::path& modelTempDir);

  // Saves the model temporary directory to a new directory alongside the osm file.
  MODEL_API void saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath);

  // Saves the model to osm and copies files from temp dir, returns the path to the osm which may have been corrected
  MODEL_API openstudio::path saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir);

  // Removes the model temporary directory.
  MODEL_API void removeModelTempDir(const openstudio::path& modelTempDir);

} // model
} // openstudio

#endif // MODEL_FILEOPERATIONS_HPP
