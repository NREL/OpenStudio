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
  #include <QColor>
  #include <QWidget>
  #include <QDialog>
  #include <QComboBox>
  #include <QMainWindow>
  #include <QAbstractItemModel>
  #include <QModelIndex>
  #include <QModelIndexList>
  #include <QDateTime> 
  #include <QDomElement>
  #include <QSqlQuery>
  #include <QSqlDatabase>
  #include <QUrl>
  #include <QNetworkRequest>
  #include <QNetworkReply>
  #include <QNetworkAccessManager>
  #include <QStandardItem>
  #include <QVariant>
  #include <QThread>
  #include <QFile>
  #include <QCoreApplication>
  #include <QApplication>
%}

class QObject
{
  QObject();
  QObject(QObject* parent);
};

%rename(Color) QColor;
class QColor
{
public:
    enum Spec { Invalid, Rgb, Hsv, Cmyk };

    QColor();
    QColor(int r, int g, int b, int a = 255);
    QColor(QRgb rgb);
    QColor(const QString& name);
    QColor(const char *name);
    QColor(const QColor &color);
    QColor(Qt::GlobalColor color);
    bool isValid() const;
    QString name() const;
    void setNamedColor(const QString& name);
    static QStringList colorNames();
    Spec spec() const;
    int alpha() const;
    void setAlpha(int alpha);
    double alphaF() const;
    void setAlphaF(qreal alpha);
    int red() const;
    int green() const;
    int blue() const;
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);
    void getRgb(int *r, int *g, int *b, int *a = 0) const;
    void setRgb(int r, int g, int b, int a = 255);
    int hue() const; // 0 <= hue < 360
    int saturation() const;
    int value() const;
    void getHsv(int *h, int *s, int *v, int *a = 0) const;
    void setHsv(int h, int s, int v, int a = 255);
    int cyan() const;
    int magenta() const;
    int yellow() const;
    int black() const;
    void getCmyk(int *c, int *m, int *y, int *k, int *a = 0);
    void setCmyk(int c, int m, int y, int k, int a = 255);
    QColor toRgb() const;
    QColor toHsv() const;
    QColor toCmyk() const;
    QColor convertTo(Spec colorSpec) const;
    static QColor fromRgb(int r, int g, int b, int a = 255);
    static QColor fromHsv(int h, int s, int v, int a = 255);
    static QColor fromCmyk(int c, int m, int y, int k, int a = 255);
    QColor light(int f = 150) const;
    QColor lighter(int f = 150) const;
    QColor dark(int f = 200) const;
    QColor darker(int f = 200) const;
    QColor &operator=(const QColor &);
    QColor &operator=(Qt::GlobalColor color);
    bool operator==(const QColor &c) const;
    bool operator!=(const QColor &c) const;
};

class QWidget : public QObject
{
public:
  void show();
  void hide();
  bool isWindow() const;
  QString windowTitle() const;
  void setWindowTitle(const QString &);
  bool isActiveWindow() const;
  void activateWindow();
  void raise();
  void lower(); 
  bool isAncestorOf(const QWidget* child) const;
  bool isEnabled() const;
  bool isEnabledTo(QWidget * ancestor) const;
  void setEnabled(bool enabled);
  bool isFullScreen() const;
  bool isHidden() const;
  bool isMaximized() const;
  bool isMinimized() const;
  bool isModal() const;
  bool isVisible() const;
  bool isVisibleTo(QWidget* ancestor) const;
  void setVisible(bool visible);
};

class QMainWindow : public QWidget
{};

class QDialog : public QWidget
{};

class QComboBox : public QWidget
{};

class QTextStream{};

class QRgb{};

class QString{};
  
%extend QString{      
  // to std::string
  std::string __str__() const{
    return toString(*self);
  }
}

class QDateTime{};

class QModelIndex{};

class QModelIndexList{};

%nodefaultctor QCoreApplication;
class QCoreApplication{};

%extend QCoreApplication{      

  void setApplicationName(const std::string& applicationName) const{
    self->setApplicationName(toQString(applicationName));
  }
    
  void setOrganizationName(const std::string& organizationName) const{
    self->setOrganizationName(toQString(organizationName));
  }
  void setOrganizationDomain(const std::string& organizationDomain) const{
    self->setOrganizationDomain(toQString(organizationDomain));
  }
}

%nodefaultctor QApplication;
class QApplication : public QCoreApplication
{};

class QFont{};

%nodefaultctor QAction;
class QAction : public QObject
{};

class QDomNode
{};

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

class QSqlQuery
{};

class QSqlDatabase
{};

class QStandardItem
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

class QThread : public QObject
{
 public:
  enum 	Priority { IdlePriority, LowestPriority, LowPriority, NormalPriority, InheritPriority };
  
  void 	exit ( int returnCode = 0 );
  bool 	isFinished () const;
  bool 	isRunning () const;
  void 	quit ();
  void 	start ( Priority priority = InheritPriority );
  void 	terminate ();
  
 protected:
  virtual void run();
  int exec();  
};

class QFile
{};

#endif

