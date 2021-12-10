#ifndef __HISTORY_H__
#define __HISTORY_H__
#include <stdbool.h>
#include "game.h"

struct s_history {
	square state;
	uint i;
	uint j;
	struct s_history* next;
	struct s_history* prev;
};

typedef struct s_history* history;

/**
 * @brief fprintf not enought memory + exit
 **/
void memoryError(void);

/**
 * @brief fprintf error on the given pointer + exit
 */
void errPointer(void);

/**
 * @brief Alloc a new empty history.
 * @return The allocated history.
 **/
history history_alloc();

/**
 * @brief Free the given history.
 * @param h The history to be free.
 **/
void history_free(history h);

/**
 * @brief Create a empty history object
 *
 * @return An empty history
 **/
history history_create_empty(void);

/**
 * @brief Delete every element after h
 *
 * @param h the head of the history
 */
void history_delete_entire_history(history h);

/**
 * @brief Test if the given history is empty or not.
 *
 * @param h the history to test.
 * @pre @p h must be a valid pointer toward an history .
 * @return true if the history is empty
 * @return false if the history isn't empty
 */
bool history_is_empty(history h);

/**
 * @brief Gets the next element of the given history.
 *
 * @param h An history.
 * @pre @p h must be a valid pointer toward an history .
 * @return the next of the given history.
 */
history history_next(history h);

/**
 * @brief Gets the previous element of the given history.
 *
 * @param h An history.
 * @pre @p h must be a valid pointer toward an history .
 * @return the previous element of the given history.
 */
history history_prev(history h);

/**
 * @brief Gets the state of the given history
 *
 * @param h An history.
 * @pre @p h must be a valid pointer toward an history .
 * @return the state of the given history
 */
square history_state(history h);

/**
 * @brief Get the coordinates i of the given history.
 *
 * @param h An history.
 * @pre @p h must be a valid pointer toward an history .
 * @return the coordinates i.
 */
uint history_i(history h);

/**
 * @brief Get the coordinates j of the given history.
 *
 * @param h An history.
 * @pre @p h must be a valid pointer toward an history .
 * @return the coordinates j.
 */
uint history_j(history h);

/**
 * @brief Insert a new element a the beginning  of the history.
 *
 * @param h An history (empty or not)
 * @param state state of the new element
 * @param i the coordinate i of the new element
 * @param j the coordinate j of the new element
 * @return The new first element of history
 */
history history_insert_first(history h, square state, uint i, uint j);
/**
 * @brief Delete the first element of the history
 *
 * @param h An history
 * @return the new first element of the history
 */
history history_delete_first(history h);

/**
 * @brief Insert a new element after p
 *
 * @param h An history
 * @param p Where we insert after the new element
 * @param state The state of the new element
 * @param i the coordinate i of the new element
 * @param j the coordinate j of the new element
 * @pre @p h must be a valid pointer toward an history.
 * @pre @p p must be a valid pointer toward an history element.
 * @return h
 */
history history_insert_after(history h, history p, square state, uint i, uint j);

/**
 * @brief Delete the element after p
 *
 * @param h An history
 * @param p where we delete after
 * @pre @p h must be a valid pointer toward an history.
 * @pre @p p must be a valid pointer toward an history element.
 * @return h
 */
history history_delete_after(history h, history p);

/**
 * @brief Insert a new element before p
 *
 * @param h An history
 * @param p Where we insert before the new element
 * @param state The state of the new element
 * @param i the coordinate i of the new element
 * @param j the coordinate j of the new element
 * @pre @p h must be a valid pointer toward an history.
 * @pre @p p must be a valid pointer toward an history element.
 * @return h
 */
history history_insert_before(history h, history p, square state, uint i, uint j);

/**
 * @brief Delete the element before p
 *
 * @param h An history
 * @param p where we delete before
 * @pre @p h must be a valid pointer toward an history.
 * @pre @p p must be a valid pointer toward an history element.
 * @return h
 */
history history_delete_before(history h, history p);

/**
 * @brief Gets the last element of the history
 *
 * @param h An history
 * @return The last element of the given history
 */
history history_last(history h);

/**
 * @brief Gets the first element of the history
 *
 * @param h An history
 * @return The first element of the history.
 */
history history_first(history h);

/**
 * @brief Add an element at the end of the list.
 *
 * @param h an history
 * @param state the state of the new element
 * @param i the coordinate i of the new element
 * @param j the coordinate j of the new element
 * @return h
 */
history history_append(history h, square state, uint i, uint j);

/**
 * @brief Delete all the elements after p
 *
 * @param h an history
 * @param p an element of h
 * @return h
 */
history history_delete_all_after(history h, history p);
#endif