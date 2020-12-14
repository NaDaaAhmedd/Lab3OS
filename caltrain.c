#include <pthread.h>
#include "caltrain.h"

void
station_init(struct station *station)
{
    station->seatsAval=0;
    station->waiting=0;
    station->ready=0;
    pthread_mutex_init(&station->mutx,NULL);
    pthread_cond_init(&station->cond_trainFull, NULL);
    pthread_cond_init(&station->cond_trainHere, NULL);
}

void
station_load_train(struct station *station, int count)
{
    if (count == 0 || station->waiting == 0){
    return;
    }
    station->seatsAval = count;
    station->ready = 0;
    pthread_mutex_lock(&station->mutx);
    pthread_cond_broadcast(&station->cond_trainHere);
    while ( station->ready != 0 || (station->seatsAval != 0 && station->waiting != 0)){
    pthread_cond_wait(&station->cond_trainFull,&station->mutx);
    }
    pthread_mutex_unlock(&station->mutx);
    station->seatsAval = 0;
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutx);
    station->waiting++;
    while(station->seatsAval == 0)
    {
        pthread_cond_wait(&station->cond_trainHere,&station->mutx);
    }
    station->seatsAval--;
    station->waiting--;
    station->ready++;
    pthread_mutex_unlock(&station->mutx);
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutx);
    station->ready--;
    if (station->ready == 0) {
            pthread_cond_signal(&station->cond_trainFull);
    }
    pthread_mutex_unlock(&station->mutx);
}
