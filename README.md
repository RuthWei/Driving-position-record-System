# Driving-position-record-System
Get position of car through GPS, save it in local file. This file is less than 81k. If record is too mach, it will delete the first ones. if needed, we can restart this system, and choose to show record in this file.

This software has two functions. One is record vehicle position information, including longitude, latitude, altitude and local time. The other is show the record in disk according to the time. If you have GPS, you can connect it to USB, and mark the 18 line in RWFunction.h (#define TEST 0) for test. if not, you can use GPS_data.txt for test. It is also real data for some where. And, this is default test case. We need to put GPS_data.txt in Debug directory of currect project. Just like : /Users/UserName/Library/Developer/Xcode/DerivedData/ProjectName/Build/Products/Debug/

该软件有两个功能，一个功能是记录车辆当前的行驶信息，包括经度、纬度、海拔高度组成的定位信息，还包括当时的时间，另一个功能是按时间先后顺序显示记录的信息。如果有GPS设备，可以把设备接到电脑的USB口，需要把RWFunction.h中第18行的#define TEST 0给注释掉，进行测试。如果没有设备，可以用GPS抓下来的数据进行测试。本代码默认是用GPS抓下来的数据进行测试。需要把GPS_data.txt放在工程的Debug目录下，比如：/Users/UserName/Library/Developer/Xcode/DerivedData/DrivingPositionRecordSystem/Build/Products/Debug/

That is all. 
Good luck for you!
