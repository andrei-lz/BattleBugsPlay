# **Battle Bugs**

## Development Setup (Windows)

1. Make sure [Ninja](https://github.com/ninja-build/ninja/releases) is installed and available on the PATH.
2. Clone the repository with submodules:
	`git clone --recurse-submodules https://github.com/owenjb42/BattleBugsPlayerTemplate`.
   Or, to get the submodules after cloning:
	`git submodule update --init --recursive`. (this is also required when submodules are added or updated)
3. Open the `BattleBugs` folder with Visual Studio, e.g. in the explorer context menu.


## Development Setup (Linux)

1. Make sure Ninja is installed, e.g. on archlinux `pacman -S ninja`.
2. Clone the repository with submodules:
	`git clone --recurse-submodules https://github.com/owenjb42/BattleBugsPlayerTemplate`.
   Or, to get the submodules after cloning:
	`git submodule update --init --recursive`. (this is also required when submodules are added or updated)
3. `mkdir build && cd build`
4. `cmake .. --preset "default"`


# Description Of How The Gameplay Works
The game is ballanced using fixed values defined in `GameRules.hpp`
## Overview

The game takes place in a continuous (non-discrete) square area with sides of length `GAME_SIZE`. Each player begins with a `Nest` and a starting number of `Bugs`, as defined by `STARTING_BUG_COUNT`, which initially spawn in the `Nest`. The game world is also populated with randomly placed `Plants`, up to a maximum limit defined by `MAX_NUMBER_OF_PLANTS`.

After the game starts, each `Player` is sent a subset of the world’s entities. This subset is determined by the visibility range from their `Nest` and `Bugs`, which is defined by a circular area with a radius of `NEST_VIEW_DISTANCE` for the `Nest` and `BUG_VIEW_DISTANCE` for each `Bug`.

Once the game world is initialized, the `Player` is prompted to take its turn. During a turn, the `Player` can issue instructions for each of its `Bugs`. Each `Bug` can perform one action per turn. The available actions include: `Move`, `Attack Bug`, `Eat Plant`, `Heal`, and `Reproduce`.

After all `Players` have submitted their actions, the game processes these actions and updates the game world. This cycle repeats until all but one `Player` have been eliminated, and the last remaining `Player` is declared the victor.

## Game Mechanics and Rules

Each `Bug` has three attributes: `Location`, `Health`, and `Energy`. `Energy` is the key resource in the game and can be collected by eating `Plants` or by killing and devouring enemy `Bugs`. However, `Energy` is also consumed when performing most actions during a turn. The `Energy` consumption is defined by the following constants: `ENERGY_COST_PER_PERCENTAGE_OF_MAX_DISTANCE_MOVED`, `ENERGY_COST_PER_ATTACK_POINT_DEALT`, `ENERGY_COST_TO_REPRODUCE`, and `ENERGY_COST_PER_HEALTH_POINT_HEALED`. Eating `Plants` does not cost `Energy`. If a `Bug` runs out of `Energy`, it can still survive but is limited to the actions `Eat Plant` and `Move`. When the `Bug` has zero `Enegy` the `Move` action is treated as a 'free' action. 

When performing an `Eat Plant` action, the `Bug` must be within `MAX_PLANT_EATING_DISTANCE` of the `Plant` for the action to be valid. Upon consuming a `Plant`, the `Bug` receives an `Energy` boost equal to `PLANT_ENERGY`. If multiple `Bugs` eat the same `Plant` in one turn the food is shared evenly. `Plants` continuously respawn in the game world at randomized locations, at a rate defined by `PLANT_SPAWN_RATE` per turn, up to the maximum limit of `MAX_NUMBER_OF_PLANTS`. 

When attacking another `Bug` using the `Attack Bug` action, the attacking `Bug` must be within `MAX_BUG_ATTACK_DISTANCE` of the target for the action to be valid. If a `Bug's` `Health` drops below zero during a turn, it dies. When this occurs, its remaining `Energy` is distributed proportionally to all `Bugs` that attacked it during that turn, based on how much attack each contributed. 

The amount of `Energy` a `Bug` has also affects its stats during gameplay. The maximum attack and movement distance a `Bug` can perform in a turn are influenced by its `Energy` level, as follows: 

### $$\text{Max Attack} = 0.5 \cdot \log_{10}(\text{energy}^2 + 2)$$

###  $$\text{Max Speed} = 0.5 + \frac{12 \cdot \text{energy}^{0.1}}{\text{energy}^{0.4} + \text{energy}^{0.1} + 1.6}$$

Similarly, the maximum `Health` of a `Bug` scales with its `Energy`: 

### $$\text{Max Health} = 5 \cdot \log_{10}(\text{energy}^2 + 2)$$

If the maximum `Health` of a `Bug` drops below its current `Health`, the current `Health` is not affected. Maximum `Health` is only relevant during a `Heal` action. The `Bug` provides member functions to query these stats (`GetMaxAttack`, `GetMaxHealth`, and `GetMaxSpeed`). The Heal action has a fixed limit per turn, defined by `MAX_HEAL_PER_TURN`. 

When performing `Heal` and `Attack Bug` actions, a `Bug` that is within its `Nest's` radius (`NEST_EFFECT_RADIUS`) gains a bonus multiplier applied to the usual maximum values. This bonus is defined by `HEAL_BONUS_FOR_BUGS_NEAR_NEST` for healing and `ATTACK_BONUS_FOR_BUGS_NEAR_NEST` for attacking. 


# Starting Your Own Player "AI"




# Game World Functions Available To Player


## Overview
This section provides a description of the functions available for querying and interacting with the game world, including retrieval of game entities such as bugs, plants, and nests, as well as utility functions for checking positions within the game world.

### `GetBugFromID`
```cpp
const Bug& GetBugFromID(const BugID id) const
```
**Description**: Retrieves a `Bug` object corresponding to the given `BugID`.  
**Parameters**:  
- `id` (BugID): The identifier of the bug to retrieve.  
**Returns**: A constant reference to the `Bug` object.

### `GetPlantFromID`
```cpp
const Plant& GetPlantFromID(const PlantID id) const  
```
**Description**: Retrieves a `Plant` object corresponding to the given `PlantID`.  
**Parameters**:  
- `id` (PlantID): The identifier of the plant to retrieve.  
**Returns**: A constant reference to the `Plant` object.

### `GetNestFromID`
```cpp
const Nest& GetNestFromID(const NestID id) const  
```
**Description**: Retrieves a `Nest` object corresponding to the given `NestID`.  
**Parameters**:  
- `id` (NestID): The identifier of the nest to retrieve.  
**Returns**: A constant reference to the `Nest` object.

### `GetBugsWithinRange`
```cpp
std::vector<BugID> GetBugsWithinRange(const XYVector& location, const double distance, const std::function<bool(const Bug&)>& callable_predicate = nullptr) const;  
```
**Description**: Returns a list of `BugID`s for all bugs within a specified range from a given location. An optional predicate function can filter bugs based on custom conditions.  
**Parameters**:  
- `location` (XYVector): The center location to search around.  
- `distance` (double): The radius of the search area.  
- `callable_predicate` (optional, std::function<bool(const Bug&)>): A callable function to filter bugs. If `nullptr`, all bugs in range are returned.  
**Returns**: A vector of `BugID`s within the specified range.

### `GetPlantsWithinRange`
```cpp
std::vector<PlantID> GetPlantsWithinRange(const XYVector& location, const double distance, const std::function<bool(const Plant&)>& callable_predicate = nullptr) const;  
```
**Description**: Returns a list of `PlantID`s for all plants within a specified range from a given location. An optional predicate function can filter plants based on custom conditions.  
**Parameters**:  
- `location` (XYVector): The center location to search around.  
- `distance` (double): The radius of the search area.  
- `callable_predicate` (optional, std::function<bool(const Plant&)>): A callable function to filter plants. If `nullptr`, all plants in range are returned.  
**Returns**: A vector of `PlantID`s within the specified range.

### `GetNestsWithinRange`
```cpp
std::vector<NestID> GetNestsWithinRange(const XYVector& location, const double distance, const std::function<bool(const Nest&)>& callable_predicate = nullptr) const;  
```
**Description**: Returns a list of `NestID`s for all nests within a specified range from a given location. An optional predicate function can filter nests based on custom conditions.  
**Parameters**:  
- `location` (XYVector): The center location to search around.  
- `distance` (double): The radius of the search area.  
- `callable_predicate` (optional, std::function<bool(const Nest&)>): A callable function to filter nests. If `nullptr`, all nests in range are returned.  
**Returns**: A vector of `NestID`s within the specified range.

### `GetBugsByPlayer`
```cpp
std::vector<BugID> GetBugsByPlayer(const PlayerID ID) const;  
```
**Description**: Returns a list of `BugID`s for all bugs owned by a specified player.  
**Parameters**:  
- `ID` (PlayerID): The identifier of the player.  
**Returns**: A vector of `BugID`s owned by the player.

### `GetNestByPlayer`
```cpp
std::vector<NestID> GetNestByPlayer(const PlayerID ID) const;  
```
**Description**: Returns a list of `NestID`s for all nests owned by a specified player.  
**Parameters**:  
- `ID` (PlayerID): The identifier of the player.  
**Returns**: A vector of `NestID`s owned by the player.

### `CountBugsByPlayer`
```cpp
size_t CountBugsByPlayer(const PlayerID ID) const;  
```
**Description**: Returns the number of bugs owned by a specified player.  
**Parameters**:  
- `ID` (PlayerID): The identifier of the player.  
**Returns**: The count of bugs owned by the player.

### `CountNestByPlayer`
```cpp
size_t CountNestByPlayer(const PlayerID ID) const;  
```
**Description**: Returns the number of nests owned by a specified player.  
**Parameters**:  
- `ID` (PlayerID): The identifier of the player.  
**Returns**: The count of nests owned by the player.

### `IsOutsideOfGameBounds`
```cpp
[[nodiscard]] bool IsOutsideOfGameBounds(const XYVector& location) const;  
```
**Description**: Checks if the specified location is outside the game bounds.  
**Parameters**:  
- `location` (XYVector): The location to check.  
**Returns**: `true` if the location is outside the game bounds; `false` otherwise.

### `FindGameBoundIntersection`
```cpp
[[nodiscard]] XYVector FindGameBoundIntersection(const XYVector& location, const XYVector& direction) const;  
```
**Description**: Calculates the point where a given direction vector from a particular location intersects with the game boundary.  
**Parameters**:  
- `location` (XYVector): The starting location.  
- `direction` (XYVector): The direction vector.  
**Returns**: The intersection point with the game bounds.

### `IsBugInNestProximity`
```cpp
[[nodiscard]] bool IsBugInNestProximity(const Bug& bug) const;  
```
**Description**: Checks if a specified bug is in proximity of its nest.  
**Parameters**:  
- `bug` (Bug): The bug to check.  
**Returns**: `true` if the bug is in proximity to a nest; `false` otherwise.

### `IsLocationInNestProximity`
```cpp
[[nodiscard]] bool IsLocationInNestProximity(const XYVector& location, const PlayerID playerID) const;  
```
**Description**: Checks if a specified location is in proximity to any nests owned by a specified player.  
**Parameters**:  
- `location` (XYVector): The location to check.  
- `playerID` (PlayerID): The player whose nests to consider.  
**Returns**: `true` if the location is in proximity to any of the player's nests; `false` otherwise.

### `AllNestWithinRange`
```cpp
std::vector<NestID> AllNestWithinRange(const Bug& bug) const;  
```
**Description**: Returns a list of all nest IDs within a certain range of the specified bug.  
**Parameters**:  
- `bug` (Bug): The bug to use as the center of the search.  
**Returns**: A vector of `NestID`s within the specified range of the bug.

### `ForEachBug`
```cpp
void ForEachBug(std::function<void(const Bug&)> func) const;  
```
**Description**: Calls the given function with each bug in the game world.  
**Parameters**:  
- `func` (std::function<void(const Bug&)>): A function to apply to each bug.

### `ForEachPlant`
```cpp
void ForEachPlant(std::function<void(const Plant&)> func) const;  
```
**Description**: Calls the given function with each plant in the game world.  
**Parameters**:  
- `func` (std::function<void(const Plant&)>): A function to apply to each plant.

### `ForEachNest`
```cpp
void ForEachNest(std::function<void(const Nest&)> func) const;  
```
**Description**: Calls the given function with each nest in the game world.  
**Parameters**:  
- `func` (std::function<void(const Nest&)>): A function to apply to each nest.

### `ForEachOfMyBugs`
```cpp
void ForEachOfMyBugs(std::function<void(const Bug&)> func) const;
```
**Description**: Calls the given function with each bug owned by the player. This function iterates over all bugs in the player's collection (`my_bugs`).  
**Parameters**:  
- `func` (std::function<void(const Bug&)>): A function to apply to each of the player's bugs.

### `ForEachOfOtherBugs`
```cpp
void ForEachOfOtherBugs(std::function<void(const Bug&)> func) const;
```
**Description**: Calls the given function with each visible bug that is not owned by the player. This function iterates over all bugs in the visible_other_bugs collection.  
**Parameters**:  
- `func` (std::function<void(const Bug&)>): A function to apply to each of the player's bugs.





# Functions For Applying Actions To Your Bugs


## Overview
Use the following functions to add actions to your bugs. Each bug can have one action per turn, any further actions added to a bug will overwrite the previous action

### `MoveBugInDirection`
```cpp
bool MoveBugInDirection(
    const Bug& player_bug,
    const XYVector& direction,
    const double amount_of_max_speed = 1.0,
    bool find_closest_legal_alternative = false
) requires std::is_same_v<GameWorldType, ClientGameWorld>;
```
**Description**: Moves the specified bug in a given direction at a percentage of the maximum speed (default 100%). An optional argument allows finding the closest legal alternative if the move goes out of bounds.  
**Parameters**:  
- `player_bug` (Bug): The bug to move.  
- `direction` (XYVector): The direction to move in.  
- `amount_of_max_speed` (double, optional): The maximum speed of the bug (default is 1.0).  
- `find_closest_legal_alternative` (bool, optional): If `true`, find the closest legal move if the intended move is invalid.  
**Returns**: `true` if the move was successful, `false` otherwise.

### `MoveBugToPoint`
```cpp
bool MoveBugToPoint(
    const Bug& player_bug,
    const XYVector& point,
    bool find_closest_legal_alternative = false
) requires std::is_same_v<GameWorldType, ClientGameWorld>;
```
**Description**: Moves the specified bug to a given point. If the point is out of bounds, it can find the closest legal alternative if specified.  
**Parameters**:  
- `player_bug` (Bug): The bug to move.  
- `point` (XYVector): The target point to move to.  
- `find_closest_legal_alternative` (bool, optional): If `true`, find the closest legal move if the target point is invalid.  
**Returns**: `true` if the move was successful, `false` otherwise.

### `AttackOtherBug`
```cpp
bool AttackOtherBug(
    const Bug& player_bug,
    const Bug& enemy_bug,
    const double amount_of_max_attack = 1.0
) requires std::is_same_v<GameWorldType, ClientGameWorld>;
```
**Description**: Commands the specified bug to attack another bug, only valid if the bug is within range.  
**Parameters**:  
- `player_bug` (Bug): The bug initiating the attack.  
- `enemy_bug` (Bug): The target bug to attack.  
- `amount_of_max_attack` (double, optional): The maximum attack strength (default is 1.0).  
**Returns**: `true` if the attack was successful, `false` otherwise.

### `EatPlant`
```cpp
bool EatPlant(const Bug& player_bug, const Plant& plant);
```
**Description**: Commands the specified bug to eat a given plant, only valid if the plant is within range.  
**Parameters**:  
- `player_bug` (Bug): The bug that will eat the plant.  
- `plant` (Plant): The plant to be eaten.  
**Returns**: `true` if the eating action was successful, `false` otherwise.

### `PerformHeal`
```cpp
bool PerformHeal(const Bug& player_bug, double amount_of_max_heal = 1.0)
requires std::is_same_v<GameWorldType, ClientGameWorld>;
```
**Description**: Commands the specified bug to heal itself. The amount of healing can be specified as a percentage of the maximum allowed in one turn (default 100%).  
**Parameters**:  
- `player_bug` (Bug): The bug performing the healing.  
- `amount_of_max_heal` (double, optional): The maximum amount of healing (default is 1.0).  
**Returns**: `true` if the healing action was successful, `false` otherwise.

### `PerformReproduction`
```cpp
bool PerformReproduction(const Bug& player_bug)
requires std::is_same_v<GameWorldType, ClientGameWorld>;
```
**Description**: Commands the specified bug to reproduce. The bug must be in the next radius to reproduce.  
**Parameters**:  
- `player_bug` (Bug): The bug that will reproduce.  
**Returns**: `true` if reproduction was successful, `false` otherwise.


