// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <string>
#include <vector>
#include <stdexcept>
#include <SSVUtils/SSVUtils.h>
#include "git-ws/GitWs/GitWs.h"

int main(int argc, char* argv[])
{
	std::vector<std::string> args;
	for(int i{1}; i < argc; ++i) 
      args.push_back(argv[i]);

   if(argc == 1) {      //no command, place "help"
      args.push_back("help");
   }

	try { 
      gitws::GitWs{args}; 
   } catch(std::runtime_error mException) { 
      ssvu::log(mException.what()); 
      return 1; 
   }
	return 0;
}
