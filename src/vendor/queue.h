#ifndef QUEUE_H
#define QUEUE_H

# ifdef __cplusplus
extern "C" {
# endif

typedef struct queue_node {
    int x, y;
    struct queue_node *next;
} queue_node_t;

# ifdef __cplusplus
}
# endif

#endif
