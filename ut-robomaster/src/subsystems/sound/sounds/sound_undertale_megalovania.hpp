#include "../sound.hpp"

const Sound SOUND_UNDERTALE_MEGALOVANIA{
    2019,
    31,
    (uint16_t[]){587,  587,  587,  0, 587, 587, 587, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 523,  523,  523,  0, 523, 523, 523, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 494,  494,  494,  0, 494, 494, 494, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 466,  466,  466,  0, 466, 466, 466, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 587,  587,  587,  0, 587, 587, 587, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 523,  523,  523,  0, 523, 523, 523, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 494,  494,  494,  0, 494, 494, 494, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 466,  466,  466,  0, 466, 466, 466, 0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 698,  698,  698,  0, 0,   0,   0,   0, 698,  698,  698,  0, 698,  698,  698,  0,
                 0,    0,    0,    0, 698, 698, 698, 0, 0,    0,    0,    0, 698,  698,  698,  0,
                 0,    0,    0,    0, 587, 587, 587, 0, 0,    0,    0,    0, 587,  587,  587,  0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 698,  698,  698,  0, 0,   0,   0,   0, 698,  698,  698,  0, 698,  698,  698,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 698,  698,  698,  0, 587,  587,  587,  0,
                 698,  698,  698,  0, 784, 784, 784, 0, 0,    0,    0,    0, 0,    0,    0,    0,
                 698,  698,  698,  0, 0,   0,   0,   0, 698,  698,  698,  0, 698,  698,  698,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 831,  831,  831,  0,
                 0,    0,    0,    0, 880, 880, 880, 0, 0,    0,    0,    0, 1047, 1047, 1047, 0,
                 0,    0,    0,    0, 880, 880, 880, 0, 0,    0,    0,    0, 0,    0,    0,    0,
                 1175, 1175, 1175, 0, 0,   0,   0,   0, 1175, 1175, 1175, 0, 0,    0,    0,    0,
                 1175, 1175, 1175, 0, 880, 880, 880, 0, 1175, 1175, 1175, 0, 1047, 1047, 1047, 0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 880,  880,  880,  0, 880,  880,  880,  0,
                 0,    0,    0,    0, 880, 880, 880, 0, 0,    0,    0,    0, 880,  880,  880,  0,
                 0,    0,    0,    0, 784, 784, 784, 0, 0,    0,    0,    0, 784,  784,  784,  0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 880,  880,  880,  0, 880,  880,  880,  0,
                 0,    0,    0,    0, 880, 880, 880, 0, 0,    0,    0,    0, 784,  784,  784,  0,
                 0,    0,    0,    0, 880, 880, 880, 0, 0,    0,    0,    0, 1175, 1175, 1175, 0,
                 0,    0,    0,    0, 880, 880, 880, 0, 784,  784,  784,  0, 0,    0,    0,    0,
                 1175, 1175, 1175, 0, 587, 587, 587, 0, 880,  880,  880,  0, 440,  440,  440,  0,
                 784,  784,  784,  0, 392, 392, 392, 0, 698,  698,  698,  0, 349,  349,  349,  0,
                 1047, 1047, 1047, 0, 523, 523, 523, 0, 698,  698,  698,  0, 349,  349,  349,  0,
                 659,  659,  659,  0, 330, 330, 330, 0, 587,  587,  587,  0, 294,  294,  294,  0,
                 466,  466,  466,  0, 233, 233, 233, 0, 523,  523,  523,  0, 554,  554,  554,  0,
                 587,  587,  587,  0, 698, 698, 698, 0, 349,  349,  349,  0, 1047, 1047, 1047, 0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 698,  698,  698,  0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 831,  831,  831,  0, 784, 784, 784, 0, 698,  698,  698,  0, 587,  587,  587,  0,
                 784,  784,  784,  0, 587, 587, 587, 0, 698,  698,  698,  0, 0,    0,    0,    0,
                 784,  784,  784,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 831, 831, 831, 0, 0,    0,    0,    0, 880,  880,  880,  0,
                 1047, 1047, 1047, 0, 0,   0,   0,   0, 880,  880,  880,  0, 831,  831,  831,  0,
                 784,  784,  784,  0, 698, 698, 698, 0, 587,  587,  587,  0, 659,  659,  659,  0,
                 698,  698,  698,  0, 0,   0,   0,   0, 784,  784,  784,  0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 1047, 1047, 1047, 0, 0,    0,    0,    0,
                 1109, 1109, 1109, 0, 0,   0,   0,   0, 831,  831,  831,  0, 0,    0,    0,    0,
                 831,  831,  831,  0, 784, 784, 784, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 349,  349,  349,  0, 0,   0,   0,   0, 392,  392,  392,  0, 0,    0,    0,    0,
                 440,  440,  440,  0, 0,   0,   0,   0, 698,  698,  698,  0, 0,    0,    0,    0,
                 659,  659,  659,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 587,  587,  587,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 659,  659,  659,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 698,  698,  698,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 784,  784,  784,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 659,  659,  659,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 880,  880,  880,  0, 831, 831, 831, 0, 784,  784,  784,  0, 740,  740,  740,  0,
                 698,  698,  698,  0, 659, 659, 659, 0, 622,  622,  622,  0, 587,  587,  587,  0,
                 554,  554,  554,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 622,  622,  622,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 698,  698,  698,  0, 587, 587, 587, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 831,  831,  831,  0, 784, 784, 784, 0, 698,  698,  698,  0, 587,  587,  587,  0,
                 784,  784,  784,  0, 587, 587, 587, 0, 698,  698,  698,  0, 0,    0,    0,    0,
                 784,  784,  784,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 831, 831, 831, 0, 0,    0,    0,    0, 880,  880,  880,  0,
                 1047, 1047, 1047, 0, 0,   0,   0,   0, 880,  880,  880,  0, 831,  831,  831,  0,
                 784,  784,  784,  0, 698, 698, 698, 0, 587,  587,  587,  0, 659,  659,  659,  0,
                 698,  698,  698,  0, 0,   0,   0,   0, 784,  784,  784,  0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 1047, 1047, 1047, 0, 0,    0,    0,    0,
                 1109, 1109, 1109, 0, 0,   0,   0,   0, 831,  831,  831,  0, 0,    0,    0,    0,
                 831,  831,  831,  0, 784, 784, 784, 0, 698,  698,  698,  0, 784,  784,  784,  0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 349,  349,  349,  0, 0,   0,   0,   0, 392,  392,  392,  0, 0,    0,    0,    0,
                 440,  440,  440,  0, 0,   0,   0,   0, 698,  698,  698,  0, 0,    0,    0,    0,
                 659,  659,  659,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 587,  587,  587,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 659,  659,  659,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 698,  698,  698,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 784,  784,  784,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 659,  659,  659,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 880,  880,  880,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 880,  880,  880,  0, 831, 831, 831, 0, 784,  784,  784,  0, 740,  740,  740,  0,
                 698,  698,  698,  0, 659, 659, 659, 0, 622,  622,  622,  0, 587,  587,  587,  0,
                 554,  554,  554,  0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 0,    0,    0,    0, 0,   0,   0,   0, 0,    0,    0,    0, 0,    0,    0,    0,
                 622,  622,  622}};