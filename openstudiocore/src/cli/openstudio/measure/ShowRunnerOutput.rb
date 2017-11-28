def show_output(result)

  puts "**MEASURE APPLICABILITY**"
  applicability = result.value.value
  if applicability ==  -1
    puts "#{applicability} = Not Applicable"
  elsif applicability == 0 
    puts "#{applicability} = Success"
  elsif applicability == 1 
    puts "#{applicability} = Fail"
  end
 
  puts "**INITIAL CONDITION**"
  if result.initialCondition.empty?
    #do nothing
  else
    puts result.initialCondition.get.logMessage
  end  
      
  puts "**FINAL CONDITION**"
  if result.finalCondition.empty?
    #do nothing
  else
    puts result.finalCondition.get.logMessage
  end    
  
  puts "**INFO MESSAGES**"  
  result.info.each do |info_msg|
    puts "#{info_msg.logMessage}"
  end

  puts "**WARNING MESSAGES**"  
  result.warnings.each do |info_msg|
    puts "#{info_msg.logMessage}"
  end

  puts "**ERROR MESSAGES**"  
  result.errors.each do |info_msg|
    puts "#{info_msg.logMessage}"
  end

  os_version = OpenStudio::VersionString.new(OpenStudio::openStudioVersion())
  min_version_registerValue = OpenStudio::VersionString.new("1.2.2")

  if os_version >= min_version_registerValue
    puts "***Machine-Readable Attributes**"
    puts OpenStudio::toJSON(result.attributes) if not result.attributes.empty?
  end

  puts "" #space between measures for readability in output
  puts ""
  
end
