root.MainMenu = {
    Type: MainMenuType.MO6TW,
    MainMenuBackgroundSprite: "MainMenuBackground",
    MenuEntriesSprites: [],
    MenuEntriesHighlightedSprites: [],
    MenuEntriesX: 0,
    MenuEntriesXOffset: 100,
    MenuEntriesFirstY: 220,
    MenuEntriesYPadding: 50,
    MenuEntriesTargetWidth: 142
};

for (var i = 0; i < 8; i++) {
    root.Sprites["MainMenuEntry" + i] = {
        Sheet: "Title",
        Bounds: {
            X: 697,
            Y: 3 + i * 32,
            Width: root.MainMenu.MenuEntriesTargetWidth,
            Height: 27
        }
    };
    root.MainMenu.MenuEntriesSprites.push("MainMenuEntry" + i);
}

for (var i = 0; i < 8; i++) {
    root.Sprites["MainMenuEntryHighlighted" + i] = {
        Sheet: "Title",
        Bounds: {
            X: 949,
            Y: 2 + i * 32,
            Width: root.MainMenu.MenuEntriesTargetWidth,
            Height: 27
        }
    };
    root.MainMenu.MenuEntriesHighlightedSprites.push("MainMenuEntryHighlighted" + i);
}

root.Sprites["MainMenuBackground"] = {
    Sheet: "MainBg",
    Bounds: { X: 0, Y: 0, Width: 1280, Height: 720 }
};
