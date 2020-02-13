#include "mainmenu.h"
#include "../../profile_internal.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace MainMenu {

Sprite MainMenuBackgroundSprite;
float MenuEntriesTargetWidth;

Impacto::MainMenu::MainMenuBase* Configure() {
  Impacto::MO6TW::MainMenu* result = new Impacto::MO6TW::MainMenu();

  MainMenuBackgroundSprite = EnsureGetMemberSprite("MainMenuBackgroundSprite");
  MenuEntriesTargetWidth = EnsureGetMemberFloat("MenuEntriesTargetWidth");

  return result;
}

}  // namespace MainMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto

