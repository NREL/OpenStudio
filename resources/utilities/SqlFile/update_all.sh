# Bash script to run a bunch of 1ZoneEvapCooler.idf throughout versions of E+...

# Add your OAUTH token here (public access only)
token=
if [ ! -z $token ]; then
  curl_token="-H \"Authorization: token $token\""
fi;

# All versions you want to run
# declare -a all_versions=("8.5.0" "8.6.0" "8.7.0" "8.8.0" "8.9.0" "9.0.1" "9.1.0" "9.2.0" "9.3.0", "9.4.0", "9.5.0", "9.6.0", "22.1.0", "22.2.0", "23.1.0", "23.2.0", "24.1.0", "24.2.0")
declare -a all_versions=("24.2.0")

# DO NOT RERUN IF SQL ALREADY THERE
rerun_if_already_there=false

if [ ! -f USA_CO_Golden-NREL.724666_TMY3.epw ]; then
  wget https://github.com/NREL/EnergyPlus/raw/develop/weather/USA_CO_Golden-NREL.724666_TMY3.epw
fi;


for ep_version in "${all_versions[@]}"; do
  ep_version_dash=$(echo $ep_version | sed -r 's/\./-/g')

  sql_file=1ZoneEvapCooler-V$ep_version_dash.sql
  if [ -f "$sql_file" ]; then
    if [[ "$rerun_if_already_there" == false ]]; then
      echo "Already found $sql_file, skipping"
      continue
    fi
  fi

  # Special case
  if [ "$ep_version" == "9.1.0" ]; then
    ep_version=91AndBugFix4
  fi
  echo ""
  echo ""
  echo "$ep_version"

  # If Idf file isn't there, get it, and do the necessary replacements
  idf_file="1ZoneEvapCooler-V$ep_version_dash.idf"
  if [ ! -f $idf_file ]; then
    wget https://raw.githubusercontent.com/NREL/EnergyPlus/v$ep_version/testfiles/1ZoneEvapCooler.idf
    mv 1ZoneEvapCooler.idf $idf_file

    echo "  Output:SQLite," >> $idf_file
    echo '    SimpleAndTabular;        !- Option Type' >> $idf_file
    echo "" >> $idf_file

    sed -i 's/Output:Variable,\*,Zone Mean Air Temperature,hourly;/Output:Variable,\*,Zone Mean Air Temperature,Annual;\n\n  Output:Variable,\*,Zone Mean Air Temperature,RunPeriod;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Environment;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Monthly;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Daily;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Hourly;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Timestep;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Detailed;/g' $idf_file

  fi;

  # If not already installed, get it from GitHub
  ep_exe=$(find /usr/local/EnergyPlus-$ep_version_dash -maxdepth 1 -name "energyplus")
  if [ -z "$ep_exe" ]; then

    # Prefer using the tar.gz (doesn't require install via sh, which needs sudo generally speaking, and more importantly needs manual input)
    # Starting 8.8.0, tar.gz is available.
    release_json=$(curl -sL $curl_token https://api.github.com/repos/NREL/EnergyPlus/releases/tags/v$ep_version)
    ubuntu_version=$(lsb_release -rs)
    tar_gz_link=$(echo $release_json  | jq -r '.assets | .[] | select(.name | contains('\"Ubuntu$ubuntu_version\"')) | select(.name | contains("tar.gz")) | .browser_download_url')
    # If not found, fall back on the generic 'Linux' installer rather than the (eg: 20.04) specific one
    if [ -z "$tar_gz_link" ]; then
      tar_gz_link=$(echo $release_json  | jq -r '.assets | .[] | select(.name | contains("Linux")) | select(.name | contains("tar.gz")) | .browser_download_url')
    fi;

    # If not found, fall back on the .sh installer
    if [ -z "$tar_gz_link" ]; then
      sh_link=$(echo $release_json| jq -r '.assets | .[] | select(.name | contains("Linux")) | select(.name | contains("sh")) | .browser_download_url')
      # sh_name=${sh_link##*/}
      echo $sh_link
      sh_name=$(basename -- $sh_link)

      if [ ! -f $sh_name ]; then
        echo "Downloading $sh_link"
        wget $sh_link
      else
        echo "$sh_name already exists"
      fi

      # Install: need user input
      chmod +x $sh_name
      sudo ./$sh_name

      ep_exe=$(find /usr/local/EnergyPlus-$ep_version_dash -name "energyplus")
      if [ -z "$ep_exe" ]; then
        echo "Couldn't find Eplus EXE for just-installed $ep_version"
      fi


    else

      #tar_gz_name=${tar_gz_link##*/}
      tar_gz_name=$(basename -- $tar_gz_link)
      if [ ! -f $tar_gz_name ]; then
        echo "Downloading $tar_gz_link"
        wget $tar_gz_link
      else
        echo "$tar_gz_name already exists"
      fi

      folder_name=${tar_gz_name%.tar.gz}
      if [ ! -d $folder_name ]; then
        echo "Extracting $tar_gz_name"
        tar xfz "$tar_gz_name"
      else
        echo "Already found $folder_name"
      fi

      ep_exe=$(find $folder_name/ -name "energyplus")

      if [ -z "$ep_exe" ]; then
        echo "Couldn't find Eplus EXE from tar.gz for $ep_version"
      else
        ep_exe="$(pwd)/$ep_exe"
      fi
    fi
  else
    echo "Found already installed $ep_version at $ep_exe"
  fi

  if [ -z "$ep_exe" ]; then
    echo "Couldn't find Eplus EXE for $ep_version"
  else
    # Run simulation
    $ep_exe -w USA_CO_Golden-NREL.724666_TMY3.epw -d out-$ep_version $idf_file
    mv out-$ep_version/eplusout.sql 1ZoneEvapCooler-V$ep_version_dash.sql
  fi
done


cmakelists=$(readlink -f "$(pwd)/../../CMakeLists.txt")
echo "Add in $cmakelists:"
for ep_version in "${all_versions[@]}"; do
  ep_version_dash=$(echo $ep_version | sed -r 's/\./-/g')
  sql_file=1ZoneEvapCooler-V$ep_version_dash.sql
  echo "  utilities/SqlFile/$sql_file"
done

sql_gtest=$(readlink -f "../../../src/utilities/sql/Test/SqlFile_GTest.cpp")
echo "Add in $sql_gtest:"
echo "In TEST_F(SqlFileFixture, Regressions):"
for ep_version in "${all_versions[@]}"; do
  ep_version_dash=$(echo $ep_version | sed -r 's/\./-/g')
  sql_file=1ZoneEvapCooler-V$ep_version_dash.sql
  echo "  regressionTestSqlFile(\"$sql_file\", 43.28, 20, 20);"
done
