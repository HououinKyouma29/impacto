root.MainMenu = {
    Type: MainMenuType.MO6TW,
    MainMenuBackgroundSprite: "MainMenuBackground",
    MainMenuX: 993,
    MainMenuY: 114,
    MenuEntriesSprites: [],
    MenuEntriesHighlightedSprite: "MenuEntriesHSprite",
    MenuEntriesX: 999,
    MenuEntriesXOffset: 100,
    MenuEntriesFirstY: 128,
    MenuEntriesYPadding: 30,
    MenuEntriesTargetWidth: 222
};

for (var i = 0; i < 7; i++) {
    root.Sprites["MainMenuEntry" + i] = {
        Sheet: "Data",
        Bounds: {
            X: 590,
            Y: i * 32,
            Width: root.MainMenu.MenuEntriesTargetWidth,
            Height: 30
        }
    };
    root.MainMenu.MenuEntriesSprites.push("MainMenuEntry" + i);
}

root.Sprites["MenuEntriesHSprite"] = {
    Sheet: "Data",
    Bounds: { X: 66, Y: 0, Width: 92, Height: 31 }
};

root.Sprites["MainMenuBackground"] = {
    Sheet: "Data",
    Bounds: { X: 1765, Y: 1, Width: 282, Height: 378 }
};
