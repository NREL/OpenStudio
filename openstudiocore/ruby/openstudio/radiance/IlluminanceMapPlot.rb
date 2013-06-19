######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'openstudio'
require 'csv'
require 'optparse'

OpenStudio::Application::instance.application

read_header1 = false
read_header2 = false
dayOfYears = Array.new
dateTimes = Array.new
directNormalIllumimances = Array.new
diffuseHorizontalIlluminances = Array.new
averageIlluminances = Array.new
mapName = ARGV[0]
last_day_of_year = -1


# options parser

options = {}
optparse = OptionParser.new do|opts|
  # Set a banner, displayed at the top
  # of the help screen.
  opts.banner = "Usage: AnnualPlot.rb [options] datafile"
   
  opts.separator ""
  opts.separator "Options:"
  
  options[:setpoint] = "300"
  opts.on( '-s', '--setpoint <value>', "Dimming setpoint" ) do|f|
    options[:setpoint] = f
  end
  
  options[:yMax] = "4000"
  opts.on( '-y', '--ymax <value>', "Y axis scale max" ) do|f|
    options[:yMax] = f
  end
  
  opts.on_tail("-h", "--help", "Show this message") do
    puts opts
    exit
  end

end  

# after all that, parse the options for crying out loud.
optparse.parse!

xmin = nil 
xmax = nil
ymin = nil
ymax = nil
z = nil
xspacing = nil
yspacing = nil
xPoints = nil
yPoints = nil

CSV.open(ARGV[0], 'r') do |row|
  next if row.nil?
  next if row.empty?
  next if /^\#\#/.match(row[0])

  if not read_header1
    ## OpenStudio Daylight Simulation Results file
    read_header1 = true
  elsif not read_header2  
  
    if row.size == 1
      row = row[0].split(' ')
    end
  
    ## Header: xmin ymin z xmax ymin z xmax ymax z xspacing yspacing
    read_header2 = true
    xmin = row[0].to_f
    ymin = row[1].to_f
    xmax = row[3].to_f
    ymax = row[7].to_f
    z = row[3].to_f
    xspacing = row[9].to_f
    yspacing = row[10].to_f
    
    xPoints = OpenStudio::deltaSpace(xmin, xmax, xspacing)
    yPoints = OpenStudio::deltaSpace(ymin, ymax, yspacing)
    
  else
    ## Data: month,day,time,directNormalIllumimance(external),diffuseHorizontalIlluminance(external),pointIlluminance [lux]
    
    date = OpenStudio::Date.new(OpenStudio::MonthOfYear.new(row[0].to_i), row[1].to_i)
    time = OpenStudio::Time.new(row[2])
    dayOfYear = date.dayOfYear
    dateTime = OpenStudio::DateTime.new(date, time)
    
    # sometimes midnight is reported as 23:59 other times as 00:00
    if time.totalHours == 0
      if dayOfYear == 1
        dayOfYear = 365
      else
        dayOfYear = dayOfYear - 1
      end
    end
    
    #puts dayOfYear.to_s + ", " + time.totalHours.to_s
    #hard-coded date (Julian day) and time (integer hour)
<<<<<<< .mine
    if dayOfYear == 81 and time.totalHours == 12
    
      puts "I found the row"
      puts row
      puts row[5..-1].size
      puts yPoints.size.to_f
      puts yPoints
      puts xPoints.size
      puts yPoints.size*xPoints.size

=======
    if dayOfYear == 356 and time.totalHours == 12
			puts "here"
>>>>>>> .r8595
      if row[5..-1].size == yPoints.size*xPoints.size

        illuminanceMatrix = OpenStudio::Matrix.new(yPoints.size, xPoints.size)
        i = 0
        j = 0
        row[5..-1].each do |illuminance|

          if i == xPoints.size
            i = 0
            j = j+1
          end

          puts "#{i}, #{j}, #{illuminance}"

          illuminanceMatrix[i,j] = illuminance.to_f

          i = i +1
        end

        puts illuminanceMatrix

        interpMethod = OpenStudio::LinearInterp # LinearInterp, NearestInterp, HoldLastInterp, HoldNextInterp
        data = OpenStudio::MatrixFloodPlotData::create(xPoints, yPoints, illuminanceMatrix, interpMethod)
        fp = OpenStudio::FloodPlot.new
        fp.floodPlotData(data)
        
        fp.colorMapRange(0, options[:yMax].to_f)
				fp.rightAxisTitle("Daylight Illuminance (lux)")
				fp.plotTitle("12/22 12:00 p.m. CLR - OpenStudio")
        fp.leftAxisTitle("y (m)")
        fp.bottomAxisTitle("x (m)")
        fp.generateImage(OpenStudio::Path.new("./illuminanceMatrix.png"))
        
        break
        
      else
        
        puts "Incorrect data size"
        
      end
    
    end
    
  end
  
end
