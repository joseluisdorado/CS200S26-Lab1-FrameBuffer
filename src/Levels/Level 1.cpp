#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Utils\OpenSaveFile.h"	// opensavefile dialog
#include "Level 1.h"


using namespace Rasterizer;
int mode = 0;

bool ShowFrameworkMenu();

void Level_1_Load()
{
}
void Level_1_Init()
{
	// Clear the frame buffer to white by default
	FrameBuffer::Clear(Rasterizer::Color(1.0f, 1.0f, 1.0f, 1.0f));

	if (mode == 1)
		FrameBuffer::Clear(Rasterizer::Color(1.0f,0.0f,0.0f,1.0f));

	if (mode == 2)
		FrameBuffer::Clear(Rasterizer::Color(0.0f, 1.0f, 0.0f, 1.0f));

	if (mode == 3) {
		Color colors[2];
		colors[0] = Color(1.0f, 0.0f, 0.0f, 1.0f);
		colors[1] = Color(0.0f, 0.0f, 1.0f, 1.0f);

		FrameBuffer::ClearCheckerboard(colors, 100);
	}

	if (mode == 4) {
		FrameBuffer::LoadFromImageFile("data/digipen-small.png");
	}

	if (mode == 5) {
		FrameBuffer::LoadFromImageFile("data/digipen-big.png");
	}

	if (mode == 6) {
		FrameBuffer::CheckerboardImage("data/digipen-small.png", 100);
	}

	// Hexadecimal representation ARGB			

	//FrameBuffer::Clear(Rasterizer::Color().FromU32(0xFFFF0000));


	
	//FrameBuffer::LoadFromImageFile("data/digipen-big.png");

	//FrameBuffer::CheckerboardImage("data/digipen-small.png", 100);

	FrameBuffer::SaveToImageFile("test.png");
}
void Level_1_Update()
{
	KeyboardInput();
	ShowFrameworkMenu();
}
void Level_1_Render()
{
	// Send content of frame buffer to 
	FrameBuffer::Present();
	// Debug
	//SaveFrameBuffer();
}
void Level_1_Free()
{
}
void Level_1_Unload()
{

}

// ----------------------------------------------------------------------------
// COMMON

void SaveFrameBuffer()
{
	// Save the data to file
	if (AEInputKeyTriggered(VK_F1))
	{
		OpenSaveFileDlg saveDlg;
		if (saveDlg.Save("Save Frame Buffer to Binary", "*.fb"))
		{
			std::string saveFile;
			if (saveDlg.GetNextFilePath(saveFile))
				Rasterizer::FrameBuffer::SaveToFile(saveFile.c_str());
		}
	}

	if (AEInputKeyTriggered(VK_F2))
	{
		OpenSaveFileDlg saveDlg;
		if (saveDlg.Save("Save Frame Buffer to Image", "*.png"))
		{
			std::string saveFile;
			if (saveDlg.GetNextFilePath(saveFile))
				Rasterizer::FrameBuffer::SaveToImageFile(saveFile.c_str());
		}
	}
	if (AEInputKeyTriggered(VK_F3))
	{
		OpenSaveFileDlg openDlg;
		if (openDlg.Open("Open Frame Buffer File", "*.fb"))
		{
			std::string selectedFile;
			if (openDlg.GetNextFilePath(selectedFile))
				Rasterizer::FrameBuffer::LoadFromFile(selectedFile.c_str());
		}
	}
}

void KeyboardInput()
{
	if (AEInputKeyTriggered('R'))
        AESetGameStateNext(AE_GS_RESTART);
	if (AEInputKeyTriggered('Q') && AEInputKeyPressed(AE_KEY_CTRL))
        AESetGameStateNext(AE_GS_QUIT);
}

bool ShowFrameworkMenu()
{
	bool isGuiActive = false;
	mode = 0;

	// show File options 
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Mode")) {

			if (ImGui::MenuItem("Clear to Red", "1"))
				mode = 1;

			else if (ImGui::MenuItem("Clear to Green", "2"))
				mode = 2;

			else if (ImGui::MenuItem("Clear to CheckerBoard", "3"))
				mode = 3;

			else if (ImGui::MenuItem("Clear to Small Image", "4"))
				mode = 4;

			else if (ImGui::MenuItem("Clear to Big Image", "5"))
				mode = 5;

			else if (ImGui::MenuItem("Clear to ImageCheckerBoard", "6"))
				mode = 6;

			ImGui::EndMenu();
		}

		auto mSize = ImGui::GetWindowHeight();
		AEVec2 mouseVP = AEGetMouseData().position + AEVec2((float)AEGetSysWindowWidth() / 2.0f, (float)AEGetSysWindowHeight() / 2.0f);
		auto inMenu = mouseVP.y >= (AEGetSysWindowHeight() - mSize);
		isGuiActive = ImGui::IsAnyItemActive() || ImGui::IsAnyItemHovered() || inMenu;

		ImGui::EndMainMenuBar();
	}

	if (mode != 0) {
		AESetGameStateNext(AE_GS_RESTART);
	}

	return isGuiActive;
}

