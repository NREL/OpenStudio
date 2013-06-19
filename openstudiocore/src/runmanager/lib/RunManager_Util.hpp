#ifndef OPENSTUDIO_RUNMANAGER_UTIL_HPP__
#define OPENSTUDIO_RUNMANAGER_UTIL_HPP__

#include "FileInfo.hpp"
#include <QFileInfo>

namespace openstudio {
namespace runmanager {

  /// A small collection of utilities used internally by the RunManager
  class RunManager_Util {

    public: 
      /// \returns a fully populated FileInfo object from a QFileInfo object
      static FileInfo dirFile(QFileInfo fi);

      /// \returns a fully populated FileInfo object from a openstudio::path object
      static FileInfo dirFile(const openstudio::path &p);

  };

}
}

#endif
