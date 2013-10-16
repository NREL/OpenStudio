/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  Copyright (c) 2013, The Pennsylvania State University.
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
#include <model/Model.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/filetypes/EpwFile.hpp>

#include <string>
#include <iostream>

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: epw2wth --input-path=./path/to/input.epw" << std::endl;
  std::cout << "   or: epw2wth input.epw" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  std::string inputPathString;
  std::string outputPathString;
  boost::program_options::options_description desc("Allowed options");

  desc.add_options()
    ("help,h", "print help message")
    ("input-path,i", boost::program_options::value<std::string>(&inputPathString), "path to input EPW file")
    ("output-path,o", boost::program_options::value<std::string>(&outputPathString), "path to output WTH file")
    ("quiet,q", "suppress progress output");

  boost::program_options::positional_options_description pos;
  pos.add("input-path", -1);
    
  boost::program_options::variables_map vm;
  // The following try/catch block is necessary to avoid uncaught
  // exceptions when the program is executed with more than one
  // "positional" argument - there's got to be a better way.
  try
  {
    boost::program_options::store(boost::program_options::command_line_parser(argc,
      argv).options(desc).positional(pos).run(), vm);
    boost::program_options::notify(vm);
  }

  catch(std::exception&)
  {
    std::cout << "Execution failed: check arguments and retry."<< std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }
  
  if(vm.count("help"))
  {
    usage(desc);
    return EXIT_SUCCESS;
  }

  if(!vm.count("input-path"))
  {
    std::cout << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }
  
  // Open the EPW file
  openstudio::path inputPath = openstudio::toPath(inputPathString);

  boost::optional<openstudio::EpwFile> epwFile;
  try
  {
    epwFile = openstudio::EpwFile(inputPath,true);
    OS_ASSERT(epwFile);
  }
  catch(std::exception&)
  {
    std::cout << "Could not open EPW file '" << inputPathString << "'";
    return EXIT_FAILURE;
  }

  openstudio::path outPath = inputPath.replace_extension(openstudio::toPath("wth").string());
  if(!outputPathString.empty())
  {
    outPath = openstudio::toPath(outputPathString);
  }

  if(!epwFile->translateToWth(outPath))
  {
    std::cout << "Translation to WTH file failed, check for errors and warnings and try again" << std::endl;
  }

  /*
  if(!epwFile->data().size())
  {
    std::cout << "Input EPW file has no data to translate" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << epwFile->city() << std::endl;
  std::cout << epwFile->data().size() << std::endl;

  

  std::string description = "Converted EPW file";

  QFile fp(openstudio::toQString(outPath));
  if(!fp.open(QFile::WriteOnly))
  {
    std::cout << "Failed to open file '" << openstudio::toString(outPath) << "'" << std::endl;
    return EXIT_FAILURE;
  }

  QTextStream stream(&fp);

  stream << "WeatherFile ContamW 2.0\n";
  stream << openstudio::toQString(description) << "\n";
  stream << QString("%1/%2\t!start date\n").arg(openstudio::month(epwFile->startDate().monthOfYear())).arg(epwFile->startDate().dayOfMonth());
  stream << QString("%1/%2\t!end date\n").arg(openstudio::month(epwFile->endDate().monthOfYear())).arg(epwFile->endDate().dayOfMonth());
  stream << "!Date\tDofW\tDtype\tDST\tTgrnd [K]\n";
  openstudio::Time delta(1,0);
  int dayofweek = epwFile->startDayOfWeek().value()+1;
  for(openstudio::Date current=epwFile->startDate();current<=epwFile->endDate();current += delta)
  {
    stream << QString("%1/%2\t%3\t%3\t0\t283.15\n")
      .arg(openstudio::month(current.monthOfYear()))
      .arg(current.dayOfMonth())
      .arg(dayofweek);
    dayofweek++;
    if(dayofweek > 7)
    {
      dayofweek=1;
    }
  }
  // Cheat to get data at the start time

  openstudio::EpwDataPoint firstPt = epwFile->data()[epwFile->data().size()-1];
  openstudio::DateTime dateTime = epwFile->data()[0].dateTime();
  std::cout << "Date/time of first data point: " << month(dateTime.date().monthOfYear()) << '/' << dateTime.date().dayOfMonth() 
    << ' ' << dateTime.time().hours() << ':' << dateTime.time().minutes() << ':' << dateTime.time().seconds() << std::endl;
  openstudio::Date date = epwFile->data()[0].date();
  std::cout << "Date of first data point: " << month(date.monthOfYear()) << '/' << date.dayOfMonth() << std::endl;
  openstudio::Time time = epwFile->data()[0].time();
  std::cout << "Time of first data point: " << time.hours() << ':' << time.minutes() << ':' << time.seconds() << std::endl;
  openstudio::Time dt = epwFile->timeStep();
  std::cout << "Time step: " << dt.hours() << "::" << dt.minutes() << std::endl;

  dateTime -= dt;

  firstPt.setDateTime(dateTime);

  */

  /*
  std::cout << month(dateTime.date().monthOfYear()) << '/' << dateTime.date().dayOfMonth() << ' '
    << dateTime.time().hours() << ':' << dateTime.time().minutes() << ':' << dateTime.time().seconds() << std::endl;

  std::cout << epwFile->startDayOfWeek().value()+1 << std::endl;

  std::cout << month(epwFile->startDate().monthOfYear()) << '/' << epwFile->startDate().dayOfMonth() << std::endl;
  */

  /*
  stream <<"!Date\tTime\tTa [K]\tPb [Pa]\tWs [m/s]\tWd [deg]\tHr [g/kg]\tIth [kJ/m^2]\tIdn [kJ/m^2]\tTs [K]\tRn [-]\tSn [-]\n";
  stream << firstPt.toWthString() << '\n';
  for(unsigned int i=0;i<epwFile->data().size();i++)
  {
    stream << epwFile->data()[i].toWthString() << '\n';
  }

  fp.close();
  */

  return EXIT_SUCCESS;
}
