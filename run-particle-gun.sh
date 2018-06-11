#!/bin/bash

MACROS_DIRECTORY=/direct/sphenix+u/$USER/macros/macros/g4simulations

DNAME=particle-gun-simulation-$(date +%F-%T)
NEVENTS=1
BATCH=1
mkdir $DNAME
cd $DNAME

BASE_NAME="simulation"
for i in $(seq 0 $(($NEVENTS/$BATCH))); do
	CONDOR_EXECUTABLE_NAME=$BASE_NAME-$i.sh
	CONDOR_OUT_NAME=$BASE_NAME-$i.out
	CONDOR_ERROR_NAME=$BASE_NAME-$i.err
	CONDOR_LOG_NAME=$BASE_NAME-$i.log
	CONDOR_JOB_NAME=$BASE_NAME-$i.job


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
	Notify_user     = giorgian.borca-tasciuc@stonybrook.edu \n
	Queue"

	echo -e $JOB | sed "s/^[ \t]*//" > $CONDOR_JOB_NAME

	ROOT_FILE="$(pwd)/G4EICDetector-$i.root"
	CONDOR_EXECUTABLE="time root -b -q Fun4All_G4_EICDetector.C\($BATCH,\\\"/dev/null\\\",\\\"$ROOT_FILE\\\"\)"
	
	echo -e "#!/bin/tcsh\ncd $MACROS_DIRECTORY;" > $CONDOR_EXECUTABLE_NAME
	echo $CONDOR_EXECUTABLE >> $CONDOR_EXECUTABLE_NAME
	chmod a+x $CONDOR_EXECUTABLE_NAME

	condor_submit "$CONDOR_JOB_NAME"
done
