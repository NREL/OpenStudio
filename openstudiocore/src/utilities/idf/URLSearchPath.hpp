/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_URLSEARCHPATH_HPP
#define UTILITIES_IDF_URLSEARCHPATH_HPP

#include "../core/Path.hpp"
#include "../idd/IddEnums.hpp"
#include <boost/optional.hpp>
#include <QUrl>

namespace openstudio
{
  class URLSearchPath
  {
    public:
      enum Relative
      {
        ToInputFile,
        ToCurrentWorkingDir
      };

      /// Create a new URLSearchPath.
      /// \param[in] t_url Url to search (local or remote, remote not yet implemented)
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      URLSearchPath(const QUrl &t_url, Relative t_relativity = ToInputFile)
        : m_url(t_url), m_relativity(t_relativity)
      {
      }

      /// Create a new URLSearchPath.
      /// \param[in] t_url Url to search (local or remote, remote not yet implemented)
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      /// \param[in] t_object_type Object type to apply this search to
      URLSearchPath(const QUrl &t_url, Relative t_relativity, IddObjectType t_object_type)
        : m_url(t_url), m_relativity(t_relativity), m_object_type(t_object_type)
      {
      }


      /// Create a new URLSearchPath.
      /// \param[in] t_path local path to search
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      URLSearchPath(const openstudio::path &t_path, Relative t_relativity = ToInputFile)
        : m_url(QUrl::fromLocalFile(toQString(t_path))), m_relativity(t_relativity)
      {
      }

      /// Create a new URLSearchPath.
      /// \param[in] t_path local path to search
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      /// \param[in] t_object_type Object type to apply this search to
      URLSearchPath(const openstudio::path &t_path, Relative t_relativity, IddObjectType t_object_type)
        : m_url(QUrl::fromLocalFile(toQString(t_path))), m_relativity(t_relativity), m_object_type(t_object_type)
      {
      }


      QUrl getUrl() const
      {
        return m_url;
      }

      Relative getRelativity() const
      {
        return m_relativity;
      }

      boost::optional<IddObjectType> getIddObjectType() const
      {
        return m_object_type;
      }


    private:
      QUrl m_url;
      Relative m_relativity;
      boost::optional<IddObjectType> m_object_type;

  };

}


#endif // UTILITIES_IDF_URLSEARCHPATH_HPP
