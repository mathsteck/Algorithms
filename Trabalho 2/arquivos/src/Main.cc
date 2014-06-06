#include <VideoStore.h>

int main(int argc, char *argv[]) {
	VideoStore *video = new VideoStore();

	video->menu();	

	delete video;

	return 0;
}
