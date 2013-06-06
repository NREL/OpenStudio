#ifndef PRJREADER_H
#define PRJREADER_H

#include <utilities/core/Logger.hpp>

#include <QTextStream>
#include <QStringList>
#include <QSharedPointer>

#include "PrjDefs.hpp"

CONTAMNAMESPACESTART
namespace prj
{

class Reader
{
public:
    explicit Reader(QTextStream *stream);
    explicit Reader(QString string, int starting=0);
    ~Reader();
    float readFloat(DECFILELINE);
    double readDouble(DECFILELINE);
    QString readString(DECFILELINE);
    int readInt(DECFILELINE);
    QString readLine(DECFILELINE);
    void read999(DECFILELINE);
    void read999(QString mesg DECCFILELINE);
    void readEnd(DECFILELINE);
    void skipSection(DECFILELINE);
    QString storeSection(DECFILELINE);
    int lineNumber(){return m_lineNumber;}
    QVector<int> readIntArray(DECFILELINEC bool terminated=false);
    template <class T> QList<T*> readSectionPointers(DECFILELINEC QString name=QString());
    template <class T> QList<T> readSectionList(DECFILELINEC QString name=QString());
    template <class T> QVector<T> readSectionVector(DECFILELINEC QString name=QString());
    template <class T> QVector<QSharedPointer<T> > readElementVector(DECFILELINEC QString name=QString());
    template <class T> T read(DECFILELINE);
    template <class T> T readNumber(DECFILELINE);


private:
    QTextStream *stream;
    int m_lineNumber;
    bool allocated;
    QStringList entries;
	void error(QString mesg DECCFILELINE);

	REGISTER_LOGGER("openstudio.contam.prj.Reader");
};

template <class T> QList<T*> Reader::readSectionPointers(DECFILELINEC QString name)
{
    QList<T*> list;
    T *object;
    int n = readInt(ARGFILELINE);
    for(int i=0;i<n;i++)
    {
        object = new T();
        list << object;
        object->read(this);
    }
    if(name.isNull())
        read999(QString("Failed to find section termination") ARGCFILELINE);
    else
        read999(QString("Failed to find %1 section termination").arg(name) ARGCFILELINE);
    return list;
}

template <class T> QList<T> Reader::readSectionList(DECFILELINEC QString name)
{
    QList<T> list;
    int n = readInt(ARGFILELINE);
    for(int i=0;i<n;i++)
    {
        T object;
        object.read(this);
        list << object;
    }
    if(name.isNull())
        read999(QString("Failed to find section termination") ARGCFILELINE);
    else
        read999(QString("Failed to find %1 section termination").arg(name) ARGCFILELINE);
    return list;
}

template <class T> QVector<T> Reader::readSectionVector(DECFILELINEC QString name)
{
    int n = readInt(ARGFILELINE);
    QVector<T> vector(n);
    for(int i=0;i<n;i++)
        vector[i].read(this);
    if(name.isNull())
        read999(QString("Failed to find section termination") ARGCFILELINE);
    else
        read999(QString("Failed to find %1 section termination").arg(name) ARGCFILELINE);
    return vector;
}

template <class T> QVector<QSharedPointer<T> > Reader::readElementVector(DECFILELINEC QString name)
{
    int n = readInt(ARGFILELINE);
    QVector<QSharedPointer<T> > vector(n);
    for(int i=0;i<n;i++)
	{
		// No reset in 4.8
        QSharedPointer<T> element(T::readElement(this));
        vector[i].swap(element);
        //vector[i].reset(T::readElement(this));
	}
    if(name.isNull())
        read999(QString("Failed to find section termination") ARGCFILELINE);
    else
        read999(QString("Failed to find %1 section termination").arg(name) ARGCFILELINE);
    return vector;
}

}
CONTAMNAMESPACEEND

#endif // PRJREADER_H
