#ifndef UI_HANDLER_H_
#define UI_HANDLER_H_

#include "defs.h"
#include "object_manager.h"

namespace game {
	class uiHandler {
	public:
		uiHandler(ObjectManager*, GLFWwindow*);
		~uiHandler();

		void initUI();
		void uiUpdate();
		void RenderUI();
		
	private:
		ObjectManager* objManager;
		GLFWwindow* window_;
		PlayerGameObject* player_;

		//Window Height and Width
		const int uiH = 100;
		const int uiW = 100;
	};
}
#endif