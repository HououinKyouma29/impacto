#include "titlemenu.h"
#include "../../../log.h"
#include "../../../window.h"
#include "../../profile_internal.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace TitleMenu {

Sprite BackgroundSprite;
Sprite CopyrightSprite;
Sprite LogoSprite;
Sprite MenuBackgroundSprite;
Sprite MenuEntriesSprites[MenuEntriesNum];
Sprite MenuEntriesHSprites[MenuEntriesNum];

float MenuEntriesX;
float MenuEntriesXOffset;
float MenuEntriesFirstY;
float MenuEntriesYPadding;
float MenuEntriesTargetWidth;
float LogoX;
float LogoY;
float CopyrightX;
float CopyrightY;

static void GetMemberSpriteArray(Sprite* arr, uint32_t count,
                                 char const* name) {
  EnsurePushMemberOfType(name, kArrayType);

  if (TopVal().Size() != count) {
    ImpLog(LL_Fatal, LC_Profile, "Expected to have %d sprites for %s\n", count,
           name);
    Window::Shutdown();
  }

  for (uint32_t i = 0; i < count; i++) {
    arr[i] = EnsureGetArrayElementSprite(i);
  }

  Pop();
}

Impacto::TitleMenu::TitleMenuBase* Configure() {
  Impacto::MO6TW::TitleMenu* result = new Impacto::MO6TW::TitleMenu();

  BackgroundSprite = EnsureGetMemberSprite("BackgroundSprite");
  CopyrightSprite = EnsureGetMemberSprite("CopyrightSprite");
  LogoSprite = EnsureGetMemberSprite("LogoSprite");
  LogoX = EnsureGetMemberFloat("LogoX");
  LogoY = EnsureGetMemberFloat("LogoY");
  CopyrightX = EnsureGetMemberFloat("CopyrightX");
  CopyrightY = EnsureGetMemberFloat("CopyrightY");
  MenuBackgroundSprite = EnsureGetMemberSprite("MenuBackgroundSprite");
  MenuEntriesTargetWidth = EnsureGetMemberFloat("MenuEntriesTargetWidth");
  MenuEntriesX = EnsureGetMemberFloat("MenuEntriesX");
  MenuEntriesXOffset = EnsureGetMemberFloat("MenuEntriesXOffset");
  MenuEntriesFirstY = EnsureGetMemberFloat("MenuEntriesFirstY");
  MenuEntriesYPadding = EnsureGetMemberFloat("MenuEntriesYPadding");

  GetMemberSpriteArray(MenuEntriesSprites, MenuEntriesNum,
                       "MenuEntriesSprites");
  GetMemberSpriteArray(MenuEntriesHSprites, MenuEntriesNum,
                       "MenuEntriesHighlightedSprites");

  return result;
}

}  // namespace TitleMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto