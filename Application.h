#pragma once

namespace ClassGame {
    void GameStartUp();
    void RenderGame();
    void EndOfTurn();
    
    // Logging window management
    void ShowLogWindow(bool* p_open = nullptr);
    void ToggleLogWindow();
    bool IsLogWindowVisible();
}
