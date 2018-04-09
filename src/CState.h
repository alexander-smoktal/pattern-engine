#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <memory>

/**
 * @brief CState A single state of automata
 */
class CState
{
public:
	/**
	 * @brief EpsilonTransitionsType Type of epsilon transitions vector
	 */
	using EpsilonTransitionsType = std::vector<std::shared_ptr<CState>>;

	/**
	* @brief TransitionsType Type of transitions map
	*/
	using TransitionsType = std::unordered_map<char, std::shared_ptr<CState>>;

	/**
	 * @brief CState Constructor
	 *
	 * @param name State name. For debug purposes
	 */
	CState(std::string name);

	bool isFinalState() const {
		return _is_final_state;
	}

	void setIsFinalState(bool is_final) {
		_is_final_state = is_final;
	}

	/**
	 * @brief addTransition Add transition from state for the character
	 *
	 * @param character Character for transition
	 * @param state State to transit to
	 */
	void addTransition(const char character, std::shared_ptr<CState> state);

	const TransitionsType transitions() const {
		return _transitions;
	}

	/**
	 * @brief addEpsilonTransition Add epsilon transition
	 *
	 * @param state State to transit to
	 */
	void addEpsilonTransition(std::shared_ptr<CState> state);

	const EpsilonTransitionsType epsilonTransitions() const {
		return _epsilons;
	}

	/**
	* @brief toString Stringifies the state for debug purposes
	*
	* @param visited A set of already printed states. If the state is in the set it just returns own name,
	*        otherwise it prints all transitions, epsilons and if end state
	*
	* @return Stringified state
	*/
	std::string toString(std::unordered_set<std::string> &visited);

private:
	EpsilonTransitionsType _epsilons;       ///< State Epsilon transitions
	TransitionsType        _transitions;    ///< State transitions
	std::string            _name;           ///< State name
	bool                   _is_final_state; ///< If state is a final state
};

