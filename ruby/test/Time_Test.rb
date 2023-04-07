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

require 'minitest/autorun'

class Time_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def numberOfSeconds(days, hours, minutes, seconds)
    return 24*60*60*days + 60*60*hours + 60*minutes + seconds
  end

  def numberOfDays(days, hours, minutes, seconds)
    return days + 1.0/(24.0)*hours + 1.0/(24.0*60.0)*minutes + 1.0/(24.0*60.0*60.0)*seconds
  end

  def test_fracDaysConstructor

    testTime = OpenStudio::Time.new(0.0)
    assert_equal(testTime.totalSeconds(), 0)

    testTime = OpenStudio::Time.new(0.5)
    assert_equal(testTime.totalSeconds(), 60*60*12)

    testTime = OpenStudio::Time.new(1.0)
    assert_equal(testTime.totalSeconds(), 60*60*24)
  end


  def test_daysHoursMinutesSecondsConstructor

    testTime = OpenStudio::Time.new(0,0,0,0)
    assert_in_delta(testTime.totalDays(), 0.0/(24*60*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 0.0/(60*60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 0.0/60, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 0, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)

    testTime = OpenStudio::Time.new(0,0,0,1)
    assert_in_delta(testTime.totalDays(), 1.0/(24*60*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 1.0/(60*60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 1.0/60, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 1, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 1, 0.00001)

    testTime = OpenStudio::Time.new(0,0,1,0)
    assert_in_delta(testTime.totalDays(), 1.0/(24*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 1.0/(60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 1.0, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 60, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 1, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)

    testTime = OpenStudio::Time.new(0,1,0,0)
    assert_in_delta(testTime.totalDays(), 1.0/24, 0.00001)
    assert_in_delta(testTime.totalHours(), 1.0, 0.00001)
    assert_in_delta(testTime.totalMinutes(), 60.0, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 60*60, 0.00001)
    assert_in_delta(testTime.days(), 0, 0.00001)
    assert_in_delta(testTime.hours(), 1, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)

    testTime = OpenStudio::Time.new(1,0,0,0)
    assert_in_delta(testTime.totalDays(), 1.0, 0.00001)
    assert_in_delta(testTime.totalHours(), 24.0, 0.00001)
    assert_in_delta(testTime.totalMinutes(), 24.0*60.0, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 24.0*60*60, 0.00001)
    assert_in_delta(testTime.days(), 1, 0.00001)
    assert_in_delta(testTime.hours(), 0, 0.00001)
    assert_in_delta(testTime.minutes(), 0, 0.00001)
    assert_in_delta(testTime.seconds(), 0, 0.00001)

    testTime = OpenStudio::Time.new(1,1,1,1)
    assert_in_delta(testTime.totalDays(), 1.0+1.0/24+1.0/(24*60)+1.0/(24*60*60), 0.00001)
    assert_in_delta(testTime.totalHours(), 24+1.0+1.0/60+1.0/(60*60), 0.00001)
    assert_in_delta(testTime.totalMinutes(), 24*60+60+1+1.0/60, 0.00001)
    assert_in_delta(testTime.totalSeconds(), 24*60*60+60*60+60+1, 0.00001)
    assert_in_delta(testTime.days(), 1, 0.00001)
    assert_in_delta(testTime.hours(), 1, 0.00001)
    assert_in_delta(testTime.minutes(), 1, 0.00001)
    assert_in_delta(testTime.seconds(), 1, 0.00001)
  end

  def test_to_s
    assert(!OpenStudio::Time.new(-1.0).to_s.nil?)
    assert(!OpenStudio::Time.new(0.0).to_s.nil?)
    assert(!OpenStudio::Time.new(0.5).to_s.nil?)
    assert(!OpenStudio::Time.new(1.0).to_s.nil?)
    assert(!OpenStudio::Time.new(2.0).to_s.nil?)
  end

end


