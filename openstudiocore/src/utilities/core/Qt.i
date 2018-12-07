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
  #include <QVariant>
  #include <QFile>
%}

class QObject
{
  QObject();
  QObject(QObject* parent);
};

class QTextStream{};

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

#if defined SWIGJAVA
%rename(toQString) QVariant::toString;
#endif

%template(QVariantVector) std::vector<QVariant>;
%template(QVariantVectorVector) std::vector<std::vector<QVariant> >;
class QVariant {
 public:
  enum Type { Invalid = 0,
              Bool = 1,
              Int = 2,
              UInt = 3,
              LongLong = 4,
              ULongLong = 5,
              Double = 6,
              String = 10,
              Url = 17,
              UserType = 127 };

  QVariant();
  explicit QVariant( const QVariant& p);
  explicit QVariant( int val );
  explicit QVariant( uint val );
  explicit QVariant( bool val );
  explicit QVariant( double val );
  explicit QVariant( const char* val );
  explicit QVariant( const QString& val );
  explicit QVariant( const QUrl& val );
  ~QVariant();

  bool canConvert( Type t ) const;
  void clear();
  bool convert( Type t );
  bool isNull() const;
  bool isValid() const;
  bool toBool() const;
  double toDouble() const;
  int toInt() const;
  QString toString() const;
  uint toUInt() const;
  QUrl toUrl() const;
  Type type() const;
  const char * typeName() const;

  bool operator!=( const QVariant& v) const;
  QVariant& operator=( const QVariant& variant);
  bool operator==(const QVariant& v) const;

  static Type nameToType( const char* name);
  const char* typeToName( Type typ );
};


#endif

