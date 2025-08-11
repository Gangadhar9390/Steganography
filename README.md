# Steganography
# 🖼️ Image Steganography – LSB Technique  

## 📌 Overview  
This project implements **image-based steganography** using the **Least Significant Bit (LSB)** method.  
It can **encode** a secret file into a `.bmp` image and later **decode** it to retrieve the hidden data — all without visible changes to the image.  

---

## ✨ Features  
✅ Hide data inside **.bmp** images using LSB substitution.  
✅ Supports `.txt`, `.c`, and `.sh` files.  
✅ **Encoding** and **Decoding** modes.  
✅ Uses a **magic string** (`#*`) to verify hidden data.  
✅ Maintains original image quality.  
✅ Clear console logs and error handling.  

---

## 📂 Project Structure  
| File / Folder       | Description |
|---------------------|-------------|
| `beautiful.bmp`     | Sample cover image |
| `secret.txt`        | Sample secret file |
| `encode.c / .h`     | Encoding logic |
| `decode.c / .h`     | Decoding logic |
| `enc_file.c`        | File handling for encoding |
| `common.h`          | Magic string definition |
| `types.h`           | Data types and enums |
| `main.c`            | Entry point |

---

## ⚙️ How It Works  

### **Encoding Process**  
1. Copy BMP header to stego image.  
2. Embed a magic string (`#*`) for identification.  
3. Store file extension length, extension, size, and data in **LSBs** of pixels.  
4. Save modified image as the **stego image**.  

### **Decoding Process**  
1. Read BMP header and verify magic string.  
2. Extract file extension, size, and data from **LSBs**.  
3. Save the extracted content as a new file.  

---

## 🚀 Usage  
<img width="956" height="101" alt="Screenshot 2025-08-11 160042" src="https://github.com/user-attachments/assets/97395509-dc4d-43d4-9d07-ae6a49af3b59" />

### **Compile**  
```bash
gcc main.c encode.c decode.c enc_file.c -o stego
---

### **Sample ouput images**
<img width="1210" height="573" alt="Screenshot 2025-08-11 161656" src="https://github.com/user-attachments/assets/ee9b32b5-5e5c-4e95-80f3-3a2d7acc030e" />

<img width="1042" height="473" alt="Screenshot 2025-08-11 161716" src="https://github.com/user-attachments/assets/8878d9eb-77a6-4aec-877e-cf6c1b9039fb" />



<img width="1048" height="498" alt="Screenshot 2025-08-11 162244" src="https://github.com/user-attachments/assets/92ec6470-0068-4a7c-a13b-98b116cbfb9e" />

<img width="728" height="382" alt="Screenshot 2025-08-11 162510" src="https://github.com/user-attachments/assets/f64b0231-6534-4a18-8f87-f03220762e0f" />

 

