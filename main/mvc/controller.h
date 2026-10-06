#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

#include "uCModules.h"
#include "customPanelComponents.h"
// #include "flagMessaging.h"
#include "model.h"
#include "view.h"

class Controller : public Panel
{
public:
    Controller(void);
    void tick(int16_t deltaMs);
    void reloadSettings(void);

    bool initialized;

private:
    Led led_console;

    Button button_console;
};

#endif
