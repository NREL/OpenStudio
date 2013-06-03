/**********************************************************************
 *  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <contam/ForwardTranslator.hpp>
#include <contam/PrjData.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Node.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Logger.hpp>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>


#include <QFile>
#include <QTextStream>
#include <QMap>

namespace openstudio 
{
    namespace contam
    {

        struct AhsData{
            QString name;              // The OS name
            int nr;                    // CONTAM's index number
            double supplyVolume;       // volume in m^3 of the supply side
            int supplyIndex;           // CONTAM's index of the supply volume
            double returnVolume;       // volume in m^3 of the return side
            int returnIndex;           // CONTAM's index of the return volume
            double outdoorAirPercent;  // minimum OA percent
            int exhaustPath;           // Exhaust path index
            int intakePath;            // Intake path index
            int recircPath;            // Recirculation path index
        };

        struct ZoneData{
            QString name;
            int nr;
            int level;
            AhsData *ahs;
            double area;
            double volume;
        };

        bool ForwardTranslator::modelToContam(const openstudio::model::Model& model, const openstudio::path& path)
        {
            ForwardTranslator translator;

            boost::optional<QString> output;
            output = translator.translateToPrj(model);
            if (!output)
                return false;

            QFile file(toQString(path));
            if(file.open(QFile::WriteOnly))
            {
                QTextStream textStream(&file);
                textStream << *output;
                file.close();
                return true;
            }

            return false;
        }

        ForwardTranslator::ForwardTranslator()
        {
        }

        // I'm sure it is a bad idea to do this all in one function, but
        // there is not a real clear way of dividing things up that won't
        // cause trouble at some point.  It clearly should be cut into
        // smaller functions, but right now I'm just going to forge ahead.
        boost::optional<QString> ForwardTranslator::translateToPrj(const openstudio::model::Model& model)
        {
            int nr;
            // Load the template
            openstudio::contam::prj::Data data(":/templates/template.prj",false);
            if(!data.valid)
                return false;
			// The template is a legal PRJ file, so it has one level. Not for long.
			data.levels.clear();
            //std::cout << "Number of paths: " << data.paths.size() << std::endl;
            //std::cout << data.afe.toStdString() << std::endl;
            QString output;
            //   QString name, descr,text;
            boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
            QString modelName = QString("OpenStudio Model");
            QString modelDescr = QString("Automatically generated OpenStudio model");
            if(building)
            {
                modelName = QString::fromStdString(building->name().get());
                modelDescr = QString("Automatically generated from \"%1\" OpenStudio model").arg(modelName);
            }

            // Translate each building story into a level and generate a lookup table by name.
            // Probably need to add something here to gripe about lack of info or maybe build it
            QMap <QString, int> levelMap;
            nr=1;
            double totalHeight = 0;
            BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, model.getModelObjects<openstudio::model::BuildingStory>())
            {
                QString name = QString::fromStdString(buildingStory.name().get());
                openstudio::contam::prj::Level level;
                level.name = QString("<%1>").arg(nr);
                levelMap[name] = nr;
                double ht = buildingStory.nominalFloortoFloorHeight();
                totalHeight += ht;
                boost::optional<double> elevation = buildingStory.nominalZCoordinate();
                double z = totalHeight;
                if(elevation)
                    z = *elevation;
                level.nr = nr;
                level.refht = QString("%1").arg(z);
                level.delht = QString("%1").arg(ht);
                data.levels << level;
                nr++;
            }

            // Translate each thermal zone and generate a lookup table by name.
            QMap <QString, int> zoneMap;
            nr=0;
            BOOST_FOREACH(openstudio::model::ThermalZone thermalZone, model.getConcreteModelObjects<openstudio::model::ThermalZone>())
            {
				nr++;
                openstudio::contam::prj::Zone zone;
                QString name = QString::fromStdString(thermalZone.name().get());
                zoneMap[name] = nr;
				zone.nr = nr;
                zone.name = QString("Zone_%1").arg(nr);
				std::cout << name << std::endl;
                boost::optional<double> volume = thermalZone.volume();
                QString volString("0.0");
                if(volume)
                {
                    volString = QString("%1").arg(*volume);
                }
                else
                {
                    LOG(Warn, "Zone '" << name.toStdString() << "' has zero volume, trying to sum space volumes");
					double vol=0.0;
					BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
					{
						vol += space.volume();
					}
					if(vol == 0.0)
					{
						LOG(Warn, "Failed to compute volume for Zone '" << name.toStdString() << "'");
					}
					else
						volString = QString("%1").arg(vol);
                }
                zone.Vol = volString;
                zone.setVariablePressure(true);
                zone.setVariableContaminants(true);
                // Set level - this is not great and will fail to create a legitimate model in cases
                // where a zone is on more than one level. There are ugly workarounds - will need to
                // think about
                int levelNr = 0;
                BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
                {
                    boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
                    if(story)
                    {
                        levelNr = levelMap[QString::fromStdString((*story).name().get())];
                        break;
                    }
                }
                if(levelNr)
                {
                    zone.pl = levelNr;
                }
                else
                {
                    LOG(Warn, "Unable to set level for zone '" << name.toStdString() << "', defaulting to level 1");
                    zone.pl = 1;
                }
                // set T0
                // set P0
				data.zones << zone;
            }

            // Create paths and generate a lookup table by name
            QMap <QString, int> pathMap;
            nr = 0;
            // Loop over surfaces and generate paths
            QList <openstudio::model::Surface*>used;
            BOOST_FOREACH(openstudio::model::Surface surface, model.getConcreteModelObjects<openstudio::model::Surface>())
            {
                openstudio::contam::prj::Path path;
				std::string bc = surface.outsideBoundaryCondition();
                if(!used.contains(&surface) && bc != "Ground")
                {
					// Get the associated thermal zone
					boost::optional<openstudio::model::Space> space = surface.space();
					if(!space)
					{
						LOG(Warn, "Unattached surface '" << surface.name().get() << "'");
						continue;
					}
					boost::optional<openstudio::model::ThermalZone> thermalZone = space->thermalZone();
					if(!thermalZone)
					{
						LOG(Warn, "Unattached space '" << space->name().get() << "'");
						continue;
					}
					// Use the lookup table to get the zone info
					int zoneNr = zoneMap.value(QString::fromStdString(thermalZone->name().get()),0);
					if(!zoneNr)
					{
						LOG(Warn, "Unable to look up airflow zone for '" << thermalZone->name().get() << "'");
						continue;
					}
					openstudio::contam::prj::Zone *zone = &(data.zones[zoneNr-1]);
					double levelHt = data.levels[zone->pl-1].delht.toDouble();
					// Get the surface area - will need to do more work here later
					double area = surface.grossArea();
					//std::cout << thermalZone->name().get() << " " << data.levels.size() << " " << zone->pl << std::endl;
					std::string type = surface.surfaceType();
                    //std::cout << bc << " " << surface.surfaceType() << std::endl;
					//std::cout << "\t" << surface.netArea() << " " << surface.grossArea() << std::endl;
                    if(bc == "Outdoors") // Is there something about wind here?
                    {
                        // Make an exterior flow path
						// Set basic info
						path.nr = ++nr;
						path.pzm = zone->nr;
						path.pzn = -1;
						path.pld = zone->pl;
						// Set flow element and height
						if(type == "RoofCeiling")
						{
							path.relHt = data.levels[zone->pl-1].delht;
						}
						else
						{
							path.relHt = QString().sprintf("%g",0.5*levelHt);
						}
						// Set the multiplier
						path.mult = QString().sprintf("%g",area);
						// Set the flag here! At some point!
                    }
                    else if (bc == "Surface")
                    {
                        boost::optional<openstudio::model::Surface> adjacentSurface = surface.adjacentSurface();
                        if(adjacentSurface)
                        {
                            // Build a path, check for issues with used?
                        }
                        else
                        {
                            LOG(Warn, "Unable to find adjacent surface for surface '" << surface.name().get() << "'");
                        }
						//used << SOMETHING THAT IDS THIS SURFACE;
                    }
					data.paths << path;
                }
            }

			return boost::optional<QString>(data.print());
			//return boost::optional<QString>(QString());

          //BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
          //{
          //    int zm = zoneMap[QString::fromStdString(thermalZone.name().get())];
          //    BOOST_FOREACH(const openstudio::model::Surface& surface, space.surfaces())
          //    {
          //        int zm=-1; // Default to ambient
          //        QString bc = surface.outdoorBoundaryCondition();
                  //if(!used.contains(&surface))
                  //{
                  //    boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
                  //    if(adjacentSurface)
                  //    {
                  //        boost::optional<model::Space> adjacentSpace = adjacentSurface->space();
                  //        if(adjacentSpace)
                  //        {
                  //            boost::optional<model::ThermalZone> adjacentZone = adjacentSpace.thermalZone();
                  //            if(adjacentZone)
                  //            {
                  //                if(thermalZone == adjacentZone)
                  //                    continue; // Same zone
                  //                else
                  //                    zm = zoneMap[QString::fromStdString(adjacentZone.name().get())];
                  //            }
             // }
          //}
     // }
 //     boost::optional<model::ThermalZone> adjacentZone;
 //     if (adjacentSurface){
 //       adjacentSpace = adjacentSurface->space();
	//zm = zoneMap[QString::fromStdString(adjacentSpace->name().get())];
	//// Should this use netArea or grossArea?
	//pathText += intPath.arg(npaths).arg(zn).arg(zm).arg(11).arg(averageZ).arg(surface.netArea());
 //     }
 //     else{
	//// Should this use netArea or grossArea?
	//pathText += extPath.arg(npaths).arg(0).arg(zn).arg(zm).arg(5).arg(0).arg(averageZ).arg(surface.netArea()).arg(0).arg(180.0*surface.azimuth()/pi);
 //     }

     // {
      //    openstudio::contam::prj::Path path;
          //int zn,zm=-1;
     // }
 //     std::string surfaceType = surface.surfaceType();

 //     if (!istringEqual("Wall", surfaceType)) continue;

 //     boost::optional<model::Space> space = surface.space();
 //     if (!space) continue;

 //     boost::optional<model::ThermalZone> zone = space->thermalZone();
 //     if (!zone) continue;

 //     zn = zoneMap.value(QString::fromStdString(space->name().get()));
 //     npaths++;
 //     double averageZ = 0;
 //     double numVertices = surface.vertices().size();
 //     BOOST_FOREACH(const Point3d& point, surface.vertices()){
 //       averageZ += point.z();
 //     }
 //     averageZ = averageZ / numVertices;

 //     boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
 //     boost::optional<model::Space> adjacentSpace;
 //     boost::optional<model::ThermalZone> adjacentZone;
 //     if (adjacentSurface){
 //       adjacentSpace = adjacentSurface->space();
	//zm = zoneMap[QString::fromStdString(adjacentSpace->name().get())];
	//// Should this use netArea or grossArea?
	//pathText += intPath.arg(npaths).arg(zn).arg(zm).arg(11).arg(averageZ).arg(surface.netArea());
 //     }
 //     else{
	//// Should this use netArea or grossArea?
	//pathText += extPath.arg(npaths).arg(0).arg(zn).arg(zm).arg(5).arg(0).arg(averageZ).arg(surface.netArea()).arg(0).arg(180.0*surface.azimuth()/pi);
 //     }

 //   }

      // Create the AHS, AHS volumes, and AHS paths. No lookup table required
      nr = 1;
      // AHS loop

//      output = data.write();
 //     ahs->nr=nahs;
 //     ahs->supplyVolume = 0.0;
 //     ahs->returnVolume = 0.0;
 //     ahs->outdoorAirPercent = 30.0;
 //     ahsList << ahs;



      // Is that a good idea?  Maybe something else should be used as a key.
 //   nr = 0;
 //   double summedElevation = 0.0;
 //   
 //   text = section3;
 //   BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, model.getModelObjects<openstudio::model::BuildingStory>()){
 //     nr++;
 //     name = QString::fromStdString(buildingStory.name().get());
 //     double ht = buildingStory.nominalFloortoFloorHeight();
 //     summedElevation += ht;
 //     boost::optional<double> elevation = buildingStory.nominalZCoordinate();
 //     double z = summedElevation;
 //     if(elevation)
	//z = *elevation;
 //     text += levelEntry.arg(nr).arg(z).arg(ht).arg(QString(name).left(15).replace(" ","_"));
 //     levelMap[name]=nr;
 //   }
 //   output += text.arg(nr)+"-999\r\n";

 //   // Current inputs: descr, Tdef, windSpd, windDir
 //   QString section1("ContamW 3.0a 0\r\n%1\r\n! rows cols ud uf    T   uT     N     wH  u  Ao    a\r\n    58   66  0  0 %2 2    0.00  0.00 0 0.850 0.200\r\n! Ta       Pb      Ws    Wd    rh  day u..\r\n293.150 101325.0  %3   %4 0.000 1 2 0 0 1 ! steady simulation\r\n293.150 101325.0  1.000 270.0 0.000 1 2 0 0 1 ! wind pressure test\r\nnull ! no weather file\r\nnull ! no contaminant file\r\nnull ! no continuous values file\r\nnull ! no discrete values file\r\nnull ! no WPC file\r\nnull ! no EWC file\r\nWPC description\r\n!  Xref    Yref    Zref   angle u\r\n   0.000   0.000   0.000   0.00 0\r\n! epsP epsS  tShift  dStart dEnd wp mf\r\n  0.01 0.01 00:00:00   1/1   1/1  0  0\r\n! latd  longtd   tznr  altd  Tgrnd u..\r\n 40.00  -90.00  -6.00     0 283.15 2 0\r\n!sim_af afcalc afmaxi afrcnvg afacnvg afrelax uac Pbldg uPb\r\n     0      1     30  1e-005  1e-006    0.75   0 50.00   0\r\n!   slae rs aflmaxi aflcnvg aflinit Tadj\r\n      0   1    100  1e-006      1    0\r\n!sim_mf slae rs maxi   relcnvg   abscnvg relax gamma ucc\r\n    0             30 1.00e-004 1.00e-015 1.250         0 ! (cyclic)\r\n          0   1  100 1.00e-006 1.00e-015 1.100 1.000   0 ! (non-trace)\r\n          0   1  100 1.00e-006 1.00e-015 1.100 1.000   0 ! (trace)\r\n!sim_sts sim_1dz sim_1dd celldx sim_vjt udx\r\n     0       1       0    0.100     0    0\r\n!tsdens relax tsmaxi\r\n     0   0.75    20\r\n!date_st time_st  date_0 time_0   date_1 time_1    t_step   t_list   t_scrn\r\n  Jan01 00:00:00  Jan01 00:00:00  Jan01 24:00:00  00:05:00 01:00:00 01:00:00\r\n!restart  date  time\r\n      0  Jan01 00:00:00\r\n!list doDlg pfsave zfsave zcsave\r\n   1     1      1      1      1\r\n!vol ach -bw cbw exp -bw age -bw\r\n  0   0   0   0   0   0   0   0 \r\n!...\r\n 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\r\n2 ! rvals:\r\n1.2041 9.8055\r\n!valZ valD valC\r\n   0    0    0\r\n!ctype conv var zref imax dtcmo\r\n   0 0.010000    0    0 1000    1\r\n-999\r\n");
 //   // For now, just keep the 2 built in contaminants (deactivated)
 //   QString section2("0 ! contaminants:\r\n2 ! species:\r\n! # s t   molwt    mdiam       edens       decay       Dm          CCdef        Cp    u...  name\r\n  1 0 0  44.0100 0.0000e+000 0.0000e+000 0.0000e+000 2.0000e-005 6.0795e-004    0.000 0 0 0 0 0 CO2\r\nCarbon dioxide tracer gas\r\n  2 0 0 146.0600 0.0000e+000 0.0000e+000 0.0000e+000 2.0000e-005 0.0000e+000    0.000 0 0 0 0 0 SF6\r\nSulfur Hexafluoride tracer gas\r\n-999\r\n");
 //   // Current input: nlevels, add level entries (x nlevel)
 //   QString section3("%1 ! levels plus icon data:\r\n! #  refHt   delHt  ni  u  name\r\n");
 //   // Current inputs: nr, elevation, height, name 
 //   QString levelEntry("%1 %2 %3 0 0 0 %4\r\n");
 //   // No schedule modifications
 //   QString section45("21 ! day-schedules:\r\n! # npts shap utyp ucnv name\r\n  1    2    0    1    0 0_percent_day\r\nContinuous operation at 0%.\r\n 00:00:00 0\r\n 24:00:00 0\r\n  2    2    0    1    0 5_percent_day\r\nContinuous operation at 5%.\r\n 00:00:00 0.05\r\n 24:00:00 0.05\r\n  3    2    0    1    0 10_percent_day\r\nContinuous operation at 10%.\r\n 00:00:00 0.1\r\n 24:00:00 0.1\r\n  4    2    0    1    0 15_percent_day\r\nContinuous operation at 15%.\r\n 00:00:00 0.15\r\n 24:00:00 0.15\r\n  5    2    0    1    0 20_percent_day\r\nContinuous operation at 20%.\r\n 00:00:00 0.2\r\n 24:00:00 0.2\r\n  6    2    0    1    0 25_percent_day\r\nContinuous operation at 25%.\r\n 00:00:00 0.25\r\n 24:00:00 0.25\r\n  7    2    0    1    0 30_percent_day\r\nContinuous operation at 30%.\r\n 00:00:00 0.3\r\n 24:00:00 0.3\r\n  8    2    0    1    0 35_percent_day\r\nContinuous operation at 35%.\r\n 00:00:00 0.35\r\n 24:00:00 0.35\r\n  9    2    0    1    0 40_percent_day\r\nContinuous operation at 40%.\r\n 00:00:00 0.4\r\n 24:00:00 0.4\r\n 10    2    0    1    0 45_percent_day\r\nContinuous operation at 45%.\r\n 00:00:00 0.45\r\n 24:00:00 0.45\r\n 11    2    0    1    0 50_percent_day\r\nContinuous operation at 50%.\r\n 00:00:00 0.5\r\n 24:00:00 0.5\r\n 12    2    0    1    0 55_percent_day\r\nContinuous operation at 55%.\r\n 00:00:00 0.55\r\n 24:00:00 0.55\r\n 13    2    0    1    0 60_percent_day\r\nContinuous operation at 60%.\r\n 00:00:00 0.6\r\n 24:00:00 0.6\r\n 14    2    0    1    0 65_percent_day\r\nContinuous operation at 65%.\r\n 00:00:00 0.65\r\n 24:00:00 0.65\r\n 15    2    0    1    0 70_percent_day\r\nContinuous operation at 70%.\r\n 00:00:00 0.7\r\n 24:00:00 0.7\r\n 16    2    0    1    0 75_percent_day\r\nContinuous operation at 75%.\r\n 00:00:00 0.75\r\n 24:00:00 0.75\r\n 17    2    0    1    0 80_percent_day\r\nContinuous operation at 80%.\r\n 00:00:00 0.8\r\n 24:00:00 0.8\r\n 18    2    0    1    0 85_percent_day\r\nContinuous operation at 85%.\r\n 00:00:00 0.85\r\n 24:00:00 0.85\r\n 19    2    0    1    0 90_percent_day\r\nContinuous operation at 90%.\r\n 00:00:00 0.9\r\n 24:00:00 0.9\r\n 20    2    0    1    0 95_percent_day\r\nContinuous operation at 95%.\r\n 00:00:00 0.95\r\n 24:00:00 0.95\r\n 21    2    0    1    0 100_percent_day\r\nContinuous operation at 100%.\r\n 00:00:00 1\r\n 24:00:00 1\r\n-999\r\n21 ! week-schedules:\r\n! # utyp ucnv name\r\n  1    1    0 0_percent\r\nContinuous operation at 0%.\r\n 1 1 1 1 1 1 1 1 1 1 1 1\r\n  2    1    0 5_percent\r\nContinuous operation at 5%.\r\n 2 2 2 2 2 2 2 2 2 2 2 2\r\n  3    1    0 10_percent\r\nContinuous operation at 10%.\r\n 3 3 3 3 3 3 3 3 3 3 3 3\r\n  4    1    0 15_percent\r\nContinuous operation at 15%.\r\n 4 4 4 4 4 4 4 4 4 4 4 4\r\n  5    1    0 20_percent\r\nContinuous operation at 20%.\r\n 5 5 5 5 5 5 5 5 5 5 5 5\r\n  6    1    0 25_percent\r\nContinuous operation at 25%.\r\n 6 6 6 6 6 6 6 6 6 6 6 6\r\n  7    1    0 30_percent\r\nContinuous operation at 30%.\r\n 7 7 7 7 7 7 7 7 7 7 7 7\r\n  8    1    0 35_percent\r\nContinuous operation at 35%.\r\n 8 8 8 8 8 8 8 8 8 8 8 8\r\n  9    1    0 40_percent\r\nContinuous operation at 40%.\r\n 9 9 9 9 9 9 9 9 9 9 9 9\r\n 10    1    0 45_percent\r\nContinuous operation at 45%.\r\n 10 10 10 10 10 10 10 10 10 10 10 10\r\n 11    1    0 50_percent\r\nContinuous operation at 50%.\r\n 11 11 11 11 11 11 11 11 11 11 11 11\r\n 12    1    0 55_percent\r\nContinuous operation at 55%.\r\n 12 12 12 12 12 12 12 12 12 12 12 12\r\n 13    1    0 60_percent\r\nContinuous operation at 60%.\r\n 13 13 13 13 13 13 13 13 13 13 13 13\r\n 14    1    0 65_percent\r\nContinuous operation at 65%.\r\n 14 14 14 14 14 14 14 14 14 14 14 14\r\n 15    1    0 70_percent\r\nContinuous operation at 70%.\r\n 15 15 15 15 15 15 15 15 15 15 15 15\r\n 16    1    0 75_percent\r\nContinuous operation at 75%.\r\n 16 16 16 16 16 16 16 16 16 16 16 16\r\n 17    1    0 80_percent\r\nContinuous operation at 80%.\r\n 17 17 17 17 17 17 17 17 17 17 17 17\r\n 18    1    0 85_percent\r\nContinuous operation at 85%.\r\n 18 18 18 18 18 18 18 18 18 18 18 18\r\n 19    1    0 90_percent\r\nContinuous operation at 90%.\r\n 19 19 19 19 19 19 19 19 19 19 19 19\r\n 20    1    0 95_percent\r\nContinuous operation at 95%.\r\n 20 20 20 20 20 20 20 20 20 20 20 20\r\n 21    1    0 100_percent\r\nContinuous operation at 100%.\r\n 21 21 21 21 21 21 21 21 21 21 21 21\r\n-999\r\n");
 //   // No wind pressure inputs
 //   QString section6("5 ! wind pressure profiles:\r\n1 13 2 high_rise_1/1\r\nHigh rise building with L/W=1\r\n   0.0  0.600\r\n  30.0  0.450\r\n  60.0  0.000\r\n  90.0 -0.550\r\n 120.0 -0.550\r\n 150.0 -0.400\r\n 180.0 -0.330\r\n 210.0 -0.400\r\n 240.0 -0.550\r\n 270.0 -0.550\r\n 300.0  0.000\r\n 330.0  0.450\r\n 360.0  0.600\r\n2 13 2 high_rise_1/4\r\nHigh rise building with L/W=1/4\r\n   0.0  0.600\r\n  30.0  0.400\r\n  60.0 -0.250\r\n  90.0 -0.700\r\n 120.0 -0.500\r\n 150.0 -0.300\r\n 180.0 -0.250\r\n 210.0 -0.300\r\n 240.0 -0.500\r\n 270.0 -0.700\r\n 300.0 -0.250\r\n 330.0  0.400\r\n 360.0  0.600\r\n3 13 2 high_rise_4/1\r\nHigh rise building with L/W = 4/1\r\n   0.0  0.600\r\n  30.0  0.500\r\n  60.0  0.200\r\n  90.0 -0.250\r\n 120.0 -0.600\r\n 150.0 -0.550\r\n 180.0 -0.500\r\n 210.0 -0.550\r\n 240.0 -0.600\r\n 270.0 -0.250\r\n 300.0  0.200\r\n 330.0  0.500\r\n 360.0  0.600\r\n4 13 2 low_rise_walls\r\nWind pressure for walls of low rise buildings \r\n   0.0  0.600\r\n  30.0  0.400\r\n  60.0  0.100\r\n  90.0 -0.450\r\n 120.0 -0.500\r\n 150.0 -0.450\r\n 180.0 -0.350\r\n 210.0 -0.450\r\n 240.0 -0.500\r\n 270.0 -0.450\r\n 300.0  0.100\r\n 330.0  0.400\r\n 360.0  0.600\r\n5 3 2 roof\r\nWind pressure profile for all roofs\r\n   0.0 -0.500\r\n 180.0 -0.500\r\n 360.0 -0.500\r\n-999\r\n");
 //   // No filter inputs
 //   QString section8ab("11 ! filter elements:\r\n1 cef 1 0.1 100 0 0 SF6_10pct\r\nfilter for SF6, 10% filter efficiency\r\n  1\r\n  SF6 0.1\r\n2 cef 1 0.1 100 0 0 SF6_20pct\r\nfilter for SF6, 20% filter efficiency\r\n  1\r\n  SF6 0.2\r\n3 cef 1 0.1 100 0 0 SF6_30pct\r\nfilter for SF6, 30% filter efficiency\r\n  1\r\n  SF6 0.3\r\n4 cef 1 0.1 100 0 0 SF6_40pct\r\nfilter for SF6, 40% filter efficiency\r\n  1\r\n  SF6 0.4\r\n5 cef 1 0.1 100 0 0 SF6_50pct\r\nfilter for SF6, 50% filter efficiency\r\n  1\r\n  SF6 0.5\r\n6 cef 1 0.1 100 0 0 SF6_60pct\r\nfilter for SF6, 60% filter efficiency\r\n  1\r\n  SF6 0.6\r\n7 cef 1 0.1 100 0 0 SF6_70pct\r\nfilter for SF6, 70% filter efficiency\r\n  1\r\n  SF6 0.7\r\n8 cef 1 0.1 100 0 0 SF6_80pct\r\nfilter for SF6, 80% filter efficiency\r\n  1\r\n  SF6 0.8\r\n9 cef 1 0.1 100 0 0 SF6_90pct\r\nfilter for SF6, 90% filter efficiency\r\n  1\r\n  SF6 0.9\r\n10 cef 1 0.1 100 0 0 SF6_95pct\r\nfilter for SF6, 95% filter efficiency\r\n  1\r\n  SF6 0.95\r\n11 cef 1 0.1 100 0 0 SF6_99pct\r\nfilter for SF6, 99% filter efficiency\r\n  1\r\n  SF6 0.99\r\n-999\r\n0 ! filters:\r\n-999\r\n");
 //   // No source/sink inputs
 //   QString section9("5 ! source/sink elements:\r\n1 SF6 brs 1kgSF6\r\n1 kg sulfur hexafluoride tracer gas instantaneously added to a zone\r\n 1 0\r\n2 SF6 ccf 1kgperhourSF6\r\n1 kg/hr sulfur hexafluoride tracer gas added to a zone\r\n 0.000277778 0 24 0\r\n3 SF6 brs 1lbSF6\r\n1 lb of sulfur hexafluoride tracer gas instantaneously added to a zone\r\n 0.453592 1\r\n4 SF6 ccf 1lbperhourSF6\r\n1 lb per hour sulfur hexafluoride tracer gas added to a zone\r\n 0.000125999 0 25 0\r\n5 CO2 ccf CO2_one_person\r\nCarbon dioxide production for one person with metabolic rate of 1 met\r\n 2.53049e-005 0 21 0\r\n-999\r\n");
 //   // No afe inputs
 //   QString section10("21 ! flow elements:\r\n1 23 plr_leak1 ClosedDoor\r\n\r\n 1.09247e-006 0.000912394 0.5 1 4 0.00064516 0 0 3 2 2 0\r\n2 23 plr_leak1 ElevDoor\r\n\r\n 1.00357e-005 0.0259606 0.65 1 4 0.0226 0 0 2 2 2 0\r\n3 25 plr_shaft ElevShaft\r\n\r\n 0.294425 29.9035 0.508448 3.048 5.85289 9.7536 0.1 1 1 1 2\r\n4 23 fan_cvf ExFan300\r\n300 cfm exhaust fan\r\n 0.141584 1\r\n5 23 plr_test1 ExtWallAvg\r\n\r\n 6.13696e-008 0.000499082 0.65 75 0.00906345 0 4\r\n6 23 plr_test1 ExtWallLeaky\r\n\r\n 1.288e-007 0.000895033 0.65 75 0.016254 0 4\r\n7 23 plr_test1 ExtWallTight\r\n\r\n 8.29484e-009 0.000103131 0.65 75 0.00187289 0 4\r\n8 23 plr_test1 FloorAvg\r\n\r\n 1.47921e-007 0.000998165 0.65 75 0.0181269 0 4\r\n9 23 plr_test1 FloorLeaky\r\n\r\n 3.1045e-007 0.00179007 0.65 75 0.032508 0 4\r\n10 23 plr_test1 FloorTight\r\n\r\n 1.99933e-008 0.000206263 0.65 75 0.00374578 0 4\r\n11 23 plr_test1 IntWallAvg\r\n\r\n 1.47921e-007 0.000998165 0.65 75 0.0181269 0 4\r\n12 23 plr_test1 IntWallLeaky\r\n\r\n 3.1045e-007 0.00179007 0.65 75 0.032508 0 4\r\n13 23 plr_test1 IntWallTight\r\n\r\n 1.99933e-008 0.000206263 0.65 75 0.00374578 0 4\r\n14 23 plr_leak1 OpenDoor\r\n\r\n 0.18167 2.75908 0.5 1 4 1.95096 0 0 1 2 2 0\r\n15 23 plr_test1 RoofAvg\r\n\r\n 6.13696e-008 0.000499083 0.65 75 0.00906345 0 4\r\n16 23 plr_test1 RoofLeaky\r\n\r\n 1.288e-007 0.000895033 0.65 75 0.016254 0 4\r\n17 23 plr_test1 RoofTight\r\n\r\n 8.29484e-009 0.000103131 0.65 75 0.00187289 0 4\r\n18 23 plr_leak3 ShaftWall\r\n\r\n 2.33047e-008 0.000218253 0.65 1 4 0 0 0.00019 2 2 2 0\r\n19 23 plr_leak1 StairDoor\r\n\r\n 5.07153e-006 0.0151628 0.65 1 4 0.0132 0 0 2 2 2 0\r\n20 25 plr_stair StairWell\r\n\r\n 0.282182 4.38745 0.5 3.048 20.4387 0 0 1 1\r\n21 23 plr_leak1 TransferGrille\r\n\r\n 0.00188779 0.131385 0.5 1 4 0.092903 0 0 1 2 2 0\r\n-999\r\n");
 //   // Current inputs: nahs, add ahs entries (x nahs)
 //   QString section13("%1 ! simple AHS:\r\n! # zr# zs# pr# ps# px# name\r\n");
 //   // Current inputs: nr, zr, zs, pr, ps, px, name
 //   QString ahsEntry("%1 %2 %3 %4 %5 %6 %7\r\n\r\n");
 //   // Current inputs: nzones, add zone entries (x nzones)
 //   QString section14("%1 ! zones:\r\n! Z#  f  s#  c#  k#  l#  relHt    Vol  T0  P0  name  clr u..  axs  1-D data:\r\n");
 //   // Current inputs: nr, flag, level, volume, T0, name
 //   QString zoneEntry("%1 %2 0 0 0 %3 0.0 %4 %5 0 %6 -1 0 0 0 0 0 0\r\n");
 //   // Current inputs: npath, path entries (x npath)
 //   QString section16("%1 ! flow paths:\r\n! P#    f  n#  m#  e#  f#  w#  a#  s#  c#  l#    X       Y      relHt  mult wPset wPmod wazm Fahs Xmax Xmin icn dir u..\r\n");
 //   // nr f n# m# e# f# w# a# s# c# l# X Y relHt mult wPset wPmod wazm Fahs Xmax Xmin icn dir 0 0 0 0 0
 //   // Current inputs: nr, zn, zm, element, elevation, multiplier,
 //   QString intPath("%1 0 %2 %3 %4 0 0 0 0 0 1 0.0 0.0 %5 %6 0 0 -1 0 0 0 23 0 0 0 0 0 0\r\n");
 //   // Current inputs: nr, flag, zn, zm, element, windprof, elevation, multiplier, wPmod, azimuth
 //   QString extPath("%1 %2 %3 %4 %5 0 %6 0 0 0 1 0.0 0.0 %7 %8 0 %9 %10 0 0 0 23 0 0 0 0 0 0\r\n");
 //   // Current inputs: nr, flag, zn, zm, ahs, schedule, Fahs
 //   QString ahsPath("%1 %2 %3 %4 0 0 0 %5 %6 0 1 0.0 0.0 0.0 1 0 0 -1 %7 0 0 0 0 0 0 0 0 0\r\n");
 //   int nr;

 //   const double pi = boost::math::constants::pi<double>();



 //   // We'll need to get these from some place at some point for
 //   // steady calculations.  For transient calculations there are
 //   // other ways to set them.  I'm not sure if Tdef really plays
 //   // much of a role, so it probably could be left as 293.15 K
 //   double Tdef=293.15;  // Default zone temperature
 //   double windSpd=0.0;  // Wind speed in m/s
 //   double windDir=0.0;  // Wind direction (0 is N) in degrees
 //   output += section1.arg(descr).arg(Tdef).arg(windSpd).arg(windDir);

 //   // Skip the contaminants for now, just keep CO2 and SF6
 //   output += section2;
 //   
 //   // Translate each building story into a level and generate a lookup table by name.
 //   // Is that a good idea?  Maybe something else should be used as a key.
 //   nr = 0;
 //   double summedElevation = 0.0;
 //   QMap <QString, int> levelMap;
 //   text = section3;
 //   BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, model.getModelObjects<openstudio::model::BuildingStory>()){
 //     nr++;
 //     name = QString::fromStdString(buildingStory.name().get());
 //     double ht = buildingStory.nominalFloortoFloorHeight();
 //     summedElevation += ht;
 //     boost::optional<double> elevation = buildingStory.nominalZCoordinate();
 //     double z = summedElevation;
 //     if(elevation)
	//z = *elevation;
 //     text += levelEntry.arg(nr).arg(z).arg(ht).arg(QString(name).left(15).replace(" ","_"));
 //     levelMap[name]=nr;
 //   }
 //   output += text.arg(nr)+"-999\r\n";

 //   // No schedule modifications
 //   output += section45;

 //   // No wind pressure profile modifications
 //   output += section6;

 //   // No kinetic reactions
 //   output += QString("0 ! kinetic reactions:\r\n-999\r\n");

 //   // No filter modifications
 //   output += section8ab;

 //   // No source/sink modifications
 //   output += section9;

 //   // No afe modifications
 //   output += section10;

 //   // No duct elements or controls
 //   output += QString("0 ! duct elements:\r\n-999\r\n0 ! control super elements:\r\n-999\r\n0 ! control nodes:\r\n-999\r\n");

 //   // Simple air handling systems need to be generated after the
 //   // aiflow zones and paths are worked out, so get a list of the
 //   // systems we want and airflow zones at the same time.
 //   QMap <QString,int> ahsMap;
 //   QMap <QString,int> zoneMap;
 //   QList <AhsData*> ahsList;
 //   QList <ZoneData*> zoneList;
 //   int nahs = 0;
 //   int nzones = 0;
 //   QString airflowZoneText = section14;
 //   BOOST_FOREACH(model::ThermalZone zone, building->thermalZones()){
 //     // Cheat for now and generate one AHS per thermal zone
 //     nahs++;
 //     AhsData *ahs = new AhsData;
 //     ahs->name = QString::fromStdString(zone.name().get());
 //     ahs->nr=nahs;
 //     ahs->supplyVolume = 0.0;
 //     ahs->returnVolume = 0.0;
 //     ahs->outdoorAirPercent = 30.0;
 //     ahsList << ahs;

 //     BOOST_FOREACH(const model::Space& space, zone.spaces()){
	//nzones++;
	//ZoneData *afzone = new ZoneData;
	//zoneList << afzone;
	//afzone->name = QString::fromStdString(space.name().get());
	//afzone->volume = space.volume();
	//afzone->nr = nzones;
	//afzone->ahs = ahs;
	//afzone->level = 1;
	//afzone->area = 0.0;
	//boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
	//if(story){
	//  afzone->level = levelMap.value(QString::fromStdString(story->name().get()));
	//  double ht = story->nominalFloortoFloorHeight();
	//  if(ht>0.0)
	//    afzone->area = afzone->volume/ht;
	//}
	//zoneMap[afzone->name] = nzones;
	//airflowZoneText += zoneEntry.arg(nzones).arg(3).arg(afzone->level).arg(afzone->volume).arg(Tdef).arg(QString(afzone->name).replace(" ","_"));
 //       }


 //     // these are probably useful, will have to ask Kyle
 //     // Kyle, should these functions be const?
 //     //boost::optional<model::ModelObject> airInletModelObject = zone.airInletModelObject();
 //     //boost::optional<model::ModelObject> returnAirModelObject = zone.returnAirModelObject();
 //     //boost::optional<model::Node> exhaustAirNode = zone.exhaustAirNode();
 //     //boost::optional<model::Node> zoneAirNode = zone.zoneAirNode();
 //   }

 //   // Add the AHS zones to the zone list
 //   for(int i=0;i<ahsList.size();i++)
 //     {
	//AhsData *ahs = ahsList.at(i);
	//nzones++;
	//ahs->returnIndex = nzones;
	//airflowZoneText += zoneEntry.arg(nzones).arg(10).arg(1).arg(ahs->returnVolume).arg(Tdef).arg(QString(ahs->name).replace(" ","_")+"(Rec)");
	//nzones++;
	//ahs->supplyIndex = nzones;
	//airflowZoneText += zoneEntry.arg(nzones).arg(10).arg(1).arg(ahs->supplyVolume).arg(Tdef).arg(QString(ahs->name).replace(" ","_")+"(Sup)");
 //     }

 //   // Loop over the spaces and connect up AHS paths
 //   int npaths = 0;
 //   QString pathText=section16;
 //   for(int i=0;i<zoneList.size();i++)
 //     {
	//npaths++;
	//ZoneData *afz = zoneList.at(i);
	//double flowRate = afz->area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation
	//pathText += ahsPath.arg(npaths).arg(8).arg(afz->ahs->supplyIndex).arg(afz->nr).arg(afz->ahs->nr).arg(0).arg(flowRate);
	//npaths++;
	//pathText += ahsPath.arg(npaths).arg(8).arg(afz->nr).arg(afz->ahs->returnIndex).arg(afz->ahs->nr).arg(0).arg(0.9*flowRate);
 //     }

 //   // Loop over surfaces and generate paths
 //   BOOST_FOREACH(const model::Surface& surface, model.getModelObjects<model::Surface>()){
 //     int zn,zm=-1;
 //     std::string surfaceType = surface.surfaceType();

 //     if (!istringEqual("Wall", surfaceType)) continue;

 //     boost::optional<model::Space> space = surface.space();
 //     if (!space) continue;

 //     boost::optional<model::ThermalZone> zone = space->thermalZone();
 //     if (!zone) continue;

 //     zn = zoneMap.value(QString::fromStdString(space->name().get()));
 //     npaths++;
 //     double averageZ = 0;
 //     double numVertices = surface.vertices().size();
 //     BOOST_FOREACH(const Point3d& point, surface.vertices()){
 //       averageZ += point.z();
 //     }
 //     averageZ = averageZ / numVertices;

 //     boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
 //     boost::optional<model::Space> adjacentSpace;
 //     boost::optional<model::ThermalZone> adjacentZone;
 //     if (adjacentSurface){
 //       adjacentSpace = adjacentSurface->space();
	//zm = zoneMap[QString::fromStdString(adjacentSpace->name().get())];
	//// Should this use netArea or grossArea?
	//pathText += intPath.arg(npaths).arg(zn).arg(zm).arg(11).arg(averageZ).arg(surface.netArea());
 //     }
 //     else{
	//// Should this use netArea or grossArea?
	//pathText += extPath.arg(npaths).arg(0).arg(zn).arg(zm).arg(5).arg(0).arg(averageZ).arg(surface.netArea()).arg(0).arg(180.0*surface.azimuth()/pi);
 //     }

 //   }

 //   // Finally, we can build the AHS paths and create the AHS entries
 //   for(int i=0;i<ahsList.size();i++)
 //     {
	//int schd = 7; // Just use 30 percent
	//AhsData *ahs = ahsList.at(i);
	//// Recirculation path
	//ahs->recircPath = ++npaths;
	//pathText += ahsPath.arg(npaths).arg(16).arg(ahs->returnIndex).arg(ahs->supplyIndex).arg(0).arg(schd).arg(0.0);
	//// Intake path
	//ahs->intakePath = ++npaths;
	//pathText += ahsPath.arg(npaths).arg(32).arg(-1).arg(ahs->supplyIndex).arg(0).arg(0).arg(0.0);
	//// Exhaust path
	//ahs->exhaustPath = ++npaths;
	//pathText += ahsPath.arg(npaths).arg(64).arg(ahs->returnIndex).arg(-1).arg(0).arg(0).arg(0.0);
 //     }

 //   QString ahsText=section13;
 //   output += ahsText.arg(ahsList.size());
 //   for(int i=0;i<ahsList.size();i++)
 //     {
	//AhsData *ahs = ahsList.at(i);
	//// Current inputs: nr, zr, zs, pr, ps, px, name
	//output += ahsEntry.arg(ahs->nr).arg(ahs->returnIndex).arg(ahs->supplyIndex).arg(ahs->recircPath).arg(ahs->intakePath).arg(ahs->exhaustPath).arg(ahs->name.replace(" ","_"));
 //     }
 //   output += "-999\r\n";

 //   output += airflowZoneText.arg(nzones) + "-999\r\n";

 //   // No IC's needed
 //   output += "0 ! initial zone concentrations:\r\n-999\r\n";

 //   // Add in the paths
 //   output += pathText.arg(npaths) + "-999\r\n";
 //  
 //   // The rest of the file is skipped
 //   output += "0 ! duct junctions:\r\n-999\r\n0 ! initial junction concentrations:\r\n-999\r\n0 ! duct segments:\r\n-999\r\n0 ! source/sinks:\r\n-999\r\n0 ! occupancy schedules:\r\n-999\r\n0 ! exposures:\r\n-999\r\n0 ! annotations:\r\n-999\r\n* end project file.\r\n";

 //   // Clean up
 //   while (!ahsList.isEmpty())
 //     delete ahsList.takeFirst();
 //   while (!zoneList.isEmpty())
 //     delete zoneList.takeFirst();

    //return boost::optional<QString>(output);

  }

  bool ForwardTranslator::modelToJson(const openstudio::model::Model& model, 
				      const openstudio::path& path)
  {
    ForwardTranslator translator;

    boost::optional<QString> output;
    output = translator.translateToJson(model);
    if (!output)
      return false;

    QFile file(toQString(path));
    if(file.open(QFile::WriteOnly)){
      QTextStream textStream(&file);
      textStream << *output;
      file.close();
      return true;
    }

    return false;
  }

  boost::optional<QString> ForwardTranslator::translateToJson(const openstudio::model::Model& model)
  {
    QString output("{");
    QString quoted("\"%1\":\"%2\"");
    QString unquoted("\"%1\":%2");
    QString namedObject("\"%1\":{%2}");
    QString object("{%1}");
    QStringList outputTxtList;
    QStringList entryTxtList;

    const double pi = boost::math::constants::pi<double>();

    QString name, descr,text;
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
    if (building){
      outputTxtList << quoted.arg("name").arg(QString::fromStdString(building->name().get()));
    }

    outputTxtList << quoted.arg("description").arg("Automatically generated OpenStudio model");

    // Get the levels.  Is there a guarantee that these are in order?  Probably not, and that
    // could be fatal if the elevation is not included.
    double summedElevation = 0.0;
    BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, 
		  model.getModelObjects<openstudio::model::BuildingStory>()){
      QStringList txtList;
      //QString name = QString::fromStdString(buildingStory.name().get());
      txtList << quoted.arg("name").arg(QString::fromStdString(buildingStory.name().get()));
      double ht = buildingStory.nominalFloortoFloorHeight();
      txtList << unquoted.arg("height").arg(ht);
      summedElevation += ht;
      boost::optional<double> elevation = buildingStory.nominalZCoordinate();
      double z = summedElevation;
      if(elevation)
	z = *elevation;
      txtList << unquoted.arg("elevation").arg(z);
      entryTxtList << QString("{")+txtList.join(",")+QString("}");
      //levelMap[name]=nr;
    }
    if(entryTxtList.size())
      outputTxtList << QString("\"levels\":[")+entryTxtList.join(",")+QString("]");

    // Generate airflow zone list
    entryTxtList.clear();
	std::vector<model::Space> spaces = model.getModelObjects<model::Space>();
	//std::cout<<spaces.size()<<'\n';
	// Loop over the spaces and generate an airflow zone
	BOOST_FOREACH(const model::Space& space, spaces)
	{
		QStringList txtList;
		double volume = space.volume();
	    txtList << quoted.arg("name").arg(QString::fromStdString(space.name().get()));
	    txtList << unquoted.arg("volume").arg(volume);
	    boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
	    if(story)
		{
			txtList << quoted.arg("level").arg(QString::fromStdString(story->name().get()));
			double ht = story->nominalFloortoFloorHeight();
			if(ht>0.0)
				txtList << unquoted.arg("area").arg(volume/ht);
		}
		boost::optional<model::ThermalZone> zone = space.thermalZone();
		if(zone)
		{
			QString zoneName = QString::fromStdString(zone->name().get());
			txtList << QString("\"%1\":[{\"system\":\"%2\"}]").arg("supplyAir").arg(zoneName);
	        txtList << QString("\"%1\":[{\"system\":\"%2\"}]").arg("returnAir").arg(zoneName);
		}
		entryTxtList << QString("{")+txtList.join(",")+QString("}");
	}

    // Loop over the zones and generate one airflow zone per space
 //   BOOST_FOREACH(const model::ThermalZone& zone, model.getModelObjects<model::ThermalZone>())
	//{
	//	QString zoneName = QString::fromStdString(zone.name().get());
	//	BOOST_FOREACH(const model::Space& space, zone.spaces()){
	//		QStringList txtList;
	//		double volume = space.volume();
	//		txtList << quoted.arg("name").arg(QString::fromStdString(space.name().get()));
	//		txtList << unquoted.arg("volume").arg(volume);
	//		
	//		boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
	//		if(story){
	//			txtList << quoted.arg("level").arg(QString::fromStdString(story->name().get()));
	//			double ht = story->nominalFloortoFloorHeight();
	//			if(ht>0.0)
	//				txtList << unquoted.arg("area").arg(volume/ht);
	//		}
	//		// This isn't quite right, but whatever
	//        txtList << QString("\"%1\":[{\"system\":\"%2\"}]").arg("supplyAir").arg(zoneName);
	//		txtList << QString("\"%1\":[{\"system\":\"%2\"}]").arg("returnAir").arg(zoneName);

	//		entryTxtList << QString("{")+txtList.join(",")+QString("}");
	//	}
 //     // these are probably useful, will have to ask Kyle
 //     // Kyle, should these functions be const?
 //     //boost::optional<model::ModelObject> airInletModelObject = zone.airInletModelObject();
 //     //boost::optional<model::ModelObject> returnAirModelObject = zone.returnAirModelObject();
 //     //boost::optional<model::Node> exhaustAirNode = zone.exhaustAirNode();
 //     //boost::optional<model::Node> zoneAirNode = zone.zoneAirNode();
	//}
    if(entryTxtList.size())
        outputTxtList << QString("\"zones\":[")+entryTxtList.join(",")+QString("]");

    // Loop over thermal zones and create air handling systems
    entryTxtList.clear();
    std::vector<model::ThermalZone> zones = model.getModelObjects<model::ThermalZone>();
    BOOST_FOREACH(const model::ThermalZone& zone, zones)
    {
        QStringList txtList;
        QString zoneName = QString::fromStdString(zone.name().get());
        txtList << quoted.arg("name").arg(zoneName);

        entryTxtList << QString("{")+txtList.join(",")+QString("}");
    }
    if(entryTxtList.size())
        outputTxtList << QString("\"ahs\":[")+entryTxtList.join(",")+QString("]");

    // Loop over surfaces and generate paths - keep track of an
    // adjacent surface list to avoid ouputing the same wall twice
    entryTxtList.clear();
    QList <model::Surface> adjacentList;
    std::vector<model::Surface> surfaces = model.getModelObjects<model::Surface>();
    //std::cout<<surfaces.size()<<'\n';
    BOOST_FOREACH(const model::Surface& surface, surfaces)
    {
        QString wallType;
        std::string surfaceType = surface.surfaceType();
        
        if(adjacentList.contains(surface))
            continue;

        // Disallow below-grade airflow.  Note that this does not
        // preclude below-grade infiltration, but it has to be modeled
        // differently.
        if(istringEqual("Ground", surface.outsideBoundaryCondition()))
            continue;

        // Get the type of partition we're dealing with - this may get
        // changed below (if it should be a floor, for example)
        if(istringEqual("Wall", surfaceType))
            wallType = QString("wall");
        else if(istringEqual("RoofCeiling", surfaceType))
            wallType = QString("roof");
        else if(istringEqual("Floor", surfaceType))
            wallType = QString("floor");
        else
            continue;

        boost::optional<model::Space> space = surface.space();
        if (!space) continue;

        //boost::optional<model::ThermalZone> zone = space->thermalZone();
        //if (!zone) continue;
        
        QStringList txtList;
        txtList << quoted.arg("name").arg(QString::fromStdString(surface.name().get()));
        QString name0 = QString::fromStdString(space->name().get());

        double averageZ = 0;
        double numVertices = surface.vertices().size();
        BOOST_FOREACH(const Point3d& point, surface.vertices())
        {
            averageZ += point.z();
        }
        averageZ = averageZ / numVertices;
        txtList << unquoted.arg("elevation").arg(averageZ);

        // Should this use netArea or grossArea?
        txtList << unquoted.arg("area").arg(surface.netArea());

        boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
        boost::optional<model::Space> adjacentSpace;
        boost::optional<model::ThermalZone> adjacentZone;
        if (adjacentSurface)
        {
            adjacentSpace = adjacentSurface->space();
            adjacentList << *adjacentSurface;
            QString name1 = QString::fromStdString(adjacentSpace->name().get());
            txtList << QString("\"zones\":[\"%1\",\"%2\"]").arg(name0).arg(name1);
            if(istringEqual("RoofCeiling", surfaceType))
                wallType=QString("floor");
        }
        else
        {
            txtList << QString("\"zones\":[\"%1\"]").arg(name0);
            txtList << unquoted.arg("azimuth").arg(180.0*surface.azimuth()/pi);
            txtList << unquoted.arg("tilt").arg(180.0*surface.tilt()/pi);
        }
        txtList << quoted.arg("type").arg(wallType);
        entryTxtList << QString("{")+txtList.join(",")+QString("}");
    } 
    if(entryTxtList.size())
        outputTxtList << QString("\"walls\":[")+entryTxtList.join(",")+QString("]");

    // Wrap up and get out
    if(outputTxtList.size())
        output += outputTxtList.join(",");
    
    output += QString("}");
    return boost::optional<QString>(output);
}
} // contam
} // openstudio
