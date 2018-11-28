#ifndef COLLATZ_H
#define COLLATZ_H

/**
 * Collatz function that initializes with k = 0 and a = {@code n}.
 *
 * Collatz function is defined as:
 * a_k = { n   , k = 0
 *       { f(a), k > 0
 *
 * f(a) = { a/2   , a is even
 *        { 3a + 1, a is odd
 */
int collatz(int n);

#endif // COLLATZ_H
