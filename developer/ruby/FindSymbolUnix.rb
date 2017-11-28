symbol = ARGV[0]
puts "looking for symbol #{symbol} in #{Dir.pwd}"

Dir.glob("./**/*.a") do |p|
  output = `nm #{p} | grep #{symbol}`
  output.split("\n").each do |line|
    line.strip!
    if /^U/.match(line)
      #puts "undefined reference in #{p}"
    else
      puts "#{p} = #{line}"
    end
  end
end

