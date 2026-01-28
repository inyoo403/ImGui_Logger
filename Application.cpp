#include "Application.h"
#include "imgui/imgui.h"
#include "Logger.h"

namespace ClassGame {
        static bool show_demo_window = true;
        static bool show_another_window = false;
        static int counter = 0;

        void GameStartUp() 
        {
            Logger& logger = Logger::GetInstance();
            logger.LogInfo("Game started successfully");
            logger.LogGameEvent("Application initialized");
        }

        void RenderGame() 
        {
            ImGui::DockSpaceOverViewport();
            
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            Logger::GetInstance().Draw("Game Log");

            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver); // Default size
            ImGui::SetNextWindowPos(ImVec2(650, 50), ImGuiCond_FirstUseEver);   // Default position
            ImGui::Begin("Game Control");
            ImGui::Text("This is the main game control panel.");
            
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Game Action"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Separator();
            ImGui::Text("Logging Test Buttons:");
            
            if (ImGui::Button("Log Game Event"))
                Logger::GetInstance().LogGameEvent("Player made a move");
            
            ImGui::SameLine();
            if (ImGui::Button("Log Warning"))
                Logger::GetInstance().LogWarning("Invalid move attempted");

            ImGui::SameLine();
            if (ImGui::Button("Log Error"))
                Logger::GetInstance().LogError("Game state corrupted");

            ImGui::End();

            ImGui::Begin("ImGui Log Demo");
            ImGui::LogButtons();
            if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!");
                ImGui::LogFinish();
            }
            ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
        }
}
