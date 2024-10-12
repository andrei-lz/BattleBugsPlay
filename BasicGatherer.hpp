#pragma once
#include "Library/GameCore/ClientGameWorld.hpp"

class BasicGatherer
{
public:
	BasicGatherer() : search_direction(XYVector(RandomNumber::Get(-1.0, 1.0), RandomNumber::Get(-1.0, 1.0))) {}


	void PlayTurn(const Bug& bug, ClientGameWorld& game, GameActions& actions)
	{
		auto nearby_plants = game.GetPlantsWithinRange(bug.location, BUG_VIEW_DISTANCE);
		if (nearby_plants.size() != 0)
		{
			const Plant* nearest_plant{ &game.GetPlantFromID(nearby_plants[0]) };

			auto best_vec_bug_to_plant = nearest_plant->location - bug.location;
			for (auto& plantID : nearby_plants)
			{
				auto& plant = game.GetPlantFromID(plantID);
				auto vec_bug_to_plant = plant.location - bug.location;
				if (vec_bug_to_plant.Magnitude() < best_vec_bug_to_plant.Magnitude())
				{
					nearest_plant = &plant;
					best_vec_bug_to_plant = vec_bug_to_plant;
				}
			}

			auto vec_bug_to_plant = nearest_plant->location - bug.location;

			if (vec_bug_to_plant.Magnitude() < MAX_PLANT_EATING_DISTANCE)
				actions.EatPlant(bug, *nearest_plant);
			else
				actions.MoveBugToPoint(bug, nearest_plant->location);
		}
		else
		{
			// Move in search direction if this is not valid we probably hit the game world edge //
			//             Attempt to turn away from game world edge and continue			     //
			if (!actions.MoveBugInDirection(bug, search_direction))
			{
				search_direction.ReverseX();
				if (!actions.MoveBugInDirection(bug, search_direction))
				{
					search_direction.ReverseX();
					search_direction.ReverseY();
					actions.MoveBugInDirection(bug, search_direction);
				}
			}
		}
	}

private:
	XYVector search_direction;
};