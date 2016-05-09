#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "AddressMapping.h"
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <map>
#include <list>

#include "SystemConfiguration.h"
#include "MemorySystem.h"
#include "MultiChannelMemorySystem.h"
#include "Transaction.h"
#include "IniReader.h"

using namespace std;
using namespace DRAMSim;

int SHOW_SIM_OUTPUT = 1;

int main(int argc, char** argv) {

	int c;
	TraceType traceType;
	string traceFileName;
	string systemIniFilename("system.ini");
	string deviceIniFilename;
	string pwdString;
	string *visFilename = NULL;
	unsigned megsOfMemory=2048;
	bool useClockCycle=true;
	
	IniReader::OverrideMap *paramOverrides = NULL; 

	unsigned numCycles=1000;
	//getopt stuff
	while (1)
	{
		static struct option long_options[] =
		{
			{"deviceini", required_argument, 0, 'd'},
			{"tracefile", required_argument, 0, 't'},
			{"systemini", required_argument, 0, 's'},

			{"pwd", required_argument, 0, 'p'},
			{"size", required_argument, 0, 'S'},
			{0, 0, 0, 0}
		};
		int option_index=0; //for getopt
		c = getopt_long (argc, argv, "t:s:c:d:o:p:S:v:qn", long_options, &option_index);
		if (c == -1)
		{
			break;
		}
		switch (c)
		{
		case 0: //TODO: figure out what the hell this does, cuz it never seems to get called
			if (long_options[option_index].flag != 0) //do nothing on a flag
			{
				printf("setting flag\n");
				break;
			}
			printf("option %s",long_options[option_index].name);
			if (optarg)
			{
				printf(" with arg %s", optarg);
			}
			printf("\n");
			break;
		case 't':
			traceFileName = string(optarg);
			break;
		case 's':
			systemIniFilename = string(optarg);
			break;
		case 'd':
			deviceIniFilename = string(optarg);
			break;
		case 'S':
			megsOfMemory=atoi(optarg);
			break;
		case 'p':
			pwdString = string(optarg);
			break;
		case '?':
			//usage();
			exit(-1);
			break;
		}
	}

	// no default value for the default model name
	if (deviceIniFilename.length() == 0)
	{
		//ERROR("Please provide a device ini file");
		// usage();
		exit(-1);
	}


	//ignore the pwd argument if the argument is an absolute path
	if (pwdString.length() > 0 && traceFileName[0] != '/')
	{
		traceFileName = pwdString + "/" +traceFileName;
	}



	MultiChannelMemorySystem *memorySystem = new MultiChannelMemorySystem(deviceIniFilename, systemIniFilename, "./", "", megsOfMemory, NULL, NULL);
  unsigned channel, rank, bank, row, col;
  unsigned tmp;
  uint64_t addr;
  ifstream in(traceFileName);
  if ( !in.is_open()) {
    cout << "Error openning file..." << endl;
    exit(1);
  }
  while (!in.eof()) {
    in >> tmp >> addr;
    addressMapping(addr, channel, rank, bank, row, col);
    cout << channel << "\t" << rank << "\t" << bank << "\t" << row << "\t" << col << endl;
  }
  return 0;
}
