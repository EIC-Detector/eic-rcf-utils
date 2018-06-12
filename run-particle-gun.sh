#!/bin/bash
# Before running this file, update the following IN THIS FILE:
# MACROS_DIRECTORY (although you can pass this in the command-line argument -m or --macros-directory)
# NEVENTS (although you can pass this in the command-line argument -n or --number-events)
# BATCH	(although you can pass this in the command-line argument -b or --batch)
# EMAIL (although you can pass this in the command-line argument -e or --email)
# Before running this file, update the following in Fun4All_G4_EICDetector.C:
# Set usegun to true
# Within the if (usegun) block
#	Update set_pid to your desired particle type. See http://home.fnal.gov/~mrenna/lutp0613man2/node44.html for the particle ids
#	Update set_name to your desired particle name
# 	Update set_mom_range to your desired momentum range
#	Update set_eta_range to your desired eta range (angle particle is shot at perpendicular to the beamline)
#	Update set_phi_range to your desired phi range (angle particle is shot at looking at the beamline)

# Usage: run-particle-gun.sh [OPTIONS]...
# -m,--macros-directory			specifies directory where g4simulation Fun4AllMacroes are
# -n,--number-events			specifies number of events to run
# -b,--batch				specifies how many events to run per batch
# -e,--email				specifies the email that condor emails once jobs are done
# -r,--results-directory			specifies which directory to store the results in
# -h,--help				displays this message

# Directory where g4simulation Fun4AllSimulations are
MACROS_DIRECTORY=/direct/sphenix+u/$USER/macros/macros/g4simulations
EMAIL='my_email@my_domain.my_ext' # email that condor emails user when jobs are done

# Name of directory where simulation results are placed
DNAME=particle-gun-simulation-$(date +%F-%T)
NEVENTS=1 # number of events to run
BATCH=1 # Run the events in batches of $BATCH 
# if you have 100 events that you run in batches of 1, 100 condor jobs will be created running each event in parallel

TEMP=`getopt -o m:n:b:e:r:h --long macros-directory:,number-events:,batch:,email:,results-directory:,help \
     -n 'run-particle-gun.sh' -- "$@"`

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
eval set -- "$TEMP"
while true ; do
    case "$1" in
        -m|--macros-directory) MACROS_DIRECTORY=$2; shift 2 ;;
        -n|--number-events) NEVENTS=$2; shift 2 ;;
        -b|--batch) BATCH=$2 ; shift 2 ;;
        -e|--email) EMAIL=$2; shift 2 ;;
        -r|--results-directory) DNAME=$2; shift 2 ;;
	-h|--help) cat << EOF
Usage: run-particle-gun.sh [OPTIONS]...
-m,--macros-directory			specifies directory where g4simulation Fun4AllMacroes are
-n,--number-events			specifies number of events to run
-b,--batch				specifies how many events to run per batch
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

mkdir $DNAME
cd $DNAME
cp -R $MACROS_DIRECTORY/* ./ # Copy over necessary macros

# Create cleanup file for after jobs are done to automatically remove old files
cat > cleanup.sh << EOF
#!/bin/bash
rm *.log *.err *.out *.C vis.mac cleanup.sh
EOF
chmod u+x cleanup.sh

BASE_NAME="simulation" # Job/shell script files are prefixed with this base
for i in $(seq 0 $(($NEVENTS/$BATCH))); do
	CONDOR_EXECUTABLE_NAME=$BASE_NAME-$i.sh
	CONDOR_OUT_NAME=$BASE_NAME-$i.out # output of job
	CONDOR_ERROR_NAME=$BASE_NAME-$i.err # name of file to place stderr of job
	CONDOR_LOG_NAME=$BASE_NAME-$i.log # name of file to place condor log
	CONDOR_JOB_NAME=$BASE_NAME-$i.job # name of job file for condor to run


	JOB=$"
	Executable      = $(pwd)/$CONDOR_EXECUTABLE_NAME	\n
	Output          = $(pwd)/$CONDOR_OUT_NAME		\n
	Error           = $(pwd)/$CONDOR_ERROR_NAME		\n
	Log             = $(pwd)/$CONDOR_LOG_NAME		\n
	Universe        = vanilla				\n
	Priority        = +0					\n
	Input           = /dev/null				\n
	GetEnv          = True					\n
	Initialdir      = $(pwd)				\n
	+Experiment     = \"sphenix\"				\n
	+Job_Type       = \"cas\"				\n
	Notification    = Always				\n
	Notify_user     = $EMAIL				\n
	Queue"

	echo -e $JOB | sed "s/^[ \t]*//" > $CONDOR_JOB_NAME

	ROOT_FILE="$(pwd)/G4EICDetector-$i.root"
	CONDOR_EXECUTABLE="time root -b -q Fun4All_G4_EICDetector.C\($BATCH,\\\"/dev/null\\\",\\\"$ROOT_FILE\\\"\)"
	
	echo -e "#!/bin/tcsh\ncd $PWD;" > $CONDOR_EXECUTABLE_NAME
	echo $CONDOR_EXECUTABLE >> $CONDOR_EXECUTABLE_NAME
	chmod a+x $CONDOR_EXECUTABLE_NAME 
	condor_submit "$CONDOR_JOB_NAME"
done
