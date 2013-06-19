#ifndef UTILITIES_CORE_RUBYEXCEPTION
#define UTILITIES_CORE_RUBYEXCEPTION

#include <stdexcept>

#include <QRegExp>
#include <QString>

namespace openstudio
{

  class RubyException : public std::runtime_error
  {
    public:

      RubyException(const std::string& msg, const std::string& location)
        : std::runtime_error(msg), m_location(location)
      {}

      virtual ~RubyException() throw() {}

      std::string location() const {return m_location;}

      std::string shortLocation() const 
      {
        std::string result;

        QRegExp regex("(\\w*\\.rb:\\d*)");
        int pos = regex.indexIn(QString::fromStdString(m_location));
        if (pos > -1) {
          result = regex.cap(1).toStdString();
        }
        
        return result;
      }

    private:

      std::string m_location;

  };

} // openstudio

#endif // UTILITIES_CORE_RUBYEXCEPTION
