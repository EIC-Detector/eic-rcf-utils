#!/bin/bash

# Before running this file, update the following:
# MACROS_DIRECTORY
# NEVENTS
# BATCH
# Email field in JOB

# Directory where g4 Fun4AllSimulations are
MACROS_DIRECTORY=/direct/sphenix+u/$USER/macros/macros/g4simulations

# Name of directory where simulation results are placed
DNAME=particle-gun-simulation-$(date +%F-%T)
NEVENTS=100 # number of events to run
BATCH=1 # Run the events in batches of $BATCH 
# if you have 100 events that you run in batches of 1, 100 condor jobs will be created running each event in parallel

mkdir $DNAME
cd $DNAME

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
	Notify_user     = my_email@my_domain.my_ext		\n
	Queue"

	echo -e $JOB | sed "s/^[ \t]*//" > $CONDOR_JOB_NAME

	ROOT_FILE="$(pwd)/G4EICDetector-$i.root"
	CONDOR_EXECUTABLE="time root -b -q Fun4All_G4_EICDetector.C\($BATCH,\\\"/dev/null\\\",\\\"$ROOT_FILE\\\"\)"
	
	echo -e "#!/bin/tcsh\ncd $MACROS_DIRECTORY;" > $CONDOR_EXECUTABLE_NAME
	echo $CONDOR_EXECUTABLE >> $CONDOR_EXECUTABLE_NAME
	chmod a+x $CONDOR_EXECUTABLE_NAME

	condor_submit "$CONDOR_JOB_NAME"
done
