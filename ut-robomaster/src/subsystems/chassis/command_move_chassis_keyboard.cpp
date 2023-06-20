#include "command_move_chassis_keyboard.hpp"

namespace commands
{
void CommandMoveChassisKeyboard::initialize() {}

void CommandMoveChassisKeyboard::execute() {

    if (drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATING) {
        chassis->input(Vector2f(0.0f), 0.0f);
    }
    
    else {
        Remote* remote = &drivers->remote;

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

        // auto-align to turret when moving
        if (inputMove.getLengthSquared() > 0.0f && inputSpin == 0.0f) {
            float deltaAngle = turretYaw - roundf(turretYaw / SNAP_ANGLE) * SNAP_ANGLE;  // nearest side
            float correction = deltaAngle / SNAP_ANGLE * 4.0f;
            inputSpin = correction / max(1.0f, abs(correction)) * TURRET_ALIGN_FACTOR;
        }
        
        chassis->input(turretRelativeMove, inputSpin);
    }
}

void CommandMoveChassisKeyboard::end(bool) { chassis->input(Vector2f(0.0f), 0.0f); }

bool CommandMoveChassisKeyboard::isFinished() const { return false; }
}  // namespace commands