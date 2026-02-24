#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

// Hàm tạo ma trận ký tự
void genMatrix(char key[]) {
    int wordUsed[26] = {0}; // Đánh dấu các chữ cái đã được đưa vào ma trận
    int r = 0, c = 0;
    
    wordUsed['J' - 'A'] = 1; // Mặc định bỏ qua J

    // Đưa từ khóa vào ma trận
    for (int i = 0; i < strlen(key); i++) {
        if (isalpha(key[i])) {
            char ch = toupper(key[i]);
            if (ch == 'J') ch = 'I'; // Đổi J thành I
            
            if (wordUsed[ch - 'A'] == 0) {
                matrix[r][c] = ch;
                wordUsed[ch - 'A'] = 1;
                c++;
                if (c == 5) {
                    c = 0;
                    r++;
                }
            }
        }
    }

    // Điền các chữ cái còn lại của bảng chữ cái vào ma trận
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (wordUsed[ch - 'A'] == 0) {
            matrix[r][c] = ch;
            wordUsed[ch - 'A'] = 1;
            c++;
            if (c == 5) {
                c = 0;
                r++;
            }
        }
    }
}

// In ma trận ra màn hình để kiểm tra
void printMatrix() {
    printf("\nMa trận Playfair (5x5):\n");
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            printf("%c ", matrix[r][c]);
        }
        printf("\n");
    }
}

// Hàm xử lý văn bản đầu vào 
void preprocess(char input[], char output[]) {
    char temp[2000];
    int k = 0;

    // 1. Lọc và chuẩn hóa
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char ch = toupper(input[i]);
            if (ch == 'J') ch = 'I';
            temp[k++] = ch;
        }
    }
    temp[k] = '\0';

    // 2. Chia cặp
    int i = 0, j = 0;
    while (i < k) {
        output[j++] = temp[i];
        
        // Nếu là ký tự cuối cùng của chuỗi (lẻ)
        if (temp[i+1] == '\0') {
            output[j++] = 'X';
            i++;
        } 
        // Nếu ký tự tiếp theo giống ký tự hiện tại
        else if (temp[i] == temp[i+1]) {
            output[j++] = 'X';
            i++; 
        } 
        // Cặp bình thường
        else {
            output[j++] = temp[i+1];
            i += 2;
        }
    }
    output[j] = '\0';
}

// Hàm tìm tọa độ của một ký tự trong ma trận
void findPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (matrix[r][c] == ch) {
                *row = r;
                *col = c;
                return;
            }
        }
    }
}

// Hàm mã hóa Playfair
void encrypt(char processed[], char ciphertext[]) {
    int len = strlen(processed);
    
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        findPosition(processed[i], &r1, &c1);
        findPosition(processed[i+1], &r2, &c2);

        // Cùng hàng (Dịch phải 1 ô)
        if (r1 == r2) {
            ciphertext[i]   = matrix[r1][(c1 + 1) % 5];
            ciphertext[i+1] = matrix[r2][(c2 + 1) % 5];
        }
        // Cùng cột (Dịch xuống 1 ô)
        else if (c1 == c2) {
            ciphertext[i]   = matrix[(r1 + 1) % 5][c1];
            ciphertext[i+1] = matrix[(r2 + 1) % 5][c2];
        }
        // Hình chữ nhật (Đổi cột cho nhau)
        else {
            ciphertext[i]   = matrix[r1][c2];
            ciphertext[i+1] = matrix[r2][c1];
        }
    }
    ciphertext[len] = '\0'; // Kết thúc chuỗi
}

int main() {
    char key[100];
    char plaintext[1000];
    char processedStr[2000];
    char ciphertext[2000];

    // Xóa bộ đệm bàn phím và nhập khóa
    printf("Nhập keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0; // Loại bỏ ký tự xuống dòng '\n' do fgets để lại

    // Sinh và in ma trận
    genMatrix(key);
    printMatrix();

    // Nhập chuỗi cần mã hóa
    printf("\nNhập văn bản cần mã hóa: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0; // Loại bỏ '\n'

    // Xử lý và in kết quả trung gian
    preprocess(plaintext, processedStr);
    printf("Văn bản nhập từ bàn phím: %s\n", processedStr);

    // Mã hóa và in kết quả cuối cùng
    encrypt(processedStr, ciphertext);
    printf("Văn bản đã mã hóa: %s\n", ciphertext);

    return 0;
}