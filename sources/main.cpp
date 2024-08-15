#include "./../includes/header.h"

int main(void) {
	WindowsApp *app = new WindowsApp();
	while(app->isClosed() != true) {
		
	}
	delete app;
	return (0);
}