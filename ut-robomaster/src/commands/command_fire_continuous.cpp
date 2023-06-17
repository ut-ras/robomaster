#include "command_fire_continuous.hpp"

namespace commands
{
using subsystems::shooter::FiringState;

void CommandFireContinuous::initialize() { shooter->setFiringState(FiringState::Firing); }

void CommandFireContinuous::execute() {}

void CommandFireContinuous::end(bool) {}

bool CommandFireContinuous::isFinished() const { return false; }
}  // namespace commands