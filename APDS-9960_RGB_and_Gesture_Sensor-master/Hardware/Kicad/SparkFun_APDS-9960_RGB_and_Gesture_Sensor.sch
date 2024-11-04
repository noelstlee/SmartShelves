EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Sparkfun APDS-9960 RGB and Gesture Sensor"
Date "2020-12-17"
Rev "v10"
Comp "Sparkfun"
Comment1 "https://creativecommons.org/Licenses/by-sa/4.0"
Comment2 "Released under the Creative Commons Attribution Share-Alike 4.0 License"
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2200 4300 2200 4400
Wire Wire Line
	1700 4300 2200 4300
Wire Wire Line
	4800 5200 4800 5025
Wire Wire Line
	4200 5025 4200 5200
Wire Wire Line
	2200 4200 2200 3850
Wire Wire Line
	1700 4200 2200 4200
Wire Wire Line
	7600 3150 7600 3750
Wire Wire Line
	5400 4725 5400 3700
Wire Wire Line
	6300 4000 6000 4000
Wire Wire Line
	6000 4000 6000 3700
Wire Wire Line
	6000 3700 5400 3700
Wire Wire Line
	4800 3700 4800 3150
Wire Wire Line
	4800 3700 4700 3700
Connection ~ 5400 3700
Connection ~ 4800 3700
Text Label 1825 4000 0    50   ~ 10
SCL
Text Label 7600 4300 0    50   ~ 10
SCL
Text Label 1825 4100 0    50   ~ 10
SDA
Text Label 7600 4400 0    50   ~ 10
SDA
Wire Wire Line
	7300 4200 7600 4200
Wire Wire Line
	7600 4050 7600 4200
Text Label 7600 4200 0    50   ~ 10
INT
Wire Wire Line
	1700 3900 1825 3900
Text Label 1825 3900 0    50   ~ 10
INT
Wire Wire Line
	8350 3600 8250 3600
Wire Wire Line
	8250 3600 8250 3750
Wire Wire Line
	8750 3600 8850 3600
Wire Wire Line
	7300 4000 7400 4000
Wire Wire Line
	7400 4000 7400 4100
Wire Wire Line
	7400 4100 7300 4100
Wire Wire Line
	1700 4400 1825 4400
Text Label 1825 4400 0    50   ~ 10
VL
Wire Wire Line
	4200 4725 4200 4400
Text Label 4200 3050 2    50   ~ 10
VL
$Comp
L sparkfun_apds_9960:Resistor R1
U 1 1 B9BAFB95
P 7600 3900
F 0 "R1" H 7600 3725 50  0000 L BNN
F 1 "10k" V 7600 3900 50  0000 C CNN
F 2 "APDS_9960:0603-RES" H 7600 3900 50  0001 C CNN
F 3 "" H 7600 3900 50  0001 C CNN
	1    7600 3900
	-1   0    0    1   
$EndComp
$Comp
L sparkfun_apds_9960:Resistor R2
U 1 1 E6A6F2CC
P 8850 3900
F 0 "R2" H 8725 3725 50  0000 L BNN
F 1 "4.7k" V 8850 3900 50  0000 C CNN
F 2 "APDS_9960:0603" H 8850 3900 50  0001 C CNN
F 3 "" H 8850 3900 50  0001 C CNN
	1    8850 3900
	-1   0    0    1   
$EndComp
$Comp
L sparkfun_apds_9960:SOLDERJUMPER_2WAYPASTE1&2&3 SJ1
U 1 1 44DD8191
P 8550 3600
F 0 "SJ1" V 8725 3600 50  0000 C BNN
F 1 "SOLDERJUMPER_2WAYPASTE1&2&3" V 8250 3500 59  0001 R BNN
F 2 "APDS_9960:SJ_3_PASTE1&2&3" H 8550 3600 50  0001 C CNN
F 3 "" H 8550 3600 50  0001 C CNN
	1    8550 3600
	0    -1   1    0   
$EndComp
$Comp
L sparkfun_apds_9960:Cap_NonPol C2
U 1 1 3B1DFF3D
P 4800 4875
F 0 "C2" H 4800 4925 50  0000 L BNN
F 1 "1uF" H 4800 4775 50  0000 L BNN
F 2 "APDS_9960:0603-CAP" H 4800 4875 50  0001 C CNN
F 3 "" H 4800 4875 50  0001 C CNN
	1    4800 4875
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:FIDUCIAL1X2 FID1
U 1 1 DB63CB8D
P 1650 6950
F 0 "FID1" H 1650 6950 50  0001 C CNN
F 1 "FIDUCIAL1X2" H 1650 6950 50  0001 C CNN
F 2 "APDS_9960:FIDUCIAL-1X2" H 1650 6950 50  0001 C CNN
F 3 "" H 1650 6950 50  0001 C CNN
	1    1650 6950
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:FIDUCIAL1X2 FID2
U 1 1 E067070F
P 1650 7150
F 0 "FID2" H 1650 7150 50  0001 C CNN
F 1 "FIDUCIAL1X2" H 1650 7150 50  0001 C CNN
F 2 "APDS_9960:FIDUCIAL-1X2" H 1650 7150 50  0001 C CNN
F 3 "" H 1650 7150 50  0001 C CNN
	1    1650 7150
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:STAND-OFF STANDOFF1
U 1 1 ABAB275D
P 1450 6950
F 0 "STANDOFF1" H 1450 6950 50  0001 C CNN
F 1 "STAND-OFF" H 1450 6950 50  0001 C CNN
F 2 "APDS_9960:STAND-OFF" H 1450 6950 50  0001 C CNN
F 3 "" H 1450 6950 50  0001 C CNN
	1    1450 6950
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:STAND-OFF STANDOFF2
U 1 1 ABB9F451
P 1450 7150
F 0 "STANDOFF2" H 1450 7150 50  0001 C CNN
F 1 "STAND-OFF" H 1450 7150 50  0001 C CNN
F 2 "APDS_9960:STAND-OFF" H 1450 7150 50  0001 C CNN
F 3 "" H 1450 7150 50  0001 C CNN
	1    1450 7150
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:SFE_LOGO_NAME_FLAME1_INCH U$1
U 1 1 BABD3B23
P 4375 7600
F 0 "U$1" H 4375 7600 50  0001 C CNN
F 1 "SFE_LOGO_NAME_FLAME.1_INCH" H 4375 7600 50  0001 C CNN
F 2 "APDS_9960:SFE_LOGO_FLAME_.1" H 4375 7600 50  0001 C CNN
F 3 "" H 4375 7600 50  0001 C CNN
	1    4375 7600
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:SFE_LOGO_FLAME.1COP LOGO1
U 1 1 8712EDD9
P 3900 7600
F 0 "LOGO1" H 3900 7600 50  0001 C CNN
F 1 "SFE_LOGO_FLAME.1_INCH" H 3900 7600 50  0001 C CNN
F 2 "APDS_9960:SFE_LOGO_NAME_FLAME_.1" H 3900 7600 50  0001 C CNN
F 3 "" H 3900 7600 50  0001 C CNN
	1    3900 7600
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:OSHW LOGO2
U 1 1 7B08F8A5
P 3175 7225
F 0 "LOGO2" H 3175 7225 50  0001 C CNN
F 1 "OSHW-LOGOS" H 3175 7225 50  0001 C CNN
F 2 "APDS_9960:OSHW-LOGO-S" H 3175 7225 50  0001 C CNN
F 3 "" H 3175 7225 50  0001 C CNN
	1    3175 7225
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:Resistor R4
U 1 1 4EACF14A
P 4500 3900
F 0 "R4" V 4575 4000 50  0000 L BNN
F 1 "DNP" V 4500 3900 50  0000 C CNN
F 2 "APDS_9960:0603-RES" H 4500 3900 50  0001 C CNN
F 3 "" H 4500 3900 50  0001 C CNN
	1    4500 3900
	0    -1   -1   0   
$EndComp
$Comp
L sparkfun_apds_9960:Resistor R3
U 1 1 41183507
P 8250 3900
F 0 "R3" H 8250 3725 50  0000 L BNN
F 1 "4.7k" V 8250 3900 50  0000 C CNN
F 2 "APDS_9960:0603" H 8250 3900 50  0001 C CNN
F 3 "" H 8250 3900 50  0001 C CNN
	1    8250 3900
	-1   0    0    1   
$EndComp
$Comp
L sparkfun_apds_9960:Cap_Pol C1
U 1 1 051D5E36
P 4200 4875
F 0 "C1" H 4200 4925 50  0000 L BNN
F 1 "100uF" H 4200 4750 50  0000 L BNN
F 2 "APDS_9960:c_2917" H 4200 4875 50  0001 C CNN
F 3 "" H 4200 4875 50  0001 C CNN
	1    4200 4875
	1    0    0    -1  
$EndComp
Text Notes 8575 3400 0    59   ~ 12
I2C Addr: 0x39
Text Notes 8575 3500 0    60   ~ 0
I2C Pullup Enable
Wire Wire Line
	1700 4100 1825 4100
Wire Wire Line
	1700 4000 1825 4000
Wire Wire Line
	8250 4300 8250 4050
Wire Wire Line
	7300 4300 8250 4300
Wire Wire Line
	8850 4400 8850 4050
Wire Wire Line
	7300 4400 8850 4400
Text Notes 4425 3550 0    50   ~ 0
Connect\nPower\nSupplies
$Comp
L sparkfun_apds_9960:SOLDERJUMPERNO SJ2
U 1 1 CD7571B3
P 4500 3700
F 0 "SJ2" H 4425 3775 50  0000 L BNN
F 1 "SOLDERJUMPERNC" H 4400 3500 59  0001 L BNN
F 2 "APDS_9960:SJ_2S" H 4500 3700 50  0001 C CNN
F 3 "" H 4500 3700 50  0001 C CNN
	1    4500 3700
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:Conn_01x06 JP1
U 1 1 2F430EF2
P 1550 4150
F 0 "JP1" H 1500 3750 59  0000 L BNN
F 1 "M06SIP" H 1350 4500 59  0000 L BNN
F 2 "APDS_9960:1X06" H 1550 4150 50  0001 C CNN
F 3 "" H 1550 4150 50  0001 C CNN
	1    1550 4150
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR0101
U 1 1 5FDF8234
P 2200 3850
F 0 "#PWR0101" H 2200 3700 50  0001 C CNN
F 1 "VCC" H 2215 4023 50  0000 C CNN
F 2 "" H 2200 3850 50  0001 C CNN
F 3 "" H 2200 3850 50  0001 C CNN
	1    2200 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5FDF8A5A
P 2200 4400
F 0 "#PWR0102" H 2200 4150 50  0001 C CNN
F 1 "GND" H 2205 4227 50  0000 C CNN
F 2 "" H 2200 4400 50  0001 C CNN
F 3 "" H 2200 4400 50  0001 C CNN
	1    2200 4400
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0103
U 1 1 5FDFE939
P 4800 3150
F 0 "#PWR0103" H 4800 3000 50  0001 C CNN
F 1 "VCC" H 4815 3323 50  0000 C CNN
F 2 "" H 4800 3150 50  0001 C CNN
F 3 "" H 4800 3150 50  0001 C CNN
	1    4800 3150
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0104
U 1 1 5FDFF54F
P 7600 3150
F 0 "#PWR0104" H 7600 3000 50  0001 C CNN
F 1 "VCC" H 7615 3323 50  0000 C CNN
F 2 "" H 7600 3150 50  0001 C CNN
F 3 "" H 7600 3150 50  0001 C CNN
	1    7600 3150
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0105
U 1 1 5FE0155A
P 8550 3150
F 0 "#PWR0105" H 8550 3000 50  0001 C CNN
F 1 "VCC" H 8565 3323 50  0000 C CNN
F 2 "" H 8550 3150 50  0001 C CNN
F 3 "" H 8550 3150 50  0001 C CNN
	1    8550 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3150 8550 3400
$Comp
L power:GND #PWR0106
U 1 1 5FE237BC
P 4200 5200
F 0 "#PWR0106" H 4200 4950 50  0001 C CNN
F 1 "GND" H 4205 5027 50  0000 C CNN
F 2 "" H 4200 5200 50  0001 C CNN
F 3 "" H 4200 5200 50  0001 C CNN
	1    4200 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5FE27561
P 5400 5200
F 0 "#PWR0107" H 5400 4950 50  0001 C CNN
F 1 "GND" H 5405 5027 50  0000 C CNN
F 2 "" H 5400 5200 50  0001 C CNN
F 3 "" H 5400 5200 50  0001 C CNN
	1    5400 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5FE2782C
P 4800 5200
F 0 "#PWR0108" H 4800 4950 50  0001 C CNN
F 1 "GND" H 4805 5027 50  0000 C CNN
F 2 "" H 4800 5200 50  0001 C CNN
F 3 "" H 4800 5200 50  0001 C CNN
	1    4800 5200
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:Cap_NonPol C3
U 1 1 58F267C5
P 5400 4875
F 0 "C3" H 5400 4925 50  0000 L BNN
F 1 "1uF" H 5400 4775 50  0000 L BNN
F 2 "APDS_9960:0603-CAP" H 5400 4875 50  0001 C CNN
F 3 "" H 5400 4875 50  0001 C CNN
	1    5400 4875
	1    0    0    -1  
$EndComp
$Comp
L sparkfun_apds_9960:APDS-9960 U1
U 1 1 F9562130
P 6800 4200
F 0 "U1" H 6700 4600 59  0000 R TNN
F 1 "APDS-9960" H 6400 3800 59  0000 L BNN
F 2 "APDS_9960:APDS-9960" H 6800 4200 50  0001 C CNN
F 3 "" H 6800 4200 50  0001 C CNN
	1    6800 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3700 4200 3700
Wire Wire Line
	4200 3700 4200 3050
Wire Wire Line
	4800 3700 4800 3900
Wire Wire Line
	4800 3900 4650 3900
Wire Wire Line
	4200 3700 4200 3900
Wire Wire Line
	4200 3900 4350 3900
Connection ~ 4200 3700
Wire Wire Line
	4800 3700 5400 3700
Wire Wire Line
	4200 3900 4200 4400
Connection ~ 4200 3900
Connection ~ 4200 4400
Wire Wire Line
	4200 4400 4800 4400
Connection ~ 4800 4400
Wire Wire Line
	4800 4400 4800 4725
Wire Wire Line
	4800 4400 6300 4400
Text Label 2200 4200 0    50   ~ 10
VCC
Text Label 2200 4375 0    50   ~ 10
GND
Text Label 5400 3700 0    50   ~ 10
VCC
Wire Wire Line
	8850 3600 8850 3750
Text Notes 1000 6375 0    70   ~ 14
MECH
Text Notes 2575 6375 0    70   ~ 14
LOGOS
Text Notes 975  3250 0    70   ~ 14
HEADER
Wire Notes Line
	1000 6400 2100 6400
Wire Notes Line
	2100 6400 2100 7675
Wire Notes Line
	2100 7675 1000 7675
Wire Notes Line
	1000 7675 1000 6400
Wire Notes Line
	2575 6400 6650 6400
Wire Notes Line
	6650 6400 6650 7675
Wire Notes Line
	6650 7675 2575 7675
Wire Notes Line
	2575 7675 2575 6400
Wire Notes Line
	1000 3275 3075 3275
Wire Notes Line
	3075 4800 1000 4800
Wire Notes Line
	1000 4800 1000 3275
Wire Notes Line
	3075 3275 3075 4800
Wire Wire Line
	5400 5025 5400 5200
Wire Wire Line
	6300 4200 6000 4200
Wire Wire Line
	6000 4200 6000 5200
$Comp
L power:GND #PWR?
U 1 1 5FDEF7E6
P 6000 5200
F 0 "#PWR?" H 6000 4950 50  0001 C CNN
F 1 "GND" H 6005 5027 50  0000 C CNN
F 2 "" H 6000 5200 50  0001 C CNN
F 3 "" H 6000 5200 50  0001 C CNN
	1    6000 5200
	1    0    0    -1  
$EndComp
$EndSCHEMATC
