// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef GITWS_COMMANDLINE_CMDLINE
#define GITWS_COMMANDLINE_CMDLINE

#include <string>
#include <vector>

namespace ssvcl
{
	class Cmd;

	class CmdLine
	{
		private:
			std::vector<Cmd*> cmds;

		public:
			Cmd& findCmd(const std::string& mName) const;
			Cmd& create(const std::initializer_list<std::string>& mNames);
			void parseCmdLine(const std::vector<std::string>& mArgs);
			const std::vector<Cmd*>& getCmds() const;
	};
}

#endif
