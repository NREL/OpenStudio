/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <eefg/Eefg.hpp>

#include <utilities/idd/IddFile.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idf/ValidityEnums.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/FileLogSink.hpp>
#include <utilities/core/String.hpp>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <set>

using namespace std;
using namespace boost;
using namespace openstudio;

/// Read in an idd, input idf and write out a "safe" idf
void __stdcall eefgValidateIdf(const char* _iddPath, const char* _idfPath, const char* _outPath)
{
  set<string> keepObjectTypes;
  
  keepObjectTypes.insert("Version");
  keepObjectTypes.insert("GlobalGeometryRules");
  keepObjectTypes.insert("Building");
  keepObjectTypes.insert("Zone");
  keepObjectTypes.insert("BuildingSurface:Detailed");
  keepObjectTypes.insert("FenestrationSurface:Detailed");
  keepObjectTypes.insert("Shading:Site:Detailed");
  keepObjectTypes.insert("Shading:Building:Detailed");
  keepObjectTypes.insert("Lights");
  keepObjectTypes.insert("People");
  keepObjectTypes.insert("ElectricEquipment");
  keepObjectTypes.insert("GasEquipment");

  path iddPath = toPath(_iddPath);
  path idfPath = toPath(_idfPath);
  path outPath = toPath(_outPath);
  
  // result is false initially
  bool result = false;

  // file to write valid idf objects to
  boost::filesystem::ofstream outFile;

  boost::optional<FileLogSink> logFile;

  // open the file
  outFile.open(outPath);
  try{

    // set the log file up after the idd is read so we don't see that output
    path logPath(outPath);
    logPath.replace_extension(toPath(".convlog").string());
    logFile = FileLogSink(logPath);
    logFile->setLogLevel(Debug);
    
    // read the idd
    OptionalIddFile oIdd = IddFile::load(iddPath);
    IddFile idd;
    if (oIdd) { idd = *oIdd; }

    // read the idf
    OptionalIdfFile oIdf;
    if (oIdd) { oIdf = IdfFile::load(idfPath, idd); }

    // check file
    if (oIdf) {
      IdfFile idf = *oIdf;

      // go through each object
      BOOST_FOREACH(IdfObject object, idf.objects()){
        
        // if this object type is in the keep list
        if (keepObjectTypes.find(object.iddObject().name()) != keepObjectTypes.end()){

          // if not valid go to next object
          if (!object.isValid(StrictnessLevel::Final)){
            continue;
          }

          // empty comment
          object.setComment("");

          // found at least one valid object
          result = true;

          // write object to file
          outFile << object;
        }
      } // BOOST_FOREACH
    } // if

  }catch(...){

    // the exception that this is catching should have already been logged

    // DLM@20091005: what if this is an exception about not being able to open outFile 
    result = false;

    LOG_FREE(Error, "eefg", "exception caught");
    
  }

  // close the out file
  outFile.close();

  LOG_FREE(Debug, "eefg", "complete");

  // remove the file appender
  logFile->disable();

  //return result;
  return;
}
