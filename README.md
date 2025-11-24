
---

## solution.md

```markdown
# Solution Explanation

## Transformation to Prefix XORs
Let $P_0, P_1, \dots, P_N$ be the prefix XORs of array $A$, where $P_0 = 0$ and $P_i = P_{i-1} \oplus A_i$.
A subarray $A[l \dots r]$ has XOR sum 0 if and only if $P_{l-1} = P_r$.

Thus, finding $K$ zero-XOR subarrays is equivalent to choosing $N+1$ values for $P$ such that the number of identical pairs in $P$ is exactly $K$.
If a value $x$ appears $c_x$ times in $P$, it contributes $\binom{c_x}{2}$ to $K$.
We need: $\sum \binom{c_i}{2} = K$ and $\sum c_i = N+1$.

## The Constraint ($A_i > 0$)
The problem states $A_i$ must be positive. This implies $P_{i-1} \oplus P_i \neq 0$, or $P_{i-1} \neq P_i$.
This means in our sequence $P$, no two adjacent elements can be the same.
For a multiset of frequencies $\{c_1, c_2, \dots, c_m\}$, a valid ordering satisfying this adjacency constraint exists if and only if:
$$ \max(c_i) \le \lceil (N+1)/2 \rceil $$

## The Algorithm
We need to partition the integer $M = N+1$ into parts $c_i$ such that $\sum \binom{c_i}{2} = K$ and $\max(c_i) \le \lceil M/2 \rceil$.

Let $Limit = \lceil M/2 \rceil$.
We can solve this using a greedy approach with feasibility checking:

1.  **Max K Check:** The maximum possible $K$ for a given $M$ is achieved by splitting $M$ into two groups of size roughly $M/2$. If $K$ is larger than this, output NO.
2.  **Constructing Partition:**
    We iterate to find the size of the next group $sz$. We want to pick the largest $sz \in [1, Limit]$ such that:
    *   We have enough "budget" $K$: $\binom{sz}{2} \le K$.
    *   The *remaining* $K$ can be formed by the *remaining* $M$ atoms without violating future constraints.
    
    To check the second condition efficiently:
    Let $remM = M - sz$ and $remK = K - \binom{sz}{2}$.
    The maximum $K$ achievable with $remM$ atoms (given the global limit $Limit$) is calculated by filling with groups of size $Limit$ as much as possible.
    If $remK \le \text{MaxPossible}(remM, Limit)$, then picking $sz$ is valid.

    We iterate $sz$ downwards from $Limit$ or $remM$. The first valid $sz$ is chosen. We repeat until $M=0$.

3.  **Constructing the Array:**
    Once we have the counts $c_1, c_2, \dots$, we assign distinct integer values to each group (e.g., $1, 2, 3 \dots$).
    We construct $P$ by filling the most frequent values at indices $0, 2, 4 \dots$ and then $1, 3, 5 \dots$ to satisfy the adjacency constraint.
     finally, $A_i = P_{i-1} \oplus P_i$.

## Complexity
The feasibility check is $O(1)$. The loop runs $O(\sqrt{K})$ or $O(N)$ times. Total time $O(N)$ per test case.
