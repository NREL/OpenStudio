#include "RunManager_Util.hpp"
#include <QDateTime>

namespace openstudio {
namespace runmanager {


  FileInfo RunManager_Util::dirFile(QFileInfo fi) 
  {
    fi.refresh();
    return FileInfo(toString(fi.fileName()),
        (fi.exists()&&fi.lastModified().isValid())?toDateTime(fi.lastModified()):openstudio::DateTime(),
        toString(fi.fileName()), /* The default key is the filename */
        toPath(fi.absoluteFilePath()),
        fi.exists());
  }


  FileInfo RunManager_Util::dirFile(const openstudio::path &p)
  {
    return dirFile(QFileInfo(toQString(p)));
  }



}
}
