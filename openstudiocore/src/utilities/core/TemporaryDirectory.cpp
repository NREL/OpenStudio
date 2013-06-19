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

