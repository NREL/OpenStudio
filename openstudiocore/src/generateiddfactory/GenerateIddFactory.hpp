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

#ifndef GENERATEIDDFACTORY_GENERATEIDDFACTORY_HPP
#define GENERATEIDDFACTORY_GENERATEIDDFACTORY_HPP

#include "IddFileFactoryData.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

#include <vector>

namespace openstudio {

typedef boost::filesystem::path path;

/** Initialize one IddFileFactoryData object per Idd file, based on user input arguments. Throws 
 *  on input error. */
IddFileFactoryDataVector constructIddFileObjects(const std::vector<std::string>& iddArgs);

/** Open the output file streams and start writing the output files. All generated files are 
 *  prefixed with outFileHeader. Call this function before parsing the IDD files. */
void initializeOutFiles(GenerateIddFactoryOutFiles& outFiles,
                        const std::vector<IddFileFactoryData>& iddFiles);

/** Finish writing the output files. Call this function after parsing all of the IDD files. */
void completeOutFiles(const openstudio::IddFileFactoryDataVector& iddFiles,
                      GenerateIddFactoryOutFiles& outFiles);

/** Get the IddFileFactoryData object in iddFiles associated with IDD file fileName. Throws if 
 *  the search is unsuccessful. */
openstudio::IddFileFactoryData getFile(const std::string& fileName,
                                       const openstudio::IddFileFactoryDataVector& iddFiles);

} // openstudio

#endif // GENERATEIDDFACTORY_GENERATEIDDFACTORY_HPP
