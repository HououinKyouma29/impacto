#pragma once

#include "../text.h"

namespace Impacto {
namespace SysMesBox {

BETTER_ENUM(SysMesBoxType, int, None, RNE, Dash, CHLCC, MO6TW, Darling)

enum SysMesBoxState { Hidden, Hiding, Showing, Shown };

extern SysMesBoxState State;

class SysMesBoxBase {
 public:
  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;

  int MessageCount;
  int ChoiceCount;
  int CurrentChoice = 255;
  bool ChoiceMade = false;
  float BoxOpacity;
  ProcessedTextGlyph Messages[8][255];
  int MessageLengths[8];
  float MessageWidths[8];
  ProcessedTextGlyph Choices[8][255];
  int ChoiceLengths[8];
  float ChoiceWidths[8];
  Animation FadeAnimation;

 protected:
  SysMesBoxState State;
};

extern SysMesBoxBase* Implementation;

void Init();
void Show();
void Hide();
void Update(float dt);
void Render();

}  // namespace SysMesBox
}  // namespace Impacto