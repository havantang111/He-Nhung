#include <mcs51/8051.h>

#define IN1 P1_1
#define IN2 P1_2
#define ENA P1_3 // bên trái
#define IN3 P1_4
#define IN4 P1_5
#define ENB P1_6 // bên phải

#define s1 P0_1
#define s2 P0_2
#define s3 P0_3
#define s4 P0_4
#define s5 P0_5
#define near P0_6

unsigned int base_speed = 0;
unsigned int max_speed = 255;
unsigned int counter = 0;
unsigned int left_speed = 0;
unsigned int right_speed = 0;

unsigned long timer1_millis = 0;

unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long time4 = 0;
unsigned long time5 = 0;
unsigned long time6 = 0;
unsigned long time7 = 0;
unsigned long time8 = 0;
unsigned long time9 = 0;
// Hàm delay 1ms
void delay_1ms() {
    unsigned int i, j;

    for (i = 0; i < 100; i++) {
        for (j = 0; j < 120; j++) {
        }
    }
}

// Hàm delay tổng quát dựa trên delay 1ms
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++) {
        }
    }
}

void tien(unsigned int speed) {
    left_speed = speed;
    right_speed = speed;
    IN1 = 1;
    IN2 = 0;
    IN3 = 1;
    IN4 = 0;
}



void lui(unsigned int speed) {
    left_speed = speed;
    right_speed = speed;

    // Đảo chiều động cơ
    IN1 = 0;
    IN2 = 1;
    IN3 = 0;
    IN4 = 1;
}

void reTrai(unsigned int speed) {
    left_speed = 10;
    right_speed = speed;
    IN1 = 1;
    IN2 = 0;
    IN3 = 1;
    IN4 = 0;
}

void rePhai(unsigned int speed) {
    left_speed = speed;
    right_speed = 10;
    IN1 = 1;
    IN2 = 0;
    IN3 = 1;
    IN4 = 0;
}

void dung(unsigned int speed) {
    left_speed = speed;
    right_speed = speed;
    IN1 = 0;
    IN2 = 0;
    IN3 = 0;
    IN4 = 0;
}
void reTraiNhanh(int speed){
    left_speed = speed - 15;
    right_speed = speed;
    IN1 = 1;
    IN2 = 0;
    IN3 = 1;
    IN4 = 0;
}
void rePhaiNhanh(int speed){
    left_speed = speed;
    right_speed = speed - 15;
    IN1 = 1;
    IN2 = 0;
    IN3 = 1;
    IN4 = 0;
}

// Chương trình ngắt Timer 0 cho PWM
void timer0_isr(void) __interrupt (1) __using (1) {
    TF0 = 0;
    TR0 = 0;
    TH0 = 0xF6; // Giá trị nạp lại cho Timer 0
    TL0 = 0x3C; // 0.1ms ở tần số 12MHz

    // Điều khiển chân ENA và ENB cho tốc độ động cơ

    if(counter < left_speed){
        ENA = 1;
    }else{
        ENB = 0;
    }
    if(counter < right_speed){
        ENB = 1;
    }else{
        ENB = 0;
    }
    counter++;
    if (counter >= max_speed) counter = 0; // Đặt lại bộ đếm sau chu kỳ PWM

    TR0 = 1;
}

// Chương trình ngắt Timer 1 cho bộ đếm mili giây
void timer1_isr(void) __interrupt (3) __using (2) {
    TF1 = 0;
    TR1 = 0;

    TH1 = 0xD0;
    TL1 = 0xB0;

    timer1_millis++; // Tăng bộ đếm mili giây

    TR1 = 1;
}

void loop() {
    if ((timer1_millis - time1) > 5) {
        while (s2 == 1 && s3 == 0 && s4 == 1) {
            tien(60);
            break;
        }
        time1 = timer1_millis;
    }

    if ((timer1_millis - time2) > 5) {
        while (s2 == 0 && s3 == 0 && s4 == 1) {
            dung(255);
            delay_ms(100);
            reTrai(60);
            delay_ms(100);
            break;
        }
        time2 = timer1_millis;
    }

    if ((timer1_millis - time3) > 5) {
        while (s2 == 1 && s3 == 0 && s4 == 0) {
            dung(255);
            delay_ms(100);
            rePhai(50);
            delay_ms(80);
            break;
        }
        time3 = timer1_millis;
    }

    if ((timer1_millis - time4) > 5) {
        while (s2 == 0 && s3 == 1 && s4 == 1) {
            dung(255);
            delay_ms(150);
            reTrai(100);
            delay_ms(80);
            while (s2 == 0 && s3 == 1 && s4 == 1) {
                tien(50);
                delay_ms(150);
                dung(255);
                delay_ms(5);
                break;
            }
            break;
        }
        time4 = timer1_millis;
    }

    if ((timer1_millis - time5) > 5) {
        while (s2 == 1 && s3 == 1 && s4 == 0) {
            dung(255);
            delay_ms(100);
            lui(100);
            delay_ms(70);
            rePhai(100);
            delay_ms(80);
            while (s2 == 1 && s3 == 1 && s4 == 0) {
                tien(60);
                delay_ms(150);
                dung(255);
                delay_ms(5);
                break;
            }
            break;
        }
        time5 = timer1_millis;
    }

    if ((timer1_millis - time4) > 5) {
        while (s2 == 0 && s3 == 0 && s4 == 1) {
            reTraiNhanh(60);
            break;
        }
        time4 = timer1_millis;
    }
    if ((timer1_millis - time5) > 5) {
        while (s2 == 1 && s3 == 1 && s4 == 0) {
            rePhaiNhanh(60);
            break;
        }
        time5 = timer1_millis;
    }
    if ((timer1_millis - time6) > 5) {
        while (s2 == 0 && s3 == 0 && s4 == 0) {
            dung(255);
            delay_ms(100);
            tien(70);
            delay_ms(90);
            lui(60);
            delay_ms(30);
            break;
        }
        time6 = timer1_millis;
    }

    if ((timer1_millis - time7) > 5) {
        while (s2 == 1 && s3 == 1 && s4 == 1) {
            lui(90);
            delay_ms(80);
            dung(255);
            delay_ms(70);
            break;
        }
        time7 = timer1_millis;
    }


    if ((timer1_millis - time8) > 5) {// vuong goc
        while (s1 == 1 && s2 == 0) {
        left_speed = 75;
        right_speed = 100;

        IN1 = 1;
        IN2 = 0;
        IN3 = 1;
        IN4 = 0;
        }
        time8 = timer1_millis;
    }


    if ((timer1_millis - time9) > 5) {// vuong goc
        while (s4 == 1 && s5 == 0) {
        left_speed = 100;
        right_speed = 75;

        IN1 = 1;
        IN2 = 0;
        IN3 = 1;
        IN4 = 0;
        }
        time9 = timer1_millis;
    }

}

// Hàm main
void main(void) {
    IN1 = 0;
    IN2 = 1;
    IN3 = 0;
    IN4 = 1;

    TMOD |= 0x02; // Chế độ Timer 0
    TH0 = 0xF6;   // Giá trị nạp lại cho Timer 0
    TL0 = 0x3C;   // Giá trị nạp lại cho Timer 0
    ET0 = 1;      // Kích hoạt ngắt Timer 0

    TMOD |= 0x10; // Chế độ Timer 1
    TH1 = 0xD0;   // Giá trị nạp lại cho Timer 1
    TL1 = 0xB0;   // Giá trị nạp lại cho Timer 1
    ET1 = 1;      // Kích hoạt ngắt Timer 1

    EA = 1;       // Kích hoạt tất cả các ngắt
    TR0 = 1;      // Khởi động Timer 0
    TR1 = 1;      // Khởi động Timer 1

    delay_ms(300);
    while (1) {
        loop();
    }
}
