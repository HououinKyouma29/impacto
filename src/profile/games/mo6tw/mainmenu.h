#pragma once 

#include "../../../hud/mainmenu.h"
#include "../../../spritesheet.h"
#include "../../../games/mo6tw/mainmenu.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace MainMenu {

Impacto::MainMenu::MainMenuBase* Configure();

extern Sprite MainMenuBackgroundSprite;
extern float MenuEntriesTargetWidth;

}  // namespace MainMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto