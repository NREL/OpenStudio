/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_CORE_PATH_HPP
#define UTILITIES_CORE_PATH_HPP

#include "../UtilitiesAPI.hpp"

#include <ostream>
#include <boost/optional.hpp>

#include "Filesystem.hpp"



// forward declarations
class QString;
class QTextStream;

namespace openstudio {



// allow wpath to be written to cout on Windows
UTILITIES_API ::std::ostream& operator<<(::std::ostream& os, const path& p);

/** path to a temporary directory. */
UTILITIES_API path tempDir();

/** path to std::string. */
UTILITIES_API std::string toString(const path& p);

/** path to QString. */
UTILITIES_API QString toQString(const path& p);

/** UTF-8 encoded char* to path*/
UTILITIES_API path toPath(const char* s);

/** UTF-8 encoded std::string to path*/
UTILITIES_API path toPath(const std::string& s);

/** QString to path*/
UTILITIES_API path toPath(const QString& q);

/** Optional path*/
typedef boost::optional<path> OptionalPath;

typedef std::pair<path,path> PathPair;

/// Return the fully expanded Windows path name ie "c:\Progra~1" into "C:\Program Files"
UTILITIES_API QString longPathName(const QString& path);

} // openstudio

// allow path to be written to QTextStream
QTextStream& operator<<(QTextStream& qts, const openstudio::path& p);


#endif // UTILITIES_CORE_PATH_HPP
