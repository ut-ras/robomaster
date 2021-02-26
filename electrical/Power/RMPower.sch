EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RMPower:PM01 U?
U 1 1 6027843B
P 8750 3900
F 0 "U?" H 9344 3646 50  0000 L CNN
F 1 "PM01" H 9344 3555 50  0000 L CNN
F 2 "" H 8750 3900 50  0001 C CNN
F 3 "" H 8750 3900 50  0001 C CNN
	1    8750 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery BT?
U 1 1 60278C5B
P 8200 4450
F 0 "BT?" H 8308 4496 50  0000 L CNN
F 1 "Battery" H 8308 4405 50  0000 L CNN
F 2 "" V 8200 4510 50  0001 C CNN
F 3 "~" V 8200 4510 50  0001 C CNN
	1    8200 4450
	1    0    0    -1  
$EndComp
$Comp
L RMPower:EA120-12V U?
U 1 1 6027BB34
P 8600 3150
F 0 "U?" V 8696 2722 50  0000 R CNN
F 1 "EA120-12V" V 8605 2722 50  0000 R CNN
F 2 "" H 8600 3150 50  0001 C CNN
F 3 "" H 8600 3150 50  0001 C CNN
	1    8600 3150
	0    -1   -1   0   
$EndComp
$Comp
L RMPower:EA120-12V U?
U 1 1 6027D700
P 9100 5250
F 0 "U?" V 9104 5188 50  0000 L CNN
F 1 "EA120-12V" V 9195 5188 50  0000 L CNN
F 2 "" H 9100 5250 50  0001 C CNN
F 3 "" H 9100 5250 50  0001 C CNN
	1    9100 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	8750 3450 8750 3600
Wire Wire Line
	8950 3450 8950 3600
Wire Wire Line
	8750 4800 8750 4950
Wire Wire Line
	8950 4800 8950 4950
Wire Wire Line
	9300 4100 9650 4100
Wire Wire Line
	9300 4250 9650 4250
Wire Wire Line
	8750 5650 8750 5800
Wire Wire Line
	8950 5650 8950 5800
Wire Wire Line
	8750 2750 8750 2600
Wire Wire Line
	8950 2750 8950 2600
Wire Wire Line
	8400 4250 8200 4250
$Comp
L power:GND #PWR?
U 1 1 60280D4C
P 8250 4100
F 0 "#PWR?" H 8250 3850 50  0001 C CNN
F 1 "GND" V 8255 3972 50  0000 R CNN
F 2 "" H 8250 4100 50  0001 C CNN
F 3 "" H 8250 4100 50  0001 C CNN
	1    8250 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	8400 4100 8250 4100
Wire Wire Line
	9300 3950 9650 3950
$EndSCHEMATC
