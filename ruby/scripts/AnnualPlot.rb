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
require 'optparse'

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

    time = nil
    if /\d+/.match(row[2])
      # from int
      time = OpenStudio::Time.new(0,row[2].to_i,0)
    else
      # from string
      time = OpenStudio::Time.new(row[2])
    end

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

    if dayOfYear != last_day_of_year
      last_day_of_year = dayOfYear

      dayOfYears << dayOfYear
      dateTimes << Array.new
      directNormalIllumimances << Array.new
      diffuseHorizontalIlluminances << Array.new
      averageIlluminances << Array.new
    end

    dateTimes[-1] << dateTime
    directNormalIllumimances[-1] << row[3].to_f
    diffuseHorizontalIlluminances[-1] << row[4].to_f

    sum = 0
    had_zero = false
    illuminances = Array.new
    row[5..-1].each do |illuminance|

      if illuminance.to_f < 0
        puts "Warning illuminance #{illuminance} less than zero at #{dateTime}, will be reset to 0"
        illuminance = 0
      end

      sum += illuminance.to_f
      illuminances << illuminance.to_f
    end

    averageIlluminances[-1] << sum/illuminances.size.to_f

  end

end

def rotate(array)
  temp = array[-1].pop
  array[0].insert(0, temp)
end

dayOfYears.pop
#dateTimes[-1].pop
#directNormalIllumimances[-1].pop
#diffuseHorizontalIlluminances[-1].pop
#averageIlluminances[-1].pop

n = dayOfYears.size

dateTimeMatrix = OpenStudio::Matrix.new(n, 24)
directNormalIllumimanceMatrix = OpenStudio::Matrix.new(n, 24)
diffuseHorizontalIlluminanceMatrix = OpenStudio::Matrix.new(n, 24)
averageIlluminanceMatrix = OpenStudio::Matrix.new(n, 24)
aboveSetpointMatrix = OpenStudio::Matrix.new(n, 24)

for i in (0..n-1)
  for j in (0..23)
    dateTimeMatrix[i,j] = (dateTimes[i][j]-dateTimes[0][0]).totalHours
    directNormalIllumimanceMatrix[i,j] = directNormalIllumimances[i][j]
    diffuseHorizontalIlluminanceMatrix[i,j] = diffuseHorizontalIlluminances[i][j]
    averageIlluminanceMatrix[i,j] = averageIlluminances[i][j]

    if averageIlluminances[i][j] > options[:setpoint].to_f
      aboveSetpointMatrix[i,j] = 1
    else
      aboveSetpointMatrix[i,j] = 0
    end
  end
end

puts
puts "Report:"
puts "#{OpenStudio::sum(aboveSetpointMatrix)} hours above setpoint of #{options[:setpoint]}"

# fp = OpenStudio::FloodPlot::create(dateTimeMatrix).get
# fp.generateImage(OpenStudio::Path.new("./dateTime.png"))

# fp = OpenStudio::FloodPlot::create(directNormalIllumimanceMatrix).get
# fp.generateImage(OpenStudio::Path.new("./directNormalIllumimance.png"))
#
# fp = OpenStudio::FloodPlot::create(diffuseHorizontalIlluminanceMatrix).get
# fp.generateImage(OpenStudio::Path.new("./diffuseHorizontalIlluminance.png"))

fp = OpenStudio::FloodPlot::create(averageIlluminanceMatrix).get
fp.colorMapRange(0, options[:yMax].to_f)
fp.rightAxisTitle("Illuminance (lux)")
fp.bottomAxisTitle("Day of Year")
fp.plotTitle("Average Workplane Illuminance (#{mapName})")
fp.generateImage(OpenStudio::Path.new("./averageIlluminance.png"))

# fp = OpenStudio::FloodPlot::create(aboveSetpointMatrix).get
# fp.generateImage(OpenStudio::Path.new("./aboveSetpoint.png"))
