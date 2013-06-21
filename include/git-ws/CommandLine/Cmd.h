#ifndef GITWS_COMMANDLINE_CMD
#define GITWS_COMMANDLINE_CMD

#include <string>
#include <vector>
#include <functional>

namespace ssvcl
{
	class Flag;
	class ArgBase;
	template<typename T> class Arg;

	class Cmd
	{
		private:
			std::vector<std::string> names;
			std::vector<ArgBase*> args; // owned
			std::vector<Flag*> flags; // owned
			std::function<void()> func;

			Flag& findFlag(const std::string& mName);

		public:
			Cmd(std::initializer_list<std::string> mNames);
			~Cmd();

			template<typename T> Arg<T>& createArg(const std::string& mDescription) { auto result(new Arg<T>(mDescription)); args.push_back(result); return *result; }
			void setArgValue(unsigned int mIndex, const std::string& mValue);
			unsigned int getArgCount();

			Flag& createFlag(const std::string& mShortName, const std::string& mLongName);
			bool isFlagActive(unsigned int mIndex);
			unsigned int getFlagCount();
			void activateFlag(const std::string& mName);

			bool hasName(const std::string& mName);
			void execute();

			Cmd& operator+=(std::function<void()> mFunc);

			const std::vector<std::string>& getNames();
			const std::vector<ArgBase*>& getArgs();
			const std::vector<Flag*>& getFlags();

			std::string getNamesString();
			std::string getArgsString();
			std::string getFlagsString();
	};
}

#endif
