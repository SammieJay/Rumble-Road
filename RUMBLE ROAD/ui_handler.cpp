#include "ui_handler.h"

namespace game {
	uiHandler::uiHandler(ObjectManager* objMngr, GLFWwindow* window, int* remainingTowers) {
		objManager = objMngr;
		window_ = window;
		numTowers = remainingTowers;
	}

	uiHandler::~uiHandler() {
		//Terminate UI
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void uiHandler::initUI() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init("#version 460");
		player_ = objManager->Player();
	}

	void uiHandler::uiUpdate() {
		//Update GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Render side panel
		RenderDebugPanel();
		
		
		//NEW WINDOW
		ImGui::SetNextWindowPos(ImVec2 (0.0f, 300.0f));
		ImGui::Begin("Game HUD2", NULL, //set flags to make ui immovable and no title
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize
		);

		if (player_->Drifting()) ImGui::Text("DRIFTING");
		else ImGui::Text("not drifting");
		ImGui::End();
	}

	void uiHandler::RenderUI() {
		//Render GUI Elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void uiHandler::newWindow(ImVec2 pos, ImVec2 scale, float alpha, char* name) {
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(scale);
		ImGui::SetNextWindowBgAlpha(alpha);
		ImGui::Begin(name, NULL, //set flags to make ui immovable and no title
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize//change this to no resize depending on scale value supplied
		);
	}


	//FUNCTIONS TO DISPLAY EACH INDIVIDUAL WINDOW

	void uiHandler::RenderDebugPanel() {
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::Begin("Game HUD", NULL, //set flags to make ui immovable and no title
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize
		);

		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

		//Health Counter
		float healthPercent = (float)player_->getHealth() / int(PLAYER_MAX_HEALTH);
		ImGui::ProgressBar(healthPercent, ImVec2(200, 20));

		//Turret Ammo Counter
		if (player_->turretReloading()) { ImGui::Text("Turret: reloading..."); }
		else { ImGui::Text("Turret: %d/%d", player_->getBulletMag(), MAX_BULLETS); }

		//Rocket Counter
		ImGui::Text("Rockets: %d/%d", player_->getRocketMag(), MAX_ROCKETS);

		ImGui::Text("Towers Remaining: %d/%d", *numTowers, NUM_TOWERS);
		ImGui::End();//end UI code
	}
}