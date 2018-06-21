#!/bin/bash
# IMPORTANT!
# Before running this file, update the following WITHIN THIS FILE
# MACROS_DIRECTORY (although this can be specified on the command line)
# EMAIL (although this can be specified on the command line)
# Before running this file, update the following in Fun4All_G4_EICDetector.C:
# Set readeictree to true
# Set usegun to false
# Also if you have $HOME/.login file that you need for setting up your enviroment before your macro runs,
# uncomment "source $HOME/.login", which can be found near the end of this script

# Usage: run-pythia.sh [OPTION]... [FILE]...
# -m,--macros-directory         specifies directory where g4simulation Fun4AllMacroes are
# -n,--number-events            specifies number of events to run per pythia file
# -e,--email                    specifies the email that condor emails once jobs are done
# -r,--results-directory        specifies which directory to store the results in
# -h,--help                     displays this message


# Directory where g4simulation Fun4AllSimulations are
MACROS_DIRECTORY=/direct/sphenix+u/$USER/macros/macros/g4simulations
EMAIL='giorgian.borca-tasciuc@stonybrook.edu' # email that condor emails user when jobs are done

# Name of directory where simulation results are placed
DNAME=$(mktemp -d -t pythia-simulation.XXX -p .)
NEVENTS=1 # number of events to run

TEMP=`getopt -o m:n:p:e:r:h --long macros-directory:,number-events:,pythia-file:,email:,results-directory:,help \
     -n 'run-pythia.sh' -- "$@"`

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
eval set -- "$TEMP"
while true ; do
    case "$1" in
        -m|--macros-directory) MACROS_DIRECTORY=$2; shift 2 ;;
        -n|--number-events) NEVENTS=$2; shift 2 ;;
        -e|--email) EMAIL=$2; shift 2 ;;
        -r|--results-directory) rmdir $DNAME; DNAME=$2; mkdir $DNAME; shift 2 ;;
	-h|--help) cat << EOF
Usage: run-pythia.sh [OPTION]... [FILE]...
-m,--macros-directory			specifies directory where g4simulation Fun4AllMacroes are
-n,--number-events			specifies number of events to run
-e,--email				specifies the email that condor emails once jobs are done
-r,--results-directory			specifies which directory to store the results in
-h,--help				displays this message
EOF
		exit 0
		;;

	--) shift; break ;;
        *) echo "Internal error!" >&2 ; exit 1 ;;
    esac
done


cd $DNAME
cp -R $MACROS_DIRECTORY/* ./ # Copy over necessary macros

# Create cleanup file for after jobs are done to automatically remove old files
cat > cleanup.sh << EOF
#!/bin/bash
rm *.log *.err *.out *.C vis.mac *.sh *.job
EOF
chmod u+x cleanup.sh
for pythia_file do
	BASE_NAME="$pythia_file" # Job/shell script files are prefixed with this base
	CONDOR_EXECUTABLE_NAME=$BASE_NAME.sh
	CONDOR_OUT_NAME=$BASE_NAME.out # output of job
	CONDOR_ERROR_NAME=$BASE_NAME.err # name of file to place stderr of job
	CONDOR_LOG_NAME=$BASE_NAME.log # name of file to place condor log
	CONDOR_JOB_NAME=$BASE_NAME.job # name of job file for condor to run

	JOB=$"
	Executable      = $(pwd)/$CONDOR_EXECUTABLE_NAME	\n
	Output          = $(pwd)/$CONDOR_OUT_NAME		\n
	Error           = $(pwd)/$CONDOR_ERROR_NAME		\n
	Log             = $(pwd)/$CONDOR_LOG_NAME		\n
	Universe        = vanilla				\n
	Priority        = +0					\n
	Input           = /dev/null				\n
	GetEnv          = False					\n
	Initialdir      = $(pwd)				\n
	+Experiment     = \"sphenix\"				\n
	+Job_Type       = \"cas\"				\n
	Notification    = Always				\n
	Notify_user     = $EMAIL				\n
	Queue"

	echo -e $JOB | sed "s/^[ \t]*//" > $CONDOR_JOB_NAME

	ROOT_FILE="$(pwd)/G4EICDetector.root"
	CONDOR_EXECUTABLE="time root -b -q Fun4All_G4_EICDetector.C\($NEVENTS,\\\"$pythia_file\\\",\\\"$ROOT_FILE\\\"\)"

	cat > $CONDOR_EXECUTABLE_NAME << EOF
#!/bin/tcsh
setenv HOME $HOME
source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
source \$i
end
# Uncomment this if you have a login file 
# source \$HOME/.login
source /opt/sphenix/core/bin/sphenix_setup.csh
cd $PWD;
$CONDOR_EXECUTABLE
EOF
	chmod a+x $CONDOR_EXECUTABLE_NAME 
	condor_submit "$CONDOR_JOB_NAME"
done
