# 🧵 Philosophers

![42 Project](https://img.shields.io/badge/42%20Project-Philosophers-blue)
![Language](https://img.shields.io/badge/Made%20with-C-00599C?logo=c)
![Status](https://img.shields.io/badge/Status-Finished-brightgreen)

---

## 📖 About the Project

**Philosophers** is a project from [42 School](https://42.fr/en/homepage/) based on the classical **Dining Philosophers Problem**, a concept introduced by Edsger Dijkstra to illustrate synchronization issues and concurrency problems in computing.

In this project, we simulate a group of philosophers who:
- Eat 🍝
- Think 🤔
- Sleep 😴

They must **eat using two forks**, but there's only **one fork between each philosopher**. This creates challenges related to resource sharing and process coordination.

To solve this problem using **multithreading**, each philosopher is represented by a **thread** 🧵, and each fork is protected using a **mutex**. This allows us to:
- Prevent **race conditions**
- Synchronize access to shared resources
- Avoid **deadlocks** (where philosophers endlessly wait for forks)

### 🧠 Concepts Involved:
- **Thread**: A lightweight, independent path of execution. In this project, each philosopher runs in its own thread.
- **Mutex (Mutual Exclusion)**: A lock mechanism that ensures only one thread can access a critical section (like a fork) at a time.
- **Deadlock**: A situation where all threads are waiting for each other indefinitely, causing the program to freeze. Avoiding this is a key challenge in the project.

---

## ⚙️ How to Use

### 🛠 Compilation

```bash
make
```
▶️ Run the program

After compiling the program using `make`, you can run it with:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### 🧪 Examples & Expected Behavior

Here are some test cases and what you should expect from them:

#### ✅ `./philo 5 800 200 200`

- **What it does**: Launches 5 philosophers with 800ms to die, 200ms to eat, and 200ms to sleep.
- **Expected behavior**:  
  All philosophers eat, sleep, and think in turns.  
  **None should die**, because the total cycle time (200 + 200 = 400ms) is well below the 800ms time-to-die.

---

#### ✅ `./philo 5 800 200 200 7`

- **What it does**: Same as above, but each philosopher must eat **7 times**.
- **Expected behavior**:  
  The simulation will run until **all philosophers have eaten 7 times**, then it will **exit cleanly**.
  No philosopher should die.

---

#### ❌ `./philo 1 800 200 200`

- **What it does**: Runs the simulation with **only one philosopher**.
- **Expected behavior**:  
  A single philosopher can **only pick up one fork**, so they will **never be able to eat**.  
  Therefore, after 800ms (time_to_die), the philosopher will **die of starvation**.

---

#### ❌ `./philo 5 310 200 100`

- **What it does**: Philosophers get 310ms to live without food. It takes 200ms to eat and 100ms to sleep.
- **Why it fails**:  
  Each philosopher needs at least **200ms (eat) + some time to get forks + sleep (100ms)**.  
  Even in perfect conditions, **some philosophers will be delayed by mutex/fork contention**.  
  This means **at least one** philosopher will not be able to eat within 310ms → they **will die**.

---

### ⚠️ Tip

Always make sure:
- `time_to_die` > `time_to_eat + time_to_sleep` (to be safe).
- For a single philosopher, death is unavoidable due to the two-fork requirement.
