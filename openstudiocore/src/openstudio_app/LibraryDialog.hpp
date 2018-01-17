#include <QDialog>
#include <vector>
#include "../utilities/core/Filesystem.hpp"

class QListWidget;

namespace openstudio {

class LibraryDialog: public QDialog
{
  Q_OBJECT

 public:

  LibraryDialog(std::vector<openstudio::path> paths,std::vector<openstudio::path> defaultPaths);

  virtual ~LibraryDialog() {};

  /** The current list of library paths */
  std::vector<openstudio::path> paths() const;

 private:

  void onAdd();
  void onRemove();
  void onRestore();

  QListWidget * m_list;
  std::vector<openstudio::path> m_defaultPaths;
};

} // openstudio

