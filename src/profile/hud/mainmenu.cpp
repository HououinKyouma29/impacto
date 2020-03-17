#include "mainmenu.h"
#include "../profile_internal.h"
#include "../games/rne/tilebackground.h"
#include "../games/rne/mainmenu.h"
#include "../games/mo6tw/mainmenu.h"
#include "../../log.h"
#include "../../window.h"

namespace Impacto {
namespace Profile {
namespace MainMenu {

using namespace Impacto::MainMenu;

MainMenuType Type = MainMenuType::None;

float MenuEntriesX;
float MenuEntriesXOffset;
float MenuEntriesFirstY;
float MenuEntriesYPadding;

void Configure() {
  if (TryPushMember("MainMenu")) {
    AssertIs(kObjectType);

    Type = MainMenuType::_from_integral_unchecked(EnsureGetMemberInt("Type"));

    if (Type == +MainMenuType::RNE) {
      Implementation = RNE::MainMenu::Configure();
    } else if (Type == +MainMenuType::MO6TW) {
      Implementation = MO6TW::MainMenu::Configure();
    }

    MenuEntriesX = EnsureGetMemberFloat("MenuEntriesX");
    MenuEntriesXOffset = EnsureGetMemberFloat("MenuEntriesXOffset");
    MenuEntriesFirstY = EnsureGetMemberFloat("MenuEntriesFirstY");
    MenuEntriesYPadding = EnsureGetMemberFloat("MenuEntriesYPadding");

    Pop();
  }
}

}  // namespace MainMenu
}  // namespace Profile
}  // namespace Impacto