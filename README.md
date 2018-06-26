# eic-utils
A collection of shell scripts, programs, and macros to make sPHENIX research easier.
# Setup
    cd
    git clone 'https://github.com/EIC-Detector/eic-utils'
    cd eic-utils
    make
    cd ../
    mkdir eic-utils-bin
    cp eic-utils/*.sh eic-utils/merge-trees eic-utils-bin/
    # Note, the following works only for the C-shell, which is the most common shell on sPHENIX computing account
    # To find out what shell you have, do 'echo $SHELL' (without the quotes)
    # It is also likely that you are using bash
    # In that case, replace the line below with: 'export PATH="$PWD/eic-utils-bin:$PATH' (without the quotes)
    echo "setenv PATH "$PWD/eic-utils-bin:$PATH" >> ~/.cshrc # Add folder to PATH for easy usage of tools
    # For bash, replace ~/.cshrc with ~/.bashrc
    source ~/.cshrc

## Scripts
### run-particle-gun.sh
`run-particle-gun.sh` is a script that streamlines the process of running simulations on CONDOR. It has the ability to break up events into batches such that they run in parallel on CONDOR, with the number of events total to run being specified by `-n` and the batch size being specified by `-b`. Before running, make sure you have cloned the [macros](https://github.com/sPHENIX-Collaboration/macros) directory from sPHENIX-Collaboration. Within the macros directory you will find `macros/macros/g4simulations`. Pass the path to the `g4simulations` directory on the command line using `-m`, or edit `run-particle-gun.sh` directly and set `MACROS_DIRECTORY` to the absolute path to the `g4simulations` directory. You will also need to remember to update your email within the `run-particle-gun.sh` file or to always pass your email on the command line using `-e`. This is because CONDOR sends users an email once there jobs are done, and as the dummy email left in `run-particle-gun.sh` will always fail, it tends to upset the system administrators. Your simulation results will be saved in your current directory, where you will find another directory in the form `particle-gun-simulation.XXX` (where `XXX` are three random characters), but it is recommended to explicitly specify the directory where to save the results using `-r`. Lastly, you will notice that once the simulation finishes, you will have many ROOT files corresponding to the results of each batch. It is recommended to merge these files into one ROOT tree for easier analysis and usage. You will want to look into the `merge-trees` documentation for this step. Lastly, before you use this script, you **must** read through the comments at the top of `run-particle-gun.sh` in order to make the necessary changes to the `Fun4All_G4_EICDetector.C` file within `g4simulations` in order for the script to function properl.

    Usage: run-particle-gun.sh [OPTION]...
    -m,--macros-directory                 specifies directory where g4simulation Fun4AllMacroes are
    -n,--number-events                    specifies number of events to run
    -b,--batch                            specifies how many events to run per batch
    -e,--email                            specifies the email that condor emails once jobs are done
    -r,--results-directory                specifies which directory to store the results in
    -d,--enable-dis                       runs each simulation through Fun4All_EICAnalysis_DIS once done
    -a,--dis-directory                    specifies directory where Fun4All_EICAnalysis_DIS file is located
    -l,--dis-library-path                 specifies install path of compiled EICAnalysis DIS libraries
    -h,--help                             displays this message
    
The script also has support for running the simulations through `Fun4All_EICAnalysis_DIS.C` as soon as they are done.The `-d` option is used to enable this. The path to the directory containing the file must be specified with `-a` if it does not match the default one specified in the script. The `EICAnalysis` module must be [compiled and installed](https://wiki.bnl.gov/sPHENIX/index.php/Example_of_using_DST_nodes) and the path to the `lib` directory in the install path of the module must be specified using `-l` if it does not match the default one specified in the script.

##### Examples
Run a simulation of 10,000 events in batches of 100, storing the results into the directory `my-simulation`

    run-particle-gun.sh -n 10000 -b 100 -r my-simulation

Run a simulation of 100 events in batches of 5, then run them through the `Fun4All_EICAnalysis_DIS.C` macro, storing the results into the directory `my-dis-analysis`. This example also demonstrates how to specify a new directory for the `Fun4All_EICAnalysis_DIS.C` and for the `lib` directory in the install path of the `EICAnalysisModule` if they are not the default ones.
    
     run-particle-gun.sh -n 100 -b 5 -r my-dis-analysis -l ~/tools/eic-analysis/lib -a ~/tools/analysis/EICAnalysis/macros/diskinematics_fun4all/

### merge-trees
`merge-trees` takes a collection of ROOT files containing the same tree produced by `run-particle-gun.sh` (or through any other process; `merge-trees` doesn't care) and merges those trees into one ROOT file. By default, if no trees are explicitly specified,  `merge-trees` will look at the first ROOT file passed and will simply merge all of the trees found in that first file.

    Usage: merge-trees [OPTIONS]... [FILE]...
    Merge trees from specified FILEs into a single ROOT file.
    
    --out, --out=    path where to save merged trees
    --tree, --tree=  which tree to read from files
    --all            merge all trees found in first file passed (default behaviour)
    --help           print this message

##### Examples
Merge the cluster calorimeter data (`ntp_cluster`) from each batch into one file named `my-simulation.root`

    merge-trees --out my-simulation.root --tree ntp_cluster *cemc*
    
Merge all of the SVTX data from each batch into one file named `Merged.root` (the default output file).

    merge-trees *svtx*
    
## Analysis Macros
### Plot-Energy-EMC.C
This macro generates a plot allowing one to see the distribution of what fraction of a particle's true energy is captured in the calorimeters for various particle types and energies. 
