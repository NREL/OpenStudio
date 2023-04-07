########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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
