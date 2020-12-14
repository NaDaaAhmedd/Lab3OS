#include <pthread.h>

struct station {
	int seatsAval;
	int waiting;
	int ready;
	pthread_mutex_t mutx;
	pthread_cond_t cond_trainFull;
	pthread_cond_t cond_trainHere;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
