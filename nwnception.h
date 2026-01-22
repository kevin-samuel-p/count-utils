#ifndef NWNCEPTION_H
#define NWNCEPTION_H

/**
 * @file nwnception.h
 * @brief Numbers-Within-Numbers (NWN) utilities.
 *
 * Provides operations on dot-separated numeric strings representing
 * nested numeric structures:
 *
 *  - Numbers Within Numbers (NWN)
 *  - Numbers Within Numbers Within Numbers (NWNWN)
 *  - Numbers Within Numbers with N Numbers (NWNWNN)
 *
 * Example representation:
 * @code
 * 5.3.1
 * @endcode
**/


/**
 * @enum NWN_Mode
 * @brief Specifies the NWN operating mode.
 *
 * Controls the number of numeric partitions and resizing behavior.
 */
enum NWN_Mode
{
    /** Exactly 2 numeric partitions */
    NWN = 2,

    /** Exactly 3 numeric partitions */
    NWNWN = 3,

    /**
     * Variable number of partitions.
     *
     * The number of partitions is determined by
     * the first numeric value.
     */
    NWNWNN
};


/**
 * @brief Increments an NWN numeric representation in-place.
 *
 * @param arr
 *      Pointer to a heap-allocated array of numeric partitions.
 *
 * @param arrSize
 *      Pointer to the current number of elements in @p arr.
 *
 * @param mode
 *      NWN mode controlling increment and resizing behavior.
 *
 * @post
 *      - The array contents are modified
 *      - The array may be reallocated (especially in @ref NWNWNN mode)
 *      - On allocation failure, *@p arr is freed and set to NULL
 *
 * @warning
 *      Callers must not use *@p arr after failure unless reinitialized.
 */
void increment_nwn(long **arr, long *arrSize, enum NWN_Mode mode);


/**
 * @brief Converts an NWN numeric array into a dot-separated string.
 *
 * @param arr
 *      Array of numeric partitions.
 *
 * @param arrSize
 *      Number of elements in @p arr.
 *
 * @return
 *      - Heap-allocated dot-separated string on success
 *      - NULL on allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 *
 * @example
 * @code
 * [5, 3, 1] -> "5.3.1"
 * @endcode
 */
char *stringify_nwn(const long *arr, long arrSize);


/**
 * @brief Computes the next NWN value from a dot-separated string.
 *
 * @param numString
 *      Dot-separated numeric string representing an NWN structure.
 *
 * @param mode
 *      NWN operating mode.
 *
 * @pre
 *      - Input must contain positive integers only
 *      - Numeric partitions must be in non-increasing order
 *
 * @par Mode behavior:
 *      - @ref NWN     : expects exactly 2 partitions
 *      - @ref NWNWN   : expects exactly 3 partitions
 *      - @ref NWNWNN  : number of partitions determined by first value
 *
 * @warning
 *      This function modifies the input buffer using strtok().
 *
 * @return
 *      - Heap-allocated string representing the next NWN value
 *      - NULL on invalid input or allocation failure
 *
 * @note
 *      The caller is responsible for freeing the returned string.
 */
char *nwns(char *numString, enum NWN_Mode mode);


#endif /* NWNCEPTION_H */