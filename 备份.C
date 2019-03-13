32 16 00 09 73 75 62 73 74 6f 70 69 63 15 16 6d 79 70 61 79 6c 6f 61 64

10 1e 00 04 4d 51 54 54 04 c2 00 14 00 02 6d 67 00 04 72 6f 6f 74 00 08 70 61 73 73 77 6f 72 64 

Emergency:0,Voltage:0,Current:0,Quantity:0,Duration:0,Er_id:0,Switch:0,Cp:0,Rfid_User:0,Version:0




is_stop:0,is_lock:0,,is_switch:0,v:2 

登陆
10 36 00 06 4D 51 49 73 64 70 03 C2 00 78 00 18 33 36 46 46 44 41 30 35 34 33 35 33 33 31 33 37 33 32 36 34 30 37 34 33 00 04 72 6F 6F 74 00 08 70 61 73 73 77 6F 72 64 


订阅
82 2F 15 16 00 2A 6D 61 63 68 69 6E 65 5F 73 75 62 73 63 72 69 62 65 2F 33 36 46 46 44 41 30 35 34 33 35 33 33 31 33 37 33 32 36 34 30 37 34 33 01

回复
90 03 15 16 01 


上报
32 B5 01 00 32 6D 61 63 68 69 6E 65 5F 73 63 68 65 64 75 6C 65 5F 75 70 6C 6F 61 64 65 72 2F 33 36 46 46 44 41 30 35 34 33 35 33 33 31 33 37 33 32 36 34 30 37 34 33 15 17 69 73 5F 73 74 6F 70 3A 30 2C 69 73 5F 6C 6F 63 6B 3A 30 2C 65 72 5F 69 64 3A 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 2C 64 75 72 61 74 69 6F 6E 3A 30 30 30 30 30 30 30 30 2C 76 6F 6C 74 61 67 65 3A 32 32 33 2C 63 75 72 72 65 6E 74 3A 30 30 30 30 30 30 2C 71 75 61 6E 74 69 74 79 3A 30 30 30 30 30 30 2C 69 73 5F 73 77 69 74 63 68 3A 30 2C 76 3A 32 

回复
40 02 15 17 


服务器下发
0x30, 
0x5A, 
0x00, 
0x2A, 
0x6D, 
0x61, 
0x63, 
0x68, 
0x69, 
0x6E, 
0x65, 
0x5F, 
0x73, 
0x75, 
0x62, 
0x73, 
0x63, 
0x72, 
0x69, 
0x62, 
0x65, 
0x2F, 
0x33, 
0x36, 
0x46, 
0x46, 
0x44, 
0x41, 
0x30, 
0x35, 
0x34, 
0x33, 
0x35, 
0x33, 
0x33, 
0x31, 
0x33, 
0x37, 
0x33, 
0x32, 
0x36, 
0x34, 
0x30, 
0x37, 
0x34, 
0x33, 
0x65, 
0x72, 
0x5F, 
0x69, 
0x64, 
0x3A, 
0x35, 
0x63, 
0x35, 
0x32, 
0x36, 
0x32, 
0x65, 
0x64, 
0x37, 
0x37, 
0x39, 
0x33, 
0x36, 
0x35, 
0x30, 
0x30, 
0x30, 
0x31, 
0x34, 
0x39, 
0x39, 
0x61, 
0x38, 
0x64, 
0x2C, 
0x73, 
0x77, 
0x69, 
0x74, 
0x63, 
0x68, 
0x3A, 
0x31, 
0x2C, 
0x6C, 
0x6F, 
0x63, 
0x6B, 
0x3A, 
0x30

 1.以下产品需要安装支持的编译器:  Simulink Coder 8.13 Simulink Real-Time 6.7 MATLAB Coder 3.4  2.此安装完成后，应按照从 www.mathworks.com/distconfig 获取的说明中所述继续配置 MATLAB Distributed Computing Server。  3.MATLAB Compiler 6.5 要求安装支持的编译器以创建 Excel 加载项  4.MATLAB Compiler SDK 6.4 要求安装以下程序:  ● .NET Framework，用于创建与 Excel 集成的 .NET 程序集和可部署的存档 ● 支持的编译器，用于创建 COM 组件、C 和 C++ 共享库 ● Java JDK，用于创建 Java 包


Get-AppxPackage -AllUsers| Foreach {Add-AppxPackage -DisableDevelopmentMode -Register "$($_.InstallLocation)\AppXManifest.xml"}



13621733137
0918004280136015c
		



#include <stdio.h>  
#include <windows.h>  
void color(short x) //自定义函根据参数改变颜色   
{  
    if(x>=0 && x<=15)//参数在0-15的范围颜色  
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);    //只有一个参数，改变字体颜色   
    else//默认的颜色白色  
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  
}  
int main()  
{         
    printf("此处为没调用颜色函数之前默认的颜色\n");  
    //调用自定义color(x)函数 改变的颜色  
    color(0);   printf("黑色\n");  
    color(1);   printf("蓝色\n");  
    color(2);   printf("绿色\n");   
    color(3);   printf("湖蓝色\n");  
    color(4);   printf("红色\n");  
    color(5);   printf("紫色\n");  
    color(6);   printf("黄色\n");   
    color(7);   printf("白色\n");  
    color(8);   printf("灰色\n");  
    color(9);   printf("淡蓝色\n");  
    color(10);  printf("淡绿色\n");  
    color(11);  printf("淡浅绿色\n");   
    color(12);  printf("淡红色\n");  
    color(13);  printf("淡紫色\n");  
    color(14);  printf("淡黄色\n");  
    color(15);  printf("亮白色\n");  
    color(16);    //因为这里大于15，恢复默认的颜色   
    printf("回到原来颜色\n");  
    return 0;
}

























































































直流侧线缆线径
交流测线缆线径1
交流测线缆线径2
高压系统集成方式
电驱动集成方式