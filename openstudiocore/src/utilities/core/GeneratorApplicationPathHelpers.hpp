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

#ifndef UTILITIES_CORE_GENERATORAPPLICATIONPATHHELPERS_HPP
#define UTILITIES_CORE_GENERATORAPPLICATIONPATHHELPERS_HPP

#include "../UtilitiesAPI.hpp"
#include <boost/filesystem/path.hpp>
#include <vector>

namespace openstudio {

typedef boost::filesystem::path path;

/** If possible, creates an openstudio::path to a system directory from outdir, otherwise throws 
 *  a std::runtime_error. For use by auto-generation applications that are writing files for 
 *  OpenStudio. Requires that the path represented by outdir exist up to the parent level of the
 *  final directory. Outputs informative messages to std::cout. */
UTILITIES_API path getApplicationOutputDirectory(const std::string& outdir);

} // openstudio

#endif //UTILITIES_CORE_PATHHELPERS_HPP
