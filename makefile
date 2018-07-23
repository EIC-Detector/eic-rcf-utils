all: bin/merge-trees bin/Plot-SVTX_Efficiency bin/Plot-FastTrack_Efficiency bin/run-particle-gun.sh bin/run-pythia.sh bin/Plot-Sigma bin/Generate-Sigma-CSV

bin/merge-trees:
	g++ -O2 -std=c++11 -o bin/merge-trees condor-tools/merge-trees.cpp `root-config --cflags --libs`
bin/Plot-SVTX_Efficiency:
	g++ -O2 -std=c++11 -o bin/Plot-SVTX_Efficiency macros/Plot-SVTX_Efficiency.cpp `root-config --cflags --libs`
bin/Plot-FastTrack_Efficiency:
	g++ -O2 -std=c++11 -o bin/Plot-FastTrack_Efficiency macros/Plot-FastTrack_Efficiency.cpp `root-config --cflags --libs`

bin/Plot-Sigma:
	g++ -O2 -std=c++11 -o bin/Plot-Sigma macros/Plot-Sigma.cpp `root-config --cflags --libs`

bin/Generate-Sigma-CSV:
	g++ -O2 -std=c++11 -o bin/Generate-Sigma-CSV macros/Generate-Sigma-CSV.cpp `root-config --cflags --libs`

bin/run-particle-gun.sh:
	cp condor-tools/run-particle-gun.sh bin/
bin/run-pythia.sh:
	cp condor-tools/run-pythia.sh bin/

add-to-path: bin/merge-trees bin/Plot-SVTX_Efficiency bin/Plot-FastTrack_Efficiency bin/run-particle-gun.sh bin/run-pythia.sh
	./add-to-path.sh

clean:
	rm -f bin/merge-trees bin/Plot-SVTX_Efficiency bin/Plot-FastTrack_Efficiency bin/Plot-Sigma bin/Generate-Sigma-CSV

