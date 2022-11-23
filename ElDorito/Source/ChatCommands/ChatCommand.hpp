#include <vector>
#include <deque>
#include <cstdint>

namespace ChatCommands
{
	int findNumberOfPlayersInGame();

	// Generic command class with handler functions for voting events. 
	class AbstractChatCommand
	{
	public:

		virtual void doOnVotePass(std::string name) = 0;
		virtual void doOnVoteStart(std::string starterName) = 0;
		virtual bool isValidArgument(std::string s, std::string& returnInfo) = 0;
		virtual void doOnVoteFail() = 0;
		virtual bool isEnabled() = 0;
		void processMessage(uint8_t sender, std::string message);
		void StartVote();
		void enable();
		void disable();
		void tick();
		
		void endVoting();
		bool isCurrentlyVoting();
		std::string getName();
		std::string getDescription();
		AbstractChatCommand(std::string name, std::string description);
		void processVote(uint8_t sender, std::string message);

	protected:
		int starterIndex = -1;
		size_t votesNeeded = 0;

	private:
		std::string name;
		std::string description;
		bool currentlyVoting = false;
		time_t voteTimeStarted = 0;
		std::vector<uint64_t> yesVoters = std::vector<uint64_t>{};

	};

	// Chat command to vote end game
	class EndGameCommand : public AbstractChatCommand
	{
	public:
		virtual void doOnVoteStart(std::string starterName);
		virtual void doOnVotePass(std::string name);
		virtual void doOnVoteFail();
		virtual bool isValidArgument(std::string s, std::string& returnInfo);
		virtual bool isEnabled();
		EndGameCommand();

	};

	// Chat command to vote skip current round. 
	class SkipRoundCommand : public AbstractChatCommand
	{
	public:
		virtual void doOnVoteStart(std::string starterName);
		virtual void doOnVotePass(std::string name);
		virtual void doOnVoteFail();
		virtual bool isValidArgument(std::string s, std::string& returnInfo);
		virtual bool isEnabled();
		SkipRoundCommand();

	};

	// Chat command to vote shuffle teams
	class ShuffleTeamsCommand : public AbstractChatCommand
	{
	public:
		virtual void doOnVoteStart(std::string starterName);
		virtual void doOnVotePass(std::string name);
		virtual void doOnVoteFail();
		virtual bool isValidArgument(std::string s, std::string& returnInfo);
		virtual bool isEnabled();
		ShuffleTeamsCommand();

	};

	// Chat command to vote kick player 
	class KickPlayerCommand : public AbstractChatCommand
	{
	public:
		virtual void doOnVoteStart(std::string starterName);
		virtual void doOnVotePass(std::string name);
		virtual void doOnVoteFail();
		virtual bool isEnabled();
		virtual bool isValidArgument(std::string s, std::string& returnInfo);
		KickPlayerCommand();

	private:
		std::string playerName;
	};

	// Chat command to vote kick player index
	class KickIndexCommand : public AbstractChatCommand
	{
	public:
		virtual void doOnVoteStart(std::string starterName);
		virtual void doOnVotePass(std::string name);
		virtual void doOnVoteFail();
		virtual bool isEnabled();
		virtual bool isValidArgument(std::string s, std::string& returnInfo);
		KickIndexCommand();

	private:
		std::string playerName;
	};
}