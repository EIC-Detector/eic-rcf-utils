all: bin/merge-trees bin/Plot-SVTX_Efficiency bin/run-particle-gun.sh bin/run-pythia.sh

bin/merge-trees:
	g++ -std=c++11 -o bin/merge-trees condor-tools/merge-trees.cpp `root-config --cflags --libs`
bin/Plot-SVTX_Efficiency:
	g++ -std=c++11 -o bin/Plot-SVTX_Efficiency macros/Plot-SVTX_Efficiency.cpp `root-config --cflags --libs`
bin/run-particle-gun.sh:
	cp condor-tools/run-particle-gun.sh bin/
bin/run-pythia.sh:
	cp condor-tools/run-pythia.sh bin/

add-to-path:
	./add-to-path.sh

clean:
	rm bin/merge-trees bin/Plot-SVTX_Efficiency


