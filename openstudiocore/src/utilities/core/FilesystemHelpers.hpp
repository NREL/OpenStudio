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

#ifndef UTILITIES_CORE_FILESYSTEMHELPERS_HPP
#define UTILITIES_CORE_FILESYSTEMHELPERS_HPP

#include "Filesystem.hpp"
#include <QByteArray>

namespace openstudio {
  namespace filesystem {
    /// reads entire file from the current read position until the end of file
    /// and returns as std::vector<char>
    UTILITIES_API std::vector<char> read(openstudio::filesystem::ifstream &t_path);

    /// reads entire file from the current read position until the end of file
    /// and returns as QByteArray
    UTILITIES_API QByteArray read_as_QByteArray(openstudio::filesystem::ifstream &t_file);

    /// reads entire file from the current read position until the end of file
    /// and returns as QByteArray
    UTILITIES_API QByteArray read_as_QByteArray(const openstudio::path &t_path);

    /// reads entire file from the current read position until the end of file
    /// and returns as std::string
    UTILITIES_API std::string read_as_string(openstudio::filesystem::ifstream &t_file);

    /// reads entire file from the current read position until the end of file
    /// and returns as std::string
    UTILITIES_API std::string read_as_string(const openstudio::path &t_path);

    /// Returns all file paths (non recursively) in the directory t_path.
    /// Result values are relative to t_path
    UTILITIES_API std::vector<openstudio::path> directory_files(const openstudio::path &t_path);

    /// Returns all file paths (recursively) in the directory t_path.
    /// Result values are relative to t_path
    UTILITIES_API std::vector<openstudio::path> recursive_directory_files(const openstudio::path &t_path);

    /// Copies file from src to dest. If there is an error, false is returned
    UTILITIES_API bool copy_file_no_throw(const openstudio::path &t_src, const openstudio::path &t_dest);

    // consider making this a operator<< overload. Did it this way for now to keep the changes straight
    UTILITIES_API void write(openstudio::filesystem::ofstream &t_file, const QString &);

    UTILITIES_API time_t last_write_time_as_time_t(const openstudio::path &t_path);
  }
}

#endif

