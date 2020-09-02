//============================================================================
// Name        : Omsintheair
// Author      : Adam El Messaoudi
// Version     : In development
// Copyright   : -
// Description : Hello World in C++, Ansi-style
// ANCHOR - Used to indicate a section in your file
// TODO - An item that is awaiting completion
// FIXME - An item that requires a bugfix
// STUB - Used for generated default snippets
// NOTE - An important note for a specific code section
// REVIEW - An item that needs additional review
// SECTION - Used to define a region (See 'Hierarchical anchors')
//============================================================================

// SECTION Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include "log.cpp"
// ANCHOR La cosa aquella que no se com es diu
using namespace std;

// SECTION Variables

void exiting()
{
	myfile.close();
	printx("Info", "Exiting");
}

int main()
{
	if (debug)
	{
		myfile.open("logs/" + currentDateTime() + "-log.txt");
	}
	atexit(exiting);
	printx("Info", "Started");
	while (true)
	{
		usleep(1000);
	}
	return 0;
}
