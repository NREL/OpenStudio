require 'pathname'

dir = ARGV[0].gsub("\\", "/")

moc_files = []
File.open("#{dir}/CMakeLists.txt", 'r') do |file|
  start_moc = false
  while line = file.gets
    if start_moc
      moc_files << line.strip
    end
    start_moc = true if /set\(.*_moc/.match(line) 
    start_moc = false if /\)/.match(line) 
  end
end

errors = []
errors[0] = ["Has signals or slots or properties but no Q_OBJECT macro:"]
errors[1] = ["Has signals or slots or properties but is not moc'd:"]
errors[2] = ["Has no signals or slots or properties but does have Q_OBJECT macro:"]
errors[3] = ["Has no signals or slots or properties but is moc'd:"]
Dir.glob("#{dir}/*.hpp").each do |f|
  
  file_name = File.basename(f)
  class_name = file_name.gsub('_Impl','').gsub('.hpp','')

  has_signals = false
  has_slots = false
  has_qobject = false
  has_qproperty = false
  has_moc = moc_files.index(file_name)
  
  File.open(f, 'r') do |file|
    while line = file.gets
      has_signals = true if /^[^\/\\\*]*signals[\s]*:/.match(line)
      has_slots = true if /^[^\/\\\*]*slots[\s]*:/.match(line)
      has_qobject = true if /^[^\/\\\*]*Q_OBJECT/.match(line)
      has_qproperty = true if /^[^\/\\\*]*Q_PROPERTY/.match(line)
    end
  end
  
  if has_signals or has_slots or has_qproperty
    if not has_qobject
      errors[0] << "  #{file_name}, #{class_name}"
    end
    if not has_moc
      errors[1] << "  #{file_name}, #{class_name}"
    end
  else
    if has_qobject
      errors[2] << "  #{file_name}, #{class_name}"
    end
    if has_moc
      errors[3] << "  #{file_name}, #{class_name}" 
    end
  end
end

errors.each_index do |i|
  errors[i].each do |error|
    puts error
  end
  puts
end
