#!/bin/bash
FILES=($(find . -name "*.out"))
for f in "${FILES[@]}"; do 
	if [[ $(grep 'All done' $f) != 'All done' ]]; then 
		echo "Waiting for '$f'..." | sed 's/\.out/\.sh/g'; 
	fi
done
