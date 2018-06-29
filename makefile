all: merge-trees Plot-SVTX_Efficiency

merge-trees:
	g++ -std=c++11 -o merge-trees condor-tools/merge-trees.cpp `root-config --cflags --libs`
Plot-SVTX_Efficiency:
	g++ -std=c++11 -o Plot-SVTX_Efficiency macros/Plot-SVTX_Efficiency.cpp `root-config --cflags --libs`


