#ifndef PRJDATA_H
#define PRJDATA_H
#include <QPoint>
#include <QSharedPointer>
#include <QVector>

#include "PrjObjects.hpp"
#include "PrjReader.hpp"

CONTAMNAMESPACESTART
namespace prj
{

class Wall
{
public:
    enum Type{General, Horizontal, Vertical};

    Wall(Icon *start, Icon *end, Type type, Zone *left=NULL, Zone *right=NULL)
        : start(start), end(end), type(type), left(left), right(right){}
    int length();
    void reorient();
    void positive();
    void getZones(int nx, int ny, int *grid, Zone *zones);
    void getZones(int nx, int ny, int *grid, QList<Zone*> zones);
    void getZones(int nx, int ny, int *grid, QList<Zone> &zones);
    void getZones(int nx, int ny, int *grid, QVector<Zone> &zones);

    Icon *start;
    Icon *end;
    Type type;
    Zone *left;  // Zone on the left side of this wall
    Zone *right; // Zone on the right side of this wall
    //Wall *other; // Pointer to wall that goes the other direction (if there is one)
};

class Loop
{
public:
    //Loop(Zone *zone, Wall *wall, QList<Wall*> walls);
    Loop(QList<QSharedPointer<Wall> > &onleft);
    //~Loop();
    bool rectangular();
    double area();
    QString summary();

    //Zone *zone;
    QList<QSharedPointer<Wall> > walls;
    bool valid;
};

class ZoneGeometry
{
public:
    //ZoneGeometry(Zone *zone):zone(zone){valid=false;}
    //ZoneGeometry(Zone *zone, QList<Wall*> walls);
    //ZoneGeometry(Zone *zone, QList<Wall> &walls);
    ZoneGeometry(Zone *zone, QList<QSharedPointer<Wall> > &walls);
    //~ZoneGeometry();
    double area();
    QString summary();

    Zone *zone;
    QList<QSharedPointer<Loop> > loops;
    bool valid;

private:
    //void buildLoops(QList<Wall*> walls);
    //void buildLoops(QList<Wall> &walls);
    void buildLoops(QList<QSharedPointer<Wall> > &walls);
};

class Data
{
public:
    Data(QString filename, bool process=true);
    //~PrjData();
    QString summary();
    bool write(QString filename);
	QString print();

    bool valid;
    RunControl rc;
    QVector<int> contaminants;
    QVector<Species> species;
    QVector<Level> levels;
    QVector<DaySchedule> daySchedules;
    QVector<WeekSchedule> weekSchedules;
    QVector<WindPressureProfile> windPressureProfiles;
    QString kinr;
    QVector<QSharedPointer<FilterElement> > filterElements;
    QString flte;
    QVector<Filter> filters;
    QString filt;
    QVector<QSharedPointer<SourceSinkElement> > sourceSinkElements;
    QVector<QSharedPointer<AirflowElement> > airflowElements;
    QString dfe;
    QString selmt;
    QString ctrl;
    QVector<Ahs> ahs;
    QVector<Zone> zones;
    QString zoneic;
    QVector<Path> paths;
    QString jct;
    QString jctic;
    QString dct;
    QVector <SourceSink> sourceSinks;
    QString osch;
    QString pexp;
    QString note;

    QList<QList<QSharedPointer<Wall> > > walls;
    QList<ZoneGeometry> geometry;
    QList<int*>grids;

private:
    void readZoneIC(Reader &input);
    QString writeZoneIC(int start);

};

//QList<QSharedPointer<Wall> > findWalls(int w, int h, int *skpd, QList<Icon> &icons);
QList<QSharedPointer<Wall> > findWalls(int w, int h, int *skpd, QVector<Icon> &icons);
//QList<Wall> findWalls(int w, int h, int *skpd, QList<Icon> &icons);
QList <QPoint> subdivide(int nx, int ny, int *map);
void fillFromPoint(int value, QPoint pt, int nx, int ny, int *map);
void fillFromPoint(int value, int x, int y, int nx, int ny, int *map);
QList<Wall*> zoneWalls(Zone *zone, QList<Wall *> walls);

template <class T> int readSection(Reader *input, T **objects, QString name)
{
    int n = input->readInt(FILELINE);
    *objects = new T[n];
    for(int i=0;i<n;i++)
    {
        (*objects)[i].read(input);
    }
    input->read999(QString("Failed to find %1 section termination").arg(name) CFILELINE);
    return n;
}

template <class T> QList<T*> readSection(Reader *input, QString name=QString())
{
    QList<T*> list;
    T *object;
    int n = input->readInt(FILELINE);
    for(int i=0;i<n;i++)
    {
        object = new T();
        list << object;
        object->read(input);
    }
    if(name.isNull())
        input->read999(QString("Failed to find section termination") CFILELINE);
    else
        input->read999(QString("Failed to find %1 section termination").arg(name) CFILELINE);
    return list;
}

}
CONTAMNAMESPACEEND

#endif // PRJDATA_H
