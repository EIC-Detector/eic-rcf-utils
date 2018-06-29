#!/bin/bash
case $SHELL in
	*csh*)
		echo "setenv PATH \"$PWD/bin:\$PATH\"" >> ~/.cshrc 
		;;
	*bash*)
		echo "export PATH=\"$PWD/bin:\$PATH\"" >> ~/.bashrc
		;;
	*zsh*)
		echo "export PATH=\"$PWD/bin:\$PATH\"" >> ~/.zshrc
		;;
	*ksh*)
		echo "export PATH=\"$PWD/bin:\$PATH\"" >> ~/.kshrc
		;;
	*)
		echo "Sorry, I don't know how to add the path to your shell type."
		echo "Normally it involves setting the \$PATH variable in your shell's rc file."
		;;
esac
