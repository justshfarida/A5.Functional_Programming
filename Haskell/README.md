# Functional solution (Haskell): e-Olymp 1964 "What is left?"

Problem: <https://www.eolymp.com/en/problems/1964>

The numbers `1..n` (1 ≤ n ≤ 10^8) are written in a row. The following cycle is repeated until one number is left:

1. remove the numbers at **even** positions,
2. renumber, remove the numbers at **odd** positions,
3. renumber, remove the numbers at positions **divisible by 3**.

Print the number that remains.

**Result: 85 / 100** (the requirement for the functional language is more than 70). Code: [`Main.hs`](Main.hs)

The solution is purely functional: only immutable lists, recursion and pattern matching. There are no variables that change, no loops and no arrays, and the closed formula is not used.

---

## 1. How the solution works

Each step of the game is a function that takes a list and returns a **new**, shorter list. Nothing is changed in place.

### 1.1 Taking a list apart with `:`

In Haskell a list is "first element `:` rest of the list", so `[1,2,3,4]` is `1 : [2,3,4]`. A pattern like `(a : b : rest)` splits a list into its first element `a`, its second element `b`, and everything after them `rest`:

```
[1,2,3,4,5]  matched with  (a : b : rest)   →   a = 1,  b = 2,  rest = [3,4,5]
```

`_` means "there is an element here, but I don't need it". That is how elements are dropped.

### 1.2 The three rules

```haskell
removeEven []             = []
removeEven [a]            = [a]
removeEven (a : _ : rest) = a : removeEven rest
```

`removeEven` takes the elements in **pairs**, keeps the first of each pair and drops the second. Example:

```
removeEven [1,2,3,4,5]
= 1 : removeEven [3,4,5]       (keep 1, drop 2)
= 1 : 3 : removeEven [5]       (keep 3, drop 4)
= 1 : 3 : [5]                  (one left over: position 5 is odd → keep)
= [1,3,5]
```

```haskell
removeOdd []             = []
removeOdd [_]            = []
removeOdd (_ : b : rest) = b : removeOdd rest
```

`removeOdd` is the mirror image: it drops the first of each pair and keeps the second (`[1,2,3,4,5]` → `[2,4]`). A single leftover element is at an odd position, so `removeOdd [_] = []` drops it.

```haskell
removeThird (a : b : _ : rest) = a : b : removeThird rest
removeThird rest               = rest
```

`removeThird` takes the elements in **groups of three** and drops the third of each group. Every call removes exactly 3 elements from the front, so the dropped elements are always at positions 3, 6, 9, …, the positions divisible by 3. No `mod 3` is needed. When fewer than 3 elements are left, none of them is at a position divisible by 3, so all are kept.

### 1.3 The game

```haskell
step1 numbers = if length numbers == 1 then head numbers else step2 (removeEven numbers)
step2 numbers = if length numbers == 1 then head numbers else step3 (removeOdd numbers)
step3 numbers = if length numbers == 1 then head numbers else step1 (removeThird numbers)
```

* `numbers` is the parameter: the list the step receives.
* If the list has exactly one element (`length numbers == 1`), that element (`head numbers`) is the answer.
* Otherwise the step does its removal and passes the new, shorter list to the next step.
* `step3` passes back to `step1`, so `step1 → step2 → step3 → step1 → …` replaces a `while` loop.
* Every step starts with its own "one left?" check, so the game stops after **any** step, not only at the end of a cycle.

Worked example, `n = 10`:

| call | list | one left? | what happens |
|---|---|---|---|
| `step1` | [1,2,3,4,5,6,7,8,9,10] | no | removeEven → [1,3,5,7,9] |
| `step2` | [1,3,5,7,9] | no | removeOdd → [3,7] |
| `step3` | [3,7] | no | removeThird → [3,7] |
| `step1` | [3,7] | no | removeEven → [3] |
| `step2` | [3] | **yes** | answer **3** |

### 1.4 Why simply chaining the rules does not work

A shorter version that checks only once per cycle:

```haskell
play numbers = if length numbers == 1 then head numbers
               else play (removeThird (removeOdd (removeEven numbers)))
```

fails for `n = 2`: `removeEven [1,2]` gives `[1]`, but `removeOdd [1]` then removes the last number. The list becomes `[]`, which never has length 1, so `play` keeps calling itself forever. That is why every step needs its own check.

---

## 2. Functional programming rules followed

| Imperative idea | What the Haskell solution uses instead |
|---|---|
| `while` loop | the functions call each other: `step1 → step2 → step3 → step1` |
| `for` loop with an index `i` | recursion that takes elements off the front of the list |
| `if (size <= 2) break;` | `if length numbers == 1 then head numbers` at the start of every step |
| changing a vector (`erase`) | every step builds a **new** list; old values never change |
| variables (`int w = 1; w++`) | none: `numbers`, `a`, `rest`, … are parameters whose values are fixed |

---

## 3. Result on e-Olymp: 85 / 100

| Tests | Verdict | Time | Memory |
|---|---|---|---|
| 1 – 26, 31 | Accepted | 10 – 35 ms | 1.6 – 3.7 MB |
| 27, 30 | Accepted | 39 ms, 46 ms | 19.5 MB, 10.4 MB |
| 32 | Accepted | 362 ms | 172.6 MB |
| 33 | Accepted | 562 ms | 273.6 MB |
| 28, 34 | Accepted | 752 ms, 739 ms | 377.5 MB |
| 35 | Accepted | 908 ms | **468.5 MB** |
| 29 | **Time limit exceeded** | 5 000 ms | 512 MB |
| 36 – 39 | **Memory overflow** | 1 074 – 1 634 ms | 512 MB |
| 40 | Accepted | — | — |

Every test that finished was accepted, so the logic is correct. The failed tests are the largest ones, and the deciding factor is **memory**, not time: memory grows with `n` until it reaches the 512 MB limit.

## 4. Why the largest tests fail

### 4.1 The whole list has to be in memory

`length numbers` has to walk through the **entire** list to count it, so Haskell must actually build every element. Right after that, the same list `numbers` is used again (`head numbers` or `removeEven numbers`). So the whole list has to stay in memory at once, with all of its elements.

### 4.2 A Haskell list uses much more memory per number than an array

A C++ `vector<int>` stores each number in 4 bytes, side by side. A Haskell list stores every element as a separate object in memory (a list cell that points to the next one, plus the number itself), and the garbage collector needs extra room while it copies data.

Using the n estimates from the C++ report (the same tests, where n was estimated from the C++ program's 4 bytes per number), this solution uses roughly **80–90 bytes per number** on e-Olymp:

| Test | Estimated n | Memory | Bytes per number |
|---|---|---|---|
| 32 | ≈ 2.2 million | 172.6 MB | ≈ 80 |
| 28 / 34 | ≈ 4.4 million | 377.5 MB | ≈ 86 |
| 35 | ≈ 5.5 million | 468.5 MB | ≈ 86 |
| 29 | ≈ 100 million | would need ≈ 8 GB | — |

So any test with more than about 6 million numbers cannot fit into 512 MB. Tests 36–39 ran out of memory, and test 29 (n = 10^8, the maximum) hit the limits as well.

