# Imperative solution (C++): e-Olymp 1964 "What is left?"

Problem: <https://www.eolymp.com/en/problems/1964>

The numbers `1..n` (1 ≤ n ≤ 10^8) are written in a row. The following cycle is repeated until one number is left:

1. remove the numbers at **even** positions,
2. renumber, remove the numbers at **odd** positions,
3. renumber, remove the numbers at positions **divisible by 3**.

Print the number that remains.

---

## 1. My first approach: simulate on a `std::vector` with `erase()`

The idea is to treat the row literally as an array and do exactly what the statement says.

* `numbers` is a vector of size `n + 1`. Index `0` is a dummy element so that the index **is** the 1-based position.
* For each step I walk the vector **from the back to the front** and `erase()` every element whose position must be removed. Going backwards matters: erasing at index `i` only shifts the elements *after* `i`, so the positions I still have to visit (all `< i`) do not change.
* After **every** step I check `numbers.size() <= 2` (only one real number left) and stop.

### Bug I found on the way

My first version checked the stopping condition only once, at the top of the `while` loop. The game can end after *any* of the three steps, though. For `n = 2`, step 1 leaves `[1]`, but my loop still ran step 2, which erased the last number. The vector became empty and `numbers[1]` read out of bounds. Comparing against a brute-force reference for `n = 1..40` showed wrong answers for `n = 2, 7–14, 39–40`. Adding the size check after each step fixed all of them.

---

## 2. Result on e-Olymp

**Score: 67 / 100 — Time limit exceeded** (limit 5 000 ms, 512 MB)

| Tests | Verdict | Time | Memory |
|---|---|---|---|
| 1 – 26 | Accepted | 9 – 26 ms | ≈ 1 MB |
| 27 | Accepted | 423 ms | 1.7 MB |
| 28, 29 | **Time limit exceeded** | 5 000 ms | 17.9 MB, **382.4 MB** |
| 30, 31 | Accepted | 138 ms, 9 ms | 1.3 MB, 1 MB |
| 32 – 35 | **Time limit exceeded** | 5 000 ms | 9.4 – 22.1 MB |
| 36 – 40 | Skipped by the judge | — | — |

So the logic is **correct** (every test that finished was accepted). The only problem is **speed**.

---

## 3. Why I got Time Limit Exceeded

### 3.1 Estimating `n` for each test from its memory usage

The program stores one `int` (4 bytes) per number, plus ≈ 1 MB of baseline memory. So

```
n ≈ (memory − 0.97 MB) / 4 bytes
```

| Test | Memory | Estimated n | Result |
|---|---|---|---|
| 30 | 1.3 MB | ≈ 90 000 | Accepted, 138 ms |
| 27 | 1.7 MB | ≈ 190 000 | Accepted, 423 ms |
| 32 | 9.4 MB | ≈ 2.2 million | TLE |
| 33 | 13.7 MB | ≈ 3.3 million | TLE |
| 28 / 34 | ≈ 17.9 MB | ≈ 4.4 million | TLE |
| 35 | 22.1 MB | ≈ 5.5 million | TLE |
| 29 | 382.4 MB | **≈ 100 million** (the maximum, 10^8) | TLE |

Every test that failed has `n` in the millions. Every test that passed has `n` in the hundreds of thousands or less.

### 3.2 `vector::erase()` in the middle is O(size)

A `std::vector` keeps its elements in one contiguous block of memory. When I erase element `i`, every element after it must be **shifted one place to the left** to close the gap. One `erase()` therefore costs about `size − i − 1` moves.

Example: deleting the 2nd element of `[1, 2, 3, 4, 5, 6]` moves 4 elements (`3, 4, 5, 6`). Deleting 5 elements one by one this way means 5 separate shifts.

### 3.3 Counting the total work: about 0.168 · n² moves

Step 1 deletes about `n/2` elements. When I erase position `i` (going backwards), the elements behind it are the survivors I already kept, about `(n − i) / 2` of them. Summing over all even `i`:

```
step 1:  Σ (n − i)/2  over even i       ≈  n²/8
step 2:  same idea on n/2 elements      ≈  (n/2)²/8  = n²/32
step 3:  on n/4 elements, keeps 2/3     ≈  (n/4)²/9  = n²/144
one cycle                               ≈  0.163 · n²
```

Each cycle keeps only `1/2 · 1/2 · 2/3 = 1/6` of the numbers, so the later cycles add only a little (×1/36, ×1/36², …). The total is

```
total moves ≈ 0.168 · n²        →  O(n²)
```

I checked this by counting the shifted elements directly in code:

| n | erase() calls | elements shifted | shifted / n² |
|---|---|---|---|
| 10 000 | 9 999 | 16 784 187 | 0.1678 |
| 100 000 | 99 999 | 1 678 556 510 | 0.1679 |
| 200 000 | 199 999 | 6 714 251 491 | 0.1679 |

The number of `erase()` calls is only `n − 1` (linear). The **shifting** inside them is what grows quadratically.

### 3.4 Timing confirms the quadratic growth

Local run (`g++ -O2`):

| n | time |
|---|---|
| 100 000 | 0.11 s |
| 500 000 | 3.1 s |
| 1 000 000 | 12.6 s |

Multiplying `n` by 10 multiplies the time by about 100. That is exactly what O(n²) predicts.

### 3.5 Projecting to the failing tests

Using test 27 on e-Olymp (n ≈ 190 000 → 423 ms) and `time ∝ n²`:

| n | predicted time |
|---|---|
| ≈ 650 000 | ≈ 5 s ← largest n that fits the limit |
| 2.2 million (test 32) | ≈ 1 minute |
| 5.5 million (test 35) | ≈ 6 minutes |
| 100 million (test 29) | ≈ 33 **hours** |

So the first approach can never pass the large tests. Micro-optimisations (fast I/O, `-O2`, `reserve`) cannot fix it, because the problem is the O(n²) algorithm itself.

Memory is **not** the problem: even for n = 10^8 the vector uses ≈ 382 MB, below the 512 MB limit.

---

## 4. How to fix it (still array processing)

The fix is to stop deleting elements one at a time. Each step should rebuild the array in **one pass**:

* keep a write index `w = 0`,
* walk through the array, and copy every element that **survives** the step to position `w`, then `w++`,
* after the pass, cut off the tail once: `numbers.erase(numbers.begin() + w, numbers.end())` (or `resize(w)`).

Every element is touched once per step, so a step costs O(size). Because each cycle keeps only 1/6 of the elements, the total work is about `1.2 · n`, which is **O(n)**. It also needs no second array, so the memory stays at ≈ 400 MB for n = 10^8.
