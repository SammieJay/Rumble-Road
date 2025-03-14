#ifndef UI_HANDLER_H_
#define UI_HANDLER_H_

#include "defs.h"
#include "object_manager.h"

namespace game {
	class uiHandler {
	public:
		uiHandler(ObjectManager*, GLFWwindow*, int*);
		~uiHandler();

		void initUI();
		void uiUpdate();
		void RenderUI();
		
	private:
		ObjectManager* objManager;
		GLFWwindow* window_;
		PlayerGameObject* player_;
		int* numTowers;

		//Window Height and Width
		const int uiH = 100;
		const int uiW = 100;

		//function to init new window with given parameters
		void newWindow(ImVec2 pos, ImVec2 scale, float alpha, char* name);

		//functions to render specific panels
		void RenderDebugPanel();
	};
}
#endif