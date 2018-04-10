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
	 * @param id State ID. For debug purposes
	 */
	CState(size_t id);

	/**
	 * @brief deinit Resolve cyclic dependencies before destroy. NFA is a cyclic graph. We
	 *        have 2 alternative ways to break cyclic dependencies of shared pointers:
	 *          - Have weak pointers for some transitions. In this case we'd need to keep 2 more collections inside;
	 *          - Use raw pointers, which would be less error-prone.
	 *        It seems both cases are worse than calling a single function before NFA destroys its graph.
	 *        The idea it to reset shared pointers if referenced state has id less than current.
	 */
	void deinit();

	/**
	 * @brief isFinalState Check if the is a final state
	 *
	 * @return If final state
	 */
	bool isFinalState() const {
		return _is_final_state;
	}

	/**
	* @brief setIsFinalState Set if the is a final state
	*
	* @param is_final If final state
	*/
	void setIsFinalState(bool is_final) {
		_is_final_state = is_final;
	}

	/**
	 * @brief addTransition Add transition from state for the character
	 *
	 * @param character Character for transition
	 * @param state State to transit into
	 */
	void addTransition(const char character, std::shared_ptr<CState> state);

	/**
	 * @brief transitions Get state transitions
	 */
	const TransitionsType transitions() const {
		return _transitions;
	}

	/**
	 * @brief addEpsilonTransition Add epsilon transition
	 *
	 * @param state State to transit into
	 */
	void addEpsilonTransition(std::shared_ptr<CState> state);

	/**
	* @brief transitions Get state epsilon transitions
	*/
	const EpsilonTransitionsType epsilonTransitions() const {
		return _epsilons;
	}

	/**
	* @brief toString Stringifies the state for debug purposes
	*
	* @param visited A set of already printed states. If the state is in the set it just returns own name,
	*        otherwise prints all transitions, epsilons and final state flag
	*
	* @return Stringified state
	*/
	std::string toString(std::unordered_set<std::size_t> &visited);

private:
	EpsilonTransitionsType _epsilons;       ///< State Epsilon transitions
	TransitionsType        _transitions;    ///< State transitions
	size_t                 _id;             ///< State ID
	bool                   _is_final_state; ///< If state is a final state
};

