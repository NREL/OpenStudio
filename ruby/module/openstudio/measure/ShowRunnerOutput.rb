########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

def show_output(result)

  puts '**MEASURE APPLICABILITY**'
  applicability = result.value.value
  if applicability ==  -1
    puts "#{applicability} = Not Applicable"
  elsif applicability == 0
    puts "#{applicability} = Success"
  elsif applicability == 1
    puts "#{applicability} = Fail"
  end

  puts '**INITIAL CONDITION**'
  if result.stepInitialCondition.empty?
    #do nothing
  else
    puts result.stepInitialCondition.get
  end

  puts '**FINAL CONDITION**'
  if result.stepFinalCondition.empty?
    #do nothing
  else
    puts result.stepFinalCondition.get
  end

  puts '**INFO MESSAGES**'
  result.stepInfo.each do |info_msg|
    puts "#{info_msg}"
  end

  puts '**WARNING MESSAGES**'
  result.stepWarnings.each do |info_msg|
    puts "#{info_msg}"
  end

  puts '**ERROR MESSAGES**'
  result.stepErrors.each do |info_msg|
    puts "#{info_msg}"
  end

  puts '***Machine-Readable Attributes**'
  values = []
  result.stepValues.each do |value|
    values << value.string
  end
  puts "[\n#{values.join(',').strip}\n]"

  puts '***Files Generated**'
  result.stepFiles.each do |file|
    puts "#{file}"
  end

  puts '' #space between measures for readability in output
  puts ''

end
