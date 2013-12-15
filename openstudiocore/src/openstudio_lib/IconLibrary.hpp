#include <openstudio_lib/OpenStudioAPI.hpp>

#include <QPixmap>
#include <map>

namespace openstudio {

class IddObjectType;

/*! This class is a simple singleton that maps IddObjectType.value() to QPixmaps
 *
 *
 * Pretty straightforward. The class stores pointers, there is no real advantage to using pointers over by value semantics
 * since Qt is doing internal referance counting in QPixMap anyway. Just check the pointer when you do a find operation on
 * it, becuase it might return NULL.
 *
 */
class OPENSTUDIO_API IconLibrary
{

public:
  //! If the class in not instantiated, this call will instantiate it. (So don't ever worry about it, just call Instance when you want to use the class)
  static IconLibrary& Instance();
  /*! gets an Icon
   \param val should be an IddObjectType.value()
   \return either a pointer to the Icon, or NULL if it isn't found
  */
  const QPixmap* findIcon( const IddObjectType & ) const;
  /*! gets a  MiniIcon
   \param val should be an IddObjectType.value()
   \return either a pointer to the MiniIcon, or NULL if it isn't found
  */
  const QPixmap* findMiniIcon( const IddObjectType & ) const;

protected:
  
private:
  
  IconLibrary();
  IconLibrary(const IconLibrary&);
  IconLibrary& operator=(const IconLibrary&);
  
  static IconLibrary* s_instance;
  
  std::map<IddObjectType, QPixmap*> m_icons;
  std::map<IddObjectType, QPixmap*> m_miniIcons;
};

}

