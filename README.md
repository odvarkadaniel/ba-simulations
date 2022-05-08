# nba-simluations
CLI that computes different simulation relations over nondeterministic Buchi automata.
Currently we support fair, direct and delayed simulation. We use parity games to compute the fair and delayed simulation relation.

Usage example:
  ./main --file=omega.ba --fair --fast --print

Usage:
  ./main --file=filename.ba { --fair | --direct | --delayed } { --fast } [dotfile.out]
Commands:
  { file / s } 	 Set an input file ( supported formats are .BA, ...)
  { fair | direct | delayed } 	 Set a simulation you want to compute
  [ fast ] 	 Set the fast algorithm for fair or delayed simulation
  [ print ] 	 Print the result relation into the terminal
  [ dot ] 	 Outputs the automaton in a DOT format into file named omega.out
  [ help / h ] 	 Prints help
