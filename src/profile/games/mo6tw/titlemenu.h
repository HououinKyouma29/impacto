#pragma once

#include "../../../hud/titlemenu.h"
#include "../../../spritesheet.h"
#include "../../../games/mo6tw/titlemenu.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace TitleMenu {

Impacto::TitleMenu::TitleMenuBase* Configure();

static int const MenuEntriesNum = 7;

extern Sprite BackgroundSprite;
extern Sprite CopyrightSprite;
extern Sprite LogoSprite;
extern Sprite MenuBackgroundSprite;
extern Sprite MenuEntriesSprites[MenuEntriesNum];
extern Sprite MenuEntriesHSprites[MenuEntriesNum];

extern float MenuEntriesX;
extern float MenuEntriesXOffset;
extern float MenuEntriesFirstY;
extern float MenuEntriesYPadding;
extern float MenuEntriesTargetWidth;
extern float LogoX;
extern float LogoY;
extern float CopyrightX;
extern float CopyrightY;

}  // namespace TitleMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto