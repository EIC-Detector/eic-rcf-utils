# eic-utils

## run-particle-gun.sh
run-particle-gun.sh is a script that streamlines the process of running simulations on CONDOR. It has the ability to break up events into batches such that they run in parallel on CONDOR, with the number of events total to run being specified by `-n` and the batch size being specified by `-b`. Before running, make sure you have cloned the [macros](https://github.com/sPHENIX-Collaboration/macros) directory from sPHENIX-Collaboration. Within the macros directory you will find `macros/macros/g4simulations`. Pass the path to the `g4simulations` directory on the command line using `-m`, or edit `run-particle-gun.sh` directly and set `MACROS_DIRECTORY` to the absolute path to the `g4simulations` directory. You will also need to remember to update your email within the `run-particle-gun.sh` file or to always pass your email on the command line using `-e`. This is because CONDOR sends users an email once there jobs are done, and as the dummy email left in `run-particle-gun.sh` will always fail, it tends to upset the system administrators. Your simulation results will be saved in your current directory, where you will find another directory in the form `particle-gun-simulation.XXX` (where `XXX` are three random characters), but it is recommended to explicitly specify the directory where to save the results using `-r`. Lastly, you will notice that once the simulation finishes, you will have many ROOT files corresponding to the results of each batch. It is recommended to merge these files into one ROOT tree for easier analysis and usage. Lastly, before you use this script, you **must** read through the comments at the top of `run-particle-gun.sh` in order to make the necessary changes to the `Fun4All_G4_EICDetector.C` file within `g4simulations` in order for the script to function properly.

    Usage: run-particle-gun.sh [OPTIONS]...
    -m,--macros-directory   specifies directory where g4simulation Fun4AllMacroes are
    -n,--number-events			specifies number of events to run
    -b,--batch              specifies how many events to run per batch
    -e,--email              specifies the email that condor emails once jobs are done
    -r,--results-directory  specifies which directory to store the results in
    -h,--help               displays this message
