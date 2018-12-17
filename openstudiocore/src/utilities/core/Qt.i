#ifndef UTILITIES_CORE_QT_I
#define UTILITIES_CORE_QT_I

// get rid of Q_OBJECT macros
#define Q_OBJECT

// ignore these
#define Q_DECLARE_METATYPE(type)

// slots remain public or private
#define slots

// all signals are turned private
#define signals private

// DLM@20091231: we need to generalize our plotting stuff
namespace Qt{
  enum GlobalColor { white, black, red, darkRed, green, darkGreen, blue, darkBlue, cyan,
                     darkCyan, magenta, darkMagenta, yellow, darkYellow, gray, darkGray,
                     lightGray, transparent, color0, color1 };

  enum ConnectionType { AutoConnection=0,
                        DirectConnection=1,
                        QueuedConnection=2,
                        BlockingQueuedConnection=3,
                        UniqueConnection=0x80};
} // Qt


%{
  #include <QObject>
  #include <QDateTime>
  #include <QDomElement>
  #include <QUrl>
  #include <QNetworkRequest>
  #include <QNetworkReply>
  #include <QNetworkAccessManager>
  #include <QFile>
%}

class QObject
{
  QObject();
  QObject(QObject* parent);
};

class QString{};

%extend QString{
  // to std::string
  std::string __str__() const{
    return toString(*self);
  }
}

class QDomNode{};

class QDomElement : public QDomNode
{};

class QDomDocument : public QDomNode
{};

class QUrl
{};

%nodefaultctor QNetworkRequest;
class QNetworkRequest
{};

%nodefaultctor QNetworkReply;
class QNetworkReply
{};

%nodefaultctor QNetworkAccessManager;
class QNetworkAccessManager : public QObject
{};


#endif

