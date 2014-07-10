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

#ifndef UTILITIES_IDF_URLSEARCHPATH_HPP
#define UTILITIES_IDF_URLSEARCHPATH_HPP

#include "../core/Path.hpp"
#include <utilities/idd/IddEnums.hxx>
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
