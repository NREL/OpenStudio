######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

read_header1 = false
read_header2 = false
dateTimes = Array.new
directNormalIllumimances = Array.new
diffuseHorizontalIlluminances = Array.new
averageIlluminances = Array.new
pointIlluminances = Array.new

CSV.open(ARGV[0], 'r') do |row|

  next if row.nil?
  next if row.empty?
  next if /^\#\#/.match(row[0])
  
  if not read_header1
    ## OpenStudio Daylight Simulation Results file
    read_header1 = true
  elsif not read_header2
    ## Header: xmin ymin z xmax ymin z xmax ymax z xspacing yspacing
    read_header2 = true
  else
  
    if row[0].nil?
      raise "Null data in illuminance map file"
    end
    
    ## Data: month,day,time,directNormalIllumimance(external),diffuseHorizontalIlluminance(external),pointIlluminance [lux]
    date = OpenStudio::Date.new(OpenStudio::MonthOfYear.new(row[0].to_i), row[1].to_i)
    time = OpenStudio::Time.new(row[2])
    dateTime = OpenStudio::DateTime.new(date, time)
    
    dateTimes << dateTime
    directNormalIllumimances << row[3].to_f
    diffuseHorizontalIlluminances << row[4].to_f
    
    sum = 0
    illuminances = Array.new
    row[5..-1].each do |illuminance|
    
      if illuminance.to_f < 0
        puts "Warning illuminance #{illuminance} less than zero at #{dateTime}, will be reset to 0"
        illuminance = 0
      end
      
      sum += illuminance.to_f
      illuminances << illuminance.to_f
    end
    averageIlluminances << sum/illuminances.size.to_f
    pointIlluminances << illuminances
  
  end
  
end

n = dateTimes.size

dateTimeVector = OpenStudio::Vector.new(n)
directNormalIllumimanceVector = OpenStudio::Vector.new(n)
diffuseHorizontalIlluminanceVector = OpenStudio::Vector.new(n)
averageIlluminanceVector = OpenStudio::Vector.new(n)

dateTimes.each_index do |i|
  dateTimeVector[i] = (dateTimes[i] - dateTimes[0]).totalHours
  directNormalIllumimanceVector[i] = directNormalIllumimances[i]
  diffuseHorizontalIlluminanceVector[i] = diffuseHorizontalIlluminances[i]
  averageIlluminanceVector[i] = averageIlluminances[i]
end

directNormalIllumimanceData = OpenStudio::VectorLinePlotData::create(dateTimeVector, directNormalIllumimanceVector)
diffuseHorizontalIlluminanceData = OpenStudio::VectorLinePlotData::create(dateTimeVector, diffuseHorizontalIlluminanceVector)
averageIlluminanceData = OpenStudio::VectorLinePlotData::create(dateTimeVector, averageIlluminanceVector)
  
lp = OpenStudio::LinePlot::create().get
lp.linePlotData(directNormalIllumimanceData, "Direct Normal Illuminance", OpenStudio::Color.new(OpenStudio::Black))
lp.linePlotData(diffuseHorizontalIlluminanceData, "Diffuse Horizontal Illuminance", OpenStudio::Color.new(OpenStudio::Red))
lp.linePlotData(averageIlluminanceData, "Average Illuminance", OpenStudio::Color.new(OpenStudio::Blue))
lp.axesFontSize(12)
lp.tickFontSize(10)
lp.bottomAxisTitle("Hours")
lp.leftAxisTitle("Average Illuminance")
lp.generateImage(OpenStudio::Path.new(ARGV[0].gsub(".ill", ".png")))
