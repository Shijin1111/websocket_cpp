## Problem: XOR-Balanced Transmission

**Time Limit:** 1 second  
**Memory Limit:** 256 MB  

You are working with a data stream represented by an array \( A \) of length \( N \), where every element \( A_i \) is a **positive integer** (and \( A_i \le 10^9 \)).

For any segment of this stream, say from position \( l \) to \( r \) (where \( 1 \le l \le r \le N \)), we can calculate the **bitwise XOR** of its elements:

\[
A_l \oplus A_{l+1} \oplus \dots \oplus A_r
\]

A segment is called **unstable** if this XOR value equals **zero**.

We define the **instability count** of the stream as the total number of unstable (zero-XOR) subsegments.

Formally, it’s the number of pairs \( (l, r) \) satisfying:

\[
A_l \oplus A_{l+1} \oplus \dots \oplus A_r = 0
\]

---

### Your Task

Given two numbers \( N \) and \( K \):

- \( N \) — the length of the array \( A \)
- \( K \) — the **desired** instability count

Your job is to construct an array \( A \) of length \( N \) such that **exactly \( K \)** subsegments of \( A \) have a bitwise XOR of zero.

If such an array is possible, print **“YES”** followed by the array.  
If it’s impossible, print **“NO”**.

---

### Input Format

- The first line contains an integer \( t \) — the number of test cases.  
- Each of the next \( t \) lines contains two integers \( N \) and \( K \).

---

### Output Format

For each test case:

- If a valid array exists, print:

  ```
  YES
  A_1 A_2 ... A_N
  ```

- Otherwise, print:

  ```
  NO
  ```

---

### Constraints

- \( 1 \le t \le 10^4 \)  
- \( 1 \le N \le 2 \times 10^5 \)  
- \( 0 \le K \le 10^{18} \)  
- The sum of all \( N \) over all test cases ≤ \( 2 \times 10^5 \)

---

### Example

**Input**
```
4
3 2
3 3
5 5
1 0
```

**Output**
```
YES
1 2 3
NO
YES
1 1 2 2 3
YES
5
```

---

### Explanation (for intuition)

- For example, with \( A = [1, 2, 3] \):

  - Subsegments whose XOR = 0 are:  
    - \( [1, 1, 2] \) has none  
    - \( [1, 2] \): XOR = 3  
    - \( [2, 3] \): XOR = 1  
    - \( [1, 2, 3] \): XOR = 0  

  So, only one subsegment produces a 0 XOR. Such counting is the heart of the problem.

You’re essentially designing the sequence so that zero-XOR segments occur exactly \( K \) times.
