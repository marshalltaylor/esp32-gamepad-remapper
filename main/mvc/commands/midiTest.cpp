#include "commands.h"
#include "globals.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

#include "midi_util.h"

int midiProgram(int argc, char *argv[]);

commandInfo_t midiCmd
    = { "midi",
        "Unknown midi tests",
        " usage: midi [g [0|1]|clock|outs|ins]\n  g - enb debug msgs\n  clock "
        "- midiClock\n  outs - outputMixer\n  ins - inputNoteList",
        &midiProgram };

int
midiProgram (int argc, char *argv[])
{
    bool all      = false;
    bool argError = false;
    if (argc == 0)
    {
        all = true;
    }

    if (all || 0 == strcmp((const char *)argv[0], "clock"))
    {
        ((MidiClockDebug *)&midiClock)->printInfo();
    }
    if (all || 0 == strcmp((const char *)argv[0], "mem"))
    {
        quantizer.inputNoteList.printList();
    }
    if (all || 0 == strcmp((const char *)argv[0], "deck"))
    {
        mainDeck.printInfo();
    }
    if (!all && 0 == strcmp((const char *)argv[0], "g"))
    {
        int selection = -1;
        if (argc >= 2)
        {
            selection = strtol(argv[1], NULL, 10);
            localPrintf("Midi graph trace %d\n", selection);
        }
        switch (selection)
        {
            default:
            case -1: {
                localPrintf("All debug disabled.\n");
                masterClock.setDebug(false);
                inputMux.setDebug(false);
                quantizer.setDebug(false);
                mainDeck.setDebug(false);
                // arp.setDebug(false);
                outputMux.setDebug(false);
                outMain.setDebug(false);
                outAux.setDebug(false);
                localPrintf("Options:\n");
                localPrintf(" 0: all\n");
                localPrintf(" 1: deck to out\n");
                localPrintf(" 2: out only\n");
                localPrintf(" 3: input to deck\n");
                break;
            }
            case 0: {
                masterClock.setDebug(true);
                inputMux.setDebug(true);
                quantizer.setDebug(true);
                mainDeck.setDebug(true);
                // arp.setDebug(true);
                outputMux.setDebug(true);
                outMain.setDebug(true);
                outAux.setDebug(true);
                break;
            }
            case 1: {
                mainDeck.setDebug(true);
                outputMux.setDebug(true);
                outMain.setDebug(true);
                outAux.setDebug(true);
                break;
            }
            case 2: {
                outMain.setDebug(true);
                outAux.setDebug(true);
                break;
            }
            case 3: {
                inputMux.setDebug(true);
                quantizer.setDebug(true);
                mainDeck.setDebug(true);
                break;
            }
        }
    }

    if (argError)
    {
        localPrintf("Bad arguement\n");
    }
    return 0;
}
