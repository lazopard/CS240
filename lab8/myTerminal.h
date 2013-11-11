/**
 * myTerminal.h
 *
 * Header file for Lab8 CS240 Fall 2013
 */

/**
 * execCommand
 * 
 * Run the given argv with argc. fdWrite is a file descriptor to write error messages.
 * If argv is NULL, or argc is less than 1, it will print some error prompt and exit.
 */

void execCommand(int argc, char** argv, int fdWrite);
