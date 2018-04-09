#include "CState.h"

CState::CState(std::string name) 
	: _epsilons()
	, _transitions()
	, _name(name)
	, _is_final_state(false) {}

void CState::addTransition(const char character, std::shared_ptr<CState> state) {
	if (!state) {
		throw std::invalid_argument("Empty epsilon transition state");
	}

	if (_transitions.count(character)) {
		throw std::invalid_argument("State already contains transition for the character");
	}

	_transitions[character] = state;
}

void CState::addEpsilonTransition(std::shared_ptr<CState> state) {
	if (!state) {
		throw std::invalid_argument("Empty epsilon transition state");
	}

	_is_final_state = false;
	_epsilons.push_back(state);
}

std::string CState::toString(std::unordered_set<std::string> &visited) {
	if (visited.count(_name)) {
		return _name;
	}

	visited.insert(_name);

	std::string result{ "((<" + _name + ">: { " };

	for (const auto &trans : _transitions) {
		result += "'" + std::string(1, trans.first) + "': " + trans.second->toString(visited) + ", ";
	}

	result += " }, [ ";

	for (const auto &eps : _epsilons) {
		result += eps->toString(visited) + ", ";
	}

	result += " ], " + std::to_string(_is_final_state) + "))";
	return result;
}
