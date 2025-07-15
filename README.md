# 🏝️ Pass the Lake

A simple 2D grid-based SFML game where a pirate must cross a treacherous lake, avoiding water tiles and navigating toward the goal tile. The game features three difficulty modes, real-time movement, and a high score timer.

<img width="595" height="600" alt="Screenshot from 2025-07-15 11-30-59" src="https://github.com/user-attachments/assets/72126f7f-49ca-427f-aae0-22d2320065de" />

<img width="595" height="600" alt="Screenshot from 2025-07-15 11-31-12" src="https://github.com/user-attachments/assets/909aece7-bbbc-46cc-96d8-671acdf90eab" />


---

## 🎮 Gameplay

- Navigate a **10x10 tile grid** with safe (`brown`), dangerous (`blue`/`turquoise`), and goal (`green`) tiles.
- Use **keyboard inputs** to move in all 8 directions (diagonal + cardinal).
- Difficulty affects how many safe tiles are generated.
- Reach the goal tile at bottom-right before drowning.
- A timer tracks your performance and updates the high score for each difficulty level.

---

## 🕹️ Controls

| Key       | Movement         |
|-----------|------------------|
| `↑` / `↓` | Up / Down        |
| `←` / `→` | Left / Right     |
| `Q`       | North-West       |
| `P`       | North-East       |
| `Z`       | South-West       |
| `M`       | South-East       |
| `Enter`   | Start game       |
| `Backspace` | Return to menu |

---

## 🧠 Features

- 📊 Difficulty selection with `Easy`, `Medium`, and `Hard` modes
- ⏱️ In-game timer + high score tracking for each mode
- 🏴‍☠️ Pirate character sprite with grid-based movement
- 🧱 Background rendering and animated blinking title
- 🔄 Game restarts after drowning or completing the level
- 🎨 Color-coded tiles:
  - **Green**: Start/End
  - **Brown**: Safe
  - **Blue**: Risky (causes drowning)
  - **Turquoise**: Very risky (50% chance of drowning)

---

## 🧩 Dependencies

- [SFML 2.6.1](https://www.sfml-dev.org/download.php)
- C++17
- CMake 3.10+
- X11 libraries (Linux)

---

## ⚙️ Build Instructions (Linux)

### 1. Clone the repository
```bash
git clone https://github.com/your-username/pass-the-lake.git
cd pass-the-lake
