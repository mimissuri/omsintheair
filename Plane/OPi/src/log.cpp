//============================================================================
// Name        : Omsintheair
// Author      : Adam El Messaoudi
// Version     : 0.0.1
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

// ANCHOR La cosa aquella que no se com es diu
using namespace std;


// SECTION Variables
ofstream myfile;

bool debug = true;

const string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;
}


int printx(string prefix, string text) {
	if (debug) {
		cout << "["+prefix+"] " << currentDateTime() << ": "+text+"\n";
	}
	else {
		myfile << "["+prefix+"] " << currentDateTime() << ": "+text+"\n";
	}
	return 0;
}