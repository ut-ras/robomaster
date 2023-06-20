#include "command_beyblade_chassis.hpp"

namespace commands
{
void CommandBeybladeChassis::initialize() {}

void CommandBeybladeChassis::execute() {

    if (drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATING) {
        chassis->input(Vector2f(0.0f), 0.0f);
    }
    
    else {
        Remote* remote = &drivers->remote;
        inputSpin = 1.0f;

        Vector2f rawMoveInput = Vector2f(
            remote->keyPressed(Remote::Key::D) - remote->keyPressed(Remote::Key::A),
            remote->keyPressed(Remote::Key::W) - remote->keyPressed(Remote::Key::S));

        float rawInputLen = rawMoveInput.getLength();

        if (rawInputLen > 0.0f) {
            Vector2f moveDir = rawMoveInput / rawInputLen;       // normalize input
            inputMove += moveDir * KEYBOARD_ACCEL * DELTA_TIME;  // incorporate input
            inputMove /= max(1.0f, inputMove.getLength());       // clamp length
        }

        else {
            // decelerate when input stops
            float len = inputMove.getLength();
            if (len > 0.0f) {
                inputMove *= max(1.0f - KEYBOARD_DECEL * DELTA_TIME / len, 0.0f);
            }
        }

        float turretYaw = turret->getTargetLocalYaw();
        Vector2f turretRelativeMove = Vector2f(inputMove);
        turretRelativeMove.rotate(turretYaw);
        
        chassis->input(turretRelativeMove, inputSpin);
    }
}

void CommandBeybladeChassis::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandBeybladeChassis::isFinished() const { return false; }
}  // namespace commands