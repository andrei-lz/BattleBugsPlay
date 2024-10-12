#include <cstdio>
#include <unordered_map>

#include "Library/GameCore/ClientGameWorld.hpp"
#include "BasicGatherer.hpp"

namespace Player
{
	std::unordered_map<BugID, BasicGatherer> m_Gatherers;

	void start(PlayerID id)
	{

	}

	void play_turn(ClientGameWorld& game)
	{
		auto& actions = game.player_game_actions;
		for (auto& bugID : game.my_bugs)
		{
			if (!m_Gatherers.contains(bugID))
				m_Gatherers.emplace(bugID, BasicGatherer());
			m_Gatherers[bugID].PlayTurn(game.GetBugFromID(bugID), game, actions);
		}
	}
}