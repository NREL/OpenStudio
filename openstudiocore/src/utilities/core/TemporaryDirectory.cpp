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

#include "TemporaryDirectory.hpp"
#include <QTemporaryFile>
#include <QDir>

namespace openstudio
{
  TemporaryDirectory::TemporaryDirectory()
  {
    openstudio::path result;

    {
      QTemporaryFile f;
      if (f.open())
      {
        result = openstudio::toPath(longPathName(f.fileName()));
      } else {
        throw std::runtime_error("Unable to create a temporary file");
      }
    }

    LOG(Info, "Creating directory '" << toString(result));

    bool test = QDir().mkpath(openstudio::toQString(result));

    if (!test)
    {
      throw std::runtime_error("Unable to create a temporary folder: " + toString(result));
    }

    m_path = result;
  }

  TemporaryDirectory::~TemporaryDirectory()
  {
    try {
      boost::filesystem::remove_all(m_path);
    } catch (const std::exception &e) {
      LOG(Error, "Unable to remove temporary directory: " << openstudio::toString(m_path) << " error: " << e.what());
    }
  }


  openstudio::path TemporaryDirectory::path()
  {
    return m_path;
  }

}

