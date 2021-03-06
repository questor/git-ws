// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <cstdio>
#include <iostream>
#include <fstream>
#include "git-ws/GitWs/GitWs.h"

using namespace std;
using namespace ssvu;
using namespace ssvu::FileSystem;
using namespace ssvcl;
using namespace ssvuj;

namespace gitws {
vector<string> GitWs::getAheadRepoPaths() {
    vector<string> result;
    for (const auto & p : repoPaths) {
        bool isAhead {false};
        for (auto & f : runShInPath(p, "git status -sb")) 
        	if (f.find("ahead") != string::npos) {
                isAhead = true;
                break;
            }
        if (isAhead) 
        	result.push_back(p);
    }
    return result;
}

vector<string> GitWs::getChangedRepoPaths() {
    vector<string> result;
    for (const auto & p : repoPaths) 
    	if (!runShInPath(p, "git diff-index --name-only --ignore-submodules HEAD --").empty()) 
    		result.push_back(p);
    return result;
}

vector<string> GitWs::getUpdatedRepoPaths() {
    vector<string> result;
    for (const auto & p : repoPaths) {
        log("checking "+p);
        auto currentRev = runShInPath(p, "git rev-parse @{0}");
        auto remoteRev = runShInPath(p, "git ls-remote origin -h refs/heads/master");

        if(remoteRev[0].find(currentRev[0]) == 0) {
            //log("\033[F"+p+" is up-to-date");
            log("\033[F\033[F");
        } else {
            log("\033[F"+p+" -> has changes on server");
            //result.push_back(p);
        }
    }
    return result;
}

vector<string> GitWs::runShInPath(const string &mPath, const string &mCommand) {
    FILE *pipe {
    	popen(string{"(cd " + mPath + ";" + mCommand + ")"} .c_str(), "r")
    };
    char buffer[1000];
    string file; vector<string> files;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        file = buffer;
        files.push_back(file.substr(0, file.size() - 1));
    }
    pclose(pipe);
    return files;
}

void GitWs::runShInRepos(const vector<string> &mRepoPaths, const string &mCommand, bool mPrintEmpty) {
    for (const auto & p : mRepoPaths) {
        vector<string> files { 
        	runShInPath(p, mCommand) 
        };

        if (files.empty() && !mPrintEmpty) 
        	continue;

        log("<<" + p + ">>", "Repo");
        for (auto & f : files) 
        	log(f, ">");
        log("", "----"); log("");
    }
}

void GitWs::initCmdHelp() {
    auto &cmd(cmdLine.create({"?", "help"}));
    auto &optArg(cmd.createOptArg<string>("", "command name"));
    cmd += [&] {
        if (!optArg) {
            log("Git-ws help");
            log("");

            for (const auto & c : cmdLine.getCmds()) {
                log(c->getNamesString() + " " + c->getArgsString() + " " + c->getOptArgsString() + " " + c->getFlagsString());
            }
        }

        auto & c(cmdLine.findCmd(optArg.get()));
        log(c.getNamesString() + " " + c.getArgsString() + " " + c.getOptArgsString() + " " + c.getFlagsString());
    };
}

void GitWs::initCmdPush() {
    auto &cmd(cmdLine.create({"push"}));
    auto &flagForce(cmd.createFlag("f", "force"));
    auto &flagAhead(cmd.createFlag("a", "ahead-only"));
    cmd += [&] {
        runShInRepos(flagAhead ? getAheadRepoPaths() : repoPaths, flagForce ? "git push -f" : "git push");
    };
}

void GitWs::initCmdPull() {
    auto &cmd(cmdLine.create({"pull"}));
    auto &flagStash(cmd.createFlag("s", "stash"));
    auto &flagForce(cmd.createFlag("f", "force-checkout"));
    auto &flagChanged(cmd.createFlag("c", "changed-only"));
    cmd += [&] {
        auto currentRepoPaths(flagChanged ? getChangedRepoPaths() : repoPaths);
        if (flagStash) 
        	runShInRepos(currentRepoPaths, "git stash");
        if (flagForce) 
        	runShInRepos(currentRepoPaths, "git checkout -f");
        runShInRepos(currentRepoPaths, "git pull");
    };
}

void GitWs::initCmdSubmodule() {
    auto &cmd(cmdLine.create({"sub", "submodule"}));
    auto &arg(cmd.createArg<string>("submodule action"));
    auto &flagChanged(cmd.createFlag("c", "changed-only"));
    cmd += [&] {
        auto currentRepoPaths(flagChanged ? getChangedRepoPaths() : repoPaths);
        if (arg.get() == "push") 
        	runShInRepos(currentRepoPaths, "git commit -am 'automated submodule update'; git push");
        else if (arg.get() == "pull") 
        	runShInRepos(currentRepoPaths, "git submodule foreach git stash; git submodule foreach git pull origin master --recurse-submodules");
        else if (arg.get() == "au") {
            runShInRepos(currentRepoPaths, "git submodule foreach git stash; git submodule foreach git pull origin master --recurse-submodules");
            runShInRepos(currentRepoPaths, "git commit -am 'automated submodule update'; git push");
        }
    };
}

void GitWs::initCmdStatus() {
    auto &cmd(cmdLine.create({"st", "status"}));
    auto &showAllFlag(cmd.createFlag("a", "showall"));
    cmd += [&] { 
    	runShInRepos(repoPaths, "git status -s --ignore-submodules=dirty", showAllFlag); 
    };
}

void GitWs::initCmdGitg() {
    auto &cmd(cmdLine.create({"gitg"}));
    auto &flagChanged(cmd.createFlag("c", "changed-only"));
    cmd += [&] {
        auto currentRepoPaths(flagChanged ? getChangedRepoPaths() : repoPaths);
        runShInRepos(currentRepoPaths, "gitg&");
    };
}

void GitWs::initCmdDo() {
    auto &cmd(cmdLine.create({"do"}));
    auto &arg(cmd.createArg<string>("command to execute"));
    auto &flagChanged(cmd.createFlag("c", "changed-only"));
    auto &flagAhead(cmd.createFlag("a", "ahead-only"));
    cmd += [&] {
        if (flagChanged && flagAhead) {
            log("-c and -a are mutually exclusive");
            return;
        }

        auto currentRepoPaths(flagChanged ? getChangedRepoPaths() : repoPaths);
        if (flagAhead) currentRepoPaths = getAheadRepoPaths();

        runShInRepos(currentRepoPaths, arg.get());
    };
}

void GitWs::initCmdQuery() {
    auto &cmd(cmdLine.create({"q", "query"}));
    auto &flagAll(cmd.createFlag("a", "show-all"));
    cmd += [&] {
        if(flagAll) {
            log("<<ALL REPO PATHS>>", "----");
            for (const auto & p : repoPaths)
                log(p);
            log("", "----"); log(""); log("");
        }
        log("<<CHANGED REPO PATHS (can commit)>>", "----");
        for (const auto & p : getChangedRepoPaths())
            log(p);
        log("", "----"); log(""); log("");
        log("<<AHEAD REPO PATHS (can push)>>", "----");
        for (const auto & p : getAheadRepoPaths())
            log(p);
        log("", "----"); log(""); log("");
    };
}

void GitWs::initCmdSearchUpdates() {
    auto &cmd(cmdLine.create({"su", "search-updates"}));
    cmd += [&] {
        log("<<SEARCHING FOR REMOTE UPDATES>>", "----");
        for (const auto &p : getUpdatedRepoPaths())
            log(p);
        log("", "----"); log(""); log("");
    };
}

void GitWs::initRepoPaths() {
    if(exists(".git-ws-repo-cache.json")) {
        log("using repo cache file", "----");

        Json::Value root(getRootFromFile(".git-ws-repo-cache.json"));
        for(Json::Value path : root["repos"]) 
            repoPaths.push_back(as<std::string>(path));

    } else {
        for (auto & p : getScan<Mode::Single, Type::Folder>("./"))
            if (exists(p + "/.git/"))
                repoPaths.push_back(p);
    }
}

void GitWs::initGenRepoCache() {
    cmdLine.create({"genCache"}) += [&] {
        std::string cachePaths = "{ \"repos\": [";
        bool first = true;
        for (auto & p : getScan<Mode::Recurse, Type::Folder>("./")) {            
            if (exists(p + "/.git/")) {
                if(!first) {
                    cachePaths += ",";
                }
                first = false;
                cachePaths += "\"" + p + "\"";
            }
        }
        cachePaths += "] }";
        fstream f;
        f.open(".git-ws-repo-cache.json", fstream::out | fstream::trunc);
        f << cachePaths;
        f.flush();
        f.close();
    };
}


void GitWs::initCmds() {
    initCmdHelp();
    initCmdPush();
    initCmdPull();
    initCmdSubmodule();
    initCmdStatus();
    initCmdGitg();
    initCmdDo();
    initCmdQuery();
    initCmdSearchUpdates();
    initGenRepoCache();
}

GitWs::GitWs(const vector<string> &mCommandLine) {
    initRepoPaths();
    initCmds();
    cmdLine.parseCmdLine(mCommandLine);
}

}	//namespace
