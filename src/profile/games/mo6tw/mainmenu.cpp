#include "mainmenu.h"
#include "../../profile_internal.h"
#include "../../../window.h"
#include "../../../log.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace MainMenu {

Sprite MainMenuBackgroundSprite;
Sprite MenuEntriesHighlightedSprite;
Sprite MenuEntriesSprites[MenuEntriesNum];

float MenuEntriesTargetWidth;
float MainMenuX;
float MainMenuY;

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

Impacto::MainMenu::MainMenuBase* Configure() {
  Impacto::MO6TW::MainMenu* result = new Impacto::MO6TW::MainMenu();

  MenuEntriesHighlightedSprite =
      EnsureGetMemberSprite("MenuEntriesHighlightedSprite");
  MainMenuBackgroundSprite = EnsureGetMemberSprite("MainMenuBackgroundSprite");
  MenuEntriesTargetWidth = EnsureGetMemberFloat("MenuEntriesTargetWidth");
  MainMenuX = EnsureGetMemberFloat("MainMenuX");
  MainMenuY = EnsureGetMemberFloat("MainMenuY");
  GetMemberSpriteArray(MenuEntriesSprites, MenuEntriesNum,
                       "MenuEntriesSprites");

  return result;
}

}  // namespace MainMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto
