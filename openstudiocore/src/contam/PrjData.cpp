#include "PrjData.hpp"

#include <QFile>
#include <iostream>
#include <math.h>

#include "PrjDefs.hpp"
#include "PrjWriter.hpp"
#include "Error.hpp"

CONTAMNAMESPACESTART
namespace prj
{

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
    temp = start;
    start = end;
    end = temp;
    Zone *zone;
    zone = left;
    left = right;
    right = zone;
}

void Wall::positive()
{
    Icon *temp;
    if(length()<0)
    {
        temp = start;
        start = end;
        end = temp;
        Zone *zone;
        zone = left;
        left = right;
        right = zone;
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

void Wall::getZones(int nx, int ny, int *grid, QList<Zone> &zones)
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

void Wall::getZones(int nx, int ny, int *grid, QVector<Zone> &zones)
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

Loop::Loop(QList<QSharedPointer<Wall> > &onleft)
{
    QList<QSharedPointer<Wall> > unused;
    QSharedPointer<Wall> wall = onleft.takeFirst();
    Icon *end = wall->start;
    Icon *lookingFor = wall->end;
    walls << wall;
    valid = false;
    bool found = false;
    //int count = 0;
    while(end != lookingFor) // This is a bad idea and should be rewritten
    {
        if(!onleft.size())
            error("Failed to build loop - insufficient walls available" CFILELINE);
        while(onleft.size()) // Ugh
        {
            wall = onleft.takeFirst();
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
            onleft << unused.takeFirst();
    }
    // If we make it here, then we've found a complete loop
    valid = true;
    //std::cout << summary().toStdString() << std::endl;
}

//Loop::~Loop()
//{
//    Wall *current;
//    QListIterator<Wall*> i(walls);
//    while(i.hasNext())
//    {
//        current = i.next();
//        //if(current->parent) // Delete those walls generated in buildLoops
//        //    delete current;
//    }
//}

double Loop::area()
{
    double value=0.0,b,h;
    //std::cout << summary().toStdString() << std::endl;
    //std::cout << "Loop address: " << this << " " << walls[0]->left->name.toStdString() << std::endl;
    if(valid)
    {
        foreach(QSharedPointer<Wall> wall, walls)
        //QListIterator<Wall*> i(walls);
        //while(i.hasNext())
        {
            //Wall *wall = i.next();
            b = wall->length();
            std::cout << wall->start->row << " " << wall->start->col << std::endl;
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
            std::cout << '*' << b << " " << h << " " << -0.5*b*h << std::endl;
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
        foreach(QSharedPointer<Wall> wall, walls)
        {
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

QString Loop::summary()
{
    QString string = QString("Loop : valid = %1\n").arg(valid);
    for(int i=0;i<walls.size();i++)
    {
        string += QString("\t%1 (%2,%3) -> (%4,%5) %6\n").arg(i).arg(walls[i]->start->row).arg(walls[i]->start->col)
                .arg(walls[i]->end->row).arg(walls[i]->end->col).arg(walls[i]->length());
    }
    return string;
}

//ZoneGeometry::ZoneGeometry(Zone *zone, QList<Wall*> walls):zone(zone)
//{
//    valid=false;
//    buildLoops(walls);
//}

ZoneGeometry::ZoneGeometry(Zone *zone, QList<QSharedPointer<Wall> > &walls):zone(zone)
{
    valid=false;
    buildLoops(walls);
}

//ZoneGeometry::~ZoneGeometry()
//{
//    qDeleteAll(loops);
//}

/*
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
*/

void ZoneGeometry::buildLoops(QList<QSharedPointer<Wall> > &walls)
{
    QList<QSharedPointer<Wall> > onleft;
    // Collect up the walls that border this zone
    for(int i=0;i<walls.size();i++)
    {
        if(walls[i]->left == zone)
            onleft << walls[i];
    }
    // Create one or more loops out of the walls
    valid = true;
    QList<QSharedPointer<Wall> > unused = onleft;
    while(unused.size()) // Need a bailout or something
    {
        //std::cout << unused.size() << std::endl;
        //Loop loop(unused);
        loops << QSharedPointer<Loop>(new Loop(unused));
        //if(!loop.valid)
        //    valid = false;
    }
}

double ZoneGeometry::area()
{
    double value = 0.0;
    if(valid)
    {
        for(int i=0;i<loops.size();i++)
        {
            value += loops[i]->area();
            std::cout << "&& " << value <<std::endl;
        }
    }
    return value;
}

QString ZoneGeometry::summary()
{
    QString string = QString("ZoneGeometry: %1 loop(s), valid = %2\n").arg(loops.size()).arg(valid);
    for(int i=0;i<loops.size();i++)
    {
        string += QString().sprintf("%p",&(loops[i])) + " "  + loops[i]->summary();
    }
    return string;
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
        input.read999(FILELINE);
        // Section 2: Species and Contaminants
        contaminants = input.readIntArray(FILELINEC false);
        species = input.readSectionVector<Species>(FILELINEC QString("species"));
        // Section 3: Level and Icon Data
        levels = input.readSectionVector<Level>(FILELINEC QString("level"));
        // Section 4: Day Schedules
        daySchedules = input.readSectionVector<DaySchedule>(FILELINEC QString("day schedule"));
        // Section 5: Week Schedules
        weekSchedules = input.readSectionVector<WeekSchedule>(FILELINEC QString("week schedule"));
        // Section 6: Wind Pressure Profiles
        windPressureProfiles = input.readSectionVector<WindPressureProfile>(FILELINEC QString("wind pressure profiles"));
        // Section 7: Kinetic Reactions
        kinr = input.storeSection(FILELINE); // Skip it
        // Section 8a: Filter Elements
        //filterElements = input.readElementVector<FilterElement>(FILELINEC QString("filter element"));
        flte = input.storeSection(FILELINE); // Skip it
        // Section 8b: Filters
        filters = input.readSectionVector<Filter>(FILELINEC QString("filter"));
        //filt = input.storeSection(FILELINE); // Skip it
        // Section 9: Source/Sink Elements
        sourceSinkElements = input.readElementVector<SourceSinkElement>(FILELINEC QString("source/sink element"));
        // Section 10: Airflow Elements
        airflowElements = input.readElementVector<AirflowElement>(FILELINEC QString("airflow element"));
        // Section 11: Duct Elements
        dfe = input.storeSection(FILELINE); // Skip it
        // Section 12a: Control Super Elements
        selmt = input.storeSection(FILELINE); // Skip it
        // Section 12b: Control Nodes
        ctrl = input.storeSection(FILELINE); // Skip it
        // Section 13: Simple Air Handling System (AHS)
        ahs = input.readSectionVector<Ahs>(FILELINEC QString("ahs"));
        // Section 14: Zones
        zones = input.readSectionVector<Zone>(FILELINEC QString("zone"));
        // Section 15: Initial Zone Concentrations
        readZoneIC(input);
        // Section 16: Airflow Paths
        paths = input.readSectionVector<Path>(FILELINEC QString("path"));
        // Section 17: Duct Junctions
        jct = input.storeSection(FILELINE); // Skip it
        // Section 18: Initial Junction Concentrations
        jctic = input.storeSection(FILELINE); // Skip it
        // Section 19: Duct Segments
        dct = input.storeSection(FILELINE); // Skip it
        // Section 20: Source/Sinks
        sourceSinks = input.readSectionVector<SourceSink>(FILELINEC QString("source/sink"));
        // Section 21: Occupancy Schedules
        osch = input.storeSection(FILELINE); // Skip it
        // Section 22: Exposures
        pexp = input.storeSection(FILELINE); // Skip it
        // Section 23: Annotations
        note = input.storeSection(FILELINE); // Skip it
        input.readEnd(FILELINE);
        fp.close();
        if(process)
        {
            // Add an ambient zone
            zones.prepend(Zone());
            zones[0].name=QString("Ambient");
            // Process the levels to get the walls of each zone
            for(int k=0;k<levels.size();k++)
            {
                Level *level = &(levels[k]);
                // Build a grid for the level
                int nij = rc.skwidth*rc.skheight;
                int *grid = new int[nij];
                grids << grid;
                for(int ij=0;ij<nij;ij++)
                    grid[ij] = 0;
                // Get all of the walls for this level
                QList <QSharedPointer<Wall> > walls = findWalls(rc.skwidth, rc.skheight, grid, level->icons);
                // Change all of the intersections to regular wall cells now that we have the walls drawn
                for(int ij=0;ij<nij;ij++)
                    if(grid[ij] > 0)
                        grid[ij] = -1;
                // Fill to find the extents of zones using zone icons
                for(int i=0;i<level->icons.size();i++)
                {
                    if(level->icons[i].icon == ZONE_ST)
                        fillFromPoint(zones[level->icons[i].nr].nr, level->icons[i].col, level->icons[i].row,
                                rc.skwidth, rc.skheight, grid);
                    else if(level->icons[i].icon == ZONE_PH)
                    {
                        if(k==0)
                            error("Illegal phantom zone on first level" CFILELINE);
                        int ij = level->icons[i].col + level->icons[i].row*rc.skwidth;
                        fillFromPoint(grids[k-1][ij], level->icons[i].col, level->icons[i].row,
                                rc.skwidth, rc.skheight, grid);
                    }
                }
                // Now we can attach zones to each wall
                QList <QSharedPointer<Wall> > others;
                for(int i=0;i<walls.size();i++)
                {
                    //std::cout << zones[0].name.toStdString() << " " << zones[0].nr << std::endl;
                    walls[i]->getZones(rc.skwidth, rc.skheight, grid, zones);
                    // std::cout << i << ' ' << walls[i]->left->nr << ' ' << walls[i]->right->nr << ' ' << walls[i]->left->name.toStdString() << std::endl;
                    // Don't need left-hand side walls for the ambient zone
                    if(walls[i]->left->nr == 0)
                        walls[i]->reorient();
                    else if(walls[i]->right->nr != 0)
                        others << QSharedPointer<Wall>(new Wall(walls[i]->end,walls[i]->start,walls[i]->type,walls[i]->right,walls[i]->left));
                }
                walls += others;
                this->walls << walls;
                // Finally, we can use this to build the loops that geometrically represent the zones
                for(int i=1;i<zones.size();i++)
                {
                    ZoneGeometry geom(&(zones[i]),walls);
                    geometry << geom;
                }
            }
        }
        valid = true;
    }
}

void Data::readZoneIC(Reader &input)
{
    int nn = input.readInt(FILELINE);
    if(nn != 0)
    {
        int nctm = contaminants.size();
        if(nn != nctm*zones.size())
            error("Mismatch between number of zones, contaminants, and initial conditions" CFILELINE);
        for(int i=0;i<zones.size();i++)
        {
            int nr = input.readInt(FILELINE);
            if(nr != i+1)
                error(QString("Mismatch between zone IC number and zone number at line %1 ").arg(input.lineNumber()) CFILELINE);
            zones[i].ic.resize(nctm);
            for(int j=0;j<nctm;j++)
                zones[i].ic[j] = input.readNumber<RX>(FILELINE);
        }
    }
    input.read999(QString("Failed to find zone IC section termination") CFILELINE);
}

QString Data::writeZoneIC(int start)
{
    QString string = QString("%1 ! initial zone concentrations:").arg(contaminants.size()*(zones.size()-start));
    if(contaminants.size())
    {
        for(int i=start;i<zones.size();i++)
        {
            string += QString("\n%1").arg(i);
		    int ncc = qMin(contaminants.size(),zones[i].ic.size());
		    if(ncc != contaminants.size())
			    std::cout << "Warning: mismatch in zone IC size and contaminant count" << std::endl;
            for(int j=0;j<ncc;j++)
                string += QString(" %1").arg(zones[i].ic[j]);
        }
    }
    return string  + QString("\n-999");
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
            text += QString("Species %1: %2\n").arg(i+1).arg(species[i].name);
    else
        text += "Species: None\n";

    for(int i=0;i<levels.size();i++)
    {
        text += QString("Level \"%1\"\n").arg(levels[i].name);
        if(walls.size())
            for(int j=0;j<walls[i].size();j++)
                text += QString("    Wall %1: %2 -> %3, left: %4, right: %5\n").arg(j).arg(walls[i][j]->start->nr)
                        .arg(walls[i][j]->end->nr).arg(walls[i][j]->left->nr).arg(walls[i][j]->right->nr);
        for(int j=0;j<levels[i].icons.size();j++)
        {
            if(!levels[i].icons[j].isWall())
                continue;
            text += QString("    Icon %1: %2 (%3,%4) nr %5\n").arg(j,4).arg(iconMap[levels[i].icons[j].icon],8)
                    .arg(levels[i].icons[j].col,3).arg(levels[i].icons[j].row,3).arg(levels[i].icons[j].nr);
        }
    }
    text += '\n';
#ifdef RX_IS_STRING
    double mul = rc.scale.toDouble()*rc.scale.toDouble();
#else
    double mul = rc.scale*rc.scale;
#endif
    for(int i=0;i<geometry.size();i++)
    {
        double levelHt,zoneVol,area;
#ifdef RX_IS_STRING
        levelHt = levels[geometry[i].zone->pl-1].delht.toDouble();
#else
        levelHt = levels[geometry[i].zone->pl-1]->delht;
#endif
#ifdef RX_IS_STRING
        zoneVol = geometry[i].zone->Vol.toDouble();
#else
        zoneVol = geometry[i].zone->Vol;
#endif
        area = zoneVol/levelHt;
        text += QString("Zone \"%1\" (%5): area = %2 (%3) m^2, level %4\n").arg(geometry[i].zone->name)
                .arg(mul*geometry[i].area()).arg(area).arg(geometry[i].zone->pl).arg(QString().sprintf("%p",&(geometry[i])));
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
            out << writeSection(daySchedules,QString("day-schedules:"));
            // Section 5: Week Schedules
            out << writeSection(weekSchedules,QString("day-schedules:"));
            // Section 6: Wind Pressure Profiles
            out << writeSection(windPressureProfiles,QString("wind pressure profiles:"));
            // Section 7: Kinetic Reactions
            out << kinr;
            // Section 8a: Filter Elements
            //out << writeSection(filterElements,QString("filter elements:"));
            out << flte;
            // Section 8b: Filters
            out << writeSection(filters,QString("filters:"));
            //out << filt;
            // Section 9: Source/Sink Elements
            out << writeSection(sourceSinkElements,QString("source/sink elements:"));
            // Section 10: Airflow Elements
            out << writeSection(airflowElements,QString("flow elements:"));
            // Section 11: Duct Elements
            out << dfe;
            // Section 12a: Control Super Elements
            out << selmt;
            // Section 12b: Control Nodes
            out << ctrl;
            // Section 13: Simple Air Handling System (AHS)
            out << writeSection(ahs, QString("Simple AHS:"));
            // Section 14: Zones
            int start=0;
            if(zones[0].name == QString("Ambient"))
                start = 1;
            out << writeSection(zones, QString("zones:"),start);
            // Section 15: Initial Zone Concentrations
            //out << zoneic;
            out << writeZoneIC(start) + '\n';
            // Section 16: Airflow Paths
            out << writeSection(paths,QString("flow paths:"));
            // Section 17: Duct Junctions
            out << jct;
            // Section 18: Initial Junction Concentrations
            out << jctic;
            // Section 19: Duct Segments
            out << dct;
            // Section 20: Source/Sinks
            out << writeSection(sourceSinks,QString("source/sinks:"));
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

QString Data::print()
{
    QString out;
    if(valid)
    {
        // Section 1: Project, Weather, Simulation, and Output Controls
        out += (rc.write() + QString("\n-999\n"));
        // Section 2: Species and Contaminants
        out += writeArray(contaminants, QString("contaminants:")) + QString('\n');
        out += writeSection<Species>(species,QString("species:"));
        // Section 3: Level and Icon Data
        out += writeSection(levels,QString("levels plus icon data:"));
        // Section 4: Day Schedules
        out += writeSection(daySchedules,QString("day-schedules:"));
        // Section 5: Week Schedules
        out += writeSection(weekSchedules,QString("day-schedules:"));
        // Section 6: Wind Pressure Profiles
        out += writeSection(windPressureProfiles,QString("wind pressure profiles:"));
        // Section 7: Kinetic Reactions
        out += kinr;
        // Section 8a: Filter Elements
        //out << writeSection(filterElements,QString("filter elements:"));
        out += flte;
        // Section 8b: Filters
        out += writeSection(filters,QString("filters:"));
        //out << filt;
        // Section 9: Source/Sink Elements
        out += writeSection(sourceSinkElements,QString("source/sink elements:"));
        // Section 10: Airflow Elements
        out += writeSection(airflowElements,QString("flow elements:"));
        // Section 11: Duct Elements
        out += dfe;
        // Section 12a: Control Super Elements
        out += selmt;
        // Section 12b: Control Nodes
        out += ctrl;
        // Section 13: Simple Air Handling System (AHS)
        out += writeSection(ahs, QString("Simple AHS:"));
        // Section 14: Zones
        int start=0;
        if(zones[0].name == QString("Ambient"))
            start = 1;
        out += writeSection(zones, QString("zones:"),start);
        // Section 15: Initial Zone Concentrations
        //out << zoneic;
        out += writeZoneIC(start) + '\n';
        // Section 16: Airflow Paths
        out += writeSection(paths,QString("flow paths:"));
        // Section 17: Duct Junctions
        out += jct;
        // Section 18: Initial Junction Concentrations
        out += jctic;
        // Section 19: Duct Segments
        out += dct;
        // Section 20: Source/Sinks
        out += writeSection(sourceSinks,QString("source/sinks:"));
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
            error(QString("Column index out of range: %1 < 0 ").arg(i) CFILELINE);
        else if (i>=w)
            error(QString("Column index out of range: %1 > %2 ").arg(i).arg(w) CFILELINE);
        j+=dj;
        if(j<0)
            error(QString("Row index out of range: %1 < 0 ").arg(j) CFILELINE);
        else if (j>=h)
            error(QString("Row index out of range: %1 > %2 ").arg(j).arg(h) CFILELINE);
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

QList<QSharedPointer<Wall> > findWalls(int w, int h, int *skpd, QVector<Icon> &icons)
{
    //QList<Icon*> wallIcons;
    QList<QSharedPointer<Wall> > walls;
    int ij;
    int other;
    for(int ij=0;ij<w*h;ij++)
        skpd[ij] = 0;
    for(int i=0;i<icons.size();i++)
    {
        if(icons[i].isWall())
        {
            /* Should check to see if icon is legit here */
            ij = icons[i].col + w*icons[i].row;
            skpd[ij] = i+1;
            //wallIcons << &(icons[i]);
        }
    }
    for(int i=0;i<icons.size();i++)
    {
        if(icons[i].isWall())
        {
            uint bits = icons[i].bits();
            if(bits & BIT_E)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col+1, icons[i].row, 1, 0, -1, w, h, skpd);
                if(other)
                {
                    //Wall wall(&(icons[i]),&(icons[other-1]),Wall::Horizontal);
                    //walls << wall;
                    walls << QSharedPointer<Wall>(new Wall(&(icons[i]),&(icons[other-1]),Wall::Horizontal));
                }
            }
            if(bits & BIT_S)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col, icons[i].row+1, 0, 1, -1, w, h, skpd);
                if(other)
                {
                    //Wall wall(&(icons[i]),&(icons[other-1]),Wall::Vertical);
                    //walls << wall;
                    walls << QSharedPointer<Wall>(new Wall(&(icons[i]),&(icons[other-1]),Wall::Vertical));
                }
            }
            /* These two will only result in actual changes if a non-canonical ordering is used */
            if(bits & BIT_W)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col-1, icons[i].row, -1, 0, -1, w, h, skpd);
                if(other)
                {
                    //Wall wall(&(icons[i]),&(icons[other-1]),Wall::Horizontal);
                    //walls << wall;
                    walls << QSharedPointer<Wall>(new Wall(&(icons[i]),&(icons[other-1]),Wall::Horizontal));
                }
            }
            if(bits & BIT_N)
            {
                //std::cout << i << ' ' << bits << std::endl;
                other = drawLine(icons[i].col, icons[i].row-1, 0, -1, -1, w, h, skpd);
                if(other)
                {
                    //Wall wall(&(icons[i]),&(icons[other-1]),Wall::Vertical);
                    //walls << wall;
                    walls << QSharedPointer<Wall>(new Wall(&(icons[i]),&(icons[other-1]),Wall::Vertical));
                }
            }
        }
    }
    return walls;
}

void fill2D(int tofill, int fillwith, int *chk, int *buf, int x, int y, int nx, int ny)
{
    int xy = x+y*nx;
    buf[xy]=fillwith;
    if(y >= 1)
        if(chk[xy-nx]==tofill && buf[xy-nx]!=fillwith)
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

QList <QPoint> subdivide(int nx, int ny, int *map)
{
    int ij=nx, value = 1;
    QList <QPoint> anchors;
    anchors << QPoint(0,0);
    fill2D(map[0],value,map,map,0,0,nx,ny);
    value = 2;
    for(int y=1;y<ny;y++) // Disallow drawing in the first row, so skip it
        for(int x=0;x<nx;x++)
        {
            if(map[ij] == 0)
            {
                anchors << QPoint(x,y);
                fill2D(map[ij],value,map,map,x,y,nx,ny);
                value += 1;
            }
            ij++;
        }
    return anchors;
}

void fillFromPoint(int value, QPoint pt, int nx, int ny, int *map)
{
    int xy = pt.x()+pt.y()*nx;
    fill2D(map[xy],value,map,map,pt.x(),pt.y(),nx,ny);
}

void fillFromPoint(int value, int x, int y, int nx, int ny, int *map)
{
    int xy = x+y*nx;
    fill2D(map[xy],value,map,map,x,y,nx,ny);
}

//QList<Wall*> zoneWalls(Zone *zone, QList<Wall *> walls)
//{
//    QList<Wall*> onleft;
//    Wall *current;
//    QListIterator<Wall*> i(walls);
//    while(i.hasNext()) // Ugh
//    {
//        current = i.next();
//        if(current->left == zone)
//            onleft << current;
//        else if(current->right == zone) // Need to swap
//            onleft << new Wall(current->end,current->start,current->type,
//                               current->right,current->left,current);
//    }
//    return onleft;
//}

}
CONTAMNAMESPACEEND
