
######################################################################
# == Synopsis 
#
#   Load an isomodel and run the isomodel code 
#
# == Usage
#
#   ruby runiso.rb ARGV[0] 
# 
#   ARGV[0] - path to OSM file for conversion
#
# == Examples (replace input.osm with your input file/path
#
#   ruby runiso.rb input.osm
#

#V0.3 28-Sep-2013 RTM
# save results to a csv file in a format similar to the OpenStudio results tab
# that means electricity in kWh (not kWh/m2), gas in MBtu


# V0.2 18-Sep-2013 RTM
# 
#print out the results file



require 'openstudio'

if ARGV.length < 1
	puts "Usage: ruby runiso.rb 'C:\\path\\to\\model.iso'"
	exit(-1)
 end

time=Time.new   # get the current clock time.   use time.ctime to get time/date for printout

input_file = ARGV[0]

# print out the name of this script with $0
puts "Running  #{$0}  on input file  #{input_file}"


basename = File.basename(input_file,".*")	# get the base file without the path to it or extensions at the end
dirname = File.dirname(input_file)	# get the path to the file without the file itself
# add a log to the end of the isofile to name log file
runiso_log_file = File.join(dirname, basename + ".runiso.log")


# ISO output in total kWh and kBtu
runiso_output_file = File.join(dirname, basename + ".runiso.csv")

# eui in kWh/m2 only output file is base filename + .eui + .csv
eui_output_file = File.join(dirname, basename + ".eui.csv")
puts "Writing processing log to #{runiso_log_file} on #{time.ctime}"
puts "Writing output to #{runiso_output_file} and #{eui_output_file}"

# create log file and start writing out 
logfile = File.new(runiso_log_file, 'w') 
logfile.puts "Starting #{runiso_log_file}  on #{time.ctime}"
logfile.puts "Running ruby #{$0} #{input_file} "


logfile.puts "Input File: #{input_file}"
logfile.puts "Output Files: #{runiso_output_file} and #{eui_output_file}"

puts "...Loading ISO Model"

logfile.puts "*****************************"
logfile.puts "  Loading ISOmodel"
logfile.puts "*****************************"

# # load the model
# model = OpenStudio::Model::Model::load(OpenStudio::Path.new(input_file))
# if model.empty?
  # puts "Model cannot be read"
  # return false
# end
# model = model.get

puts "...Creating ISOmodel object"
logfile.puts "Creating ISOmodel object"

userModel = OpenStudio::ISOModel::UserModel.new
userModel.save(OpenStudio::Path.new("blankmodel.ism"))

puts "...Loading ISOmodel from file #{input_file}"
logfile.puts "Loading ISOmodel from file #{input_file}"
puts

userModel.load(OpenStudio::Path.new(input_file))


puts userModel.weatherFilePath

puts "...Running ISOmodel Simulation"

logfile.puts "*****************************"
logfile.puts "  Running ISOmodel Simulation"
logfile.puts "*****************************"



t1 = Time.now
simisoModel= userModel.toSimModel
simResults = simisoModel.simulate
t2 = Time.now
elapsed = ((t2 - t1)*1000).to_i

puts "Elapsed time = #{elapsed} milliseconds"
logfile.puts "Elapsed time = #{elapsed} milliseconds"

puts
puts "...Saving ISOmodel Simulation Results"
puts

logfile.puts "*****************************"
logfile.puts "  Saving ISOmodel Simulation Results"
logfile.puts "*****************************"




  
 # copy results to a more useful table by creating a 14x12 arrays called elec_enduse and gas_enduse
 
elec_enduse=Array.new(14) {Array.new(12,0.0)}
gas_enduse=Array.new(14) {Array.new(12,0.0)}
(0..13).each do |cat|
	(0..11).each do |month|
		elec_enduse[cat][month] = simResults.monthlyResults[month].getEndUse(OpenStudio::EndUses.fuelTypes[0], OpenStudio::EndUses.categories[cat])
		gas_enduse[cat][month] = simResults.monthlyResults[month].getEndUse(OpenStudio::EndUses.fuelTypes[1], OpenStudio::EndUses.categories[cat])
	end	
end
  
  
floorArea=userModel.floorArea
months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'] 

# openstudio::OPENSTUDIO_ENUM(EndUseFuelType ,((Electricity)(Electricity))((Gas)(Natural Gas))((OtherFuel)(Other Fuel))((DistrictCooling)(District Cooling))((DistrictHeating)(District Heating))((Water)(Water)) 	 )	
#openstudio::OPENSTUDIO_ENUM(EndUseCategoryType ,((Heating)(Heating))((Cooling)(Cooling))((InteriorLights)(Interior Lighting))((ExteriorLights)(Exterior Lighting))((InteriorEquipment)(Interior Equipment))((ExteriorEquipment)(Exterior Equipment))((Fans)(Fans))((Pumps)(Pumps))((HeatRejection)(Heat Rejection))((Humidifier)(Humidification))((HeatRecovery)(Heat Recovery))((WaterSystems)(Water Systems))((Refrigeration)(Refrigeration))((Generators)(Generators)) 	 )	
 puts "Monthly Energy Use By Category in kWh/m2"
 puts "Month \tEHeat \tECool \tEIntLt \tEExtLt \tEInEqp \tEFan \tEPump \tEDHW \tGHeat \tGInEqp \tGDHW" 
 
 logfile.puts "Monthly Energy Use By Category in kWh/m2"
 logfile.puts "Month \tEHeat \tECool \tEIntLt \tEExtLt \tEInEqp  \tEFan \tEPump \tEDHW \tGHeat \tGInEqp \tGDHW" 
 
 totaluse=Array.new(13,0.0)
 
 #totaluse=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
 
 (0..11).each do |month|
	print months[month]
	logfile.print months[month], "\t"

	# electric heating
	use=elec_enduse[0][month]
	totaluse[0] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	# electric cooling
	use=elec_enduse[1][month]
	totaluse[1] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	# electric interior lighting
	use=elec_enduse[2][month]
	totaluse[2] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	# electric exterior lighting
	use=elec_enduse[3][month]
	totaluse[3] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	#electric interior equipment
	use=elec_enduse[4][month]
	totaluse[4] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	
	
	#electric fan
	use=elec_enduse[6][month]
	totaluse[5] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)

	#electric pump
	use=elec_enduse[7][month]
	totaluse[6] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	#electric DHW
	use=elec_enduse[11][month]
	totaluse[7] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	#gas heat
	use=gas_enduse[0][month]	
	totaluse[8] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	

	#gas Int Equip
	use=gas_enduse[4][month]	
	totaluse[9] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	

	# gas DHW
	use=gas_enduse[11][month]	
	totaluse[10] +=use
	printf("\t%3.2f",use)
	logfile.printf("\t%3.2f",use)
	
	print "\n"
	logfile.print "\n"
end

(0..11).each do |i|
	printf("----\t")
	logfile.printf("----\t")
end
print "\n"
logfile.print "\n"

print "Total\t"
logfile.print "Total\t"


(0..10).each do |i|
	printf("%3.2f\t",totaluse[i])
	logfile.printf("%3.2f\t",totaluse[i])
end

print "\n\n"
logfile.print "\n\n"

electotal = 0.0
(0..7).each do |i|
electotal += totaluse[i]
end

gastotal = 0.0
(8..10).each do |i|
gastotal += totaluse[i]
end


total=electotal+gastotal

isototal = simResults.totalEnergyUse();

printf("Elec Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu \n",electotal,electotal*0.3171,electotal*floorArea,electotal*floorArea*3.412)
printf("Gas Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",gastotal,gastotal*0.3171,gastotal*floorArea,gastotal*floorArea*3.412)
printf("All Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",total,total*0.3171,total*floorArea,total*floorArea*3.412)
printf("Total From ISOResults: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n", isototal, isototal*0.3171, isototal*floorArea,isototal*floorArea*3.412)

logfile.printf("Elec Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu \n",electotal,electotal*0.3171,electotal*floorArea,electotal*floorArea*3.412)
logfile.printf("Gas Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",gastotal,gastotal*0.3171,gastotal*floorArea,gastotal*floorArea*3.412)
logfile.printf("All Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",total,total*0.3171,total*floorArea,total*floorArea*3.412)

logfile.printf("Total From ISOResults: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n", isototal, isototal*0.3171, isototal*floorArea,isototal*floorArea*3.412)

# save output file to CSV format

categories=["Heating","Cooling","Interior Lighting","Exterior Lighting","Interior Equipment","Exterior Equipment", \
	"Fans","Pumps","Heat Rejection","Humidification","Heat Recovery","Water Systems","Refrigeration","Generators"]

elec_cat_total=Array.new(12,0.0)
gas_cat_total=Array.new(12,0.0)

elec_month_total = Array.new(14,0.0)
gas_month_total = Array.new(14,0.0)

elec_total = 0.0
gas_total = 0.0
# find the category totals for each month and total for year
(0..11).each do |month|
	(0..13).each do |cat|
		elec_cat_total[month] += elec_enduse[cat][month]
		gas_cat_total[month] += gas_enduse[cat][month]
	end
	elec_total += elec_cat_total[month]
	gas_total += gas_cat_total[month]
end

# open kWh and kBtu output file
outfile = File.new(runiso_output_file, 'w')

# find the monthl totals for each category
(0..13).each do |cat|
	(0..11).each do |month|
		elec_month_total[cat]+= elec_enduse[cat][month]
		gas_month_total[cat]+= gas_enduse[cat][month]
	end
end

# generate the electricity consumption table in kWh
outfile.puts "Electricity Consumption (kWh)"
outfile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"

(0..13).each do |cat|
	outfile.print categories[cat],", "
	(0..11).each do |month|
		outfile.printf("%1.5g, ",elec_enduse[cat][month]*floorArea)
	end
	outfile.printf("%1.5g \n",elec_month_total[cat]*floorArea)
end

outfile.print "Total,"
(0..11).each do |month|
	outfile.printf("%1.5g, ", elec_cat_total[month]*floorArea)
end
outfile.printf("%1.5g \n", elec_total*floorArea)
outfile.print"\n"


# generate the gas consumption table in MMBtu (millions of Btu)
outfile.puts "Gas Consumption (MMBtu)"
outfile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"
kWh2MMBTU = 3.412/1000
(0..13).each do |cat|
	outfile.print categories[cat],", "
	(0..11).each do |month|
		outfile.printf("%4.3g, ",gas_enduse[cat][month]*kWh2MMBTU*floorArea)
	end
	outfile.printf("%4.3g \n",gas_month_total[cat]*kWh2MMBTU*floorArea)
end


outfile.print "Total,"
(0..11).each do |month|
	outfile.printf("%4.3g, ", gas_cat_total[month]*kWh2MMBTU*floorArea)
end
outfile.printf("%4.3g \n", gas_total*kWh2MMBTU*floorArea)
outfile.close

	
logfile.puts "Output CSV generation is complete,  #{runiso_output_file} has been saved to disk"
puts "Output CSV generation is complete,  #{runiso_output_file} has been saved to disk"
	
# open EUIfile
euifile = File.new(eui_output_file, 'w')

# generate the electricity consumption table in kWh
euifile.puts "Electricity EUI (kWh/m2)"
euifile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"

(0..13).each do |cat|
	euifile.print categories[cat],", "
	(0..11).each do |month|
		euifile.printf("%1.5g, ",elec_enduse[cat][month])
	end
	euifile.printf("%1.5g \n",elec_month_total[cat])
end

euifile.print "Total,"
(0..11).each do |month|
	euifile.printf("%1.5g, ", elec_cat_total[month])
end
euifile.printf("%1.5g \n", elec_total)
euifile.print"\n"


# generate the gas consumption table in MMBtu (millions of Btu)
euifile.puts "Gas EUI (kWh/m2)"
euifile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"
(0..13).each do |cat|
	euifile.print categories[cat],", "
	(0..11).each do |month|
		euifile.printf("%4.3g, ",gas_enduse[cat][month])
	end
	euifile.printf("%4.3g \n",gas_month_total[cat])
end

euifile.print "Total,"
(0..11).each do |month|
	euifile.printf("%4.3g, ", gas_cat_total[month])
end
euifile.printf("%4.3g \n", gas_total)

euifile.puts 
euifile.puts "Total EUIs (kWh/m2)"
euifile.puts "Month , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"

euifile.print "Electric,"
(0..11).each do |month|
	euifile.printf("%1.5g, ", elec_cat_total[month])
end
euifile.printf("%1.5g \n", elec_total)

euifile.print "Gas,"
(0..11).each do |month|
	euifile.printf("%4.3g, ", gas_cat_total[month])
end
euifile.printf("%4.3g \n", gas_total)

euifile.print "Total,"
(0..11).each do |month|
	euifile.printf("%4.3g, ", elec_cat_total[month]+gas_cat_total[month])
end
euifile.printf("%4.3g \n", elec_total+gas_total)

euifile.close
logfile.puts "EUI CSV generation is complete,  #{eui_output_file} has been saved to disk"
puts "EUI CSV generation is complete,  #{eui_output_file} has been saved to disk"

logfile.puts "Closing logfile"
logfile.close
