#pragma once

#include "../../hud/mainmenu.h"
#include "../../spritesheet.h"

namespace Impacto {
namespace Profile {
namespace MainMenu {

extern Impacto::MainMenu::MainMenuType Type;

static int const MenuEntriesNum = 8;
  
extern Sprite MenuEntriesSprites[8];
extern Sprite MenuEntriesHSprites[8];
extern float MenuEntriesX;
extern float MenuEntriesXOffset;
extern float MenuEntriesFirstY;
extern float MenuEntriesYPadding;

void Configure();

}  // namespace MainMenu
}  // namespace Profile
}  // namespace Impacto