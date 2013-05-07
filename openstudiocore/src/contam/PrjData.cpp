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

void Wall::getZones(int nx, int ny, int *grid, Zone *zones)
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

void Wall::getZones(int nx, int ny, int *grid, QList<Zone*> zones)
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

    right = zones[grid[right_ij]];
    left = zones[grid[left_ij]];
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
        // spcs = input.storeSection(); // Skip it
        contaminants = input.readIntArray(false);
        species = input.readSection<Species>(); // Read the species section
        //input.skipSection(__FILE__,__LINE__); // Skip it
        // Section 3: Level and Icon Data
        //nlevels = readSection(&input, &levels, QString("level")); // Read the levels section
        //levels = readSection<Level>(&input, QString("level")); // Read the levels section
        levels = input.readSection<Level>(); // Read the levels section
        //nlevels = levels.size();
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
        //nzones = readSection(&input, &zones, QString("zone")); // Read the zone section
        //nzones = readZones(&input, &zones); // Read the zone section
        //int zoneLine = input.lineNumber()+1;
        //QString zoneText = input.storeSection(__FILE__,__LINE__);
        //std::cout << zoneText.toStdString() << std::endl;
        //PrjReader zoneInput(zoneText,zoneLine);
        zones = input.readSection<Zone>();
        //return;
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
        if(process)
        {
            // Add an ambient zone
            zones.prepend(new Zone());
            zones[0]->name=QString("Ambient");
            // Process the levels to get the walls of each zone
            QListIterator<Level*> i(levels);
            while(i.hasNext())
            {
                Level *level = i.next();
                // Build a grid for the level
                int nij = rc.skwidth*rc.skheight;
                int * grid = new int[nij];
                grids << grid;
                for(int ij=0;ij<nij;ij++)
                    grid[ij] = 0;
                // Get all of the walls for this level
                QList <Wall*> walls = findWalls(rc.skwidth, rc.skheight, grid, level->nicon, level->icons);
                for(int i=0;i<walls.size();i++)
                    walls[i]->reorient();
                this->walls += walls;
                // Change all of the intersection to regular wall cells now that we have the walls
                for(int ij=0;ij<nij;ij++)
                    if(grid[ij] > 0)
                        grid[ij] = -1;
                // Fill to find the extents of zones using zone icons
                for(int i=0;i<level->nicon;i++)
                    if(level->icons[i].icon == ZONE_ST)
                        fillFromPoint(zones[level->icons[i].nr]->nr, level->icons[i].col, level->icons[i].row,
                                rc.skwidth, rc.skheight, grid);
                // Now we can attach zones to each wall
                for(int i=0;i<walls.size();i++)
                    walls[i]->getZones(rc.skwidth, rc.skheight, grid, zones);
                // Finally, we can use this to build the loops that geometrically represent the zones
                for(int i=1;i<zones.size();i++)
                    geometry << new ZoneGeometry(zones[i],walls);
            }
        }
        valid = true;
    }
}

Data::~Data()
{
    qDeleteAll(zones);
    qDeleteAll(levels);
    qDeleteAll(walls);
    qDeleteAll(geometry);
    qDeleteAll(grids);
}

QString Data::summary()
{
    QString text;
    QMap <int,QString> iconMap; // Should this be static or something?

    iconMap[FLOW_E] = QString("FLOW_E");
    iconMap[FLOW_W] = QString("FLOW_W");
    iconMap[FLOW_N] = QString("FLOW_N");
    iconMap[FLOW_S] = QString("FLOW_S");
    iconMap[ZONE_ST] = QString("ZONE_ST");
    iconMap[ZONE_PH] = QString("ZONE_PH");
    iconMap[ZONE_AM] = QString("ZONE_AM");
    iconMap[CONT_SS_C93] = QString("CONT_SS_C93");
    iconMap[WL_EW] = QString("WL_EW");
    iconMap[WL_NS] = QString("WL_NS");
    iconMap[WL_ES] = QString("WL_ES");
    iconMap[WL_SW] = QString("WL_SW");
    iconMap[WL_NW] = QString("WL_NW");
    iconMap[WL_NE] = QString("WL_NE");
    iconMap[WL_NES] = QString("WL_NES");
    iconMap[WL_ESW] = QString("WL_ESW");
    iconMap[WL_NSW] = QString("WL_NSW");
    iconMap[WL_NEW] = QString("WL_NEW");
    iconMap[WL_NESW] = QString("WL_NESW");
    iconMap[OPNG] = QString("OPNG");
    iconMap[DOOR] = QString("DOOR");
    iconMap[LG_OPNG] = QString("LG_OPNG");
    iconMap[LG_DOOR] = QString("LG_DOOR");
    iconMap[FAN_E] = QString("FAN_E");
    iconMap[FAN_W] = QString("FAN_W");
    iconMap[FAN_N] = QString("FAN_N");
    iconMap[FAN_S] = QString("FAN_S");
    iconMap[NOTE] = QString("NOTE");
    iconMap[BOX_S] = QString("BOX_S");
    iconMap[BOX_R] = QString("BOX_R");
    iconMap[S_AHS] = QString("S_AHS");
    iconMap[PEXP] = QString("PEXP");
    iconMap[PERSON] = QString("PERSON");
    iconMap[CONT_SS] = QString("CONT_SS");
    iconMap[CONT_RX] = QString("CONT_RX");
    iconMap[WMS_EW] = QString("WMS_EW");
    iconMap[WMS_NS] = QString("WMS_NS");
    iconMap[WDW_EW] = QString("WDW_EW");
    iconMap[WDW_NS] = QString("WDW_NS");
    iconMap[GEN_MASS] = QString("GEN_MASS");
    iconMap[RAD_INT] = QString("RAD_INT");
    iconMap[HEAT_SRC] = QString("HEAT_SRC");
    iconMap[DCT_EW] = QString("DCT_EW");
    iconMap[DCT_NS] = QString("DCT_NS");
    iconMap[DCT_EWk] = QString("DCT_EWk");
    iconMap[DCT_NSk] = QString("DCT_NSk");
    iconMap[DCT_ES] = QString("DCT_ES");
    iconMap[DCT_SW] = QString("DCT_SW");
    iconMap[DCT_NW] = QString("DCT_NW");
    iconMap[DCT_NE] = QString("DCT_NE");
    iconMap[DCT_X] = QString("DCT_X");
    iconMap[DCT_EWg] = QString("DCT_EWg");
    iconMap[DCT_NSg] = QString("DCT_NSg");
    iconMap[DCT_NSn] = QString("DCT_NSn");
    iconMap[DCT_EWe] = QString("DCT_EWe");
    iconMap[DCT_NSs] = QString("DCT_NSs");
    iconMap[DCT_EWw] = QString("DCT_EWw");
    iconMap[JCT] = QString("JCT");
    iconMap[JCT_CA] = QString("JCT_CA");
    iconMap[JCT_CB] = QString("JCT_CB");
    iconMap[JCT_CAB] = QString("JCT_CAB");
    iconMap[IOJ] = QString("IOJ");
    iconMap[IOJ_CA] = QString("IOJ_CA");
    iconMap[IOJ_CB] = QString("IOJ_CB");
    iconMap[CL_EW] = QString("CL_EW");
    iconMap[CL_NS] = QString("CL_NS");
    iconMap[CL_EWk] = QString("CL_EWk");
    iconMap[CL_NSk] = QString("CL_NSk");
    iconMap[CL_ES] = QString("CL_ES");
    iconMap[CL_SW] = QString("CL_SW");
    iconMap[CL_NW] = QString("CL_NW");
    iconMap[CL_NE] = QString("CL_NE");
    iconMap[CL_X] = QString("CL_X");
    iconMap[CA_N] = QString("CA_N");
    iconMap[CA_E] = QString("CA_E");
    iconMap[CA_S] = QString("CA_S");
    iconMap[CA_W] = QString("CA_W");
    iconMap[CS_N] = QString("CS_N");
    iconMap[CS_E] = QString("CS_E");
    iconMap[CS_S] = QString("CS_S");
    iconMap[CS_W] = QString("CS_W");
    iconMap[CTRLN] = QString("CTRLN");
    iconMap[CTRLP] = QString("CTRLP");
    iconMap[CTRLS] = QString("CTRLS");
    iconMap[CTRLO] = QString("CTRLO");
    iconMap[CTRLI] = QString("CTRLI");
    iconMap[RADIO_Y] = QString("RADIO_Y");
    iconMap[RADIO_N] = QString("RADIO_N");
    iconMap[BUTTON] = QString("BUTTON");
    iconMap[CHECK_L] = QString("CHECK_L");
    iconMap[CHECK_Y] = QString("CHECK_Y");
    iconMap[CHECK_R] = QString("CHECK_R");
    iconMap[CHECK_N] = QString("CHECK_N");
    iconMap[BLOCK] = QString("BLOCK");
    iconMap[BLOCK_B] = QString("BLOCK_B");
    iconMap[BLOCK_C] = QString("BLOCK_C");
    iconMap[BLOCK_T] = QString("BLOCK_T");
    iconMap[SIMGA] = QString("SIMGA");
    iconMap[MU] = QString("MU");
    iconMap[OMEGA] = QString("OMEGA");
    iconMap[DELTA] = QString("DELTA");
    iconMap[PLSMN] = QString("PLSMN");
    iconMap[GTEQ] = QString("GTEQ");
    iconMap[LTEQ] = QString("LTEQ");
    iconMap[SUB_M] = QString("SUB_M");
    iconMap[SUB_N] = QString("SUB_N");
    iconMap[APPROX] = QString("APPROX");
    iconMap[DEGRS] = QString("DEGRS");
    iconMap[EXP_4] = QString("EXP_4");
    iconMap[EXP_3] = QString("EXP_3");
    iconMap[SQRT] = QString("SQRT");
    iconMap[EXP_N] = QString("EXP_N");
    iconMap[EXP_2] = QString("EXP_2");
    iconMap[SUB_2] = QString("SUB_2");

    text += "Contaminants:";
    if(contaminants.size())
        for(int i=0;i<contaminants.size();i++)
            text += QString(" %1").arg(contaminants[i]);
    else
        text += " None";
    text += '\n';

    if(species.size())
        for(int i=0;i<species.size();i++)
            text += QString("Species %1: %2\n").arg(i+1).arg(species[i]->name);
    else
        text += "Species: None\n";

//    for(int i=0;i<levels.size();i++)
//    {
//        text += QString("Level \"%1\"\n").arg(levels[i]->name);
//        for(int j=0;j<levels[i]->nicon;j++)
//        {
//            //if(!levels[i]->icons[j].isWall())
//            //    continue;
//            text += QString("\tIcon %1: %2 (%3,%4)\n").arg(j,4).arg(iconMap[levels[i]->icons[j].icon],8)
//                    .arg(levels[i]->icons[j].col,3).arg(levels[i]->icons[j].row,3);
//        }
//    }
//    text += '\n';
#ifdef RX_IS_STRING
    double mul = rc.scale.toDouble()*rc.scale.toDouble();
#else
    double mul = rc.scale*rc.scale;
#endif
    for(int i=0;i<geometry.size();i++)
    {
        double levelHt,zoneVol,area;
#ifdef RX_IS_STRING
        levelHt = levels[geometry[i]->zone->pl-1]->delht.toDouble();
#else
        levelHt = levels[geometry[i]->zone->pl-1]->delht;
#endif
#ifdef RX_IS_STRING
        zoneVol = geometry[i]->zone->Vol.toDouble();
#else
        zoneVol = geometry[i]->zone->Vol;
#endif
        area = zoneVol/levelHt;
        text += QString("Zone \"%1\": area = %2 (%3) m^2, level %4\n").arg(geometry[i]->zone->name)
                .arg(mul*geometry[i]->area()).arg(area).arg(geometry[i]->zone->pl);
    }
    return text;
}

bool Data::write(QString filename)
{
    if(valid)
    {
        QFile fp(filename);
        if (fp.open(QFile::WriteOnly))
        {
            QTextStream out(&fp);
            // Section 1: Project, Weather, Simulation, and Output Controls
            out << (rc.write() + QString("\n-999\n"));
            // Section 2: Species and Contaminants
            out << writeArray(contaminants, QString("contaminants:")) + QString('\n');
            out << writeSection<Species>(species,QString("species:"));
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
            QList <Zone*> outZones = zones;
            if(outZones[0]->name == QString("Ambient"))
                outZones.takeFirst();
            out << writeSection(outZones, QString("zones:"));
            // Section 15: Initial Zone Concentrations
            out << zoneic;
            // Section 16: Airflow Paths
            out << writeSection(paths, QString("flow paths:"));
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

QList<Wall*> findWalls(int w, int h, int *skpd, int nicons, Icon *icons)
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
