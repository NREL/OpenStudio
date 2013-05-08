#include "PrjData.hpp"

#include <QFile>
//#include <iostream>
#include <math.h>
#include "IconDefs.hpp"
#include "PrjWriter.hpp"

namespace openstudio {
namespace contam {
namespace prj {

int Wall::length()
{
    int value = 0;
    if(type == Wall::Horizontal)
        value = end->col -  start->col;
    else if(type == Wall::Vertical)
        value = end->row -  start->row;
    return value;
}

void Wall::reorient()
{
    Icon *temp;
    if(length()<0)
    {
        temp = start;
        start = end;
        end = temp;
    }
}

void Wall::getZones(int nx, int ny, int *grid, QList<Zone> zones)
{
    int ij = start->col + start->row*nx;
    int right_ij, left_ij;
    if(type == Wall::Horizontal)
    {
        right_ij = ij + 1 + nx;
        left_ij = ij + 1 - nx;
    }
    else if(type == Wall::Vertical)
    {
        right_ij = ij + nx - 1;
        left_ij = ij + nx + 1;
    }
    else
        return; // Bail out!

    right = &(zones[grid[right_ij]]);
    left = &(zones[grid[left_ij]]);
}

Loop::Loop(Zone *zone, QList<Wall *> *onleft):zone(zone)
{
    QList<Wall*> unused;
    Wall *wall = onleft->takeFirst();
    Icon *end = wall->start;
    Icon *lookingFor = wall->end;
    walls << wall;
    valid = false;
    bool found = false;

    while(end != lookingFor) // This is a bad idea
    {
        while(onleft->size()) // Ugh
        {
            //std::cout << count++ << std::endl;
            wall = onleft->takeFirst();
            if(wall->start == lookingFor)
            {
                walls << wall;
                lookingFor = wall->end;
                found = true;
                break;
            }
            unused << wall;
        }
        if(!found)
            // If we ever get here, we have not found the icon we're lookingFor
            return;
        // Put back any unused walls
        while(unused.size())
            *onleft << unused.takeFirst();
    }
    // If we make it here, then we've found a complete loop
    valid = true;
}

Loop::~Loop()
{
    Wall *current;
    QListIterator<Wall*> i(walls);
    while(i.hasNext())
    {
        current = i.next();
        if(current->parent) // Delete those walls generated in buildLoops
            delete current;
    }
}

double Loop::area()
{
    double value=0.0,b,h;
    if(valid)
    {
        QListIterator<Wall*> i(walls);
        while(i.hasNext())
        {
            Wall *wall = i.next();
            b = wall->length();
            if(wall->type == Wall::Horizontal)
            {
                h = wall->start->row;
                b = -b;
            }
            else if(wall->type == Wall::Vertical)
            {
                h = wall->start->col;
            }
            else
            {
                // Somedaaaaay, your time will come!
                h = 0.0;
            }
            // std::cout << '*' << -0.5*b*h << std::endl;
            value -= 0.5*b*h;
        }
    }
    return value;
}

bool Loop::rectangular()
{
    bool result=false;
    double x=0.0, y=0.0;
    double tolerance = 1.0e-6;
    if(valid)
    {
        QListIterator<Wall*> i(walls);
        while(i.hasNext())
        {
            Wall *wall = i.next();
            if(wall->type==Wall::Horizontal)
                x += wall->length();
            else if(wall->type==Wall::Vertical)
                y += wall->length();
            // Add general here, someday
        }
        //std::cout<<x<<' '<<y<<std::endl;
        result = fabs(x+y) < tolerance;
    }
    return result;
}

ZoneGeometry::ZoneGeometry(Zone *zone, QList<Wall*> walls):zone(zone)
{
    valid=false;
    buildLoops(walls);
}

ZoneGeometry::~ZoneGeometry()
{
    qDeleteAll(loops);
}

void ZoneGeometry::buildLoops(QList<Wall*> walls)
{
    QList<Wall*> onleft;
    Wall *current;
    // Collect up the walls that border this zone
    QListIterator<Wall*> i(walls);
    while(i.hasNext()) // Ugh
    {
        current = i.next();
        if(current->left == zone)
            onleft << current;
        else if(current->right == zone) // Need to swap
            onleft << new Wall(current->end,current->start,current->type,
                               current->right,current->left,current);
    }
    // Create one or more loops out of the walls
    valid = true;
    QList<Wall*> unused = onleft;
    while(unused.size()) // Need a bailout or something
    {
        Loop *loop = new Loop(zone,&unused);
        loops << loop;
        if(!loop->valid)
            valid = false;
    }
}

double ZoneGeometry::area()
{
    double value = 0.0;
    if(valid)
    {
        QListIterator<Loop*> i(loops);
        while(i.hasNext()) // Ugh
            value += i.next()->area();
    }
    return value;
}

Data::Data(QString filename, bool process)
{
    QFile fp(filename);

    valid = false;
    if (fp.open(QFile::ReadOnly))
    {
        QTextStream stream(&fp);
        Reader input(&stream);

        // Section 1: Project, Weather, Simulation, and Output Controls
        rc.read(&input); // Read the run control section
        input.read999();
        // Section 2: Species and Contaminants
        ctmandspcs = input.storeSection(); // Skip it
        //contaminants = input.readIntArray(false);
        //species = input.readSection<Species>(); // Read the species section
        // Section 3: Level and Icon Data
        //nlevels = readSection(&input, &levels, QString("level")); // Read the levels section
        //levels = readSection<Level>(&input, QString("level")); // Read the levels section
        levels = input.readSection<Level>(); // Read the levels section
        // Section 4: Day Schedules
        dsch = input.storeSection(); // Skip it
        // Section 5: Week Schedules
        wsch = input.storeSection(); // Skip it
        // Section 6: Wind Pressure Profiles
        wpf = input.storeSection(); // Skip it
        // Section 7: Kinetic Reactions
        kinr = input.storeSection(); // Skip it
        // Section 8a: Filter Elements
        flte = input.storeSection(); // Skip it
        // Section 8b: Filters
        filt = input.storeSection(); // Skip it
        // Section 9: Source/Sink Elements
        cse = input.storeSection(); // Skip it
        // Section 10: Airflow Elements
        afe = input.storeSection(); // Skip it
        // Section 11: Duct Elements
        dfe = input.storeSection(); // Skip it
        // Section 12a: Control Super Elements
        selmt = input.storeSection(); // Skip it
        // Section 12b: Control Nodes
        ctrl = input.storeSection(); // Skip it
        // Section 13: Simple Air Handling System (AHS)
        ahs = input.readSection<Ahs>();
        // Section 14: Zones
        zones = input.readSection<Zone>();
        // Section 15: Initial Zone Concentrations
        zoneic = input.storeSection(); // Skip it
        // Section 16: Airflow Paths
        paths = input.readSection<Path>();
        // Section 17: Duct Junctions
        jct = input.storeSection(); // Skip it
        // Section 18: Initial Junction Concentrations
        jctic = input.storeSection(); // Skip it
        // Section 19: Duct Segments
        dct = input.storeSection(); // Skip it
        // Section 20: Source/Sinks
        css = input.storeSection(); // Skip it
        // Section 21: Occupancy Schedules
        osch = input.storeSection(); // Skip it
        // Section 22: Exposures
        pexp = input.storeSection(); // Skip it
        // Section 23: Annotations
        note = input.storeSection(); // Skip it
        input.readEnd();
        fp.close();
        /*
        if(process)
        {
            // Add an ambient zone
            Zone ambient;
            ambient.name = QString("Ambient");
            zones.prepend(ambient);
            //zones.prepend(new Zone());
            //zones[0]->name=QString("Ambient");
            // Process the levels to get the walls of each zone
            for(int k=0;k<levels.size();k++)
            {
                // Build a grid for the level
                int nij = rc.skwidth*rc.skheight;
                int * grid = new int[nij];
                grids << grid;
                for(int ij=0;ij<nij;ij++)
                    grid[ij] = 0;
                // Get all of the walls for this level
                QList <Wall*> walls = findWalls(rc.skwidth, rc.skheight, grid, levels[k].icons.size(), 
                    levels[k].icons);
                for(int i=0;i<walls.size();i++)
                    walls[i]->reorient();
                this->walls += walls;
                // Change all of the intersection to regular wall cells now that we have the walls
                for(int ij=0;ij<nij;ij++)
                    if(grid[ij] > 0)
                        grid[ij] = -1;
                // Fill to find the extents of zones using zone icons
                for(int i=0;i<levels[k].icons.size();i++)
                    if(levels[k].icons[i].icon == ZONE_ST)
                        fillFromPoint(zones[levels[k].icons[i].nr].nr, levels[k].icons[i].col, levels[k].icons[i].row,
                                rc.skwidth, rc.skheight, grid);
                // Now we can attach zones to each wall
                for(int i=0;i<walls.size();i++)
                    walls[i]->getZones(rc.skwidth, rc.skheight, grid, zones);
                // Finally, we can use this to build the loops that geometrically represent the zones
                for(int i=1;i<zones.size();i++)
                    geometry << new ZoneGeometry(&(zones[i]),walls);
            }
        }
        */
        valid = true;
    }
}

QString Data::write()
{
    QString out;
    if(valid)
    {
        // Section 1: Project, Weather, Simulation, and Output Controls
        out += (rc.write() + QString("\n-999\n"));
        // Section 2: Species and Contaminants
        //out += writeArray(contaminants, QString("contaminants:")) + QString('\n');
        //out += writeSection<Species>(species,QString("species:"));
        out += ctmandspcs;
        // Section 3: Level and Icon Data
        out += writeSection(levels,QString("levels plus icon data:"));
        // Section 4: Day Schedules
        out += dsch;
        // Section 5: Week Schedules
        out += wsch;
        // Section 6: Wind Pressure Profiles
        out += wpf;
        // Section 7: Kinetic Reactions
        out += kinr;
        // Section 8a: Filter Elements
        out += flte;
        // Section 8b: Filters
        out += filt;
        // Section 9: Source/Sink Elements
        out += cse;
        // Section 10: Airflow Elements
        out += afe;
        // Section 11: Duct Elements
        out += dfe;
        // Section 12a: Control Super Elements
        out += selmt;
        // Section 12b: Control Nodes
        out += ctrl;
        // Section 13: Simple Air Handling System (AHS)
        out += writeSection(ahs, QString("Simple AHS:"));
        // Section 14: Zones
        QList <Zone> outZones = zones;
        if(outZones.size())
            if(outZones[0].name == QString("Ambient"))
                outZones.takeFirst();
        out += writeSection(outZones, QString("zones:"));
        // Section 15: Initial Zone Concentrations
        out += zoneic;
        // Section 16: Airflow Paths
        //out += writeSection(paths, QString("flow paths:"));
        // Section 17: Duct Junctions
        out += jct;
        // Section 18: Initial Junction Concentrations
        out += jctic;
        // Section 19: Duct Segments
        out += dct;
        // Section 20: Source/Sinks
        out += css;
        // Section 21: Occupancy Schedules
        out += osch;
        // Section 22: Exposures
        out += pexp;
        // Section 23: Annotations
        out += note;
        out += QString("* end project file.\n");
    }
    return out;
}

bool Data::writeFile(QString filename)
{
    QString string;
    if(valid)
    {
        QFile fp(filename);
        if (fp.open(QFile::WriteOnly))
        {
            QTextStream out(&fp);
            // Section 1: Project, Weather, Simulation, and Output Controls
            out << (rc.write() + QString("\n-999\n"));
            // Section 2: Species and Contaminants
            //out << writeArray(contaminants, QString("contaminants:")) + QString('\n');
            //out << writeSection<Species>(species,QString("species:"));
            out << ctmandspcs;
            // Section 3: Level and Icon Data
            out << writeSection(levels,QString("levels plus icon data:"));
            // Section 4: Day Schedules
            out << dsch;
            // Section 5: Week Schedules
            out << wsch;
            // Section 6: Wind Pressure Profiles
            out << wpf;
            // Section 7: Kinetic Reactions
            out << kinr;
            // Section 8a: Filter Elements
            out << flte;
            // Section 8b: Filters
            out << filt;
            // Section 9: Source/Sink Elements
            out << cse;
            // Section 10: Airflow Elements
            out << afe;
            // Section 11: Duct Elements
            out << dfe;
            // Section 12a: Control Super Elements
            out << selmt;
            // Section 12b: Control Nodes
            out << ctrl;
            // Section 13: Simple Air Handling System (AHS)
            out << writeSection(ahs, QString("Simple AHS:"));
            // Section 14: Zones
            QList <Zone> outZones = zones;
            if(outZones.size())
                if(outZones[0].name == QString("Ambient"))
                    outZones.takeFirst();
            out << writeSection(outZones, QString("zones:"));
            // Section 15: Initial Zone Concentrations
            out << zoneic;
            // Section 16: Airflow Paths
            //out << writeSection(paths, QString("flow paths:"));
            // Section 17: Duct Junctions
            out << jct;
            // Section 18: Initial Junction Concentrations
            out << jctic;
            // Section 19: Duct Segments
            out << dct;
            // Section 20: Source/Sinks
            out << css;
            // Section 21: Occupancy Schedules
            out << osch;
            // Section 22: Exposures
            out << pexp;
            // Section 23: Annotations
            out << note;
            out << QString("* end project file.\n");
            fp.close();
            return true;
        }
    }
    return false;
}


int drawLine(int i, int j, int di, int dj, int value, int w, int h, int *skpd)
{
    int ij = i+j*w;
    if(skpd[ij] != 0)
        return 0;
    while(skpd[ij] == 0)
    {
        skpd[ij] = value;
        i+=di;
        if(i<0)
        {
            LOG_FREE(Error,"openstudio.contam.prj.Reader","Column index out of range: " << i << " < 0 ");
        }
        else if (i>=w)
        {
            LOG_FREE(Error,"openstudio.contam.prj.Reader","Column index out of range: " << i << " > " << w);
        }
        j+=dj;
        if(j<0)
        {
            LOG_FREE(Error,"openstudio.contam.prj.Reader","Row index out of range: " << j << " < 0 ");
        }
        else if (j>=h)
        {
            LOG_FREE(Error,"openstudio.contam.prj.Reader","Row index out of range: " << j << " > " << h);
        }
        ij = i+j*w;
    }
    return skpd[ij];
}

void drawWalls(int w, int h, int *skpd, int nicons, Icon *icons)
{
    //QList<Icon*> wallIcons;
    int ij;
    for(int ij=0;ij<w*h;ij++)
        skpd[ij] = 0;
    for(int i=0;i<nicons;i++)
    {
        if(icons[i].isWall())
        {
            /* Should check to see if icon is legit here */
            ij = icons[i].col + w*icons[i].row;
            skpd[ij] = i+1;
            //wallIcons << &(icons[i]);
        }
    }
//    for(int i=0;i<nicons;i++)
//    {
//        if(icons[i].isWall())
//        {
//            /* Should check to see if icon is legit here */
//            ij = icons[i].col + w*icons[i].row;
//            skpd[ij] = icons[i].nr;
//        }
//    }

}

QList<Wall*> findWalls(int w, int h, int *skpd, int nicons, QList<Icon> icons)
{
    //QList<Icon*> wallIcons;
    QList<Wall*> walls;
    int ij;
    int other;
    for(int ij=0;ij<w*h;ij++)
        skpd[ij] = 0;
    for(int i=0;i<nicons;i++)
    {
        if(icons[i].isWall())
        {
            /* Should check to see if icon is legit here */
            ij = icons[i].col + w*icons[i].row;
            skpd[ij] = i+1;
            //wallIcons << &(icons[i]);
        }
    }
    for(int i=0;i<nicons;i++)
    {
        if(icons[i].isWall())
        {
            uint bits = icons[i].bits();
            if(bits & BIT_E)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col+1, icons[i].row, 1, 0, -1, w, h, skpd);
                if(other)
                    walls << new Wall(&(icons[i]),&(icons[other-1]),Wall::Horizontal);
            }
            if(bits & BIT_S)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col, icons[i].row+1, 0, 1, -1, w, h, skpd);
                if(other)
                    walls << new Wall(&(icons[i]),&(icons[other-1]),Wall::Vertical);
            }
            /* These two will only result in actual changes if a non-canonical ordering is used */
            if(bits & BIT_W)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col-1, icons[i].row, -1, 0, -1, w, h, skpd);
                if(other)
                    walls << new Wall(&(icons[i]),&(icons[other-1]),Wall::Horizontal);
            }
            if(bits & BIT_N)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col, icons[i].row-1, 0, -1, -1, w, h, skpd);
                if(other)
                    walls << new Wall(&(icons[i]),&(icons[other-1]),Wall::Vertical);
            }
//            switch(icons[i].icon)
//            {
//            case(WL_ES):
//                other = drawLine(icons[i].col+1, icons[i].row, 1, 0, -1, w, h, skpd);
//                if(other)
//                    walls << new Wall(&(icons[i]),&(icons[other]));
//                other = drawLine(icons[i].col, icons[i].row+1, 0, 1, -1, w, h, skpd);
//                if(other)
//                    walls << new Wall(&(icons[i]),&(icons[other]));
//                break;
//            }
        }
    }
    return walls;
}


void fill2D(int tofill, int fillwith, int *chk, int *buf, int x, int y, int nx, int ny)
{
    uint xy = x+y*nx;
    buf[xy]=fillwith;
    if(y >= 1)
        if(chk[xy-nx]==tofill && buf[x-nx]!=fillwith)
            fill2D(tofill, fillwith, chk, buf, x, y-1, nx, ny);
    if(y+1 < ny)
        if(chk[xy+nx]==tofill && buf[xy+nx]!=fillwith)
            fill2D(tofill, fillwith, chk, buf, x, y+1, nx, ny);
    if(x >= 1)
        if(chk[xy-1]==tofill && buf[xy-1]!=fillwith)
            fill2D(tofill, fillwith, chk, buf, x-1, y, nx, ny);
    if(x+1 < nx)
        if(chk[xy+1]==tofill && buf[xy+1]!=fillwith)
            fill2D(tofill, fillwith, chk, buf, x+1, y, nx, ny);
    return;
}

void fillFromPoint(int value, int x, int y, int nx, int ny, int *map)
{
    int xy = x+y*nx;
    fill2D(map[xy],value,map,map,x,y,nx,ny);
}

QList<Wall*> zoneWalls(Zone *zone, QList<Wall *> walls)
{
    QList<Wall*> onleft;
    Wall *current;
    QListIterator<Wall*> i(walls);
    while(i.hasNext()) // Ugh
    {
        current = i.next();
        if(current->left == zone)
            onleft << current;
        else if(current->right == zone) // Need to swap
            onleft << new Wall(current->end,current->start,current->type,
                               current->right,current->left,current);
    }
    return onleft;
}

}
}
}
