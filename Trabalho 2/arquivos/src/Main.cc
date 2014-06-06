/*
 * Authors:
 *    * Caio Lopes - 
 *    * Matheus Steck Cardoso - mathsteck@gmail.com
 *    * Murilo Pratavieira - murilo.pratavieira@gmail.com
 *
 * */

#include <VideoStore.h>

int main(int argc, char *argv[]) {
    VideoStore *video = new VideoStore();

    video->menu();

    delete video;

    return 0;
}
