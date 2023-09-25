/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GENERATEIDDFACTORY_GENERATEIDDFACTORY_HPP
#define GENERATEIDDFACTORY_GENERATEIDDFACTORY_HPP

#include "IddFileFactoryData.hpp"
#include "../utilities/core/Filesystem.hpp"

#include <vector>

namespace openstudio {

using path = openstudio::filesystem::path;

/** Initialize one IddFileFactoryData object per Idd file, based on user input arguments. Throws
 *  on input error. */
IddFileFactoryDataVector constructIddFileObjects(const std::vector<std::string>& iddArgs);

/** Open the output file streams and start writing the output files. All generated files are
 *  prefixed with outFileHeader. Call this function before parsing the IDD files. */
void initializeOutFiles(GenerateIddFactoryOutFiles& outFiles, const std::vector<IddFileFactoryData>& iddFiles);

/** Finish writing the output files. Call this function after parsing all of the IDD files. */
void completeOutFiles(const openstudio::IddFileFactoryDataVector& iddFiles, GenerateIddFactoryOutFiles& outFiles);

/** Get the IddFileFactoryData object in iddFiles associated with IDD file fileName. Throws if
 *  the search is unsuccessful. */
openstudio::IddFileFactoryData getFile(const std::string& fileName, const openstudio::IddFileFactoryDataVector& iddFiles);

}  // namespace openstudio

#endif  // GENERATEIDDFACTORY_GENERATEIDDFACTORY_HPP
