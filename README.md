# 🎵 MP3 Tag Reader using C

## 📘 Overview
The **MP3 Tag Reader** is a command-line application written in **C language** to **read and edit ID3 tags** (metadata) embedded in MP3 audio files.  
It extracts key information such as **Title, Artist, Album, Year, Comments, and Genre**, and also allows editing these fields efficiently.

---

## ⚙️ Features
- 📖 **View MP3 Tags** — Display all tag details of an MP3 file.
- ✏️ **Edit MP3 Tags** — Modify specific fields such as title, artist, album, etc.
- 🧮 **Endian Conversion** — Handles byte-ordering for MP3 tag data.
- 🧰 **File Handling** — Reads and rewrites binary MP3 files safely.
- 🧠 **Error Handling** — Detects invalid files and missing arguments gracefully.

---

## 🧠 Concepts & Skills Demonstrated
- File handling in C (`fopen`, `fread`, `fwrite`, `fseek`)
- Bitwise and endian operations
- Command-line argument parsing
- Dynamic memory allocation
- Modular programming and function prototyping

---

## 🧰 Technologies Used
| Category | Tools / Concepts |
|-----------|------------------|
| **Language** | C |
| **Compiler** | GCC |
| **File Type** | Binary (MP3) |
| **Headers** | stdio.h, stdlib.h, string.h, stdint.h |
| **Platform** | Linux / Windows (Terminal) |

---

## 🚀 How to Run

### 1️⃣ Clone the repository
```bash
git clone https://github.com/yourusername/mp3-tag-reader.git
cd mp3-tag-reader
