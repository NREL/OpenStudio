/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GENERATEIDDFACTORY_GENERATORAPPLICATIONPATHHELPERS_HPP
#define GENERATEIDDFACTORY_GENERATORAPPLICATIONPATHHELPERS_HPP

#include "../utilities/core/Filesystem.hpp"
#include <vector>

namespace openstudio {

using path = openstudio::filesystem::path;

/** If possible, creates an openstudio::path to a system directory from outdir, otherwise throws
 *  a std::runtime_error. For use by auto-generation applications that are writing files for
 *  OpenStudio. Requires that the path represented by outdir exist up to the parent level of the
 *  final directory. Outputs informative messages to std::cout. */
path getApplicationOutputDirectory(const std::string& outdir);

}  // namespace openstudio

#endif  //GENERATEIDDFACTORY_GENERATORAPPLICATIONPATHHELPERS_HPP
