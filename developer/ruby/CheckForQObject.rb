dir = ARGV[0].gsub("\\", "/")

Dir.glob("#{dir}/*.hpp").each do |f|

  has_signals = false
  has_slots = false
  has_qobject = false
  
  File.open(f, 'r') do |file|
    while line = file.gets
      has_signals = true if /signals/.match(line)
      has_slots = true if /slots/.match(line)
      has_qobject = true if /Q_OBJECT/.match(line)
    end
  end
  
  if has_signals or has_slots
    if not has_qobject
      puts "#{f} has signals or slots but no Q_OBJECT macro"
    end
  else
    if has_qobject
      puts "#{f} has no signals or slots but does have Q_OBJECT macro"
    end
  end
end