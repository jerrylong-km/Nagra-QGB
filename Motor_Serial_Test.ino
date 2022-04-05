/*直流电机控制测试程序*/
#define MotorSpeed_Pin 2  //设置#2（PWM）针脚为电机转速控制端子
#define Spin_Direction_Pin 4 //设置#4针脚为电机旋转方向控制端子
#define MotorBreak_Pin 6  //设置#6针脚为电机刹车控制端子

const byte numChars = 32; //存储从串口读取的字符
char receivedChars[numChars]; //存储串口读取字符形成的字符串数组
String Received_String =""; //存储串口返回的字符串
String Angle_Str = ""; //存储角度数据-字符串类型
int TensionArmAngle = 0; //存储角度数据-整数型
int MotorSpeed = 0;//设置电机转速/力矩
boolean newData = false;
int Angle = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(MotorSpeed_Pin,OUTPUT);
  pinMode(Spin_Direction_Pin,OUTPUT);
  pinMode(MotorBreak_Pin,OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600); //左侧旋转编码器串口
  Serial2.begin(9600); //右侧旋转编码器串口
  Serial.println("<Arduino is ready>");
}

void loop() {
  //从Serial端口读取控制电机旋转的数据
  TensionArmAngle=RotaryEncoderAngle();
  
  //将编码器角度值转换为控制电机转速的PWM值
  MotorSpeed = map(TensionArmAngle,39,1,6,30);
  
  //显示串口输入的数值
  Serial.print("MotorSpeed:");
  Serial.println(MotorSpeed);

  // 控制电机旋转:
  digitalWrite(Spin_Direction_Pin,HIGH);
  digitalWrite(MotorBreak_Pin,HIGH);
  analogWrite(MotorSpeed_Pin,MotorSpeed);

}

int RotaryEncoderAngle() {
  //将从串口读取的字符合成字符串，以回车符作为每个字符串的结束符号
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
            
    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // 当读取到回车符时，表示读取的字符串结束
            ndx = 0;
            newData = true;
        }
    }

    //将接收到的角度信息转化为整数
    if (newData == true) 
    {
        Received_String = receivedChars;
        Angle_Str = Received_String.substring(6,10);
        Angle = Angle_Str.toInt();
       newData = false;
    }

    return Angle;
}
