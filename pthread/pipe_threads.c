/*
 * =====================================================================================
 *
 *       Filename:  pipe-threads.c
 *
 *    Description:  一个流水线modern的线程实例
 *
 *        Created:  03/04/2012 10:21:09 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<pthread.h>
#include	<unistd.h>
#include	<zhao/tools.h>
//
// a pipeline stage struct. One for echa thread, 

typedef struct stage_tag {
    pthread_mutex_t mutex ; //protect data
    pthread_cond_t avail;  //data available
    pthread_cond_t ready; //data proces ready
    int data_ready;      //data persent, 上一阶段的结果
    int data;             // data to process
    pthread_t thread;
    struct stage_tag * next;
} stage_t;

// pipeline  mode
typedef struct pipe_tag {
    pthread_mutex_t mutex;
    stage_t *head;
    stage_t *tail;
    int stages; //number for stages
    int active; // processed stages count
} pipe_t ;

// send notify to the specified pipe stage,  Thread use this
// to pass along the modified data item
int
pipe_send(stage_t *stage, int process_data)
{
    int status;

    printf("thread:%u in send \n", (int)pthread_self());
    sleep(1);
    status = pthread_mutex_lock(&stage->mutex);
    if (status != 0) return status ;

    /* if there's data in the pipe stage, wait for it to be consumed */
    while (stage->data_ready) {
        //如果上一个pthread处理下一个时, read还没清空，就表示
        //下一个pthread还没处理完，pthread就wait到ready上

        printf("thread:%u in send wait read \n", (int)pthread_self());
        status = pthread_cond_wait(&stage->ready, &stage->mutex);
        if (status != 0) {
            pthread_mutex_unlock(&stage->mutex);
            return status;
        }
    }

    printf("thread:%u in send wait read up \n", (int)pthread_self());
    /* send the new process data */
    // 自己的数据有前一个thread设置
    stage->data = process_data ;
    stage->data_ready = 1 ; //mthread叫醒head
    status = pthread_cond_signal(&stage->avail);//自己睡在vavild上前一个thread叫醒
    if (status != 0) {
        pthread_mutex_unlock(&stage->mutex); 
        return status ;
    }
    status = pthread_mutex_unlock(&stage->mutex);
    return status ;
}

// thread handle 
void *
pipe_stage(void *arg) {
    stage_t *this= (stage_t *)arg;
    stage_t *next = this->next;

    PCHECK(pthread_mutex_lock(&this->mutex));

    while (1) { //process forever
        while (this->data_ready != 1) {
            printf("thread:%u send wait avail \n", (int)pthread_self());
            PCHECK(pthread_cond_wait(&this->avail, &this->mutex));
        }

        printf("thread:%u send wait avail up\n", (int)pthread_self());
        sleep(1);
        pipe_send(next, this->data + 1);
        this->data_ready = 0; //自己清空自己的ready
        PCHECK(pthread_cond_signal(&this->ready));
        //	PCHECK(pthread_cond_signal(&next->ready));
    }
}

// create stages initaliztion
int pipe_create(pipe_t *pipe, int stages)
{
    int pipe_index;
    stage_t **link = &pipe->head, *new_stage, *stage;

    //init
    PCHECK(pthread_mutex_init(&pipe->mutex, NULL));
    pipe->stages = stages;
    pipe->active = 0;

    for (pipe_index = 0; pipe_index <= stages; ++pipe_index) {
        EV_TEST(NULL, new_stage, (stage_t*)malloc(sizeof(stage_t)));
        PCHECK(pthread_mutex_init(&new_stage->mutex, NULL));
        PCHECK(pthread_cond_init(&new_stage->avail, NULL));
        PCHECK(pthread_cond_init(&new_stage->ready, NULL));
        new_stage->data_ready = 0;
        *link = new_stage;
        link = &new_stage->next;
    }
    *link = (stage_t*)NULL;
    pipe->tail = new_stage;

    for (stage = pipe->head; stage->next != NULL; stage = stage->next) {
        PCHECK(pthread_create(&stage->thread, NULL, pipe_stage, (void*)stage));
    }

    return 0;
}

int
pipe_start(pipe_t *pipe, int value) {

    PCHECK(pthread_mutex_lock(&pipe->mutex));
    pipe->active++;
    PCHECK(pthread_mutex_unlock(&pipe->mutex));
    //诛仙乘首先进来
    pipe_send(pipe->head, value);
    return 0;
}

int
pipe_result(pipe_t *pipe, int *result) {
    stage_t *tail = pipe->tail;
    int empty = 0;
    printf("thread:%u result\n", (int)pthread_self());

    PCHECK(pthread_mutex_lock(&pipe->mutex));
    if (pipe->active <= 0) 
        empty = 1;
    else 
        pipe->active--;

    PCHECK(pthread_mutex_unlock(&pipe->mutex));
    if (empty) return 0;

    while (!tail->data_ready)
        pthread_cond_wait(&tail->avail, &tail->mutex);
    *result = tail->data;
    tail->data_ready = 0;
    pthread_cond_signal(&tail->ready);
    pthread_mutex_unlock(&tail->mutex);
    return 1;
}

int main(int argc, char *argv[]) {
    pipe_t pipe;
    int value, result;
    char line[128];

    pipe_create(&pipe, 2);
    printf("Enter inter value, or '=' for next result\n");

    while (1) {
        printf("Data> ");
        E_TEST(NULL, fgets(line, sizeof(line), stdin));
        if (strlen(line) <= 1) continue;
        if (strlen(line) <= 2 && line[0] == '=') {
            if (pipe_result(&pipe, &result))
                printf("Result is %d\n", result);
            else
                printf("Pipe is empty\n");
        } else {
            if (sscanf(line, "%d", &value) < 1)
                fprintf(stderr, "Enter an integer value\n");
            else 
                pipe_start(&pipe, value);
        }
    }
    return 0 ;
}

