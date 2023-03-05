#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

static void node_swap(struct list_head *head,
                      struct list_head *node1,
                      struct list_head *node2)
{
    node1->next = node2->next;
    node2->next = node2->next->prev = node1;
    node2->prev = node1->prev;
    node1->prev = node1->prev->next = node2;
}

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *li = malloc(sizeof(struct list_head));
    if (!li)
        return NULL;

    INIT_LIST_HEAD(li);

    return li;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *el, *els;
    list_for_each_entry_safe (el, els, head, list)
        q_release_element(el);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *el = NULL;

    if (!head || !s)
        return false;

    el = malloc(sizeof(element_t));
    if (!el)
        return false;

    el->value = strdup(s);
    list_add(&el->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *el = NULL;

    if (!head || !s)
        return false;

    el = malloc(sizeof(element_t));
    if (!el)
        return false;

    el->value = strdup(s);
    list_add_tail(&el->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    element_t *el;
    el = list_first_entry(head, element_t, list);
    if (sp)
        strlcpy(sp, el->value, bufsize);
    list_del(&el->list);

    return el;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    element_t *el;
    el = list_last_entry(head, element_t, list);
    if (sp)
        strlcpy(sp, el->value, bufsize);
    list_del(&el->list);

    return el;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int size = 0;
    struct list_head *li;

    if (!head)
        return 0;

    list_for_each (li, head)
        size++;

    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    int size = q_size(head);
    int mid_idx = size / 2;
    int i = 0;
    element_t *el = NULL;

    list_for_each_entry (el, head, list) {
        if (mid_idx == i++) {
            list_del(&el->list);
            q_release_element(el);
            return true;
        }
    }

    return false;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *curr;
    list_for_each (curr, head) {
        if (curr->next == head) {
            head->prev = curr;
            break;
        }
        node_swap(head, curr, curr->next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *li;
    struct list_head *li_old_next;
    li_old_next = head->next;
    head->next = head->prev;
    head->prev = li_old_next;
    list_for_each (li, head) {
        li_old_next = li->next;
        li->next = li->prev;
        li->prev = li_old_next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    /* Bubble sort */
    struct list_head *curr;
    struct list_head *next = NULL;
    for (int i = q_size(head); i > 0; i--) {
        curr = head->next;
        next = curr->next;
        while (next != head) {
            element_t *curr_el = list_entry(curr, element_t, list);
            element_t *next_el = list_entry(next, element_t, list);
            if (strcmp(curr_el->value, next_el->value) > 0) {
                node_swap(head, curr, next);
                next = curr->next;
                continue;
            }

            curr = curr->next;
            next = curr->next;
        }
    }
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;

    queue_contex_t *qctx = NULL;
    struct list_head *first = list_entry(head->next, queue_contex_t, chain)->q;
    list_for_each_entry (qctx, head, chain) {
        struct list_head *heada = first;
        struct list_head *headb = qctx->q;
        element_t *ela = NULL;
        element_t *elb = NULL;

        if (heada == headb)
            continue;

        list_for_each_entry (elb, headb, list) {
            struct list_head *prev = elb->list.prev;
            struct list_head *next = elb->list.next;
            list_for_each_entry (ela, heada, list) {
                if (strcmp(elb->value, ela->value) > 0)
                    continue;
                break;
            }
            /* Insert b element to a */
            elb->list.prev = ela->list.prev;
            elb->list.next = &ela->list;
            ela->list.prev = ela->list.prev->next = &elb->list;
            /* Remove b element from b */
            prev->next = next;
            next->prev = prev;
            elb = list_entry(next, element_t, list);
        }
    }

    return q_size(first);
}
