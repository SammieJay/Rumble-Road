#include "ui_handler.h"

namespace game {
	uiHandler::uiHandler(ObjectManager* objMngr, GLFWwindow* window) {
		objManager = objMngr;
		window_ = window;
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

		//Run UI Code
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
		if (player_->turretReloading()) {ImGui::Text("Turret: reloading...");}
		else {ImGui::Text("Turret: %d/%d", player_->getBulletMag(), MAX_BULLETS);}

		//Rocket Counter
		ImGui::Text("Rockets: %d/%d", player_->getRocketMag(), MAX_ROCKETS);
		
		
		ImGui::End();//end UI code
	}

	void uiHandler::RenderUI() {
		//Render GUI Elements
		ImGui::Render();
		//glViewport(0, 0, uiW, uiH);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}