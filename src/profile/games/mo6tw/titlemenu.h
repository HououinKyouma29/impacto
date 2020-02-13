#pragma once

#include "../../../hud/titlemenu.h"
#include "../../../spritesheet.h"
#include "../../../games/mo6tw/titlemenu.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace TitleMenu {

Impacto::TitleMenu::TitleMenuBase* Configure();

extern Sprite BackgroundSprite;
extern Sprite CopyrightSprite;
extern Sprite LogoSprite;

extern float LogoX;
extern float LogoY;
extern float CopyrightX;
extern float CopyrightY;

}  // namespace TitleMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto