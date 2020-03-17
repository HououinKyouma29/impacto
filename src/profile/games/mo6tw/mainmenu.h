#pragma once

#include "../../../hud/mainmenu.h"
#include "../../../spritesheet.h"
#include "../../../games/mo6tw/mainmenu.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace MainMenu {

Impacto::MainMenu::MainMenuBase* Configure();

static int const MenuEntriesNum = 7;

extern Sprite MainMenuBackgroundSprite;
extern Sprite MenuEntriesSprites[MenuEntriesNum];
extern Sprite MenuEntriesHighlightedSprite;

extern float MenuEntriesTargetWidth;
extern float MainMenuX;
extern float MainMenuY;

}  // namespace MainMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto