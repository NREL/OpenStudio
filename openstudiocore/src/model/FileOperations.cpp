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

#include "FileOperations.hpp"

#include "Model.hpp"

#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/Assert.hpp"

#include <OpenStudio.hxx>

#include <QDir>
#include <QMessageBox>
#include <QTemporaryFile>




namespace openstudio {
namespace model {

  bool removeDir(const QString &dirName)
  {
    bool result = true;

    LOG_FREE(Info, "removeDir", "removeDir '" << toString(dirName) << "'");

    QDir dir(dirName);
    if (dir.exists(dirName)) {
      bool test = true;

      for (const QFileInfo& info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
      {
        if (info.isDir()) {
          test = removeDir(info.absoluteFilePath());
          if (!test){
            LOG_FREE(Error, "removeDir", "Failed to remove directory '" << toString(info.absoluteFilePath()) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        } else{
          test = QFile::remove(info.absoluteFilePath());
          if (!test){
            LOG_FREE(Error, "removeDir", "Failed to remove file '" << toString(info.absoluteFilePath()) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        }
      }
      test = dir.rmdir(dirName);
      if (!test){
        LOG_FREE(Error, "removeDir", "Failed to remove directory '" << toString(dirName) << "'");
        result = false;

        // DLM: do we really want to give up here?
        //return result;
      }
    }

    return result;
  }

  bool synchDirStructures(const QString &srcPath, const QString &dstPath)
  {
    bool result = true;
    bool test = true;

    QDir srcDir(srcPath);
    QDir dstDir(dstPath);

    if (srcDir.canonicalPath() == dstDir.canonicalPath()){
      LOG_FREE(Warn, "synchDirStructures", "Cannot synch destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "' because they resolve to the same location");
      return true; // already synched
    }

    LOG_FREE(Info, "synchDirStructures", "Synching destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "'");

    // remove all files in dst as well as any directories in dst that are not in src
    for (const QFileInfo &dstItemInfo : dstDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
      QString srcItemPath = srcPath + "/" + dstItemInfo.fileName();
      QString dstItemPath = dstPath + "/" + dstItemInfo.fileName();

      QFileInfo srcItemInfo = QFileInfo(srcItemPath);

      if (dstItemInfo.isDir()){

        if (srcItemInfo.exists() && srcItemInfo.isDir()){

          // directory also exists in source
          test = synchDirStructures(srcItemPath, dstItemPath);
          if (!test){
            LOG_FREE(Error, "synchDirStructures", "Failed to synch destination '" << toString(dstItemPath) << "' with source '" << toString(srcItemPath) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }

        } else{

          // directory does not exist in source
          test = removeDir(dstItemPath);
          if (!test){
            LOG_FREE(Error, "synchDirStructures", "Failed to remove directory '" << toString(dstItemPath) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        }

      } else if (dstItemInfo.isFile()){
        test = QFile::remove(dstItemInfo.absoluteFilePath());
        if (!test){
          LOG_FREE(Error, "synchDirStructures", "Failed to remove file '" << toString(dstItemInfo.absoluteFilePath()) << "'");
          result = false;

          // DLM: do we really want to give up here?
          //return result;
        }
      }
    }

    return result;
  }

  bool copyDir(const QString &srcPath, const QString &dstPath)
  {
    bool result = true;
    bool test = true;

    LOG_FREE(Info, "copyDir", "copyDir '" << toString(srcPath) << "' to '" << toString(dstPath) << "'");

    // ensure directory exists
    QFileInfo dstInfo(dstPath);
    QString fileName = dstInfo.fileName();
    if (!dstInfo.exists() || !dstInfo.isDir()){
      QDir parentDstDir(QFileInfo(dstPath).path());
      LOG_FREE(Info, "copyDir", "Creating directory named = '" << toString(fileName) << "' in parentDstDir = '" << toString(parentDstDir.path()) << "'");
      if (!parentDstDir.mkpath(fileName)){
        LOG_FREE(Error, "copyDir", "Failed to create directory = '" << toString(fileName) << "' in parentDstDir = '" << toString(parentDstDir.path()) << "'");
        return false;
      }
    }

    QDir srcDir(srcPath);

    // remove all files in dst as well as any directories in dst that are not in src
    test = synchDirStructures(srcPath, dstPath);
    if (!test){
      LOG_FREE(Error, "copyDir", "Failed to synch destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "'");
      result = false;
    }

    // copy all files in src to dst
    for (const QFileInfo &srcItemInfo : srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
      QString srcItemPath = srcPath + "/" + srcItemInfo.fileName();
      QString dstItemPath = dstPath + "/" + srcItemInfo.fileName();

      QFileInfo dstItemInfo = QFileInfo(dstItemPath);

      if (srcItemInfo.isDir()) {
        test = copyDir(srcItemPath, dstItemPath);
        if (!test){
          LOG_FREE(Error, "copyDir", "Failed to copy directory '" << toString(srcItemPath) << "' to '" << toString(dstItemPath) << "'");
          result = false;

          // DLM: do we really want to give up here?
          //return false;
        }
      } else if (srcItemInfo.isFile()) {
        test = QFile::copy(srcItemPath, dstItemPath);
        if (!test){
          LOG_FREE(Error, "copyDir", "Failed to copy file '" << toString(srcItemPath) << "' to '" << toString(dstItemPath) << "'");
          result = false;

          // DLM: do we really want to give up here?
          //return false;
        }
      }
    }

    return result;
  }

  openstudio::path createModelTempDir()
  {
    openstudio::path result;
    auto tempFile = new QTemporaryFile();

    if (tempFile->open())
    {
      QString path = longPathName(tempFile->fileName());

      // must close tempFile so you can create directory with same name on Windows
      delete tempFile;

      QDir modelTempDir(path);
      LOG_FREE(Info, "createModelTempDir", "Creating directory '" << toString(modelTempDir.path()) << "'");
      bool test = modelTempDir.mkpath(modelTempDir.path());
      OS_ASSERT(test);

      result = toPath(path);

      return result;
    }

    delete tempFile;

    OS_ASSERT(false);

    return openstudio::path();
  }

  bool initializeModelTempDir(const openstudio::path& osmPath, const openstudio::path& modelTempDir)
  {
    bool result = true;

    if (osmPath.empty() || !openstudio::filesystem::exists(osmPath)){

      LOG_FREE(Debug, "initializeModelTempDir", "OSM path '" << toString(osmPath) << "' is empty or does not exist");
      result = false;

    } else{
      openstudio::path fileName = osmPath.filename();
      openstudio::path destination = modelTempDir / fileName;
      LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(osmPath) << "' to '" << toString(destination) << "'");

      // copy osm file
      bool test = QFile::copy(toQString(osmPath), toQString(destination));
      if (!test){
        LOG_FREE(Error, "initializeModelTempDir", "Could not copy '" << toString(osmPath) << "' to '" << toString(destination) << "'");
      }

      // Copy all files from existing resources dir into temp dir when opening
      openstudio::path sourceDir = osmPath.parent_path() / osmPath.stem();
      openstudio::path destDir = modelTempDir / toPath("resources");
      if (openstudio::filesystem::exists(sourceDir)){
        LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(sourceDir) << "' to '" << toString(destDir) << "'");

        test = copyDir(toQString(sourceDir), toQString(destDir));
        if (!test){
          LOG_FREE(Error, "initializeModelTempDir", "Could not copy '" << toString(sourceDir) << "' to '" << toString(destDir) << "'");
          result = false;
        }
      }
    }

    return result;
  }

  bool updateModelTempDir(openstudio::model::Model& model, const openstudio::path& modelTempDir)
  {
    bool modified = false;

    // Create folder layout
    openstudio::path runpath = modelTempDir / openstudio::toPath("resources/run");
    openstudio::path scriptspath = modelTempDir / openstudio::toPath("resources/scripts");
    openstudio::path filespath = modelTempDir / openstudio::toPath("resources/files");

    if (!openstudio::filesystem::exists(runpath))
    {
      openstudio::filesystem::create_directories(runpath);
      modified = true;
    }

    if (!openstudio::filesystem::exists(scriptspath))
    {
      openstudio::filesystem::create_directories(scriptspath);
      modified = true;
    }

    if (!openstudio::filesystem::exists(filespath))
    {
      openstudio::filesystem::create_directories(filespath);
      modified = true;
    }

    // weather file is fixed in OSDocument::fixWeatherFileOnOpen

    return modified;
  }

  void saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath)
  {
    bool test = true;

    // must remove file, QFile::copy does not overwrite
    QFileInfo osmInfo(toQString(osmPath));
    if (osmInfo.exists() && osmInfo.isFile()){
      test = QFile::remove(toQString(osmPath));
      if (!test){
        LOG_FREE(Error, "saveModelTempDir", "Could not remove previous osm at '" << toString(osmPath) << "'");
      }
    }

    // copy osm file
    openstudio::path srcPath = modelTempDir / osmPath.filename();
    test = QFile::copy(toQString(srcPath), toQString(osmPath));
    if (!test){
      LOG_FREE(Error, "saveModelTempDir", "Could not copy osm from '" << toString(srcPath) << "' to '" << toString(osmPath) << "'");
    }

    if (QFileInfo(toQString(osmPath)).exists()) {

      // copy resources
      openstudio::path srcDir = modelTempDir / toPath("resources");
      openstudio::path dstDir = osmPath.parent_path() / osmPath.stem();

      LOG_FREE(Debug, "saveModelTempDir", "Copying " << toString(srcDir) << " to " << toString(dstDir));

      test = copyDir(toQString(srcDir), toQString(dstDir));
      if (!test){
        LOG_FREE(Error, "saveModelTempDir", "Could not copy '" << toString(srcDir) << "' to '" << toString(dstDir) << "'");
      }

    }
  }

  openstudio::path saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir)
  {

    openstudio::path modelPath = osmPath;

    if (getFileExtension(osmPath).empty()) {
      modelPath = setFileExtension(osmPath, modelFileExtension(), false, true);
    }

    // save osm to temp directory, saveModelTempDir will copy to real location
    openstudio::path tempModelPath = modelTempDir / modelPath.filename();
    model.save(tempModelPath, true);

    LOG_FREE(Debug, "saveModel", "Saved model to '" << toString(tempModelPath) << "'");

    saveModelTempDir(modelTempDir, modelPath);

    return modelPath;
  }

  void removeModelTempDir(const openstudio::path& modelTempDir)
  {
    removeDir(toQString(modelTempDir));
  }

} // model
} // openstudio

